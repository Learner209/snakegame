snakegame: main.o game.o snake.o terrain.o battle.o snake_battle.o
	g++ -o snakegame main.o game.o snake.o terrain.o battle.o snake_battle.o -lcurses
main.o: main.cpp game.h
	g++ -c main.cpp
game.o: game.cpp snake.h battle.h
	g++ -c game.cpp
snake.o: snake.cpp terrain.h
	g++ -c snake.cpp
snake_battle.o: snake_battle.cpp snake.h
	g++ -c snake_battle.cpp
battle.o : battle.cpp game.h snake_battle.h
	g++ -c battle.cpp
terrain.o: terrain.cpp
	g++ -c terrain.cpp
clean:
	rm *.o
	rm snakegame
	rm record.dat
