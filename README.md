# Mini Game Hub
This is a console-based Mini Game Hub project in C++. 
It includes four interactive games: Snake, Tic Tac Toe, Minesweeper, and Zombie Escape Maze. 
You can play each game via a menu-driven interface.

## How to Run

1. **Clone the repository:**
git clone https://github.com/your-username/your-repo.git
cd your-repo

2. **Compile all C++ files:**
g++ main.cpp Snake.cpp TicTacToe.cpp Minesweeper.cpp Zombie.cpp -o MiniGames

3. **Run the executable:**
./MiniGames   # Linux/macOS
MiniGames.exe # Windows

## Menu System

When you run the program, you’ll see a menu:

===== Mini Game Hub =====
1. Snake
2. Tic Tac Toe
3. Minesweeper
4. Zombie Escape Maze
5. Exit

Enter the number of the game you want to play.

## 🐍 1. Snake

**Objective:** Eat fruits to grow your snake and increase your score. Avoid hitting walls or yourself.

**Controls:**

* Arrow Keys: Move the snake (Up, Down, Left, Right)
* P: Pause / Unpause
* T: Toggle AI Mode (let the computer play)
* R: Restart game
* X: Exit game

**Gameplay Tips:**

* Eating a fruit adds 10 points to your score.
* The snake grows longer as it eats.
* Game ends if the snake hits a wall or itself.
* AI Mode will move the snake automatically using a simple greedy algorithm.


## ❌ 2. Tic Tac Toe

**Objective:** Align 3 of your marks (`X` or `O`) in a row, column, or diagonal.

**Controls:**

* Enter position (1-9) to place your mark.
* Positions are numbered as:

1 | 2 | 3
---------
4 | 5 | 6
---------
**7 | 8 | 9**

**Gameplay Tips:**

* Player `X` always starts.
* The AI uses minimax with memoization (dynamic programming) to make optimal moves.
* The game can end in a win or a draw.

## 💣 3. Minesweeper

**Objective:** Reveal all safe cells without hitting a mine.

**Controls:**

* Input row col action:

  * `r` = Reveal a cell
  * `f` = Flag a suspected mine

**Gameplay Tips:**

* Board size: 9×9
* Mines: 10 hidden mines randomly placed
* Revealing a cell with `0` adjacent mines automatically opens neighboring cells.
* Flag all suspected mines to help track dangerous spots.
* Game ends if you reveal a mine (lose) or reveal all safe cells (win).

## 🧟 4. Zombie Escape Maze

**Objective:** Reach the exit (`E`) while avoiding zombies (`Z`).

**Controls:**

* W: Move up
* S: Move down
* A: Move left
* D: Move right
* Q: Quit game

**Gameplay Tips:**

* Zombies move toward the player using a greedy pathfinding algorithm.
* A guaranteed path from player to exit ensures the game is solvable.
* If a zombie touches the player, the game ends (Game Over).
* Reach the exit safely to win.

## Features

* Menu-driven interface for easy navigation
* AI modes in Snake and Tic Tac Toe
* Dynamic gameplay with random elements (fruits, mines, walls, zombie positions)
* High score saving for Snake
* Cross-game keyboard controls for a smooth console experience

---

## Notes

* Only works on Windows due to `conio.h` and `windows.h`.
* Compilation requires a C++ compiler supporting C++11 or above.
* You can add new games by:

  1. Creating a new `.cpp` file
  2. Adding its prototype in `main.cpp`
  3. Adding a menu option to call the function
