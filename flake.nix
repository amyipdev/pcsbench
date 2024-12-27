# SPDX-License-Identifier: GPL-2.0-or-later
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {self, nixpkgs, flake-utils}:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in {
        packages.default = pkgs.stdenv.mkDerivation {
          name = "pcsbench";
          src = ./.;
          buildInputs = with pkgs; [
            glib
            pkg-config
          ];
          installPhase = ''
            cp pcsbench $out
          '';
        };
      }
    );
}
