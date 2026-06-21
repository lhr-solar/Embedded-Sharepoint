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

          python = pkgs.python310.withPackages (ps: with ps; [ ps.pip ps.can ]);

          # Base packages (common to all)
          basePackages = [
            pkgs.gcc
            # Pin clang + clang-tools to LLVM 16 together. clang-format 16 matches
            # the committed .clang-format (a modern dump: enum AlignConsecutiveMacros,
            # AlignArrayOfStructures); the default LLVM 11 here is too old to parse
            # it. Bumping both avoids the LLVM-11 clang shadowing clang-tools' 16.
            pkgs.clang_16
            pkgs.clang-tools_16
            pkgs.lld
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
            # gcc-arm-embedded is added once via the platform-guarded `armGcc`
            # below (it is not packaged on every system).
            python
            # Fast pip/venv used by nix-hook.sh; only on nixpkgs that package it
            # (filtered out below when null, e.g. the 23.05 pin). nix-hook falls
            # back to pip, or uses a uv that's already on PATH.
            (pkgs.uv or null)
            pkgs.openocd
            pkgs.dfu-util
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
