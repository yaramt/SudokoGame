CC = gcc
OBJS = main.o Game.o SolveMode.o Parser.o Checkers.o boardGame.o listStrc.o Stack.o boardStrc.o doubleList.o possibleValuesList.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Game.h
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c Game.h SolveMode.h Parser.h Checkers.h doubleList.h boardGame.h boardStrc.h Stack.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Checkers.o: Checkers.c Checkers.h
	$(CC) $(COMP_FLAG) -c $*.c
possibleValuesList.o: possibleValuesList.c possibleValuesList.h Checkers.h
	$(CC) $(COMP_FLAG) -c $*.c
doubleList.o: doubleList.c doubleList.h Checkers.h boardGame.h
	$(CC) $(COMP_FLAG) -c $*.c
boardGame.o: boardGame.c boardGame.h Checkers.h
	$(CC) $(COMP_FLAG) -c $*.c
listStrc.o: listStrc.c listStrc.h Checkers.h
	$(CC) $(COMP_FLAG) -c $*.c
Stack.o: Stack.c Stack.h Checkers.h
	$(CC) $(COMP_FLAG) -c $*.c
boardStrc.o: boardStrc.c boardStrc.h Checkers.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h Checkers.h
	$(CC) $(COMP_FLAG) -c $*.c
SolveMode.o: SolveMode.c SolveMode.h Checkers.h possibleValuesList.h Game.h listStrc.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c	
clean:
	rm -f $(OBJS) $(EXEC)
