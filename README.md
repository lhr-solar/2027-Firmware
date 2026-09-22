# 2027 Firmware Monorepo

Welcome to the LHRs firmware monorepo! 😊

This guide should give you a good idea of how to navigate the codebase and build your first program.

## Getting Started

A monorepo is just like it sounds. One repo to rule them all. Prior to a monorepo we had separate code repositories with firmware for each board. A monorepo lets us better enforce common standards and utilties for all firmware that runs on the car.

#### Installation
Hopefully this is painless 💔

We currently use Nix to create standardized environments to develop in. You'll notice looking in the `flake.nix` file that there are a lot of required packages. However, with Nix we can define them once and build  the same environment across machines. This is important since we need to support WSL and macOS users on the team.

To start off, run the nix install script **with root access**. It needs root priveleges to create files and edit shell scripts.
```
sudo ./nix_install.sh
```
This will probably take ~5-10 minutes. If it takes longer, touch some grass and come back.

Once that's done, run 
```
direnv allow
```
Direnv is an extension to your shell that will automatically start a Nix environment when you enter the monorepo directory. This is pretty convenient since you won't have to run `nix develop` manually.

If you've followed the last 2 steps properly then when you open a new terminal window and cd (changedir) into the monorepo you should see
```
direnv: loading ~/LHR/2027-Firmware/.envrc
direnv: using flake
direnv: nix-direnv: Using cached dev shell
ARM cross-compiler available
Dev environment loaded for x86_64-linux!
```
That's all the setup you need! If you're experiencing difficulties, ping Ravi Shah (monorepo on-call) in the #software channel. He's eager to help out and get his name out there.

Behind the scenes, your environment now has python libraries, microcontroller utilities, a compiler, and more loaded in. Pretty neat.

#### Structure

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

#### Building Firmware

One of the first things to understand is how code is built in C.


