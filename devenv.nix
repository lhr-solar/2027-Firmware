# pkgs -> nix package set
{ pkgs, ... }:

let
  # arm-none-eabi toolchain (may not exist everywhere)
  armGcc = pkgs.gcc-arm-embedded or null;

  armGccMessage = if armGcc != null
                  then "ARM cross-compiler available"
                  else "No ARM cross-compiler available";

  basePackages = [
    pkgs.gcc
    pkgs.clang
    pkgs.clang-tools
    pkgs.lld
    pkgs.cmake
    pkgs.ninja
    pkgs.pkg-config
    pkgs.ncurses
    pkgs.picocom
    pkgs.git
    pkgs.wget
    pkgs.gnupg
    pkgs.cacert
    pkgs.gnumake
    pkgs.binutils
    pkgs.parallel
    pkgs.sl
    pkgs.openocd
    armGcc # filtered out ltr if doesnt exist
  ];

  # Extra debug/flash tools, only if available
  debugPackages =
  if pkgs.stdenv.hostPlatform.isLinux then [
    pkgs.gdb
    pkgs.stlink
  ] else if pkgs.stdenv.isDarwin then [
    pkgs.lldb # gdb issues on mac
    pkgs.stlink
  ] else [];

  
in
{
  # filter out nulls
  packages = builtins.filter (p: p != null) (basePackages ++ debugPackages);

  # devenv's nomenclature for nix shellHook
  enterShell = ''
    # print on term startup
    echo "${armGccMessage}"

    if [ -f "./nix-hook.sh" ]; then
      chmod +x ./nix-hook.sh
      source ./nix-hook.sh
    fi

    echo "Dev environment loaded!"
  '';

  # `devenv --profile python shell` 
  profiles.python.module = {
    languages.python.enable = true;
    languages.python.version = "3.10";
    languages.python.venv.enable = true;
    languages.python.venv.requirements = ./requirements.txt;
  };
}
