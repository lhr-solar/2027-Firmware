# 2027 Firmware Monorepo

[![collect_build_results](https://github.com/lhr-solar/2027-Firmware/actions/workflows/collect_build_results.yml/badge.svg)](https://github.com/lhr-solar/2027-Firmware/actions/workflows/collect_build_results.yml)

Welcome to the LHRs firmware monorepo! 😊

This guide should give you a good idea of how to navigate the codebase and build your first program.

## Getting Started

A monorepo is just like it sounds. One repo to rule them all. Prior to a monorepo we had separate code repositories with firmware for each board. A monorepo lets us better enforce common standards and utilties for all firmware that runs on the car.

### Installation
Hopefully this is painless 💔

We currently use [Nix](https://nixos.org/guides/how-nix-works/) to create standardized environments to develop in. You'll notice looking in the `flake.nix` file that there are a lot of required packages. However, with Nix we can define them once and build  the same environment across machines. This is important since we need to support Windows (WSL), Linux, and macOS users on the team.

To start off, run the nix install script **with root access**. It needs root priveleges to create files and edit shell scripts.
```
sudo ./nix_install.sh
```
This will probably take 5-10 minutes. If it takes longer, touch some grass and come back.

Once that's done, run 
```
direnv allow .
```
Direnv is an extension to your shell that will automatically start a Nix environment when you enter the monorepo directory. This is pretty convenient since you won't have to run `nix develop` manually.

If you've followed the last 2 steps properly then when you open a new terminal window and cd (change directory) into the monorepo you should see
```
direnv: loading ~/LHR/2027-Firmware/.envrc
direnv: using flake
direnv: nix-direnv: Using cached dev shell
ARM cross-compiler available
Dev environment loaded for x86_64-linux!
```
That's all the setup you need! If you're experiencing difficulties, ping Aarav Mahesh (monorepo on-call) in the [#software channel](https://lhrsol.slack.com/archives/C44RUHW1Z). He's eager to help out and get his name out there.

Behind the scenes, your environment now has python libraries, microcontroller utilities, a compiler, and more loaded in. Pretty neat.

### Structure

It's important at a high level to understand how the monorepo is laid out. 

Most if not all of your development should be done within the `firmware/` folder. 

Firmware is scaffolded by system-ish:

```
firmware/
├── psys/        # power systems boards
├── controls/    # controls boards
└── telemetry/   # telemetry boards
...
```
Board firmware are subfolders of its system. For example, `psys/LVC` would contain LV Carrier firmware or `controls/VCU` would be for Vehicle Control Unit firmware. Try to use a concise name of the board.

We've tried to make the setup of a new folder straightforward with the `new_board.py` script in the `firmware/` directory. It's a simple (totally not clauded) utility for creating a board subfolder.

In `firmware/`, run 
```python3
python new_board.py
```
This should prompt you with some questions about your board and create a folder for you to start writing firmware in!

Ex: if I made LVC, a power systems board

```
firmware/psys/LVC
├── Board.cmake
├── CMakeLists.txt
├── Makefile
├── build_all_tests.sh
├── config/
├── core/
├── drivers/
└── tests/
```
Don't worry too much about these files mean just yet. We'll get there in the "**Building Firmware**" section. 

Other folders to be aware of ...
`platform/` - shared libraries for firmware development
`bootloader/` - USB and CAN bootloader source   
`templates/` - templates for autogenerating board build files 

### Building Firmware

One of the first things to understand is how code is built in C.

__Source files__ (.c): Source files contain code that handles the majority of your logic. You'll see these files in the `Src/` folders. Source files also may contain a `main()` function, the entry point into your program.
__Header files__ (.h): Header files contain declarations to software components that will be used in your source files. You'll see these files in the `Inc/` folders. 
__Object files__ (.o): Object files are intermediate files generated after compiling your code. Don't worry too much about these; you probably will never see them anyway.
__Binaries__ (.elf, .bin): Binaries are the end product of this process. They contain the machine code that the MCU can understand and run. `.elf` files and `.bin` files are both types of binaries. However, `.elf` files also have debug symbols that are useful when running debuggers.

At a high level

```
Src/*.c ─┐      ╔═════════╗                  ╔══════╗
        ──┤ ──► ║ COMPILE ║ ──► .o files ──► ║ LINK ║ ──► .elf / .bin
Inc/*.h ─┘      ╚═════════╝                  ╚══════╝
```

Every `.c` file is compiled on its own into a `.o`. `.h` files are inlined (copy-pasted) into `.c` files that choose to `#include` them. After code is compiled, linking combines sources to generate one final binary.

This will make more sense as you start to look at existing code and write your own. It's nice to know, for example, what a `linker error` means when you stumble upon it.

Managing the process of building code is hard. What files get combined with what other files? How can you avoid rebuilding all your code if only a small change is made? To better manage this we use CMake.

CMake is a build system generator. You'll see `CMakeLists.txt` and `Board.cmake` files in your board folders. If ever, you should only need to modiy the `Board.cmake` file, as `CMakeLists.txt` statically describes how to include code from other parts of the codebase like `platform/`.

`Board.cmake` is made to be as easy to read and change. It tells the build system what files in your folder should be included in the build process. If you every feel special and want to make your folders, you can add their relative paths to `BOARD_INCLUDE_DIRS` (for directories containing `.h` files) and `BOARD_OTHER_SOURCES` (for .c files).

You'll also see a `Makefile`. A makefile contains recipes for performing commands. For example, to build a test you'd have to run `make TEST=<test>` or `make test-all` to build all tests. These commands call CMake build functions to actually compile and link your code.

The default structure for a board folder contains `core/`, `config/`, `tests/`, and `drivers/`. 
- `core/` contains your production code - this is the full application that is ultimately what is flashed onto the car. **Run with** `make`, `make all`, or `make prod-all` (for multiple boards)
- `tests/` contains test files - prior to writing production code you'll want to write small tests to prove out various parts of your firmware. All source files here should end with `_test.c`. **Run with** `make TEST=<test>`
- `drivers/` contains libraries you write to interface with hardware components. There isn't a solid line in the sand about what a driver is, but it's a nice way to modularize your code.
- `config/` contains header files to configure parameters in your firmware. A common example is to map specific MCU pins you'll be using.

In your board folder, run
```
make
```
This should run the build system and generate a binary with your production code! The code itself is just an empty `main()` function, but hey at least it worked.

You should see a new folder called `build/` was created. This is where the build system stores the created binary and other build files. If you ever want to delete your build folder and start anew run `make clean`.

### Flashing

"Flash" just means to put the binary you compiled onto the memory of the MCU. Once the chip has been flashed/programmed its contents will persist through power cycles.

In your board folder, you can easily flash a binary by running
```
make flash
```
Currently,this calls STM's `st-flash` command under the hood and writes your code to address `0x08000000`, the start of user flash. This should change soon when our bootloader setup is done. Flashing may take a while depending on code size but you should see a _"Jolly good"_ message once your flash is complete. 

Now, if you press the reset button on your board, your code starts running! Wow. Very cool.

Note: for boards with multiple physical boards on the car (i.e. lighting) you will need to run `make flash board_num=<number>` to specify which board's binary to flash. 

**Troubleshooting**
Flashing code can be finnicky. Some good commands to note:

`st-info --probe` - prints out valid STM debuggers. If you don't see an MCU family and flash size printed out, something is very wrong.
`st-flash --erase` - erases user flash on the MCU. Nice way to reset when flashing code is completely bricked and you have no clue why.

### Lifecyle of a Pull Request
- cover how to make a pull request, draft vs ready to review, codeowners, review process, merging
### CI Pipeline
- prob incorporated in the previous section? i.e. builds need to pass in the PR lol
- most important is a high level description of what's being built (all tests, all board prod code in nix env)
