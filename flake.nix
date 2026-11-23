{
  outputs =
    { nixpkgs, self }:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      stdenv = with pkgs; (overrideCC fastStdenv gcc14);
      system = "x86_64-linux";
    in
    {
      packages.${system} = with pkgs; {
        default = stdenv.mkDerivation {
          pname = "realm-cli";
          version = "1.0";
          src = ./.;

          nativeBuildInputs = [
            cmake
            pkg-config
          ];

          buildInputs = [ self.packages.${system}.realm-cpp ];
        };

        realm-cpp = stdenv.mkDerivation {
          pname = "realm-cpp";
          version = "0-unstable-2025-09-24";

          src = pkgs.fetchFromGitHub {
            owner = "realm";
            repo = "realm-cpp";
            rev = "4ebdd34b82357dba0220a598fb24a7f3afd7fe7b";
            hash = "sha256-fFcuFruBfg/liysgL/iCq/OMZaTJ72bnbcfHZ2mQl4s=";
            fetchSubmodules = true;
          };

          patches = [ ./realm-cpp.patch ];
          nativeBuildInputs = [ cmake ];

          propagatedBuildInputs = [
            libuv
            openssl
            zlib
          ];

          cmakeFlags = [
            "-DREALM_CPP_NO_TESTS=1"
            "-DREALM_USE_SYSTEM_OPENSSL=1"
            "-DREALM_CORE_SUBMODULE_BUILD=0"
            # disable tests to compile faster
            "-DREALM_BUILD_LIB_ONLY=1"
          ];

          NIX_CFLAGS_COMPILE = [
            "-Wno-changes-meaning"
            "-Wno-error"
          ];
        };
      };
    };

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
}
