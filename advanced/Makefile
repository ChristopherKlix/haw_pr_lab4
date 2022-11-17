tictactoe: main.o engine.o view.o util.o keypress.o minimax.o
	cc main.o engine.o view.o util.o keypress.o minimax.o -o tictactoe
	make clean

main.o: ./src/main.c
	cc -c ./src/main.c

engine.o: ./src/engine.c ./src/engine.h
	cc -c ./src/engine.c

view.o: ./src/view.c ./src/view.h
	cc -c ./src/view.c

util.o: ./src/util.c ./src/util.h
	cc -c ./src/util.c

keypress.o: ./src/keypress.c ./src/keypress.h
	cc -c ./src/keypress.c

minimax.o: ./src/minimax.c ./src/minimax.h
	cc -c ./src/minimax.c

clean:
	rm *.o
