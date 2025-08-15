{
  description = "LHRs STM32 Embedded Dev";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/23.05";
  };
  outputs = { self, nixpkgs }:
    let
      mkShellFor = system:
        let
          pkgs = import nixpkgs { inherit system; };
          armGcc = pkgs.gcc-arm-embedded;
          # Python with desired packages
          python = pkgs.python310.withPackages (ps: with ps; [
            mkdocs
            pyserial
          ]);
          # Base packages
          basePackages = [
            pkgs.gcc
            pkgs.gdb
            pkgs.clang
            pkgs.clang-tools
            pkgs.lld
            pkgs.bear
            pkgs.cmake
            pkgs.pkg-config
            pkgs.ncurses
            pkgs.picocom
            pkgs.openocd
            pkgs.stlink
            pkgs.git
            pkgs.wget
            pkgs.gnupg
            pkgs.cacert
            pkgs.gnumake
            pkgs.binutils
            python
          ];
          # Only add armGcc if it exists
          packageList = if armGcc != null then basePackages ++ [ armGcc ] else basePackages;
          # Nix-level shellHook message
          armGccMessage = if armGcc != null
                          then "ARM cross-compiler available"
                          else "No ARM cross-compiler available";
        in
        pkgs.mkShell {
          packages = packageList;
          shellHook = ''
            echo "${armGccMessage}"
            export PATH=$PATH:${armGcc}/bin
            echo "Dev environment loaded for ${system}!"
          '';
        };
    in
    {
      devShells = {
        "x86_64-linux" = { default = mkShellFor "x86_64-linux"; };
        "aarch64-linux" = { default = mkShellFor "aarch64-linux"; };
      };
    };
}