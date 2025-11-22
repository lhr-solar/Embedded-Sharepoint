{
  description = "LHRs STM32 Embedded Dev";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/23.05";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];

      mkShellFor = system:
        let
          pkgs = import nixpkgs { inherit system; };

          # arm-none-eabi toolchain (may not exist everywhere)
          armGcc = pkgs.gcc-arm-embedded or null;

          python = pkgs.python310.withPackages (ps: with ps; [ ]);

          # Base packages (common to all)
          basePackages = [
            pkgs.gcc
            pkgs.clang
            pkgs.clang-tools
            pkgs.lld
            pkgs.bear
            pkgs.cmake
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
            python
          ];

          # Extra debug/flash tools, only if available
          debugPackages =
            if pkgs.stdenv.isLinux then [
              pkgs.gdb
              pkgs.openocd
              pkgs.stlink
            ] else if pkgs.stdenv.isDarwin then [
              pkgs.openocd
              pkgs.stlink
              pkgs.gcc-arm-embedded
              pkgs.lldb
              pkgs.openocd or null
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

            # Provide lsusb-mac alias
            if [[ "$OSTYPE" == "darwin"* ]]; then
              lsusb_mac() {
                system_profiler SPUSBDataType
              }
              export -f lsusb_mac
              echo "Run: lsusb_mac (macOS USB info)"
            
              ls_stm32_dev_port() {
                  ls /dev/cu.*
              }
              export -f ls_stm32_dev_port
              echo "On Mac run: ls_stm32_dev_port (to list STM32 serial port)"
            else
              echo "Run: lsusb (Linux USB info)"
            fi

            if [ ! -d .venv ]; then
              python3 -m venv .venv
              echo "Creating python venv"
            fi
            source .venv/bin/activate
            echo "Installing python requirements"
            if [ -f requirements.txt ]; then
              pip install -r requirements.txt
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
