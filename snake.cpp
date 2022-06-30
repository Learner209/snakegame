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


bool Snake::touchFood()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mFood == newHead)
    {
        return true;
    }
    else
    {
        return false;
    }
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
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            // what you need to do when the current direction of the snake is Up
            // and the user inputs a new direction?  TODO
            if (newDirection == Direction::Left) this->mDirection = Direction::Left;
            else if (newDirection == Direction::Right) this->mDirection = Direction::Right;
            break;
        }
        case Direction::Down:
        {
            // what you need to do when the current direction of the snake is Down
            // and the user inputs a new direction? TODO
            if (newDirection == Direction::Left) this->mDirection = Direction::Left;
            else if (newDirection == Direction::Right) this->mDirection = Direction::Right;
            break;
        }
        case Direction::Left:
        {
            // what you need to do when the current direction of the snake is Left
            // and the user inputs a new direction? TODO
            if (newDirection == Direction::Up) this->mDirection = Direction::Up;
            else if (newDirection == Direction::Down) this->mDirection = Direction::Down;
            break;
        }
        case Direction::Right:
        {
            // what you need to do when the current direction of the snake is Right
            // and the user inputs a new direction? TODO
            if (newDirection == Direction::Up) this->mDirection = Direction::Up;
            else if (newDirection == Direction::Down) this->mDirection = Direction::Down;
            break;
        }
    }
    return false;
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
		 * TODO
		 * move the snake forward.
     * If eat food, return true, otherwise return false
     */
    SnakeBody newHead = this->createNewHead();
    if (touchFood()) {
        this->mSnake.insert(this->mSnake.cbegin(), newHead);
        return true;
    }
    this->mSnake.insert(this->mSnake.cbegin(), newHead);
    this->mSnake.pop_back();
    return false;
}

bool Snake::checkCollision()
{
    if (this->hitWall() || this->hitSelf())
    {
        return true;
    }
    else
    {
        return false;
    }
}


int Snake::getLength()
{
    return this->mSnake.size();
}