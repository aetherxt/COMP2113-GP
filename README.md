# COMP2113 Group Project——Group 111
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

---

## Team Members
* **YANG Chengxi** (UID: 3036667206)
* **Law Marcus** (UID:  )

---

## Game Description
**TUI GOLF** is a text-based, strategic golf landing zone simulator wrapped in an RPG progression system. Instead of a traditional arcade sports game, this project focuses on precision and tactical planning. 

Players start with base strength and accuracy. In each round, they select a course difficulty (Par 3, Par 4, or Par 5) which generates a completely randomized 2D text map. By inputting their desired **Strength (1-100)** and **Direction angle (-45° to 45°)**, the game utilizes trigonometric physics to calculate and visually render the exact "Landing Zone" on the map. Players must navigate around procedurally generated Sand Traps (`S`), Trees (`^`), and Roughs (`#`).

---

## Game Features & Code Requirements
Here is a list of our implemented features and how the **6 core coding requirements** seamlessly support them:

1. **Procedural Course Generation (Supported by *Generation of random events*)** 
   Every hole generates a unique layout. In `course.cpp`, we utilize `<cstdlib>` (`srand` and `rand()`) to procedurally generate the terrain. The fairway's direction drifts randomly, and hazards like sand traps (`S`) and trees (`^`) naturally spawn based on randomized placement logic, ensuring infinite replayability.

2. **RPG Progression & Shop System (Supported by *Data structures for storing data*)** 
   Players earn coins to permanently upgrade their golfer. We use `struct` to neatly package these states: `Player` Struct (in `player.h`) stores `coins`, `strength`, and `accuracy`. Similarly, the `Course` Struct (in `course.h`) packages the map data.

3. **Adaptive UI and Map Sizing (Supported by *Dynamic memory management*)** 
   The game grid is implemented using dynamic arrays via the C++ Standard Template Library (`std::vector`). When `generateCourse()` is called, the map dynamically resizes itself (`c.map.resize()`) based on the required dimensions for different Pars, managing heap memory automatically and preventing memory leaks.

4. **Persistent Save Data (Supported by *File input/output*)**
   Your progression is never lost. In `player.cpp`, we use `<fstream>` (`ifstream` and `ofstream`) to interact with a local file named `player.txt`. It saves the player's stats seamlessly and restores them the next time the game boots up.

5. **Modular Architecture (Supported by *Program codes in multiple files*)** 
   The project is strictly modularized for better maintainability. Operations are divided into specialized files: `menu.cpp` (UI logic), `game.cpp` (physics & rendering), `course.cpp` (generation algorithms), and `player.cpp` (I/O).

6. **Strategic Depth (Supported by *Multiple Difficulty Levels*)** 
   Our game incorporates a built-in difficulty selector tied to the theme of Golf. Before every round, the player chooses:
   * **Par 3:** The shortest and easiest course.
   * **Par 4:** Medium length and balanced difficulty.
   * **Par 5:** The longest and most challenging course, requiring multiple highly calculated shots to reach the green.

---

## Non-Standard Libraries
**No non-standard 3rd-party libraries were used in this project.** The game is built entirely using standard C/C++11 libraries to ensure maximum compatibility across all operating systems.

However, our core "Landing Predictor" feature heavily relies on the standard `<cmath>` library. We utilized trigonometric functions (`sin()`, `cos()`, `round()`) alongside `M_PI` in `game.cpp` to accurately convert the player's angular direction and strength input into 2D Cartesian coordinates (X, Y), allowing us to precisely plot the ball's landing zone on the terminal grid.

---

## Compilation and Execution (Quick Start)
A `Makefile` is provided in the root directory for standard compilation.

**1. Compile the Game**
Open your terminal in the project directory and run the following command:
```bash
make
```

**2. Start the Game**
Once the compilation is successful, start the golf simulator by running:
```bash
./game
```

**3. Clean up**
When you are finished playing, you can easily remove all compiled object files (.o) and the executable binary to keep your directory clean by running:
```bash
make clean
```
