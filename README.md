# Pâte Graphique: Thermal Run

A lightweight 2D platformer prototype in C++ using SDL2, a tiny bundled JSON parser, and optional Discord Social/Game SDK hooks. The prototype showcases local multiplayer synchronized through Discord lobby metadata, collectible pâte coins, powerups, enemies, and a boss fight.

## Features
- Basic platformer physics: running, jump + double jump, gravity, friction
- Tiles with collision, moving enemies, coins, and powerups
- Boss prototype "The Overclocker" that spawns overheating zones
- JSON level loading (`assets/levels/demo.json`)
- UI: simple menu, HUD with health bars
- Rebindable keyboard input (F1 rebinds jump as an example)
- Discord Social/Game SDK integration stubs for lobby creation, presence, and user metadata
- CMake build targeting Windows and Linux

## Building
```bash
cmake -S . -B build
cmake --build build
```

The build expects SDL2 and SDL2_image to be installed. On Windows, point CMake to your SDL2 installations or use vcpkg. On Linux, install `libsdl2-dev` and `libsdl2-image-dev` packages.

### Discord SDK
Set `-DBUILD_WITH_DISCORD=ON`, `-DDISCORD_SDK_INCLUDE_DIR` and `-DDISCORD_SDK_LIB` to your SDK paths to link against the official Game SDK. The included `DiscordManager` implements a stub so the game will still compile without the SDK.

## Running
```bash
./build/PateGraphique
```

Press **Enter** on the menu to start. Use **A/D** to move, **Space** to jump (double jump supported). Collect coins and powerups, avoid enemies, and confront the boss. Use **F1** to demo rebinding jump to **K**.

## Project Layout
```
src/
  core/          – Game loop and SDL bootstrap
  discord/       – Discord SDK facade
  entities/      – Player, enemies, boss
  levels/        – Level state and JSON loader
  input/         – Input mapping/rebinding
  ui/            – Menu and HUD widgets
assets/levels/   – Demo JSON level
third_party/     – Minimal bundled nlohmann::json-compatible header
```

## Multiplayer Notes
The Discord integration is host-authoritative and currently simulates lobby creation and membership locally. Replace the stub calls inside `DiscordManager` with actual `discord::Core` usage to sync presence and user avatars across clients.
