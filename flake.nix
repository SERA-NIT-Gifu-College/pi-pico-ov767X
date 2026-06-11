{
    description = "Nix Dev Shell for Raspberry Pi Pico SDK.";

    inputs = {
      nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    };

    outputs = { self, nixpkgs }:
    let
        pkgs = import nixpkgs { system = "x86_64-linux"; };
        my-pico-sdk = pkgs.pico-sdk.override {
            withSubmodules = false;
        };
    in
    {
        devShells.x86_64-linux.default = pkgs.mkShell {
            name = "test pico";
            packages = with pkgs; [
                clang
                cmake
                doxygen
                gcc-arm-embedded
                minicom
                openocd-rp2040
                picotool
                pioasm
                pkg-config
                python3
                tio
            ] ++ [ my-pico-sdk ];
            shellHook = ''
                export GCC_ARM_PATH="${pkgs.gcc-arm-embedded}"
                export PICO_SDK_PATH="${my-pico-sdk}/lib/pico-sdk"
            '';
        };
    };
}
