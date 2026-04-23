# GUI Implementation Plan (Technical)

## Tech Stack
- Language: C++17
- GUI framework: SFML 3.x
- Build: CMake, GUI enabled via `-DNIMONSPOLY_ENABLE_SFML=ON`
- Assets: PNG for board/tiles/tokens (optional), TTF fonts for UI text

## Architecture (UI Layer Only)
- GUI must not contain game logic.
- UI receives read-only state via `GameStateView` and DTOs.
- Interfaces:
  - `include/ui/IGameView.hpp`
  - `include/ui/IGameInput.hpp`
- Implementations:
  - `include/ui/GUIView.hpp` + `src/views/GUIView.cpp`
  - `include/ui/GUIInput.hpp` + `src/views/GUIInput.cpp`

## Rendering Pipeline
- Main loop (SFML only):
  1) Poll events (`window.pollEvent()`)
  2) Update GUI input state (mouse, click, key)
  3) Render board, tokens, overlays, prompts
- Fixed framerate cap (e.g., 60 FPS) or vsync.
- `GUIView::showBoard()` draws from the latest cached `GameStateView`.

## Coordinate System & Layout
- Board: 40 tiles, indices 0..39 (corners at 0,10,20,30).
- Convert tile index -> position by side (top/right/bottom/left).
- Layout regions:
  - Board frame
  - Center area (free space + HUD)
  - Side panels (log, prompt, player info)

## Asset Management
- Default: procedural shapes (rectangles, circles) if no asset.
- Optional assets:
  - Board background PNG
  - Tile card PNGs
  - Token sprites
- Provide a small loader (texture cache keyed by string path).

## Input Handling
- GUIInput converts UI interactions into `IGameInput` responses.
- Use a minimal UI state machine:
  - Idle
  - Menu selection
  - Yes/No prompt
  - Numeric input
  - Card selection
- Store pending prompt type and resolve on click/keypress.

## UI Elements (MVP)
- Board tiles + token positions
- Active player indicator
- Turn info (turn, max turn)
- Transaction log (rolling list of last N actions)
- Prompt overlay (buy, tax, auction, card choice)

## Data Flow
- Game logic produces `GameStateView` + UI DTOs.
- GUI stores last state; render uses cached state each frame.
- GUIInput returns selections to controller when requested.

## Build/Run
- Configure + build GUI:
  - `cmake -S . -B build-gui -DNIMONSPOLY_ENABLE_SFML=ON`
  - `cmake --build build-gui`
  - `./bin/game`

## Implementation Milestones
1) Basic render: board grid + tokens (no assets).
2) HUD: turn info + player list.
3) Prompt overlay: yes/no, menu selection.
4) Log panel + card display.
5) Asset integration (optional).
