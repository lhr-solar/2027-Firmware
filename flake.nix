{
  description = "LHRs Embedded Dev";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
    # source of the stlink 1.7.0 recipe only (nothing else is taken from it)
    nixpkgs-stlink-pin.url = "github:NixOS/nixpkgs/nixos-23.11";
  };

  outputs = { self, nixpkgs, nixpkgs-stlink-pin }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];

      mkShellFor = system:
        let
          pkgs = import nixpkgs { inherit system; };

          # arm-none-eabi toolchain (may not exist everywhere)
          armGcc = pkgs.gcc-arm-embedded or null;

          python = pkgs.python311;

          # Base packages (common to all)
          basePackages = [
            pkgs.gcc
            pkgs.clang
            pkgs.clang-tools
            pkgs.lld
            pkgs.bear
            pkgs.cmake
            pkgs.ninja
            pkgs.pkg-config
            pkgs.ncurses
            pkgs.picocom
            pkgs.minicom
            pkgs.git
            pkgs.wget
            pkgs.gnupg
            pkgs.cacert
            pkgs.gnumake
            pkgs.binutils
            pkgs.parallel
            pkgs.sl
            pkgs.gcc-arm-embedded
            python
            pkgs.uv
            pkgs.openocd
          ];

          stlink_pkg = (pkgs.callPackage
            "${nixpkgs-stlink-pin}/pkgs/development/tools/misc/stlink/default.nix" { }
          ).overrideAttrs (old: {
            # 1.7.0's CMakeLists predates CMake 4 (dropped cmake_minimum_required < 3.5)
            cmakeFlags = old.cmakeFlags ++ [ "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" ];
          });

          # Extra debug/flash tools, only if available
          debugPackages =
            if pkgs.stdenv.isLinux then [
              pkgs.gdb
              stlink_pkg
            ] else if pkgs.stdenv.isDarwin then [
              pkgs.lldb
              stlink_pkg
            ] else [];

          # Remove nulls
          packageList = builtins.filter (x: x != null)
            (basePackages ++ debugPackages ++ (if armGcc != null then [ armGcc ] else []));

          armGccMessage = if armGcc != null
                          then "ARM cross-compiler available"
                          else "No ARM cross-compiler available";
        in
        pkgs.mkShell {
          packages = packageList;
          shellHook = ''
            echo "${armGccMessage}"
            ${if armGcc != null then "export PATH=$PATH:${armGcc}/bin" else ""}

            if [ -f "./nix-hook.sh" ]; then
              # Make it executable
              chmod +x ./nix-hook.sh
              source ./nix-hook.sh
            fi

            echo "Dev environment loaded for ${system}!"
          '';
        };
    in {
      devShells = nixpkgs.lib.genAttrs systems (system: {
        default = mkShellFor system;
      });
    };
}