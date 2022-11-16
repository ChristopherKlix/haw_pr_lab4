main: main.o game.o view.o
	cc main.o game.o view.o -o tictactoe
	make clean

main.o: main.c
	cc -c main.c

game.o: game.c game.h
	cc -c game.c

view.o: view.c view.h
	cc -c view.c

clean:
	rm *.o
