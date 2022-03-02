# SudokoGame
Sudoku is a number-placement puzzle where the objective is to fill a NxN grid (usually 9x9) with digits so that each column, each row, and each block contain all digits from 1 to N. For each cell in the grid, we denote by its neighbors the other cells in its row, column, and block. The main rule is that the value of each cell is different from the values of its neighbors. A Sudoku puzzle is completed successfully once all N2 cells are filled with legal values.
There are three modes: "Edit mode" or "Solve mode" or "Init mode". The game starts in Init mode. 
_________________________________________________________________________________________________________________________________________________________________________________

# Valid User Commands:
- solve X <br />
Starts a puzzle in Solve mode, loaded from a file with the name "X", where X includes a full or relative path to the file.<br />
This command is always available, in Solve, Edit, and Init modes. Any unsaved work on the current game board is lost.
- edit [X] <br />
Starts a puzzle in Edit mode, loaded from a file with the name "X", where X includes a full or relative path to the file. <br />
The parameter X is optional. If no parameter is supplied, an empty 9x9 board is generated.<br />
This command is always available, in Solve, Edit, and Init modes. Any unsaved work on the current game board is lost.
- mark_errors X <br />
Sets the "mark errors" setting to X, where X is either 0 or 1. <br />
When this parameter is set to 1, erroneous values are displayed followed by an asterisk. The default value is 1. <br />
This command is only available in Solve mode.
- print_board <br />
Prints the board. This command is only available in Edit and Solve modes.
- set X Y Z
Sets the value of cell <X,Y> to Z. <br />
This command is only available in Edit and Solve modes.<br />
The user may empty a cell by setting Z=0. <br />
In solve mode, fixed cells can't be updated. <br />
- validate <br />
Validates the current board using ILP, ensuring it is solvable. <br />
This command is only available in Edit and Solve modes. <br />
If the board is erroneous, the program prints an error message and the command is not executed.<br />
The command prints whether the board is found to be solvable, or not.
- guess X <br />
Guesses a solution to the current board using LP, with threshold X. <br />
This command is only available in Solve mode. <br />
The parameter X is a float and represents the threshold used for the LP solution. <br />
If the board is erroneous, the program prints an error message and the command is not executed. <br />
The command fills all cell values with a score of X or greater. If several values hold for the same cell, randomly one is chosen. <br />
- generate X Y <br />
Generates a puzzle by randomly filling X empty cells with legal values, running ILP to solve the board, and then clearing all but Y random cells.<br />
This command is only available in Edit mode. <br />
If the board does not contain X empty cells, then that is an error. <br />
- undo <br />
Undo a previous move done by the user. <br />
This command is only available in Edit and Solve modes. <br />
- redo <br />
Redo a move previously undone by the user. <br />
This command is only available in Edit and Solve modes.<br />
- save X <br />
Saves the current game board to the specified file, where X includes a full or relative path to the file. <br />
This command is only available in Edit and Solve modes. <br />
- hint X Y <br />
Give a hint to the user by showing the solution of a single cell X,Y. <br />
This command is only available in Solve mode. <br />
If the board is erroneous, cell <X,Y> is fixed, or cell <X,Y> already contains a value, it is an error. <br />
Run ILP to solve the board. If the board is unsolvable, it is an error. Otherwise, print to the user the value of cell <X,Y> found by the ILP solution. <br />
- guess_hint X Y <br />
Show a guess to the user for a single cell X,Y. <br />
This command is only available in Solve mode. <br />
If the board is erroneous, cell <X,Y> is fixed, or cell <X,Y> already contains a value, it is an error. <br />
Run LP (not ILP!) for the current board. If the board is unsolvable, it is an error. Otherwise, print to the user all legal values of cell <X,Y> and their scores.
- num_solutions <br />
Print the number of solutions for the current board.<br />
This command is only available in Edit and Solve modes.<br />
If the board is erroneous it is an error.<br />
Run an exhaustive backtracking for the current board. The exhaustive backtracking algorithm exhausts all options for the current board, instead of terminating when a solution is found. Once done, the program prints the number of solutions found by the algorithm. <br />
- autofill <br />
Automatically fill "obvious" values – cells which contain a single legal value. <br />
This command is only available in Solve mode. <br />
- reset <br />
Undo all moves, reverting the board to its original loaded state. <br />
This command is only available in Edit and Solve modes. <br />
- exit <br />
Terminates the program. This command is available in all three modes.
