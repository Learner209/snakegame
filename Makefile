snakegame: main.o game.o snake.o terrain.o
	g++ -o snakegame main.o game.o snake.o terrain.o -lcurses
main.o: main.cpp game.h
	g++ -c main.cpp
game.o: game.cpp snake.h
	g++ -c game.cpp
snake.o: snake.cpp terrain.h
	g++ -c snake.cpp
terrain.o: terrain.cpp
	g++ -c terrain.cpp
clean:
	rm *.o
	rm snakegame
	rm record.dat