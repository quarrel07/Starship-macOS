# Starship — macOS build

A macOS-focused fork of [Starship](https://github.com/HarbourMasters/Starship) (the Star Fox 64
port built on [libultraship](https://github.com/Kenix3/libultraship)) that makes the project **build
and run cleanly on modern macOS** and packages it as a proper, self-contained **`Starship.app`**. All
changes are macOS-only and do not affect the Windows, Linux, or Switch builds.

> **This repository and its releases contain no game assets.** A legally-obtained copy of a supported
> Star Fox 64 ROM (US 1.0 or US 1.1) is required to build/run, exactly as with upstream. The app
> prompts you for the ROM on first launch and extracts the assets locally.

This fork tracks upstream **v2.0.0 ("Barnard Alfa")**.

## What this fork adds

### 🛠️ Builds on modern macOS (Apple clang 21)
Upstream fails to compile with current Xcode/Apple clang because the `Torch` asset tool fetches an
old `spdlog` whose bundled `fmt` forces compile-time format-string checks via `consteval`, which
newer Clang rejects as non-constant expressions. This fork neutralizes that check, so a plain
`cmake --build` succeeds out of the box.

### 🎛️ No-fuss SDL2 discovery
A stale or partial `/Library/Frameworks/SDL2.framework` (left behind by some installers) shadows a
working Homebrew/MacPorts SDL2 and breaks `find_package(SDL2)`. libultraship's macOS dependency
script now searches frameworks **last** and adds the Homebrew prefix to the search path, so the build
finds the right SDL2 with no manual `-DSDL2_DIR` flag.

### 📦 A real, self-contained `Starship.app`
The normal build now produces `Starship.app` directly (upstream only made one via `cpack`). The
bundle is fully self-contained and ready to move to `/Applications`:
- runtime resources packed into `Contents/Resources` (`starship.o2r`, `config.yml`, the asset
  definitions used by the first-run ROM extractor, and the controller DB);
- all Homebrew dylibs relinked into `Contents/Frameworks` (`@executable_path/../Frameworks/`), so it
  runs on Macs without Homebrew installed;
- a macOS-shaped app icon (rounded squircle with proper padding), and an ad-hoc code signature.

`starship.o2r` (the packed port assets) is generated as a build dependency, so a plain build yields a
complete app — no separate manual packaging step.

### 🗂️ Tidy data folder
The app keeps its writable data — config, save files, logs, and `mods/` — in
**`~/Library/Application Support/com.starship`**, and extracts `sf64.o2r` there from your ROM on
first run. (Set via the bundle's `LSEnvironment SHIP_HOME` plus a matching fallback in code, so it
also works when launched from a terminal.)

## Releases & prebuilt build
Like upstream, **no build here contains game assets** — you supply your own ROM at run time.

- **Prebuilt binary:** download `Starship-v2.0.0-macOS-arm64.zip` from the
  [Releases page](https://github.com/quarrel07/Starship/releases), unzip, and move `Starship.app` to
  your Applications folder. Apple Silicon only. The build is ad-hoc signed, so the first time you open
  it, right-click the app → **Open** to get past Gatekeeper. On first launch it asks for your Star
  Fox 64 ROM and extracts the assets into `~/Library/Application Support/com.starship`.
- **Versioning:** releases track upstream — `vX.Y.Z-macos` is the macOS build of upstream `vX.Y.Z`.
  This release (`v2.0.0-macos`) is built from the latest upstream sources, so it may be slightly
  ahead of upstream's own prebuilt binaries.

## Building from source
Prerequisites (Homebrew): `cmake ninja sdl2 libpng glew libzip nlohmann-json tinyxml2 spdlog
vorbis-tools`, plus the Xcode command-line tools. Then:

```bash
git clone --recurse-submodules https://github.com/quarrel07/Starship.git
cd Starship
cmake -H. -Bbuild-cmake -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake            # -> build-cmake/Starship.app
```

Run `build-cmake/Starship.app` and pick your ROM when prompted. (See `docs/BUILDING.md` for the full
upstream instructions, including `ExtractAssets`/`GeneratePortO2R` targets.)

To build a thinner, machine-local app that links against your Homebrew dylibs instead of bundling
copies, configure with `-DSTARSHIP_BUNDLE_DEPS=OFF`.

## Dependency forks
The two build fixes live in submodules, so this fork repoints them at forks that carry the changes.
A recursive clone pulls everything needed:

| Submodule | Fork (branch) | What changed | Upstream |
|---|---|---|---|
| `tools/Torch` | `quarrel07/Torch` (`macos-clang-fmt-fix`) | Neutralize bundled-`fmt` `consteval` so it builds on Clang 21 | [HarbourMasters/Torch](https://github.com/HarbourMasters/Torch) |
| `libultraship` | `quarrel07/libultraship` (`macos-sdl2-framework-fix`) | Prefer package-manager SDL2 over a stale system framework | [Kenix3/libultraship](https://github.com/Kenix3/libultraship) |

The `.app` packaging and the `com.starship` data folder live in this repository (see
`cmake/macos/apple_bundle.cmake`, `Info.plist`, and `src/port/Game.cpp`).

## Relationship to upstream & credits
This is a thin, macOS-only fork — all gameplay, the port itself, and the engine come from the
upstream projects. The intent is to contribute these fixes back upstream.

- **Starship** (Star Fox 64 port) — [HarbourMasters](https://github.com/HarbourMasters/Starship),
  lead developers **SonicDcer** and **Lywx**.
- **libultraship** — [Kenix3](https://github.com/Kenix3/libultraship) and contributors.
- **Torch** (asset extraction tool) — [HarbourMasters](https://github.com/HarbourMasters/Torch).
- macOS build/packaging fork by **[@quarrel07](https://github.com/quarrel07)**.

Star Fox 64 is © Nintendo. This project is not affiliated with or endorsed by Nintendo, and ships no
copyrighted assets.
