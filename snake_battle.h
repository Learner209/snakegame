#ifndef SNAKE_BATTLE_H
#define SNAKE_BATTLE_H

#include <vector>
#include "snake.h"

#include <vector>

// Snake class should have no dependency on the GUI library
class snake_battle
{
public:
    //Snake();
    snake_battle(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength,int mode);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();             //ÉúŽæÄ£Êœ³õÊŒ»¯
    void initializeSnake_run();         //ÅÜ¿áÄ£Êœ³õÊŒ»¯
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    bool touchFood();
    // Check if the snake is dead
    bool hitWall();
    bool hitSelf();
    bool checkCollision();

    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    bool moveFoward();
    bool magnetFood();                      //ÓÃÓÚÅÐ¶ÏŸßÓÐÎüÌúÊ¯Ê±ÊÇ·ñ¿ÉÒÔÎüµœÊ³Îï
    int& getMagnetTime();                   //·µ»ØÎüÌúÊ¯Ê±Œä
    void through();                         //ÁîÌ°³ÔÉßŽ©Çœ
    Direction getdirection();
    //int& get_mode();

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;
    std::vector<SnakeBody> mSnake;
    std::vector<std::pair<char,SnakeBody>> sFood;
    int time_magnet;
    int snake_mode;
};

#endif
