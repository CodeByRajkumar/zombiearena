# 🧟 Zombie Arena Game

A simple 2D top-down zombie survival game built with **C++** and **SFML**. Survive waves of zombies in an arena using bullets, pickups, and strategic movement.

---

## Features

- Multiple zombie types and hordes  
- Player shooting and health management  
- Dynamic background and arena  
- Sound effects and animations

---

## Files

- `zombiearena.cpp` — Main game logic and utility functions  
- `player.cpp` — Player class and controls  
- `zombie.cpp` — Zombie AI and behavior  
- `bullet.cpp` — Bullet movement and collision

---

## Requirements

- SFML 2.5 or newer  
- C++17 or higher  
- SFML libraries linked during compilation

---

## How to Run

```bash
git clone https://github.com/CodeByRajkumar/zombiearena.git
cd zombiearena
g++ zombiearena.cpp -o zombiearena -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio
