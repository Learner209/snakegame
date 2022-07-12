#include "snake_battle.h"
#include <cstdlib>
#include <ctime>


snake_battle::snake_battle(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength,int mode): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength),snake_mode(mode)
{
    if(snake_mode==1)
        this->initializeSnake();
    else if(snake_mode==2)
        this->initializeSnake_run();
    this->setRandomSeed();
}

void snake_battle::setRandomSeed()
{

    std::srand(std::time(nullptr));
}

void snake_battle::initializeSnake()
{
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

void snake_battle::initializeSnake_run()
{
    int centerY = this->mGameBoardHeight / 2;
    for(int i = 0;i < 6; i++)
        this->mSnake.push_back(SnakeBody(6-i,centerY));
    this->mDirection = Direction::Right;
}

bool snake_battle::isPartOfSnake(int x, int y)
{
    for(SnakeBody i: this -> getSnake()){
        if(i.getX() == x && i.getY() == y)
            return true;
    }
    return false;
}

bool snake_battle::hitWall()
{
    int headx = this -> getSnake()[0].getX();
    int heady = this -> getSnake()[0].getY();
    if(headx == 0 || heady == 0 || headx == mGameBoardWidth-1 || heady == mGameBoardHeight-1)
        return true;
    return false;
}


bool snake_battle::hitSelf()
{
    for(int i=1; i<this -> getSnake().size(); i++){
        if(this -> getSnake()[i]==this -> getSnake()[0])
            return true;
    }
    return false;
}


bool snake_battle::touchFood()
{
    SnakeBody newHead=this->getSnake()[0];
    if (this->mFood == newHead)
    {
        return true;
    }
    return false;
}

void snake_battle::senseFood(SnakeBody food)
{
    this->mFood = food;
}

std::vector<SnakeBody>& snake_battle::getSnake()
{
    return this->mSnake;
}

Direction snake_battle::getdirection()
{
    return this->mDirection;
}

bool snake_battle::changeDirection(Direction newDirection)
{
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            if(newDirection==Direction:: Down)
                break;
            this->mDirection=newDirection;
            break;
        }
        case Direction::Down:
        {
            if(newDirection==Direction:: Up)
                break;
            this->mDirection=newDirection;
            break;

        }
        case Direction::Left:
        {
            if(newDirection==Direction:: Right)
                break;
            this->mDirection=newDirection;
            break;
        }
        case Direction::Right:
        {
            if(newDirection==Direction:: Left)
                break;
            this->mDirection=newDirection;
            break;
        }
    }
    return false;
}


SnakeBody snake_battle::createNewHead()
{
    int headx=this->mSnake[0].getX();
    int heady=this->mSnake[0].getY();
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            heady--;
            break;
        }
        case Direction::Down:
        {
            heady++;
            break;
        }
        case Direction::Left:
        {
            headx--;
            break;
        }
        case Direction::Right:
        {
            headx++;
            break;
        }
    }
    SnakeBody head=SnakeBody(headx,heady);
    this->mSnake.insert(this->mSnake.begin(),head);
    SnakeBody newHead = this->mSnake[0];
    return newHead;
}

void snake_battle::through()
{
    int headx=this->mSnake[0].getX();
    int heady=this->mSnake[0].getY();
    SnakeBody head;
    if(mDirection == Direction::Up && heady == 0)
        head = SnakeBody(headx,mGameBoardHeight-2);
    else if(mDirection == Direction::Down && heady==mGameBoardHeight-1)
        head = SnakeBody(headx,1);
    else if(mDirection == Direction::Left && headx == 0)
        head = SnakeBody(mGameBoardWidth-2,heady);
    else
        head = SnakeBody(1,heady);
    this->mSnake.insert(this->mSnake.begin(),head);
    this->mSnake.pop_back();
}
bool snake_battle::moveFoward()
{

    SnakeBody newHead = this->createNewHead();
    if(this->touchFood() || (this->magnetFood() && this->time_magnet>0)   ){
        return true;
    }
    this->mSnake.pop_back();
    return false;
}


bool snake_battle::magnetFood()
{
    SnakeBody newHead=this->getSnake()[0];
    for(int i=newHead.getX()-3;i<newHead.getX()+4;i++){
        for(int j=newHead.getY()-3;j<newHead.getY()+4;j++){
            SnakeBody temp=SnakeBody(i,j);
            if(this->mFood == temp)
                return true;
        }
    }
    return false;
}
bool snake_battle::checkCollision()
{
    if (this->hitWall() || this->hitSelf() )
    {
        return true;
    }
    else
    {
        return false;
    }
}


int snake_battle::getLength()
{
    return this->mSnake.size();
}

int& snake_battle::getMagnetTime()
{
    return this->time_magnet;
}