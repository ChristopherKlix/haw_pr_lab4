main: main.o game.o view.o
	cc main.o game.o view.o -o tictactoe
	make clean

main.o: ./src/main.c
	cc -c ./src/main.c

game.o: ./src/game.c ./src/game.h
	cc -c ./src/game.c

view.o: ./src/view.c ./src/view.h
	cc -c ./src/view.c

clean:
	rm *.o
