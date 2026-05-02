{
  description = "LHRs Embedded Dev";

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
            pkgs.arduino-cli
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
            pkgs.sl
            pkgs.gcc-arm-embedded
            python
            pkgs.openocd
          ];

          # Extra debug/flash tools, only if available
          debugPackages =
            if pkgs.stdenv.isLinux then [
              pkgs.gdb
              pkgs.stlink
            ] else if pkgs.stdenv.isDarwin then [
              pkgs.stlink
              pkgs.lldb
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
