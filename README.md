# COMP2113 Group Project —— Group 111
## TUI Golf: Terminal Golf Simulator

## Team Members

- **YANG Chengxi** (UID: 3036667206)
- **Law Marcus** (UID: 3036516940)

---

## Game Demo

Game demo video:
https://drive.google.com/file/d/1pv4mCR4ja-QFqwFt3COMYE0vK-Aku0kr/view?usp=sharing

## Game Description

**TUI GOLF** is a hardcore, terminal-based golf simulation game. Set entirely within the console, players experience procedurally generated golf courses with highly variable terrains. The game simulates a physics-based landing zone where players must strategically adjust their shot strength and direction, taking into account terrain penalties such as roughs, trees, sand traps, and water hazards.

By challenging different holes (Par 3, Par 4, Par 5), players can earn coins based on their performance (e.g., scoring an Eagle or a Birdie). These coins can then be spent in the Shop to permanently upgrade the player's attributes (Strength and Accuracy), creating an engaging progression system.

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
3. (0) To return to the main menu.

---

## Game Rules

1. **Goal:** Sink the ball (O) into the hole (H) using the fewest possible strokes.

2. **Terrain Penalties:** Different terrains strictly limit your maximum shot strength:

**Fairway/Green (.):** Max Strength 100

**Rough (#):** Max Strength 80

**Tree (^):** Max Strength 60

**Sand (S):** Max Strength 30

**Water (~):** Sinking into water incurs a 2-stroke penalty.

**Out of Bounds (OOB):** Hitting the ball out of the map incurs a 1-stroke penalty, and the ball is reset to the closest valid terrain.

3. Economy: Finishing a hole below or at Par rewards you with coins equal to the stroke difference.

---

## Additional strategic elements:

- The landing zone increases in size with shot strength, making the probability that the shot lands in the fairway lower, creating a risk-reward dynamic.
- Sand traps (`S`) are placed around the green, and landing in them reduces the  strength you can use on your next shot.
- Water hazards (`~`) are placed on the inside of dog-legs; landing in water incurs a 2-stroke penalty.

---

## Game Features & Code Requirements Explanation

**1. Procedural Course Generation & Physics (Generation of random events)**

Every hole generates a unique layout. In `course.cpp`, we utilize `<cstdlib>` (`srand` and `rand()`) to procedurally generate the terrain. The fairway varies smoothly in width using a sinusoidal model, and includes occasional dog-legs. Furthermore, the game features stochastic ball landing: the `generateBallLanding()` function calculates an elliptical "Landing Zone" based on your strength and accuracy, and randomly drops the ball within that geometry to simulate real-life uncontrollable factors.

**2. Shop System & Map Data (Data structures for storing data)**

Players can earn coins to permanently increase their stats. We use `struct` to package these states: The `Player` struct stores `coins`, `strength`, and `accuracy`. Similarly, the `Course` struct encapsulates the map dimensions and uses a `std::set<std::pair<int, int>>` to store water hazards, allowing for fast, optimized penalty detection.

**3. Adaptive Map Sizing (Dynamic memory management)**

The game grid is implemented using dynamic arrays via the `<vector>` library in the STL. When `generateCourse()` is called, the 2D map dynamically resizes itself (`c.map.resize()`) based on the required dimensions for different Pars. This ensures memory is efficiently allocated and prevents memory leaks without needing manual `new` or `delete` management. 

**4. Persistent Player Stats (File input/output)**

In `player.cpp`, we use `<fstream>` (`ifstream` and `ofstream`) to interact with a local file named `player.txt`. It serializes and saves the player's attributes and coin balance when the game ends or a purchase is made, and loads them when the game starts up again to ensure no loss of progress.

**5. Modular Game Design (Program codes in multiple files)**

The project is decoupled into specialized files for high maintainability: `main.cpp` drives the logic, `menu.cpp` handles the TUI and shop, `game.cpp` manages the gameplay loop, `course.cpp` handles procedural generation and ANSI color rendering, `shot.cpp` calculates landing zone physics, and `player.cpp` manages file I/O.

**6. Game Difficulty (Multiple Difficulty Levels)**

Before every round, the player chooses the difficulty of the generated course by selecting a par value:
* **Par 3**: Shortest and easiest course.
* **Par 4**: Medium length and balanced difficulty.
* **Par 5**: The longest and most challenging course, requiring multiple calculated shots and avoiding numerous hazards to reach the green.

---

## Non-Standard Libraries

**No non-standard 3rd-party libraries were used in this project.**

---
