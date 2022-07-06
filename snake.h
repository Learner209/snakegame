//
// Created by bitch on 6/29/22.
//

#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    int getX() const;
    int getY() const;
    void setX(int);
    void setY(int);
    bool operator == (const SnakeBody& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no dependency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    bool touchFood();
    // Check if the snake is dead
    bool hitWall();
    bool hitSelf();
    bool checkCollision();

    virtual bool changeDirection(Direction newDirection = Direction::Up);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    bool moveFoward();
    void hasWalls(bool);
    void manualOrMachine(bool);

protected:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;
    std::vector<SnakeBody> mSnake;
    bool has_walls;
    bool manual = true;
    //auxiliary function
    int randomInteger(int low, int high);
};


#endif
