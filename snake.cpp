//
// Created by bitch on 6/29/22.
//

#include "snake.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include "snake.h"

#include <cassert>

SnakeBody::SnakeBody()
{
}


SnakeBody::SnakeBody(int x, int y): mX(x), mY(y)
{
}

int SnakeBody::getX() const
{
    return mX;
}

int SnakeBody::getY() const
{
    return mY;
}

void SnakeBody::setX(int mx)
{
    mX = mx;
}

void SnakeBody::setY(int my)
{
    mY = my;
}

bool SnakeBody::operator == (const SnakeBody& snakeBody)
{
    // TODO overload the == operator for SnakeBody comparision.

    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());

}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
}

void Snake::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

bool Snake::isPartOfSnake(int x, int y)
{
    // TODO check if a given point with axis x, y is on the body of the snake.

    int l = this->getLength();
    for(int i = 0; i < l ; i++){
        if (mSnake[i].getX() == x && mSnake[i].getY() == y) return true;
    }
    return false;

}

/*
 * Assumption:
 * Only the head would hit wall.
 */
bool Snake::hitWall()
{
    // TODO check if the snake has hit the wall

    return mSnake[0].getX() == 0 ||
            mSnake[0].getX() == mGameBoardWidth - 1 ||
            mSnake[0].getY() == 0 ||
            mSnake[0].getY() == mGameBoardHeight - 1;
}

/*
 * The snake head is overlapping with its body
 */
bool Snake::hitSelf()
{
    // TODO check if the snake has hit itself.
    int l = getLength();
    for(int i = 2; i < l ; i++){
        if (mSnake[i].getX() ==  mSnake[0].getX() && mSnake[i].getY() == mSnake[0].getY()) return true;
    }
    return false;

}

void Snake::senseFood(SnakeBody food)
{
    this->mFood = food;
}

std::vector<SnakeBody>& Snake::getSnake()
{
    return this->mSnake;
}

bool Snake::changeDirection(Direction newDirection)
{
    if (manual)
    {
        switch (this->mDirection)
        {
            case Direction::Up:
            {
                if (newDirection == Direction::Left) this->mDirection = Direction::Left;
                else if (newDirection == Direction::Right) this->mDirection = Direction::Right;
                return true;
            }
            case Direction::Down:
            {
                if (newDirection == Direction::Left) this->mDirection = Direction::Left;
                else if (newDirection == Direction::Right) this->mDirection = Direction::Right;
                return true;
            }
            case Direction::Left:
            {
                if (newDirection == Direction::Up) this->mDirection = Direction::Up;
                else if (newDirection == Direction::Down) this->mDirection = Direction::Down;
                return true;
            }
            case Direction::Right:
            {
                if (newDirection == Direction::Up) this->mDirection = Direction::Up;
                else if (newDirection == Direction::Down) this->mDirection = Direction::Down;
                return true;
            }
        }
        return true;
    }
    else
    {
        int probability = this->randomInteger(0,10);
        if (probability < 6) return true;
        switch(this->mDirection)
        {
            case Direction::Up:
            {
                if ((this->mFood.getY() == this->mSnake[0].getY() + 1 || this->mFood.getY() == this->mSnake[0].getY() - 1 ) && probability < 3)
                {
                    return true;
                }
                else if ((this->mFood.getY() > this->mSnake[0].getY() && probability < 2)
                         || (this->mFood.getY() == this->mSnake[0].getY() && probability < 7)
                         || (this->mFood.getY() < this->mSnake[0].getY() && probability < 9))
                {
                    if (this->mFood.getX() > this->mSnake[0].getX())
                    {
                        this->mDirection = Direction::Right;
                    }
                    else
                    {
                        this->mDirection = Direction::Left;
                    }
                }
                return true;
            }
            case Direction::Down :
            {
                if ((this->mFood.getY() == this->mSnake[0].getY() + 1 || this->mFood.getY() == this->mSnake[0].getY() - 1 ) && probability < 3)
                {
                    break;
                }
                else if ((this->mFood.getY() < this->mSnake[0].getY() && probability < 2)
                         || (this->mFood.getY() == this->mSnake[0].getY() && probability < 7)
                         || (this->mFood.getY() > this->mSnake[0].getY() && probability < 9))
                {
                    if (this->mFood.getX() > this->mSnake[0].getX())
                    {
                        this->mDirection = Direction::Right;
                    }
                    else
                    {
                        this->mDirection = Direction::Left;
                    }
                }
                return true;
            }
            case Direction::Left:
            {
                if ((this->mFood.getX() == this->mSnake[0].getX() + 1 || this->mFood.getX() == this->mSnake[0].getX() - 1 ) && probability < 3)
                {
                    break;
                }
                else if ((this->mFood.getX() < this->mSnake[0].getX() && probability < 2)
                         || (this->mFood.getX() == this->mSnake[0].getX() && probability < 7)
                         || (this->mFood.getX() > this->mSnake[0].getX() && probability < 9))
                {
                    if (this->mFood.getY() > this->mSnake[0].getY())
                    {
                        this->mDirection = Direction::Up;
                    }
                    else
                    {
                        this->mDirection = Direction::Down;
                    }
                }
                return true;
            }
            case Direction::Right:
            {
                if ((this->mFood.getX() == this->mSnake[0].getX() + 1 || this->mFood.getX() == this->mSnake[0].getX() - 1 ) && probability < 3)
                {
                    break;
                }
                else if ((this->mFood.getX() > this->mSnake[0].getX() && probability < 2)
                         || (this->mFood.getX() == this->mSnake[0].getX() && probability < 7)
                         || (this->mFood.getX() < this->mSnake[0].getX() && probability < 9))
                {
                    if (this->mFood.getY() > this->mSnake[0].getY())
                    {
                        this->mDirection = Direction::Up;
                    }
                    else
                    {
                        this->mDirection = Direction::Down;
                    }
                }
                return true;
            }
        }
        return true;
    }
    return true;

}


SnakeBody Snake::createNewHead()
{
    /* TODO
     * read the position of the current head
     * read the current heading direction
     * add the new head according to the direction
     * return the new snake
     */

    SnakeBody currentHead = this->mSnake[0];
    int width = currentHead.getX();
    int height = currentHead.getY();
    switch(this->mDirection){
        case Direction::Up : height -= 1; break;
        case Direction::Down : height += 1; break;
        case Direction::Left : width -= 1; break;
        case Direction::Right : width += 1; break;
    };
    SnakeBody newHead(width, height);
    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
bool Snake::moveFoward()
{
    /*
    * move the snake forward.
     * If eat food, return true, otherwise return false
     */
    SnakeBody newHead = this->createNewHead();

    //No-wall
    if (!has_walls)
    {
        if (newHead.getY() == 0) newHead.setY(mGameBoardHeight - 2);
        else if (newHead.getY() == mGameBoardHeight - 1) newHead.setY(1);
        else if (newHead.getX() == 0) newHead.setX(mGameBoardWidth - 2);
        else if (newHead.getX() == mGameBoardWidth - 1) newHead.setX(1);
    }

    if (newHead == mFood) {
        this->mSnake.insert(this->mSnake.cbegin(), newHead);
        return true;
    }
    this->mSnake.insert(this->mSnake.cbegin(), newHead);
    this->mSnake.pop_back();
    return false;
}

bool Snake::checkCollision()
{
    if (has_walls && (this->hitWall() || this->hitSelf()))
    {
        return true;
    }
    else if (!has_walls && this->hitSelf())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Snake::hasWalls(bool has_walls) {
    this->has_walls = has_walls;
}

int Snake::getLength()
{
    return this->mSnake.size();
}

void Snake::manualOrMachine(bool flag)
{
    this->manual = !flag;
}


int Snake::randomInteger(int low, int high)
{
    return low + rand() % (high - low);
}