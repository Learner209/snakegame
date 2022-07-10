#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "terrain.h"
#include <memory>

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
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength, terrain Terrain = Plain, int difficulty = 0);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Checking API for generating random food
    int isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    bool touchFood();
    // Check if the snake is dead
    bool hitWall() const;
    bool hitSelf() const;
    bool hitMountain() const;
    //Machine Snake mechanism
    bool toHitWall() const;
    bool toHitSelf() const;
    bool toHitMountain() const;

    bool checkCollision();

    bool changeDirection(Direction newDirection = Direction::Up);
    void simulateDirection(int, int);
    std::vector<SnakeBody>& getSnake();
    int getLength() const;
    SnakeBody createNewHead() const;
    bool moveFoward();
    void hasWalls(bool);
    void manualOrMachine(bool);
    std::unique_ptr<Terrain> mTerrain;

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
    int machineSmartness = 0;
    //auxiliary function
    int randomInteger(int low, int high) const;

    void circuitCheck();
};


#endif
