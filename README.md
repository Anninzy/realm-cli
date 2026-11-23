# realm-cli

A makeshift CLI for MongoDB's (deprecated) Realm database / Atlas Device SDKs,
specifically `.realm` files.

## Who

Probably for insane nix users playing osu\
Currently needs patching / modifying source to personalize settings

## Where

Dug up the records and their corresponding fields from
`~/.local/share/osu/client.realm` with
[Realm Studio](https://github.com/realm/realm-studio/releases) and a bunch of
trial and errors

## How

### NixOS

```nix
inputs = {
    realm-cli = {
      inputs.nixpkgs.follows = "nixpkgs";
      url = "github:anninzy/realm-cli";
    };
};
```

```nix
home-manager.users.<user> = { lib, pkgs, ... }: {
    home.activation.setupOsu = lib.hm.dag.entryAfter [ "writeBoundary" ] "${inputs.realm-cli.packages.${pkgs.stdenv.hostPlatform.system}.default}/bin/realm-cli";
}
```

### Development

```bash
nix build
```

## What

- [ ] Actually make it a CLI that accepts arguments instead of hardcoding
      everything
- [ ] Maybe a not so destructive mode using
      `rulesetSettings.where([](auto const& setting) { return setting.RulesetName == "osu" && setting.Key == "HitAnimations"; })[0].Value = "False";`
- [ ] Find out if I can generate UUID instead of hardcoding it (or maybe just
      set it to some random string e.g. "1", "2", etc.)

## Why

Procrastination for impending deadlines

## Wragabagledingdong

### Build

- https://github.com/realm/realm-cpp/tree/community
- https://wiki.nixos.org/wiki/C
- https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html
- https://www.mongodb.com/docs/atlas/device-sdks/sdk/cpp/install/#std-label-cpp-install
- maybe theres a better way to package `realm-cpp` than my llm generated
  `realm-cpp.patch`

### main.cpp

- https://github.com/realm/realm-cpp/blob/community/tests/db/uuid_tests.cpp
- https://www.mongodb.com/docs/atlas/device-sdks/sdk/cpp/quick-start
- https://github.com/realm/realm-cpp/tree/main/examples
- https://www.mongodb.com/docs/atlas/device-sdks/sdk/cpp
