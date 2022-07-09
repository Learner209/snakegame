#include "snake.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

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
    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength, terrain map): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
    this->mTerrain.reset(new Terrain(gameBoardWidth, gameBoardHeight, map));
    this->mTerrain->initializeTerrain(0);
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

int Snake::isPartOfSnake(int x, int y)
{
    int l = this->getLength();
    for(int index = 0; index < l ; index++){
        if (mSnake[index].getX() == x && mSnake[index].getY() == y) return index;
    }
    return -1;
}

/*
 * Assumption:
 * Only the head would hit wall.
 */
bool Snake::hitWall() const
{
    return mSnake[0].getX() == 0 ||
            mSnake[0].getX() == mGameBoardWidth - 1 ||
            mSnake[0].getY() == 0 ||
            mSnake[0].getY() == mGameBoardHeight - 1;
}

/*
 * The snake head is overlapping with its body
 */
bool Snake::hitSelf() const
{
    //check if the snake has hit itself.
    int l = getLength();
    for(int i = 3; i < l ; i++){
        if (mSnake[i].getX() ==  mSnake[0].getX() && mSnake[i].getY() == mSnake[0].getY()) return true;
    }
    return false;

}

bool Snake::toHitWall() const
{
    //check if the snake is yet to hit the wall
    if (has_walls)
    {
        return (mSnake[0].getX() == 1 && this->mDirection == Direction::Left) ||
               (mSnake[0].getX() == mGameBoardWidth - 2  && this->mDirection == Direction::Right) ||
               (mSnake[0].getY() == 1 && this->mDirection == Direction::Up)||
               (mSnake[0].getY() == mGameBoardHeight - 2 && this->mDirection == Direction::Down);
    }
    else
    {
        return false;
    }

}

bool Snake::toHitSelf() const
{
    // check if the snake is yet to hit itself.
    SnakeBody toMove = this->createNewHead();
    int l = getLength();
    for(int i = 3; i < l ; i++){
        if (mSnake[i].getX() ==  toMove.getX() && mSnake[i].getY() == toMove.getY()) return true;
    }
    return false;
}

bool Snake::hitMountain() const
{
    std::vector<Block> Terrains = this->mTerrain->getTerrains();
    int l = Terrains.size();
    SnakeBody head = this->mSnake[0];
    for (int i = 0; i < l; i++)
    {
        if (head.getX() == Terrains[i].first && head.getY() == Terrains[i].second)
        {
            return true;
        }
    }
    return false;
}
void Snake::circuitCheck()
{
    SnakeBody newHead = this->createNewHead();
    SnakeBody head = this->mSnake[0];
    switch(this->mDirection){
        case Direction::Up:
        {
            if (this->isPartOfSnake(head.getX(), head.getY() - 1) != -1)
            {
                int index = this->isPartOfSnake(head.getX(), head.getY() - 1);
                if (this->mSnake[index - 1].getX() < head.getX())
                {
                    this->mDirection = Direction::Right;
                    return;
                }
                this->mDirection = Direction::Left;
                return;
            }
            if (this->isPartOfSnake(newHead.getX(), newHead.getY() - 1) != -1)
            {
                int index = this->isPartOfSnake(newHead.getX(), newHead.getY() - 1);
                if (this->mSnake[index - 1].getX() < newHead.getX())
                {
                    this->mDirection = Direction::Right;
                    return;
                }
                this->mDirection = Direction::Left;
                return;
            }
            if (this->isPartOfSnake(newHead.getX() - 1, newHead.getY()) != -1
                && this->isPartOfSnake(newHead.getX() + 1, newHead.getY()) != -1)
            {
               int lp = this->isPartOfSnake(newHead.getX() - 1, newHead.getY());
               int rp = this->isPartOfSnake(newHead.getX() + 1, newHead.getY());
                this->mDirection = (lp > rp)? Direction::Left: Direction::Right;
            }
            return;
        }
        case Direction::Down:
        {
            if (this->isPartOfSnake(head.getX(), head.getY() + 1) != -1)
            {
                int index = this->isPartOfSnake(head.getX(), head.getY() + 1);
                if (this->mSnake[index - 1].getX() < head.getX())
                {
                    this->mDirection = Direction::Right;
                    return;
                }
                this->mDirection = Direction::Left;
                return;
            }
            if (this->isPartOfSnake(newHead.getX(), newHead.getY() + 1) != -1)
            {
                int index = this->isPartOfSnake(newHead.getX(), newHead.getY() + 1);
                if (this->mSnake[index - 1].getX() < newHead.getX())
                {
                    this->mDirection = Direction::Right;
                    return;
                }
                this->mDirection = Direction::Left;
                return;
            }

            if (this->isPartOfSnake(newHead.getX() - 1, newHead.getY()) != -1
                && this->isPartOfSnake(newHead.getX() + 1, newHead.getY()) != -1)
            {
                int lp = this->isPartOfSnake(newHead.getX() - 1, newHead.getY());
                int rp = this->isPartOfSnake(newHead.getX() + 1, newHead.getY());
                this->mDirection = (lp > rp)? Direction::Left: Direction::Right;
            }
            return;
        }
        case Direction::Left:
        {
            if (this->isPartOfSnake(head.getX() - 1, head.getY()) != -1)
            {
                int index = this->isPartOfSnake(head.getX() - 1, head.getY());
                if (this->mSnake[index - 1].getY() < head.getY())
                {
                    this->mDirection = Direction::Down;
                    return;
                }
                this->mDirection = Direction::Up;
                return;
            }
            if (this->isPartOfSnake(newHead.getX() - 1, newHead.getY()) != -1)
            {
                int index = this->isPartOfSnake(newHead.getX() - 1, newHead.getY());
                if (this->mSnake[index - 1].getY() < newHead.getY())
                {
                    this->mDirection = Direction::Down;
                    return;
                }
                this->mDirection = Direction::Up;
                return;
            }
            if (this->isPartOfSnake(newHead.getX(), newHead.getY() - 1) != -1
                && this->isPartOfSnake(newHead.getX(), newHead.getY() + 1) != -1)
            {
                int up = this->isPartOfSnake(newHead.getX(), newHead.getY() - 1);
                int dp = this->isPartOfSnake(newHead.getX(), newHead.getY() + 1);
                this->mDirection = (up > dp)? Direction::Up: Direction::Down;
            }
            return;
        }
        case Direction::Right:
        {
            if (this->isPartOfSnake(head.getX() + 1, head.getY()) != -1)
            {
                int index = this->isPartOfSnake(head.getX() + 1, head.getY());
                if (this->mSnake[index - 1].getY() < head.getY())
                {
                    this->mDirection = Direction::Down;
                    return;
                }
                this->mDirection = Direction::Up;
                return;
            }
            if (this->isPartOfSnake(newHead.getX() + 1, newHead.getY()) != -1)
            {
                int index = this->isPartOfSnake(newHead.getX() + 1, newHead.getY());
                if (this->mSnake[index - 1].getY() < newHead.getY())
                {
                    this->mDirection = Direction::Down;
                    return;
                }
                this->mDirection = Direction::Up;
                return;
            }
            if (this->isPartOfSnake(newHead.getX(), newHead.getY() - 1) != -1
                && this->isPartOfSnake(newHead.getX() , newHead.getY() + 1) != -1)
            {
                int up = this->isPartOfSnake(newHead.getX(), newHead.getY() - 1);
                int dp = this->isPartOfSnake(newHead.getX(), newHead.getY() + 1);
                this->mDirection = (up > dp)? Direction::Up: Direction::Down;
            }
            return;
        }
    }
    return;

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

void Snake::simulateDirection(int difficulty, int delay) {
    int probability = this->randomInteger(0,20);
    /* MisTackle: the probability of missing the food
     * strike: the capability of hitting the food rather than revolving around it
     * moveBack: the sharpness of sensing the next food
     * collision: the ability to avoid collision*/

    int misTackle = difficulty * pow(1.2, delay), strike = (20 - difficulty * 4) * pow(0.8, delay);
    int moveBack = (20 - 5 * difficulty) * pow(0.9, delay), collision = difficulty / 2;

    SnakeBody head = this->mSnake[0];

    switch(this->mDirection)
    {
        case Direction::Up:
        {
            if (this->toHitWall())
            {
                if (head.getX() == 1)
                {
                    this->mDirection = Direction::Right;
                    return;
                }
                else if (head.getX() == this->mGameBoardWidth - 2)
                {
                    this->mDirection = Direction::Left;
                    return;
                }
                else if (probability < collision)
                {
                    return;
                }
                this->mDirection = (probability < 10)? Direction::Left : Direction::Right;
                return;
            }

            this->circuitCheck();

            if (this->isPartOfSnake(head.getX() - 1, head.getY()) != -1
                || this->isPartOfSnake(head.getX() + 1, head.getY()) != -1)
            {
                return;
            }

            bool move_back = (this->has_walls)? (this->mFood.getY() > head.getY()) && (probability < moveBack):
                             ((this->mFood.getY() - this->mSnake[0].getY() > this->mGameBoardHeight + head.getY() - this->mFood.getY() - 1) ?
                             (this->mFood.getY() > head.getY() && probability < moveBack * 0.15) :
                             (this->mFood.getY() > head.getY() && probability < moveBack));

            if ((this->mFood.getY() == this->mSnake[0].getY() + 1 || this->mFood.getY() == head.getY() - 1)
                  && probability < misTackle && this->mFood.getX() != head.getX()
                 || this->mFood.getY() == this->mSnake[0].getY() && probability < strike
                 || move_back)
            {
                if (this->mFood.getX() > head.getX())
                {
                    this->mDirection = Direction::Right;
                }
                else if(this->mFood.getX() < head.getX())
                {
                    this->mDirection = Direction::Left;
                }
                else
                {
                    if (head.getX() == 1)
                    {
                        this->mDirection = Direction::Right;
                    }
                    else if (head.getX() == this->mGameBoardWidth - 2)
                    {
                        this->mDirection = Direction::Left;
                    }
                    else
                    {
                        this->mDirection = (probability < 10)? Direction::Left : Direction::Right;
                    }
                }
            }
            return;
        }
        case Direction::Down:
        {
            if (this->toHitWall())
            {
                if (head.getX() == 1)
                {
                    this->mDirection = Direction::Right;
                    return;
                }
                else if (head.getX() == this->mGameBoardWidth - 2)
                {
                    this->mDirection = Direction::Left;
                    return;
                }
                else if (probability < collision)
                {
                    return;
                }
                this->mDirection = (probability < 10)? Direction::Left : Direction::Right;
                return;
            }

            this->circuitCheck();

            if (this->isPartOfSnake(head.getX() - 1, head.getY()) != -1
                 || this->isPartOfSnake(head.getX() + 1, head.getY()) != -1)
            {
                return;
            }

            bool move_Back = (this->has_walls)? (this->mFood.getY() < head.getY()) && (probability < moveBack):
                             ((head.getY() - this->mFood.getY() > this->mGameBoardHeight - head.getY() + this->mFood.getY() - 1) ?
                             (this->mFood.getY() < head.getY() && probability < moveBack * 0.15) :
                             (this->mFood.getY() < head.getY() && probability < moveBack));

            if ((this->mFood.getY() == head.getY() + 1 || this->mFood.getY() == head.getY() - 1 )
                && probability < misTackle && this->mFood.getX() != head.getX()
                || this->mFood.getY() == head.getY() && probability < strike
                || move_Back)
            {
                if (this->mFood.getX() > head.getX())
                {
                    this->mDirection = Direction::Right;
                }
                else if(this->mFood.getX() < head.getX())
                {
                    this->mDirection = Direction::Left;
                }
                else
                {
                    if (head.getX() == 1)
                    {
                        this->mDirection = Direction::Right;
                    }
                    else if (head.getX() == this->mGameBoardWidth - 2)
                    {
                        this->mDirection = Direction::Left;
                    }
                    else
                    {
                        this->mDirection = (probability < 10)? Direction::Left : Direction::Right;
                    }
                }
            }
            return;
        }
        case Direction::Left:
        {
            if (this->toHitWall())
            {
                if (head.getY() == 1)
                {
                    this->mDirection = Direction::Down;
                    return;
                }
                else if (head.getY() == this->mGameBoardHeight - 2)
                {
                    this->mDirection = Direction::Up;
                    return;
                }
                else if (probability < collision)
                {
                    return;
                }
                this->mDirection = (probability < 10)? Direction::Up : Direction::Down;
                return;
            }

            this->circuitCheck();

            if (this->isPartOfSnake(head.getX(), head.getY() - 1) != -1
                || this->isPartOfSnake(head.getX(), head.getY() + 1) != -1)
            {
                return;
            }

            bool move_Back = (this->has_walls)? (this->mFood.getX() > head.getX()) && (probability < moveBack):
                             ((this->mFood.getX() - head.getX() > this->mGameBoardWidth - this->mFood.getX() + head.getX() - 1) ?
                             (this->mFood.getX() > head.getX() && probability < 0.15 * moveBack) :
                             (this->mFood.getX() > head.getX() && probability < moveBack));

            if ((this->mFood.getX() == head.getX() + 1 || this->mFood.getX() == head.getX() - 1 )
                && probability < misTackle && this->mFood.getY() != head.getY()
                || this->mFood.getX() == head.getX() && probability < strike
                || move_Back)
            {
                if (this->mFood.getY() > head.getY())
                {
                    this->mDirection = Direction::Down;
                }
                else if(this->mFood.getY() < head.getY())
                {
                    this->mDirection = Direction::Up;
                }
                else
                {
                    if (head.getY() == 1)
                    {
                        this->mDirection = Direction::Down;
                    }
                    else if (head.getY() == this->mGameBoardHeight - 2)
                    {
                        this->mDirection = Direction::Up;
                    }
                    else
                    {
                        this->mDirection = (probability < 10)? Direction::Up : Direction::Down;
                    }
                }
            }
            return;
        }
        case Direction::Right:
        {
            if (this->toHitWall())
            {
                if (head.getY() == 1)
                {
                    this->mDirection = Direction::Down;
                    return;
                }
                else if (head.getY() == this->mGameBoardHeight - 2)
                {
                    this->mDirection = Direction::Up;
                    return;
                }
                else if (probability < collision)
                {
                    return;
                }
                this->mDirection = (probability < 10)? Direction::Up : Direction::Down;
            }

            this->circuitCheck();

            if (this->isPartOfSnake(head.getX(), head.getY() - 1) != -1
                || this->isPartOfSnake(head.getX(), head.getY() + 1) != -1)
            {
                return;
            }

            bool move_Back = (this->has_walls)? (this->mFood.getX() < head.getX()) && (probability < moveBack):
                             ((head.getX() - this->mFood.getX() > this->mGameBoardWidth - 1 - head.getX() + this->mFood.getX()) ?
                             (this->mFood.getX() < head.getX() && probability < 0.15 * moveBack) :
                             (this->mFood.getX() < head.getX() && probability < moveBack));

            if ((this->mFood.getX() == head.getX() + 1 || this->mFood.getX() == head.getX() - 1 )
                && probability < misTackle  && this->mFood.getY() != head.getY()
                || this->mFood.getX() == head.getX() && probability < strike
                || move_Back)
            {
                if (this->mFood.getY() > head.getY())
                {
                    this->mDirection = Direction::Down;
                }
                else if(this->mFood.getY() < head.getY())
                {
                    this->mDirection = Direction::Up;
                }
                else
                {
                    if (head.getY() == 1)
                    {
                        this->mDirection = Direction::Down;
                    }
                    else if (head.getY() == this->mGameBoardHeight - 2)
                    {
                        this->mDirection = Direction::Up;
                    }
                    else
                    {
                        this->mDirection = (probability < 10)? Direction::Up : Direction::Down;
                    }
                }
            }
            return;
        }
    }
}

SnakeBody Snake::createNewHead() const
{
    /* read the position of the current head
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
    if (this->mTerrain->getattr() == Mountain && this->hitMountain())
    {
        return true;
    }
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

int Snake::getLength() const
{
    return this->mSnake.size();
}

void Snake::manualOrMachine(bool flag)
{
    this->manual = !flag;
}


int Snake::randomInteger(int low, int high) const
{
    return low + rand() % (high - low);
}