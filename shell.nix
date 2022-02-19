{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer
  ];
  nativeBuildInputs = with pkgs; [
    (haskellPackages.ghcWithPackages
      (ps: [ ps.shake ]))
    pkg-config
  ];
  shellHook = ''
    NIX_CFLAGS_COMPILE="$(pkg-config --cflags sdl2) $NIX_CFLAGS_COMPILE"
    NIX_CFLAGS_COMPILE="$(pkg-config --cflags sdl2_image) $NIX_CFLAGS_COMPILE"
    NIX_CFLAGS_COMPILE="$(pkg-config --cflags sdl2_ttf) $NIX_CFLAGS_COMPILE"
    NIX_CFLAGS_COMPILE="$(pkg-config --cflags sdl2_mixer) $NIX_CFLAGS_COMPILE"
  '';
}
