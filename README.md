# **Sudoku Game**

This project was completed as part of the *Software Project* course during my undergraduate studies at **Tel Aviv University**.  
**Sudoku** is a number-placement puzzle where the objective is to fill a NxN grid (usually 9x9) with digits so that each column, each row, and each block contain all digits from **1 to N**. For each cell in the grid, its "neighbors" are the other cells in its row, column, and block. The main rule is that the value of each cell is **different** from the values of its neighbors. A Sudoku puzzle is successfully completed when all **N²** cells are filled with legal values.

This program uses **Gurobi Optimizer's** linear programming API.

In this specific implementation of the Sudoku game, there are three **Game Modes**:

- **Edit mode**
- **Solve mode**
-  **Init mode**

![Sudoku](https://user-images.githubusercontent.com/98098222/156612505-e4962cde-55bb-452a-80f9-4da9e26bae6c.png)

*Fixed cells* are displayed followed by a dot (**.**)  
*Erroneous values* are displayed followed by an asterisk (**\***).

---

# 📜 **API**

The game starts in **Init mode**. The following are the commands that the user can type:

- **`solve X`**  
  Starts a puzzle in **Solve mode**, loaded from a file with the name **"X"**, where X includes a full or relative path to the file.  
  This command is available in **Solve**, **Edit**, and **Init** modes. Any unsaved work on the current game board is lost.

- **`edit [X]`**  
  Starts a puzzle in **Edit mode**, loaded from a file with the name **"X"**, where X includes a full or relative path to the file.  
  The parameter **X** is optional. If no parameter is supplied, an empty **9x9 board** is generated.  
  This command is available in **Solve**, **Edit**, and **Init** modes. Any unsaved work on the current game board is lost.

- **`mark_errors X`**  
  Sets the "mark errors" setting to **X**, where **X** is either **0** or **1**.  
  When this parameter is set to **1**, erroneous values are displayed followed by an asterisk (**\***). The default value is **1**.  
  This command is available only in **Solve mode**.

- **`print_board`**  
  Prints the board. This command is available in **Edit** and **Solve** modes.

- **`set X Y Z`**  
  Sets the value of cell **<X,Y>** to **Z**.  
  This command is available in **Edit** and **Solve** modes.  
  The user may empty a cell by setting **Z=0**.  
  In **Solve mode**, fixed cells can't be updated.

- **`validate`**  
  Validates the current board using **ILP**, ensuring it is solvable.  
  This command is available in **Edit** and **Solve** modes.  
  If the board is erroneous, the program prints an error message, and the command is not executed.  
  The command prints whether the board is found to be **solvable** or not.

- **`guess X`**  
  Guesses a solution to the current board using **LP**, with threshold **X**.  
  This command is only available in **Solve mode**.  
  The parameter **X** is a float and represents the threshold used for the **LP solution**.  
  If the board is erroneous, the program prints an error message, and the command is not executed.  
  The command fills all cell values with a score of **X** or greater. If several values hold for the same cell, randomly one is chosen.

- **`generate X Y`**  
  Generates a puzzle by randomly filling **X** empty cells with legal values, running **ILP** to solve the board, and then clearing all but **Y** random cells.  
  This command is available only in **Edit mode**.  
  If the board does not contain **X** empty cells, then that is an error.

- **`undo`**  
  Undo a previous move made by the user.  
  This command is available in **Edit** and **Solve** modes.

- **`redo`**  
  Redo a move previously undone by the user.  
  This command is available in **Edit** and **Solve** modes.

- **`save X`**  
  Saves the current game board to the specified file, where **X** includes a full or relative path to the file.  
  This command is available in **Edit** and **Solve** modes.

- **`hint X Y`**  
  Gives a hint by showing the solution of a single cell **<X,Y>**.  
  This command is available in **Solve mode**.  
  If the board is erroneous, cell **<X,Y>** is fixed, or cell **<X,Y>** already contains a value, it is an error.  
  The program runs **ILP** to solve the board. If the board is unsolvable, it is an error. Otherwise, the value of cell **<X,Y>** found by the **ILP solution** is printed.

- **`guess_hint X Y`**  
  Shows a guess to the user for a single cell **<X,Y>**.  
  This command is available in **Solve mode**.  
  If the board is erroneous, cell **<X,Y>** is fixed, or cell **<X,Y>** already contains a value, it is an error.  
  The program runs **LP** (not ILP!) for the current board. If the board is unsolvable, it is an error. Otherwise, all legal values of cell **<X,Y>** and their scores are printed.

- **`num_solutions`**  
  Prints the number of solutions for the current board.  
  This command is available in **Edit** and **Solve** modes.  
  If the board is erroneous, it is an error.  
  The program runs an **exhaustive backtracking** for the current board and prints the number of solutions found.

- **`autofill`**  
  Automatically fills "obvious" values – cells that contain a single legal value.  
  This command is available only in **Solve mode**.

- **`reset`**  
  Undoes all moves, reverting the board to its original loaded state.  
  This command is available in **Edit** and **Solve** modes.

- **`exit`**  
  Terminates the program. This command is available in all three modes.
