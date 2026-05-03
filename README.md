# COMP2113 Group Project

Group 111
![](https://img.shields.io/badge/Language-C%2B%2B11-blue.svg) ![](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-lightgrey.svg)

	  _______  _     _  ___      _______  _______  ___      _______ 
	 |       || |   | ||   |    |       ||       ||   |    |       |
	 |_     _|| |   | ||   |    |    ___||   _   ||   |    |    ___|
	   |   |  | |   | ||   |    |   | __ |  | |  ||   |    |   |___ 
	   |   |  | |___| ||   |    |   ||  ||  |_|  ||   |___ |    ___|
	   |   |  |       ||   |    |   |_| ||       ||       ||   |    
	   |___|  |_______||___|    |_______||_______||_______||___|    

> "Success in golf depends less on strength of body than upon strength of mind and character."
>
> — Arnold Palmer

## Team Members
* **YANG Chengxi** (UID: 3036667206)
* **Law Marcus** (UID: )

---

## Game Description
**TUI GOLF** is a text-based, strategic golf landing zone simulator wrapped in an RPG progression system. Instead of a traditional arcade sports game, this project focuses on precision and tactical planning. 

Players start with base strength and accuracy. In each round, they select a course difficulty (Par 3, Par 4, or Par 5) which generates a completely randomized 2D text map. By inputting their desired **Strength (1-100)** and **Direction angle (-45° to 45°)**, the game utilizes trigonometric physics to calculate and visually render the exact "Landing Zone" on the map. Players must navigate around procedurally generated Sand Traps (`S`), Trees (`^`), and Roughs (`#`).

---

## Game Features
1. **RPG Progression & Shop System:** Earn coins (planned) and spend them in the Main Menu shop to permanently upgrade your golfer's `Strength` (+10) and `Accuracy` (+2).
2. **Procedural Course Generation:** Every hole played generates a unique layout with randomized doglegs (fairway curves), sand traps, and tree lines.
3. **Trigonometric Landing Predictor:** The game calculates the exact landing ellipse based on the player's effective power and angular direction, rendering a visual red indicator (`/ \ | -`) on the terminal map.
4. **Persistent Save Data:** Your coins, strength, and accuracy are automatically saved to your local drive and loaded the next time you boot the game.

---

## Coding Requirements Explanation
Here is how our codebase fulfills the 6 core coding elements required by the course:

1. **Generation of random events**
The course map is never static. In `course.cpp`, we utilize `<cstdlib>` (`srand` and `rand()`) to procedurally generate the terrain. The fairway's direction drifts randomly, and hazards like sand traps (`S`) and trees (`^`) naturally spawn based on randomized placement logic.

2. **Data structures for storing data**
We use `struct` to neatly package game and player states:
* `Player` Struct (in `player.h`): Stores `coins`, `strength`, and `accuracy`.
* `Course` Struct (in `course.h`): Stores `par`, `width`, `height`, and the 2D map vector.

3. **Dynamic memory management**
The game grid is implemented using dynamic arrays via the C++ Standard Template Library (`std::vector`). When `generateCourse()` is called, the map dynamically resizes itself (`c.map.resize()`) based on the required dimensions, managing heap memory automatically and preventing memory leaks.

4. **File input/output (Loading/Saving data)**
Game states are persistent. In `player.cpp`, we use `<fstream>` (`ifstream` and `ofstream`) to interact with a local file named `player.txt`. It saves the player's progression and seamlessly restores it the next time the game boots up.

5. **Program codes in multiple files**
The project architecture is strictly modularized for better maintainability:
* `main.cpp`: Driver code.
* `menu.cpp / .h`: Terminal UI and Shop logic.
* `game.cpp / .h`: User input parsing, physics calculations, and rendering.
* `course.cpp / .h`: Terrain generation algorithms.
* `player.cpp / .h`: Data structures and File I/O.

6. **Multiple Difficulty Levels**
Our game incorporates a built-in difficulty selector tied seamlessly into the theme of Golf. Before every round, the player is prompted to select their preferred level of challenge:
* **Par 3:** The shortest and easiest course.
* **Par 4:** Medium length and balanced difficulty.
* **Par 5:** The longest and most challenging course, requiring multiple highly calculated shots to reach the green.
