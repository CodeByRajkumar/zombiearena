# 🧟 Zombie Arena Game

A simple top-down **Zombie Survival Arena** game built using **C++** and **SFML (Simple and Fast Multimedia Library)**. The objective is to survive waves of zombies for as long as possible.

---

## 🚀 Features

- Smooth 2D top-down shooter mechanics
- Real-time zombie horde spawning
- Bullet firing and collision detection
- Health and ammo pickups
- Multiple zombie types (Bloater, Chaser, Crawler)
- Randomized zombie spawn positions within the arena
- Sound effects and visual effects (blood, hit, reload, etc.)

---

## 🧩 Code Structure

| File              | Description |
|-------------------|-------------|
| `zombiearena.cpp` | Main game loop, handles SFML window, input, game state, updates, and rendering. |
| `player.cpp`      | Defines the `Player` class and its movement, health, and update logic. |
| `zombie.cpp`      | Defines the `Zombie` class with movement AI and types. |
| `bullet.cpp`      | Defines bullet behavior, including movement and collisions. |
| `zombiearena.cpp` | Contains utility functions for creating the background and spawning the zombie horde. |

---

## 🔧 Important Functions

### `int createBackground(VertexArray& rVA, IntRect arena);`

- **Purpose:** Dynamically creates the background tile map based on the arena size.
- **Parameters:**  
  - `VertexArray& rVA`: Reference to the SFML vertex array for background tiles  
  - `IntRect arena`: Rectangle defining the bounds of the arena  
- **Returns:** Tile size used for the grid.

---

### `Zombie* createHorde(int numZombies, IntRect arena);`

- **Purpose:** Spawns a dynamically allocated array of `Zombie` objects within the arena.
- **Parameters:**  
  - `int numZombies`: Number of zombies to spawn  
  - `IntRect arena`: Defines the spawning boundaries  
- **Returns:** Pointer to the first zombie in the dynamically allocated array.

---

## 🛠️ Requirements

- **SFML** (version 2.5 or later recommended)  
  Download: [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)

- **C++17** or later
- A C++ IDE like VSCode or Visual Studio
- Proper linking of SFML libraries (graphics, window, audio, system)

---

## 🖼️ Assets

The `graphics/`, `sound/`, and `fonts/` directories contain all required textures, sounds, and fonts for the game to run smoothly.

---

## 🧪 How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/CodeByRajkumar/zombiearena.git
