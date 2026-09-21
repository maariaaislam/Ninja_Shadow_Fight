# Ninja Fight

**Ninja Fight** is a multi-level 2D action game developed in C++ using the iGraphics framework, GLUT/OpenGL rendering, and Win32 multimedia support. The project is designed as a set of cooperating systems coordinated by a central state machine. It seamlessly combines side-scrolling platforming mechanics, horizontal arena fighting logic, and top-down exploration/combat mechanics into a single event-driven architecture.

## Table of Contents

* [Project Overview](#project-overview)
* [Game and Code Progression Cycle](#game-and-code-progression-cycle)
* [System Architecture & State Management](#system-architecture--state-management)
* [Input Pipeline Logic](#input-pipeline-logic)
* [Movement and Physics Mechanics](#movement-and-physics-mechanics)
* [Combat and Collision Mechanics](#combat-and-collision-mechanics)
* [AI Decision Logic](#ai-decision-logic)
* [Controls](#controls)
* [Project Structure](#project-structure)
* [Build and Run](#build-and-run)
* [Configuration and Save Data](#configuration-and-save-data)
* [Team & Contributions](#team--contributions)

## Project Overview

Ninja Fight operates on an event-driven callback loop executing at a fixed interval of roughly **30 milliseconds** (`updateTimer`) in a **1024 x 600** window. To optimize performance and maintain deterministic gameplay mechanics, the game creates long-lived system objects once at startup, resetting only the necessary logical components when a level begins rather than repeatedly allocating and destroying memory.

The game features three distinct game mechanics and logic models:

* **Level 1 (Side-scroller):** Platform mechanics featuring velocity-based physics, jump buffering, coyote time, traps, collectibles, shadow enemies, and dynamic camera tracking logic.
* **Level 2 (Arena Fighter):** A one-on-one fighting mechanic with jump physics, timed attack releases, dodge/block mechanics, power modes, healing, and a deterministic timer-driven AI logic that scales across three phases.
* **Level 3 (Exploration & Wave Combat):** A ten-map top-down exploration graph utilizing foot-based, single-pixel substep collision mechanics. Solving three map puzzles triggers the logic to open a portal to a four-directional, multi-wave arena battle.

## Game and Code Progression Cycle

The core of Ninja Fight relies on a strict, unidirectional progression cycle for both gameplay mechanics and code execution. 

### Gameplay Progression Cycle
1. **Active Play:** The player navigates the level mechanics, overcoming AI logic and environmental hazards.
2. **Completion Trigger:** Reaching an exit (Level 1), depleting phase HP (Level 2), or clearing all waves (Level 3) triggers the end-stage logic.
3. **Result Calculation:** The game mechanics compile the final score based on base points, remaining HP, unused heals, and clear-time bonuses.
4. **Data Handoff:** The result is submitted to `Scoreboard.h`, which handles the file I/O logic to write to `level_progress.txt`.
5. **UI Synchronization:** `iMain.cpp` commands the Level Select UI to read the new data, updating the visual rendering logic for locks and earned stars.

### Code Execution Progression Cycle (Safe Feature-Addition)
When adding new mechanics or logic to the codebase, the framework follows this strict lifecycle:
`Define Data/State` ➔ `Load Assets Once` ➔ `Reset Deterministic Values` ➔ `Consume Input Logic` ➔ `Update Simulation Mechanics` ➔ `Draw from State` ➔ `Submit Result` ➔ `Sync Presentation`.

## System Architecture & State Management

The entire game logic operates as a top-level finite-state machine coordinated by `iMain.cpp`. This ensures that only one major system's mechanics are evaluated during the update loop.

### Game State Logic (`gameState`)

| State | Purpose & Exits | 
| ----- | ----- | 
| `STATE_LOADING (-1)` | Displays the loading screen. Exits to Home when the timer maximizes. | 
| `STATE_HOME (0)` | Main menu with animated staged buttons. | 
| `STATE_LEVEL_SELECT (1)` | Displays unlocked levels, stars, and hover text logic. | 
| `STATE_LEVEL1 (2)` | Runs the side-scrolling platformer mechanics. Submits result on exit overlap. | 
| `STATE_SCOREBOARD (3)` | Reads `level_progress.txt` to display best scores. | 
| `STATE_SETTINGS (4)` | Manages audio, remappable controls, and settings logic. | 
| `STATE_LEVEL2 (5)` | Runs the three-phase 1v1 arena fighter mechanics. | 
| `STATE_LEVEL3 (6)` | Runs the top-down exploration and 4-way arena combat logic. | 

## Input Pipeline Logic

To handle both smooth movement and precise combat mechanics, `InputState` strictly separates raw keyboard events to prevent logic bugs:

1. **Held Inputs:** Used for continuous mechanics like movement and blocking. These remain true every frame the key is physically pressed down, allowing for smooth, continuous physics logic.
2. **Pressed-Edge Inputs:** Used for jumping, dashing, healing, and attacking mechanics. These create a one-frame logic flag on the transition from key-up to key-down, which is cleared at the end of the active level's update. This guarantees one physical button press equals exactly one mechanical action.

## Movement and Physics Mechanics

Because the game features three distinct genres, it employs three separate mathematical movement mechanics.

### Level 1: Side-Scrolling Platformer
* **Physics & Camera Logic:** Movement relies on velocity-based physics. The physics engine operates in "world coordinates," while the rendering logic subtracts the camera's X position to translate this to the screen.
* **Collision Mechanics:** Vertical collision uses discrete `PlatformLine` objects. The logic checks if the player's previous and new Y positions cross a platform's height while falling; if so, Y velocity is zeroed.
* **Responsiveness Mechanics:** Implements **jump buffering** (queueing a jump input) and **coyote time** (allowing a jump shortly after walking off a ledge).

### Level 2: Horizontal Arena Fighter
* **1D Combat Plane Logic:** The physics mechanics are restricted to a horizontal plane with Y reserved exclusively for jumps and knockbacks.
* **Friction Mechanics:** Ground movement applies constant acceleration, but releasing the input immediately triggers a logic block that applies a heavy friction multiplier ($0.62$) to prevent sliding.

### Level 3: Top-Down Exploration & Combat
* **Diagonal Normalization Logic:** To prevent moving diagonally from breaking speed mechanics, diagonal inputs multiply both axes by $0.7071$.
* **Anti-Tunneling Substep Mechanics:** Movement vectors are broken down into substeps no larger than **one pixel** to ensure perfect collision logic.
* **Foot-Based Collision Logic:** A five-point "foot footprint" is tested against walk-zone and block-zone rectangles; the simulation stops the player at the exact pixel before an invalid region.

## Combat and Collision Mechanics

Visual rendering logic is deliberately decoupled from physical collision mechanics to prevent erratic damage application.

* **Hit Tick Logic:** An attack animation does not immediately deal damage. The state machine logic waits for a specific `stateTimer` tick that perfectly aligns with the visible impact frame.
* **One-Hit Lock Mechanics:** Once an attack registers its hit tick, an `attackHitDone` boolean flag is set to ensure the same attack state cannot trigger a second hit logic loop.
* **Directional Melee Corridors:** Level 3 attack mechanics project a rectangular directional corridor. The logic scans all living enemies within this corridor and applies damage only to the nearest target.
* **Object Pooling Mechanics:** Projectiles and effects are managed via fixed-size arrays, avoiding expensive runtime memory allocation logic.

## AI Decision Logic

Enemy mechanics are driven by timer-based, deterministic decision loops rather than complex behavioral trees.

* **Distance Evaluation Logic:** If the player is outside an attack threshold, the enemy mechanics dictate a chase using a normalized directional vector. Once inside, the AI checks its cooldown timers to select an action.
* **Phase Scaling Mechanics (Level 2):** The logic engine dynamically alters the villain's maximum HP (320 -> 760), base movement speed, damage reduction multiplier, and move-selection probability.
* **Entity Separation Logic (Level 3):** To prevent multiple AI enemies from stacking, the controller logic evaluates the center distance between every active enemy pair. If closer than **70 pixels**, the mechanics push them apart symmetrically.

## Controls

Controls can be remapped via the Settings logic.

| Action | Level 1 (Platformer) | Level 2 & 3 (Combat) | 
| ----- | ----- | ----- | 
| **Move** | A/D or Arrows | A/D/W/S or Arrows | 
| **Jump** | W / Up / Space | W / Up / Space (L2 only) | 
| **Melee / Punch** | \- | J / Left Mouse | 
| **Kick** | \- | K / Right Mouse | 
| **Heavy Attack** | \- | F | 
| **Throw Shuriken** | J, K, L, I or 1-4 | L / Middle Mouse (L3) / J (L2) | 
| **Dash** | E | E | 
| **Block / Defense** | S (Crouch/Fast Fall) | Hold Q | 
| **Power Mode** | \- | P | 
| **Heal** | \- | H | 

## Project Structure

* **`iMain.cpp`**: Runtime entry point, input normalization logic, and global FSM manager.
* **`Gameplay.h`**: Shared physics mechanics, `NinjaPlayer` state machine, `Camera2D`.
* **`Level*.h`**: Distinct gameplay mechanics, stage logic, and AI controllers.
* **`Scoreboard.h` / `Settings.h`**: Persistence logic models for text file I/O.
* **`New.h` / `Screens.h`**: UI presentation and rendering logic.
* **`Images/`**: 305 image files sorted by gameplay role, using a load-once, select-by-ID rendering logic.

## Build and Run

1. Open `Ninja_Fight.sln` in Visual Studio (Targets Win32 / x86).
2. The project relies on legacy Win32 graphics (OpenGL, GLU, GLUT, GLAUX). Ensure `GLUT32.DLL` is available.
3. Ensure your Platform Toolset is compatible (retarget from `v120` if necessary).
4. **CRITICAL:** Set the Visual Studio Debugging Working Directory to `$(ProjectDir)` so the rendering logic can resolve relative paths.
5. **Audio Mechanics:** Add supported tracks to the `Music/` folder and configure `MUSIC_*_PATH` macros in `Music.h`.

## Configuration and Save Data

The game uses two plain text files to store logic and progress:

* **`game_settings.txt`**: Saves audio toggles, master volume, and remapped key codes.
* **`level_progress.txt`**: Saves best scores, stars, gold collected, and level unlock status.

## Team & Contributions

This project was built collaboratively, with team members owning specific aspects of the game mechanics, logic implementation, and rendering.

* **Maria Islam (ID: 00725105101086)** 
  * *Contributions:* Hero character rendering (Level-2 and Level-3), movement rendering, and fighting execution mechanics through score count logic.

* **[Teammate 2 Name] (ID: [Teammate 2 ID])**
  * *Contributions:* [To be filled]

* **[Teammate 3 Name] (ID: [Teammate 3 ID])**
  * *Contributions:* [To be filled]

* **[Teammate 4 Name] (ID: [Teammate 4 ID])**
  * *Contributions:* [To be filled]
