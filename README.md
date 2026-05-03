# COMP2113 Group Project —— Group 111

## TUI Golf: Terminal Golf Simulator

## Team Members

- **YANG Chengxi** (UID: 3036667206)
- **Law Marcus** (UID: 3036516940)

---

## Game Description

**TUI GOLF** is a text-based, strategic golf simulator with an upgrade system. The player is encouraged to make strategic decisions on shot strength and direction while navigating randomly generated courses. The player is rewarded with coins based on their performance, which can be used to permanently upgrade their stats in a shop.

Players start with a base strength and accuracy. In each round, they select a course difficulty (Par 3, Par 4, or Par 5) which randomly generates a course based on the selected par. By inputting their desired **Strength (1-100)** and **Direction angle (-180° to 180°)**, the game renders the "Landing Zone" on the map. The player can then choose to readjust their shot or take it. When the player gets the ball on the green, they will complete the hole and be awarded with coins based on how many shots under the course par they achieved.Players must navigate around procedurally generated Sand Traps (`S`), Trees (`^`), Roughs (`#`), and Water Hazards (`~`).

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

---

## How To Play

1. Select (1) Play Game (2) Upgrade (0) Exit.

**Play Game**

1. Choose a course difficulty: (3) Par 3, (4) Par 4, (5) Par 5.
2. Input your desired shot strength (1-100) and direction angle (-180° to 180°).
3. View the landing zone on the map and decide to either (1) Readjust or (2) Take Shot.
4. Repeat until you get the ball on the green, get coins based on your performance

**Upgrade**

1. View your current stats and coins.
2. Choose to upgrade either (1) Strength or (2) Accuracy for 2 coins per upgrade.
3. (0) tTo return to the main menu.

## Game Features & Code Requirements

Here is a list of our implemented features and how the **6 core coding requirements** seamlessly support them:

1. **Procedural Course Generation (Generation of random events)**

   Every hole generates a unique layout. In `course.cpp`, we utilize `<cstdlib>` (`srand` and `rand()`) to procedurally generate the terrain. The fairway now varies smoothly in width between 5 and 12 characters wide using a sinusoidal model, and the centerline will include occasional dog-legs (bends in the course). Hazards such as sand traps (`S`) are placed randomly near the green, roughs (`#`) and trees (`^`) are placed along the edge of the fairway, and water hazards (`~`) are placed on the inside of dog-legs to increase challenge.

2. **Shop System and Course Generation (Data structures for storing data)**

   Players can earn coins to permanently increase their stats. We use `struct` to package these states: `Player` Struct (in `player.h`) stores `coins`, `strength`, and `accuracy`. Similarly, the `Course` Struct (in `course.h`) packages the map data.

3. **Adaptive Map Sizing (Dynamic memory management)**

   The game grid is implemented using dynamic arrays via the `<vector>` library. When `generateCourse()` is called, the map dynamically resizes itself (`c.map.resize()`) based on the required dimensions for different Pars. The TUI renders a colored map and a landing-zone preview overlay before each shot; terrain and hazards are color-coded (including water in blue) for clarity.

4. **Persistent Player Stats (File input/output)**

   In `player.cpp`, we use `<fstream>` (`ifstream` and `ofstream`) to interact with a local file named `player.txt`. It saves the player's stats when the game ends and loads them when the game starts up again.

5. **Modular Game Design (Program codes in multiple files)**

   The project is split across specialized files for maintainability: `menu.cpp` handles the menu, `game.cpp` and `shot.cpp` handle the main gameplay loop and landing-zone rendering, `course.cpp` generates the course, and `player.cpp` manages player data and saves.

6. **Game Difficulty (Multiple Difficulty Levels)**

   Before every round, the player chooses the difficulty of the generated course by selecting a par value:

- **Par 3:** Shortest and easiest course
- **Par 4:** Medium length and balanced difficulty.
- **Par 5:** The longest and most challenging course, requiring multiple calculated shots to reach the green.

Additional strategic elements:

- The landing zone increases in size with shot strength, making the probability that the shot lands in the fairway lower, creating a risk-reward dynamic.
- Sand traps (`S`) are placed around the green, and landing in them reduces the  strength you can use on your next shot.
- Water hazards (`~`) are placed on the inside of dog-legs; landing in water incurs a 2-stroke penalty.

---

## Non-Standard Libraries

**No non-standard 3rd-party libraries were used in this project.**

---
