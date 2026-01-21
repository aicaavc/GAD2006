## Advanced Game Development – Labworks

This section summarizes the labwork projects developed for the **Advanced Game Development** course. Each task focuses on core gameplay programming concepts in **Unreal Engine (C++)**, including movement systems, command pattern applications, grid-based world interaction, and multiplayer synchronization.

---

###  Labwork 1 – Stamina System

A stamina system was implemented in the `ACOAAvatar` class, tracking and controlling player's sprinting ability.

#### Properties:
- `Stamina`, `MaxStamina`
- `StaminaDrainRate`, `StaminaGainRate`
- `RunSpeed`, `bRunning`, `bStaminaDrained`

#### Core Logic:
- Player starts at `MaxStamina` (100).
- Stamina drains while walking/running.
- Stamina regenerates when idle.
- When stamina hits 0, player enters a `StaminaDrained` state and can no longer run until fully recovered.

#### Methods:
- `UpdateMovementParams()` — adjusts movement speed based on stamina status.
- `RunPressed()`, `RunReleased()` — modified to consider `bStaminaDrained`.
- `Tick()` — stamina logic handled per frame.

> Walking state is checked using:  
> `GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking`

---

###  Labwork 2 – Undo Feature (Command Pattern)

This lab implements an **Undo/Redo** mechanic for tile-based movement using the Command Pattern.

#### Key Classes:
- `MoveCommand`:
  - `void Revert()` — reverses previous move logic.
- `GameManager`:
  - `bool UndoLastMove()` — pops last move from `CommandPool` and reverts it.
  - Returns `false` if no previous commands exist.

#### Interaction:
- `Backspace` key bound in **Level Blueprint** to trigger `UndoLastMove()`.
- `PrintString()` used to display success/failure on screen.

---

###  Labwork 3 – Tile Placement System

Developed a **tile-based placement system** using custom Actors and PlayerController logic.

#### Classes:
- `ATileBase` — base tile blueprint (`UInstancedStaticMeshComponent`).
- `ATileGameManager` — handles map grid and tile spawning.
- `ATilePlayerController` — traces clicks, communicates with GameManager.

#### Map System:
- 2D C++ array represents the tile grid.
- `MapExtendsInGrids` = 25 (default grid range ±X/Y).
- `CurrentTileIndex` tracks selected tile in `TileTypes`.

#### Interactions:
- Scroll Wheel — cycles through 8+ tile types.
- Right Click — rotates selected tile (Yaw +90°).
- A `UStaticMeshComponent` highlights current selection.

---

###  Labwork 4 – Multiplayer Running State & Customization

Expanded gameplay with **networked movement** and **cosmetic customization**.

#### Features:
- 7 cosmetic slots: `Face`, `Hair`, `Beard`, `Eyebrows`, `Chest`, `Legs`, `Hands`.
- `Run` functionality (Hold **Shift**) replicated using `ReplicatedUsing`.

#### Networking:
- `bIsRunning` flag replicated across all clients.
- Movement speed and animation states synchronized for all players.

---

> Each labwork was developed and tested using custom C++ classes within Unreal Engine's actor/component system.
