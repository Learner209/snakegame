#include "battle.h"

#include <string>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;
Battle::Battle():Game()
{

    /*this->mWindows.resize(3);
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->mLeaderBoard.assign(this->mNumLeaders, 0);*/
    this->mInformationHeight = 6;
    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();
}

Battle::~Battle()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Battle::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Battle::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    if(mode==1){
        mvwprintw(this->mWindows[0], 2, 1, "Your special condition and remained time now:");
        string y;
        string x;
        if(time_confusion>0){
            x="Condition: Confusion";

            y="Remained time: " + to_string(time_confusion/1000)+"s";

        }
        else if(time_invincible>0){
            x="Condition: Invincible";

            y="Remained time: " + to_string(time_invincible/1000)+"s";

        }

        else{
            x="Condition: Normal";
        }
        if(time_magnet>0){
            x+="(With magnet of "+to_string(time_magnet/1000)+"s.)";
        }
        if(check_double>0){
            y+="(Next "+to_string(check_double)+" food has double points.)";
        }
        else if(check_zero>0){
            y+="(Next "+to_string(check_zero)+" food has no points.)";
        }
        mvwprintw(this->mWindows[0], 3, 1, "%s", x.c_str());
        mvwprintw(this->mWindows[0], 4, 1, "%s", y.c_str());
        wrefresh(this->mWindows[0]);
    }
    else if(mode==2){
        mvwprintw(this->mWindows[0], 2, 1, "Remained time:");
        string x=to_string(time_all_run/1000);
        x+="S";
        mvwprintw(this->mWindows[0], 3, 1, "%s", x.c_str());
        wrefresh(this->mWindows[0]);
    }
    else if(mode==3){
        string x="Reminded HP of BOSS: ";
        x+=to_string(livesBOSS);
        x+="        Condition of BOSS: ";
        if(conditionBOSS==1)
            x+="Normal";
        else if(conditionBOSS==2)
            x+="Crazy";
        else if(conditionBOSS==3)
            x+="Can't be harmed";
        else
            x+="Weak("+to_string(time_weak/1000)+"S)";
        mvwprintw(this->mWindows[0], 2, 1, "%s", x.c_str());
        if(conditionBOSS!=3){
            if(conditionBOSS==1 || conditionBOSS==2){
                string x3=to_string(10-time1)+" seconds away from the next attack";
                mvwprintw(this->mWindows[0], 3, 1, "%s", x3.c_str());
            }
            else if(conditionBOSS==4){
                string x3="BOSS is weak now,please attack it seizing the chance";
                mvwprintw(this->mWindows[0], 3, 1, "%s", x3.c_str());
            }
        }
        else if(mechanism==1){
            string x3="Please enter the special character: ";
            x3+=print_char;
            mvwprintw(this->mWindows[0], 3, 1, "%s", x3.c_str());
        }
        else if(mechanism==2){
            string x3="Please go through the special orbit.";
            mvwprintw(this->mWindows[0], 3, 1, "%s", x3.c_str());
            string x4="Remained Time: ";
            x4+=to_string(time_mechanism2/1000);
            mvwprintw(this->mWindows[0], 4, 1, "%s", x4.c_str());
        }
        else if(mechanism==3){
            string x3="Please push the boxes to the BOSS";
            mvwprintw(this->mWindows[0], 3, 1, "%s", x3.c_str());
            string x4="Remained Time: ";
            x4+=to_string(time_mechanism3/1000);
            mvwprintw(this->mWindows[0], 4, 1, "%s", x4.c_str());
        }
        else if(mechanism==4){
            string x3="Please push the doors to make all doors the same";
            mvwprintw(this->mWindows[0], 3, 1, "%s", x3.c_str());
            string x4="Remained Time: ";
            x4+=to_string(time_mechanism4/1000);
            mvwprintw(this->mWindows[0], 4, 1, "%s", x4.c_str());
        }
        wrefresh(this->mWindows[0]);
    }
}

void Battle::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Battle::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Battle::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Battle::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");
    if(mode==1){
        mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
        mvwprintw(this->mWindows[2], 11, 1, "Points");
    }
    else if(mode==2  || mode==3){
        mvwprintw(this->mWindows[2], 8, 1, "Lives");
        mvwprintw(this->mWindows[2], 11, 1, "Revivecoins");
    }
    if(mode==3){
        string x1="ATK: ";
        x1+=to_string(harm);
        mvwprintw(this->mWindows[2], 14, 1, "%s", x1.c_str());
        string x2="CritChance: ";
        x2+=to_string(critChance*10);
        x2+="%";
        mvwprintw(this->mWindows[2], 15, 1, "%s", x2.c_str());
        string x4="SnakeCondition: ";
        string x3;
        if(time_confusion>0){
            x3+="Confusion(";
            x3+=to_string(time_confusion/1000);
            x3+="S)";
        }
        else if(time_invincible>0){
            x3+="Invincible(";
            x3+=to_string(time_invincible/1000);
            x3+="S)";
        }
        else{
            x3+="Normal";
        }
        mvwprintw(this->mWindows[2], 16, 1, "%s", x4.c_str());
        mvwprintw(this->mWindows[2], 17, 1, "%s", x3.c_str());
        string x5="Weapon: "+weapon;
        string x6="Armor: "+armor;
        string x7="Jewelry: "+jewelry;
        mvwprintw(this->mWindows[2], 18, 1, "%s", x5.c_str());
        mvwprintw(this->mWindows[2], 19, 1, "%s", x6.c_str());
        mvwprintw(this->mWindows[2], 20, 1, "%s", x7.c_str());
        string x8="Sanity Points: ";
        mvwprintw(this->mWindows[2], 21, 1, "%s", x8.c_str());
        string x9 =to_string(50-checkConfusion);
        mvwprintw(this->mWindows[2], 22, 1, "%s", x9.c_str());
        std::string LivesString;
        if(this->lives>=0)
            LivesString = std::to_string(this->lives);
        else
            LivesString = std::to_string(0);
        mvwprintw(this->mWindows[2], 9, 1, "%s", LivesString.c_str());
        std::string pointString = std::to_string(this->reviveCoins);
        mvwprintw(this->mWindows[2], 12, 1, "%s", pointString.c_str());
        wrefresh(this->mWindows[2]);
    }
    wrefresh(this->mWindows[2]);
}

bool Battle::renderResult()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    int index = 0;
    int offset = 4;
    std::vector<std::string> menuItems;
    if(victory){
        mvwprintw(menu, 1, 1, "Congratulations!");
        if(mode==2){
            mvwprintw(menu, 2, 1, "You have caught up with the alien enermy!");
            menuItems = {"Continue", "Quit"};
        }
        else if(mode==3){
            mvwprintw(menu, 2, 1, "You have overcome the alien enermy!");
            menuItems = {"Restart", "Quit"};
        }
    }
    else{
        mvwprintw(menu, 1, 1, "Sorry!");
        if(mode==2)
            mvwprintw(menu, 2, 1, "You haven't caught up with the alien enermy!");
        else if(mode==3)
            mvwprintw(menu, 2, 1, "You haven't overcome the alien enermy!");
        menuItems = {"Restart", "Quit"};
    }
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, "%s", menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, "%s", menuItems[1].c_str());
    wrefresh(menu);
    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        if(victory){
            if(mode==2)
                mode=3;
            else if(mode==3)
                mode=0;
        }
        else{
            if(mode==3)
                mode=0;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Battle::weatherRevive()
{
    time_temp1=clock();
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Yes", "No"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Do you want to revive?");
    std::string pointString = "(Consume one revivecoin)";
    mvwprintw(menu, 2, 1, "%s", pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, "%s", menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, "%s", menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, "%s", menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);
    time_temp2=clock();
    if (index == 0)
    {
        if(mode==3){
            time_invincible=3000;
            time_confusion=0;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void Battle::changeMode(int mode_choice)
{
    this->mode = mode_choice;
}

void Battle::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, "%s", pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Battle::renderDifficulty() const
{
    std::string difficultyString;
    if(this->mDifficulty>=0)
        difficultyString = std::to_string(this->mDifficulty);
    else
        difficultyString = std::to_string(0);
    mvwprintw(this->mWindows[2], 9, 1, "%s", difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

void Battle::renderLives()
{
    std::string LivesString;
    if(this->lives>=0)
        LivesString = std::to_string(this->lives);
    else
        LivesString = std::to_string(0);
    mvwprintw(this->mWindows[2], 9, 1, "%s", LivesString.c_str());
    wrefresh(this->mWindows[2]);
}

void Battle::renderRevivecoins()
{
    std::string pointString = std::to_string(this->reviveCoins);
    mvwprintw(this->mWindows[2], 12, 1, "%s", pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Battle::initializeGame()
{
    switch(this->mode)
    {
        case 1:
            this->mPtrSnake = std::make_unique<Snake>(this->mGameBoardWidth, this->mGameBoardHeight, 2,1);
            mPoints=0;
            mDifficulty=0;
            mDifficulty_Extra=0;
            mDelay=200;
            mBaseDelay=200;
            check_double=0;
            check_zero=0;
            sFood.clear();
            sTerrain.clear();
            createRandomFood();
            confusion=0;
            invincible=0;
            time_confusion=0;
            time_invincible=0;
            time_magnet=0;
            break;
        case 2:
            this->mPtrSnake = std::make_unique<Snake>(this->mGameBoardWidth, this->mGameBoardHeight, 6,2);
            mDelay=200;
            sFood_run.clear();
            sTerrain_run.clear();
            time_all_run=100000;
            lives=5;
            reviveCoins=5;
            victory=true;
            break;
        case 3:
            this->mPtrSnake = std::make_unique<Snake>(this->mGameBoardWidth, this->mGameBoardHeight, 6,2);
            mBaseDelay=200;
            bomb.clear();
            ammunition.clear();
            swamp.clear();
            soldier.clear();
            victory=true;
            livesBOSS=500;
            conditionBOSS=1;
            harm =3;
            critChance=0;
            harmBOSS=2;
            attackBossWay=1;
            boss=SnakeBody(mGameBoardWidth/2,mGameBoardHeight/2);
            weapon=string(1,'A' + (rand() % 3 + 1) -1);
            //weapon="A";
            armor=string(1,'A' + (rand() % 3 + 1) -1);
            //armor="C";
            jewelry=string(1,'A' + (rand() % 3 + 1) -1);
            //jewelry="A";
            if(weapon=="A")
                harm=6;
            if(armor=="C")
                harmBOSS=1;
            if(jewelry=="A")
                critChance=5;
            else
                critChance=2;
            time_confusion=0;
            time_invincible=0;
            time_temp1=0;
            time_temp2=0;
            time_ammuition=0;
            time_bomb=0;
            time_lowspeed=0;
            createRandomFood();
            if_crack=false;
            if_crack2=false;
            if_crack3=false;
            if_crack4=false;
            if_worked=false;
            if_worked2=false;
            if_worked3=false;
            if_worked4=false;
            time_mechanism=0;
            time_mechanism2=0;
            time_mechanism3=0;
            time_mechanism4=0;
            checkConfusion=0;
            orbit.clear();
            Box.clear();
            DoorLeft.clear();
            DoorRight.clear();
    }
}

void Battle::createRandomFood()         //需要加入一些判断
{
    while(true){
        int foodx=(rand() % (this->mGameBoardWidth-2))+1;
        int foody=(rand() % (this->mGameBoardHeight-2))+1;
        SnakeBody FOOD(foodx,foody);
        int check=0;

        for(SnakeBody x:this->mPtrSnake->getSnake()){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        if(mode==1){
            for(std::pair<char,SnakeBody> x:this->sTerrain){
                if(x.second==FOOD){
                    check=1;
                    break;
                }
            }
            for(std::pair<char,SnakeBody> x:this->sFood){
                if(x.second==FOOD){
                    check=1;
                    break;
                }
            }
        }
        else if(mode==3){
            if(FOOD==boss)
                check=1;
            if(FOOD==mFood)
                check=1;
            for(SnakeBody x:bomb){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:swamp){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:soldier){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(std::pair<SnakeBody,SnakeBody> x:ammunition){
                if(x.second==FOOD){
                    check=1;
                    break;
                }
            }
        }
        if(check==0){
            this->mFood=FOOD;
            this->mPtrSnake->senseFood(FOOD);
            return;
        }
        else
            continue;
    }
}

void Battle::createRandomFood_run()
{
    while(true){
        int foodx=(rand() % (this->mGameBoardWidth-10))+10;
        int foody=(rand() % (this->mGameBoardHeight-2))+1;
        SnakeBody FOOD(foodx,foody);
        int check=0;
        for(SnakeBody x:this->mPtrSnake->getSnake()){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        for(std::pair<char,SnakeBody> x:this->sTerrain_run){
            if(x.second==FOOD){
                check=1;
                break;
            }
        }
        for(SnakeBody x:this->sFood_run){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        if(check==0){
            sFood_run.push_back(FOOD);
            return;
        }
        else
            continue;
    }
}

void Battle::renderboss()
{
    mvwaddch(this->mWindows[1], this->boss.getY(), this->boss.getX(), '&');
    wrefresh(this->mWindows[1]);
}

void Battle::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wrefresh(this->mWindows[1]);
}

void Battle::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wrefresh(this->mWindows[1]);
}

void Battle::controlSnake()
{
    int key;
    key = getch();

    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        default:
        {
            break;
        }
    }
    if(mechanism==3){
        SnakeBody head=mPtrSnake->createNewHead();
        mPtrSnake->getSnake().pop_back();
        int i;
        for(i=0;i<Box.size();i++){
            if(Box[i]==head){
                Box[i]=mPtrSnake->createNewHead();
                mPtrSnake->getSnake().erase(mPtrSnake->getSnake().begin());
                break;
            }
        }
    }
    else if(this->mPtrSnake->moveFoward()){
        this->createRandomFood();
        if(mode==1){
            if(this->check_double>0){
                this->mPoints+=2;
                this->check_double--;
            }
            else if(this->check_zero>0){
                this->check_zero--;
            }
            else
                this->mPoints+=1;
        }
        else if(mode==3){
            lives+=2;
            this->mPtrSnake->getSnake().pop_back();
        }
    }


}

void Battle::control_mach2()
{
    int key;
    key = getch();

    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        default:
        {
            break;
        }
    }
    this->mPtrSnake->createNewHead();
    this->mPtrSnake->getSnake().pop_back();
}

void Battle::controlSnake_run()
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            this->mPtrSnake->moveFoward();
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            this->mPtrSnake->moveFoward();
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        default:
        {
            break;
        }
    }
    this->mPtrSnake->moveFoward();
}

void Battle::controlSnake_confusion()
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        default:
        {
            break;
        }
    }
    if(mechanism==3){
        SnakeBody head=mPtrSnake->createNewHead();
        mPtrSnake->getSnake().pop_back();
        int i;
        for(i=0;i<Box.size();i++){
            if(Box[i]==head){
                Box[i]=mPtrSnake->createNewHead();
                mPtrSnake->getSnake().erase(mPtrSnake->getSnake().begin());
                break;
            }
        }
    }
    else if(this->mPtrSnake->moveFoward()){
        this->createRandomFood();
        if(mode==1){
            if(this->check_double>0){
                this->mPoints+=2;
                this->check_double--;
            }
            else if(this->check_zero>0){
                this->check_zero--;
            }
            else
                this->mPoints+=1;
        }
        else if(mode==3){
            lives+=2;
            this->mPtrSnake->getSnake().pop_back();
        }
    }
}

void Battle::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    if(mode==1)
        this->renderLeaderBoard();
    if(mechanism==2)
        this->renderOrbit();
}

void Battle::adjustDelay()
{
    if(mode==3){
        this->mDifficulty = this->lives / 5 ;
        this->mDelay = this->mBaseDelay * pow(0.85,this->mDifficulty);
        return;
    }
    this->mDifficulty = this->mPoints / 5 + this->mDifficulty_Extra;
    if(mDifficulty<0)
        mDifficulty=0;
    this->mDelay = this->mBaseDelay * pow(0.85,this->mDifficulty);
}

void Battle::createRandomSpecialFood()
{
    while(true){
        int foodx=(rand() % (this->mGameBoardWidth-2))+1;
        int foody=(rand() % (this->mGameBoardHeight-2))+1;
        SnakeBody FOOD(foodx,foody);
        int check=0;
        for(SnakeBody x:this->mPtrSnake->getSnake()){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        if(FOOD==mFood)
            check=1;
        for(std::pair<char,SnakeBody> x:this->sTerrain){
            if(x.second==FOOD){
                check=1;
                break;
            }
        }
        if(check==0)
        {
            int kind=rand() % 4 +1;
            char food_kind;
            if(kind==1)
                food_kind='*';
            else if(kind==2)
                food_kind='$';
            else if(kind==3)
                food_kind='%';
            else
                food_kind='?';
            std::pair<char, SnakeBody> p1;
            p1 = std::make_pair(food_kind, FOOD);
            this->sFood.push_back(p1);
            return;
        }
        else
            continue;
    }
}

void Battle::createRandomSpecialTerrain()
{
    while(true){
        int foodx=(rand() % (this->mGameBoardWidth-2))+1;
        int foody=(rand() % (this->mGameBoardHeight-2))+1;
        SnakeBody FOOD(foodx,foody);
        int check=0;
        for(SnakeBody x:this->mPtrSnake->getSnake()){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        if(FOOD==mFood)
            check=1;
        for(std::pair<char,SnakeBody> x:this->sFood){
            if(x.second==FOOD){
                check=1;
                break;
            }
        }
        if(check==0)
        {
            int kind=rand() % 5 +1;
            char food_kind;
            if(kind==1)
                food_kind='~';
            else if(kind==2)
                food_kind='H';
            else if(kind==3)
                food_kind='h';
            else if(kind==4){
                food_kind='{';
                FOOD=SnakeBody(0,foody);
            }
            else{
                food_kind='}';
                FOOD=SnakeBody(mGameBoardWidth-1,foody);
            }

            std::pair<char, SnakeBody> p1;
            p1 = std::make_pair(food_kind, FOOD);
            this->sTerrain.push_back(p1);
            return;
        }
        else
            continue;
    }
}

void Battle::adjustScene()
{

    if(time1%5==0 && time1>=5){
        if(sFood.size()<3)
        {
            this->createRandomSpecialFood();
        }
        else if(time1%10==0 && time1>=10)
        {
            sFood.erase(sFood.begin());
            //this->createRandomSpecialFood();
        }
        time1++;
    }
    int len_max=mDifficulty*2;
    if(time2 %10==0 && time2>=10){
        if(sTerrain.size()<=len_max)
        {
            this->createRandomSpecialTerrain();
        }
        else if(time2%15==0 && time2>=15)
        {
            sTerrain.erase(sTerrain.begin());
        }
        time2++;
    }
}

void Battle::adjustSceneRun()
{
    if(time1%5==0 && time1>=5){
        createRandomFood_run();
        time1=0;
    }
    if(time2%5==0 && time2>=5){
        createRandomBomb();
        time2=0;
    }
    if(time4%13==0 && time4>=13){
        createWarning();
        time4=0;
    }
}

void Battle::adjustSceneCrucial()
{
    if(time1%10==0 && time1>=10 && time_weak<=0){
        bossAttack();
        time1=0;
    }
    if(armor=="A" && time4%10==0 && time4>=10){
        time_invincible=3000;
        time_confusion=0;
        time4=0;
    }

    if(conditionBOSS==2){
        if(time2%20==0 && time2>=20 ){
            moveboss();
            time2=0;
        }
    }
    if(time_ammuition>0){
        for(int i=0;i<8;i++){
            pair<SnakeBody,SnakeBody> p = make_pair(ammunition[i].first,ammunition[i].second);
            SnakeBody a=SnakeBody(p.second.getX()+p.first.getX()*ammu,p.second.getY()+p.first.getY()*ammu);
            SnakeBody b=SnakeBody(p.first.getX(),p.first.getY());
            pair<SnakeBody,SnakeBody> q =make_pair(b,a);
            ammunition.push_back(q);
        }
        ammu++;
    }
    if(checkConfusion>=50){
        time_confusion=15000;
        checkConfusion=0;
        time_invincible=0;
    }
    if(time_invincible>0){
        mDelay=50;
    }


}

void Battle::bossAttack()
{
    if(attackBossWay==1)
        way1();
    else if(attackBossWay==2)
        way2();
    else if(attackBossWay==3)
        way3();
    else
        way4();
    if(attackBossWay<4)
        attackBossWay++;
    else
        attackBossWay=1;
}

void Battle::way1()
{
    bomb.clear();
    time_warning=1500;
    time_bomb=7500;
    int num=30;
    while(num>0){
        while(true){
            int foodx=(rand() % (this->mGameBoardWidth-2))+1;
            int foody=(rand() % (this->mGameBoardHeight-2))+1;
            SnakeBody FOOD(foodx,foody);
            int check=0;
            for(SnakeBody x:this->mPtrSnake->getSnake()){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:this->bomb){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(pair<SnakeBody,SnakeBody> x:this->ammunition){
                if(x.second==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:this->swamp){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:this->soldier){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            if(boss==FOOD)
                check=1;
            if(FOOD==mFood)
                check=1;
            if(check==0){
                bomb.push_back(FOOD);
                break;
            }
            else
                continue;
        }
        num--;
    }
}

void Battle::way2()
{
    ammunition.clear();
    time_ammuition=6000;
    ammu=1;
    int bossx=boss.getX();
    int bossy=boss.getY();
    for(int i=-1;i<2;i++){
        for(int j=-1;j<2;j++){
            if(i==0 && j==0)
                continue;
            pair<SnakeBody,SnakeBody> p=make_pair(SnakeBody(i,j),SnakeBody(bossx+i,bossy+j));
            if(SnakeBody(bossx+i,bossy+j)==mFood)
                createRandomFood();
            ammunition.push_back(p);
        }
    }

}

void Battle::way3()
{
    swamp.clear();
    time_swamp=7000;
    int bossx=boss.getX();
    int bossy=boss.getY();
    SnakeBody center;
    int num=5;
    while(num>0){
        while(true){
            int foodx=(rand() % (this->mGameBoardWidth-3))+2;
            int foody=(rand() % (this->mGameBoardHeight-3))+2;
            center=SnakeBody(foodx,foody);
            int check=0;
            for(int i=-1;i<2;i++){
                if(check==0){
                    for(int j=-1;j<2;j++){
                        SnakeBody surr=SnakeBody(bossx+i,bossy+j);
                        if(surr==center){
                            check=1;
                            break;
                        }
                    }
                }
            }
            if(check==1)
                continue;
            else
                break;
        }
        int centerx=center.getX();
        int centery=center.getY();
        for(int i=-1;i<2;i++){
            for(int j=-1;j<2;j++){
                SnakeBody swampij=SnakeBody(centerx+i,centery+j);
                swamp.push_back(swampij);
            }
        }
        num--;
    }
}

void Battle::way4()
{

    int num=3;
    while(num>0){
        while(true){
            int foodx=(rand() % (this->mGameBoardWidth-2))+1;
            int foody=(rand() % (this->mGameBoardHeight-2))+1;
            SnakeBody FOOD(foodx,foody);
            int check=0;
            for(SnakeBody x:this->mPtrSnake->getSnake()){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:this->bomb){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(pair<SnakeBody,SnakeBody> x:this->ammunition){
                if(x.second==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:this->swamp){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:this->soldier){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            if(boss==FOOD)
                check=1;
            if(FOOD==mFood)
                check=1;
            if(check==0){
                soldier.push_back(FOOD);
                break;
            }
            else
                continue;
        }
        num--;
    }
}

void Battle::attack()
{
    if(this->mPtrSnake->getSnake()[0]==boss){
        if(lives>5){
            //lives-=5;
            livesBOSS-=harm*5;
            if(time_invincible<=0){
                lives-=5;
                checkConfusion+=5;
            }
            if(weapon=="B")
                time1-=5;
            if(jewelry=="C" && conditionBOSS==4)
                livesBOSS-=harm*5;
            int r=rand() % 10 + 1;
            if(r<=critChance)
                livesBOSS-=harm*5;

        }
        else if(time_invincible<=0){
            checkConfusion+=lives;
            lives=0;
        }
    }
    if(reserve>8000)
        lightAttack();

}

void Battle::lightAttack()
{

    light.clear();
    if(reserve>10000){
        reserve=0;
        return;
    }
    Direction dir=mPtrSnake->getdirection();
    int headx=mPtrSnake->getSnake()[0].getX();
    int heady=mPtrSnake->getSnake()[0].getY();
    if(dir==Direction::Down){
        while(heady<mGameBoardHeight){
            light.push_back(SnakeBody(headx,heady));
            heady++;
        }
    }
    else if(dir==Direction::Up){
        while(heady>0){
            light.push_back(SnakeBody(headx,heady));
            heady--;
        }
    }
    else if(dir==Direction::Left){
        while(headx>0){
            light.push_back(SnakeBody(headx,heady));
            headx--;
        }
    }
    else{
        while(headx<mGameBoardWidth){
            light.push_back(SnakeBody(headx,heady));
            headx++;
        }
    }
    for(SnakeBody x : light){
        mvwaddch(this->mWindows[1], x.getY(), x.getX(), '+');
        wrefresh(this->mWindows[1]);
        if(x==boss)
            livesBOSS-=1;
    }

}

void Battle::moveboss()
{
    while(true){
        int foodx=(rand() % (this->mGameBoardWidth-2))+1;
        int foody=(rand() % (this->mGameBoardHeight-2))+1;
        SnakeBody FOOD(foodx,foody);
        int check=0;
        for(SnakeBody x:this->mPtrSnake->getSnake()){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        if(FOOD==boss)
            check=1;
        if(FOOD==mFood)
            check=1;
        for(SnakeBody x:bomb){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        for(SnakeBody x:swamp){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        for(SnakeBody x:soldier){
            if(x==FOOD){
                check=1;
                break;
            }
        }
        for(std::pair<SnakeBody,SnakeBody> x:ammunition){
            if(x.second==FOOD){
                check=1;
                break;
            }
        }

        if(check==0){
            this->boss=FOOD;
            return;
        }
        else
            continue;
    }
}
void Battle::checkSFood()
{
    if(mode==1){
        for(int i=0;i<this->sFood.size();i++){
            std::pair<char,SnakeBody> x=this->sFood[i];
            if(x.second==this->mPtrSnake->getSnake()[0])
            {
                if(x.first=='*'){
                    int judge=rand() % 10 +1;
                    if(judge<=3)
                        this->mDifficulty_Extra++;
                        //else if(mDifficulty>0)
                    else
                        this->mDifficulty_Extra--;
                }
                else if(x.first=='$'){
                    int judge=rand() % 10 + 1;
                    if(judge<=4){
                        check_double=5;
                        check_zero=0;
                    }
                    else if(judge<=7){
                        check_zero=5;
                        check_double=0;
                    }
                    else{
                        this->time_magnet=20000;
                        this->mPtrSnake->getMagnetTime()=20000;
                    }
                }

                else if(x.first=='%'){
                    int judge=rand() % 10 +1;
                    if(judge<=3)
                        this->createRandomSpecialTerrain();
                    else
                        this->delRandSTerrain();
                }
                else
                    this->rand_operation();

                sFood.erase(sFood.begin()+i);
                return;
            }
        }
    }
    if(mode==2){
        for(int i=0;i<this->sFood_run.size();i++){
            SnakeBody x=this->sFood_run[i];
            if(x==this->mPtrSnake->getSnake()[0]){
                lives++;
                sFood_run.erase(sFood_run.begin()+i);
                return;
            }
        }
    }
}

void Battle::delRandSTerrain()
{
    int x=sTerrain.size();
    if(x==0)
        return;
    int y=rand() % x + 1;
    sTerrain.erase(sTerrain.begin()+y-1);
}

void Battle::rand_operation()
{
    int judge=rand() % 5 + 1;
    if(judge<=2){
        this->time_confusion=15000;
        this->time_invincible=0;
    }
    else{
        this->time_invincible=10000;
        this->time_confusion=0;
    }

}

void Battle::checkSTerrain()
{
    if(mode==1){
        for(int i=0;i<this->sTerrain.size();i++){
            std::pair<char,SnakeBody> x=this->sTerrain[i];
            if(x.second==this->mPtrSnake->getSnake()[0])
            {
                if(x.first=='~'){
                    this->time_invincible=5000;
                    this->time_confusion=0;
                }
                else if(x.first=='H'){
                    if(time_invincible<=0)
                        this->mPtrSnake->getSnake()[0]=SnakeBody(0,0);
                }

                else if(x.first=='h'){
                    sTerrain.erase(sTerrain.begin()+i);
                    i--;
                    this->mPtrSnake->getSnake().erase(this->mPtrSnake->getSnake().begin());
                    mPoints--;
                }
                else{
                    this->mPtrSnake->through();
                }
                return;
            }
        }
    }
    else if(mode==2){
        for(int i=0;i<sTerrain_run.size();i++){
            std::pair<char,SnakeBody> x=this->sTerrain_run[i];
            int px=x.second.getX();
            int py=x.second.getY();
            if(this->mPtrSnake->isPartOfSnake(px,py)){
                lives--;
                sTerrain_run.erase(sTerrain_run.begin()+i);
                i--;
                return;
            }
        }
    }
    else if(mode==3){
        if(time_invincible<=0){
            if(time_warning<=0 && time_bomb>0){
                for(int i=0;i<bomb.size();i++){
                    SnakeBody x=this->bomb[i];
                    int px=x.getX();
                    int py=x.getY();
                    if(this->mPtrSnake->isPartOfSnake(px,py)){
                        lives-=harmBOSS;
                        checkConfusion+=harmBOSS;
                        bomb.erase(bomb.begin()+i);
                        i--;
                    }
                }
            }
            if(time_ammuition>0){
                for(int i=0;i<ammunition.size();i++){
                    SnakeBody x=ammunition[i].second;
                    int px=x.getX();
                    int py=x.getY();
                    if(this->mPtrSnake->isPartOfSnake(px,py)){
                        lives-=harmBOSS;
                        checkConfusion+=harmBOSS;
                    }
                }
            }
            if(time_swamp>0){
                for(int i=0;i<swamp.size();i++){
                    SnakeBody x=this->swamp[i];
                    int px=x.getX();
                    int py=x.getY();
                    if(this->mPtrSnake->isPartOfSnake(px,py)){
                        time_lowspeed=5000;
                    }
                }
            }
        }
        for(int i=0;i<soldier.size();i++){
            SnakeBody x=this->soldier[i];
            int px=x.getX();
            int py=x.getY();
            if(this->mPtrSnake->isPartOfSnake(px,py)){
                if(armor=="B")
                    lives++;
                else if(time_invincible<=0){
                    lives-=harmBOSS;
                    checkConfusion+=harmBOSS;
                }
                soldier.erase(soldier.begin()+i);
                i--;
            }
        }
        if(time_lowspeed>0)
            mDelay=300;
        else if(time_invincible>0)
            mDelay=50;

    }
}

void Battle::moveScene()
{

    for(int i=0;i<sFood_run.size();i++){
        int x=sFood_run[i].getX()-1;

        if(x==0){
            sFood_run.erase(sFood_run.begin()+i);
            i--;
            continue;
        }
        int y=sFood_run[i].getY();
        sFood_run[i]=SnakeBody(x,y);
    }
    for(int i=0;i<sTerrain_run.size();i++){
        int x=sTerrain_run[i].second.getX()-1;
        if(x==0){
            sTerrain_run.erase(sTerrain_run.begin()+i);
            i--;
            continue;
        }
        int y=sTerrain_run[i].second.getY();
        pair<char,SnakeBody> p=make_pair(sTerrain_run[i].first,SnakeBody(x,y));
        sTerrain_run[i]=p;
    }
    for(int i=0;i<mPtrSnake->getSnake().size();i++){
        SnakeBody a=mPtrSnake->getSnake()[i];
        int x=a.getX()-1;
        mPtrSnake->getSnake()[i]=SnakeBody(x,a.getY());
    }
}

void Battle::createRandomBomb()
{
    int num=3;
    while(num>0){
        while(true){
            int foodx=(rand() % (this->mGameBoardWidth-10))+10;
            int foody=(rand() % (this->mGameBoardHeight-2))+1;
            SnakeBody FOOD(foodx,foody);
            int check=0;
            for(SnakeBody x:this->mPtrSnake->getSnake()){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(std::pair<char,SnakeBody> x:this->sTerrain_run){
                if(x.second==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:this->sFood_run){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            if(check==0){
                pair<char,SnakeBody> p=make_pair('*',FOOD);
                sTerrain_run.push_back(p);
                break;
            }
            else
                continue;
        }
        num--;
    }
}



void Battle::createWarning()
{
    time_warning=2000;
    time_light=5000;
    int x=(rand() % (this->mGameBoardHeight-2))+1;
    pos_light=x;
}

void Battle::renderWarning()
{
    mvwaddch(this->mWindows[1], pos_light, mGameBoardWidth-1, '!');
    wrefresh(this->mWindows[1]);
}

void Battle::renderLight()
{
    for(int i=0;i<mGameBoardWidth;i++){
        mvwaddch(this->mWindows[1], pos_light, i, '-');
        wrefresh(this->mWindows[1]);
    }
}

void Battle::checkLight()
{
    for(SnakeBody x:this->mPtrSnake->getSnake()){
        if(x.getY()==pos_light && time_light>0 &&time_warning<=0)
            lives=0;
    }
}
void Battle::runGame_main()
{
    bool moveSuccess;
    int key;
    this->time_begin=clock();
    time1=0;
    time2=0;
    while (true)
    {
        this->time_temp=clock();
        this->renderBoards();
        if(time_confusion>0)
            this->controlSnake_confusion();
        else
            this->controlSnake();
        if(time_invincible>0){
            this->mDelay=50;
        }
        else{
            this->adjustDelay();
        }
        this->checkSFood();
        this->checkSTerrain();
        this->renderSnake();
        this->renderFood();
        this->renderDifficulty();
        this->renderPoints();
        this->adjustScene();
        this->renderSFood();
        this->renderSTerrain();
        if(this->time_invincible <=0 && this->mPtrSnake->checkCollision())
            break;
        if(this->time_invincible >0 && this->mPtrSnake->hitWall())
            this->mPtrSnake->through();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        refresh();
        this->time_finish=clock();
        this->time1+=time_finish/1000-time_temp/1000;
        this->time2+=time_finish/1000-time_temp/1000;
        int time_this_step=time_finish-time_temp;
        if(time_magnet>0){
            time_magnet-=time_this_step;
            this->mPtrSnake->getMagnetTime()-=time_this_step;
        }
        if(time_confusion>0){
            time_confusion-=time_this_step;
        }
        if(time_invincible>0){
            time_invincible-=time_this_step;
        }
    }
}

void Battle::runGame_run()
{
    this->time_begin=clock();
    time1=0;
    time2=0;
    time4=0;
    while (true)
    {
        this->time_temp=clock();
        this->renderBoards();
        this->controlSnake_run();
        this->moveScene();
        this->adjustSceneRun();
        this->checkSFood();
        this->checkSTerrain();
        this->checkLight();
        this->renderSnake();
        this->renderSFood();
        this->renderSTerrain();
        this->renderLives();
        this->renderRevivecoins();
        if(time_warning>0)
            this->renderWarning();
        else if(time_light>0)
            this->renderLight();
        if(this->mPtrSnake->hitWall())
            lives=0;
        if(this->checkDeath()){
            if(reviveCoins==0){
                victory=false;
            }
            else if(weatherRevive()){
                lives=5;
                reviveCoins--;
                time_light=0;
            }
            else
                victory=false;
        }
        if(!victory)
            break;
        if(time_all_run<=0)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        refresh();
        this->time_finish=clock();
        this->time1+=time_finish/1000-time_temp/1000;
        this->time2+=time_finish/1000-time_temp/1000;

        this->time4+=time_finish/1000-time_temp/1000;
        int time_this_step=time_finish-time_temp;
        if(time_warning>0)
            time_warning-=time_this_step;
        if(time_light>0)
            time_light-=time_this_step;
        time_all_run-=time_this_step;
    }
}

void Battle::runGame_crucial()
{
    this->time_begin=clock();
    time1=0;
    time2=0;
    time4=0;
    while (true)
    {

        if(!if_worked && livesBOSS>300 && livesBOSS<=400){
            mechanism=1;
            conditionBOSS=3;
            if_worked=true;
        }
        else if(!if_worked2 && livesBOSS>200 && livesBOSS<=300){
            mechanism=2;
            conditionBOSS=3;
            if_worked2=true;
        }
        else if(!if_worked3 && livesBOSS>100 && livesBOSS<=200){
            mechanism=3;
            conditionBOSS=3;
            if_worked3=true;
        }
        else if(!if_worked4 && livesBOSS>0 && livesBOSS<=100){
            mechanism=4;
            conditionBOSS=3;
            if_worked4=true;
        }
        if(mechanism==5 && time_weak<=0){
            conditionBOSS=2;
            harmBOSS*=2;
            mechanism=6;
        }
        if(livesBOSS<=0)
            break;
        if(conditionBOSS!=3)
        {
            this->time_temp=clock();
            this->renderBoards();
            if(jewelry=="B" || time_confusion<=0)
                this->controlSnake();
            else
                this->controlSnake_confusion();
            this->adjustSceneCrucial();
            this->attack();
            //this->checkSFood();
            this->checkSTerrain();
            this->renderFood();
            this->renderammu();
            this->renderbomb();
            this->renderswamp();
            this->rendersoldier();
            this->renderboss();
            this->renderSnake();
            this->renderSFood();
            this->renderSTerrain();
            if(time_invincible<=0 && time_lowspeed<=0)
                this->adjustDelay();
            if(this->time_invincible <=0 && this->mPtrSnake->checkCollision()){
                checkConfusion+=lives;
                lives=0;
            }
            if(this->time_invincible >0 && this->mPtrSnake->hitWall())
                this->mPtrSnake->through();
            if(this->checkDeath()){
                if(reviveCoins==0){
                    victory=false;
                }
                else if(weatherRevive()){
                    lives=5;
                    reviveCoins--;
                }
                else
                    victory=false;
            }
            if(!victory)
                break;


            std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
            refresh();

            this->time_finish=clock();
            this->time_finish-=(time_temp2-time_temp1);
            time_temp1=0;
            time_temp2=0;
            if(time_weak<=0)
                this->time1+=time_finish/1000-time_temp/1000;
            this->time2+=time_finish/1000-time_temp/1000;
            this->time4+=time_finish/1000-time_temp/1000;
            int time_this_step=time_finish-time_temp;
            if(time_warning>0)
                time_warning-=time_this_step;
            if(time_bomb>0)
                time_bomb-=time_this_step;
            if(time_ammuition>0)
                time_ammuition-=time_this_step;
            if(time_swamp>0)
                time_swamp-=time_this_step;
            if(time_invincible>0)
                time_invincible-=time_this_step;
            if(time_lowspeed>0)
                time_lowspeed-=time_this_step;
            if(weapon=="C")
                reserve+=time_this_step;
            if(time_confusion>0){
                time_confusion-=time_this_step;
            }
            if(time_weak>0){
                time_weak-=time_this_step;
            }
            else if(mechanism==5){
                conditionBOSS=2;
                harmBOSS*=2;
                mechanism=6;
            }
            else if(mechanism!=6)
                conditionBOSS=1;

        }
        else if(mechanism==1)
        {
            specialMechanism1();
        }
        else if(mechanism==2)
        {
            specialMechanism2();
        }
        else if(mechanism==3)
        {
            specialMechanism3();
        }
        else if(mechanism==4)
        {
            specialMechanism4();
        }

    }
}

void Battle::specialMechanism1()
{
    int words=10;
    time_mechanism=10000;
    while(words>0 && time_mechanism>=0){
        int x1=clock();
        int a=rand() % 26 + 1;
        print_char= 'a' + a -1;
        this->renderBoards();
        int key=getch();
        while(key !=print_char){
            key=getch();
        }
        words--;
        int x2=clock();
        time_mechanism-=(x2-x1);
    }
    if(words==0)
        if_crack=true;
    if(if_crack){
        time_weak=20000;
        conditionBOSS=4;
        mechanism=0;
    }
    else{
        conditionBOSS=1;
        checkConfusion+=lives;
        lives=0;
        mechanism=0;
    }
    createRandomFood();
}

void Battle::specialMechanism2()
{
    time_invincible=0;
    time_confusion=0;
    time_mechanism2=50000;
    this->mPtrSnake = std::make_unique<Snake>(this->mGameBoardWidth, this->mGameBoardHeight, 6,2);
    createOrbit();
    while(this->mPtrSnake->getSnake()[0].getX()!=69 && time_mechanism2>=0){
        this->time_temp=clock();
        this->renderBoards();
        this->renderSnake();

        this->controlSnake();
        if(this->mPtrSnake->hitWall())
        {
            mPtrSnake->through();
        }
        SnakeBody head=mPtrSnake->getSnake()[0];
        if(head.getX()>=20){
            int index;
            for(index=0;index<orbit.size();index++){
                if(orbit[index].second==head)
                    break;
            }
            if(index==orbit.size())
                this->mPtrSnake = std::make_unique<Snake>(this->mGameBoardWidth, this->mGameBoardHeight, 6,2);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        refresh();
        this->time_finish=clock();
        this->time_finish-=(time_temp2-time_temp1);
        time_temp1=0;
        time_temp2=0;
        int time_this_step=time_finish-time_temp;
        time_mechanism2-=time_this_step;
    }
    if(this->mPtrSnake->getSnake()[0].getX()==69)
    {
        if_crack2=true;
    }
    if(if_crack2){
        time_weak=20000;
        conditionBOSS=4;
        mechanism=0;
    }
    else{
        conditionBOSS=1;
        checkConfusion+=lives;
        lives=0;
        mechanism=0;
    }
    createRandomFood();
}

void Battle::specialMechanism3()
{
    createBox();
    time_invincible=0;
    time_confusion=0;
    time_mechanism3=60000;
    while(!checkBox()  && time_mechanism3>=0)
    {
        this->time_temp=clock();
        this->renderBoards();
        this->controlSnake();
        this->renderboss();
        this->renderSnake();
        this->renderBox();
        if(this->mPtrSnake->hitWall())
        {
            mPtrSnake->through();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
        refresh();
        this->time_finish=clock();
        this->time_finish-=(time_temp2-time_temp1);
        time_temp1=0;
        time_temp2=0;
        int time_this_step=time_finish-time_temp;
        if(time_mechanism3>0)
            time_mechanism3-=time_this_step;
    }
    if(checkBox())
        if_crack3=true;
    if(if_crack3){
        time_weak=20000;
        conditionBOSS=4;
        mechanism=0;
    }
    else{
        conditionBOSS=1;
        checkConfusion+=lives;
        lives=0;
        mechanism=0;
    }
    createRandomFood();
}

void Battle::specialMechanism4()
{
    createDoor();
    time_invincible=0;
    time_confusion=0;
    time_mechanism4=60000;
    while(!checkDoor() && time_mechanism4>=0)
    {
        this->time_temp=clock();
        this->renderBoards();
        this->controlSnake();
        this->changeDoor();

        this->renderboss();
        this->renderSnake();
        this->renderDoor();

        if(this->mPtrSnake->hitWall())
        {
            mPtrSnake->through();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
        refresh();

        this->time_finish=clock();
        this->time_finish-=(time_temp2-time_temp1);
        time_temp1=0;
        time_temp2=0;
        int time_this_step=time_finish-time_temp;
        if(time_mechanism4>0)
            time_mechanism4-=time_this_step;
    }
    if(checkDoor())
        if_crack4=true;
    if(if_crack4){
        time_weak=20000;
        conditionBOSS=4;
        mechanism=5;
    }
    else{
        conditionBOSS=1;
        checkConfusion+=lives;
        lives=0;
        mechanism=5;
    }
    createRandomFood();
}

void Battle::createOrbit()
{
    int last;
    for(int i=0;i<5;i++){
        int y=rand() % (mGameBoardHeight-2) + 1;
        if(i!=0){
            int small = last < y ? last : y;
            int big = last > y ? last : y;

            for(int k = small;k<=big;k++){
                SnakeBody b = SnakeBody(20 + 10 * i,k);
                pair<char,SnakeBody> p = make_pair('~',b);
                orbit.push_back(p);
            }
        }
        last=y;
        for(int j=0;j<10;j++){
            SnakeBody a =SnakeBody(20+10*i+j,y);
            pair<char,SnakeBody> p = make_pair('~',a);
            orbit.push_back(p);
        }
    }
}

void Battle::createDoor()
{
    int i=0;
    while(i<10){
        while(true){
            int foodx=(rand() % (this->mGameBoardWidth-10))+5;
            int foody=(rand() % (this->mGameBoardHeight-10))+5;
            SnakeBody FOOD(foodx,foody);
            int check=0;
            if(FOOD==boss)
                check=1;
            for(SnakeBody x : mPtrSnake->getSnake()){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:DoorLeft){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            if(check==1)
                continue;
            else{
                DoorLeft.push_back(FOOD);
                break;
            }

        }
        i++;
    }
    i=0;
    while(i<10){
        while(true){
            int foodx=(rand() % (this->mGameBoardWidth-10))+5;
            int foody=(rand() % (this->mGameBoardHeight-10))+5;
            SnakeBody FOOD(foodx,foody);
            int check=0;
            if(FOOD==boss)
                check=1;
            for(SnakeBody x : mPtrSnake->getSnake()){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:DoorLeft){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x:DoorRight){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            if(check==1)
                continue;
            else{
                DoorRight.push_back(FOOD);
                break;
            }

        }
        i++;
    }
}

void Battle::changeDoor()
{
    SnakeBody head =mPtrSnake->getSnake()[0];
    for(int i=0;i<DoorLeft.size();i++){
        if(head == DoorLeft[i]){
            DoorLeft.erase(DoorLeft.begin()+i);
            DoorRight.push_back(head);
            return;
        }
    }
    for(int i=0;i<DoorRight.size();i++){
        if(head == DoorRight[i]){
            DoorRight.erase(DoorRight.begin()+i);
            DoorLeft.push_back(head);
            return;
        }
    }


}

void Battle::renderDoor() const
{
    for(SnakeBody p : DoorLeft){
        mvwaddch(this->mWindows[1], p.getY(), p.getX(), '[');
        wrefresh(this->mWindows[1]);
    }
    for(SnakeBody p : DoorRight){
        mvwaddch(this->mWindows[1], p.getY(), p.getX(), ']');
        wrefresh(this->mWindows[1]);
    }
}


void Battle::renderOrbit() const
{
    for(pair<char,SnakeBody> p : orbit){
        mvwaddch(this->mWindows[1], p.second.getY(), p.second.getX(), p.first);
        wrefresh(this->mWindows[1]);
    }
}

void Battle::createBox()
{
    int i=0;
    while(i<4){
        while(true){
            int foodx=(rand() % (this->mGameBoardWidth-10))+5;
            int foody=(rand() % (this->mGameBoardHeight-10))+5;
            SnakeBody FOOD(foodx,foody);
            int check=0;
            if(FOOD==boss)
                check=1;
            for(SnakeBody x : mPtrSnake->getSnake()){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            for(SnakeBody x : Box){
                if(x==FOOD){
                    check=1;
                    break;
                }
            }
            if(check==1)
                continue;
            else{
                Box.push_back(FOOD);
                break;
            }

        }
        i++;
    }
}

void Battle::renderBox() const
{

    for(SnakeBody p : Box){
        mvwaddch(this->mWindows[1], p.getY(), p.getX(), '$');
        wrefresh(this->mWindows[1]);
    }
}

bool Battle::checkBox()
{
    for(int i=0;i<Box.size();i++){
        if(Box[i]==boss){
            Box.erase(Box.begin()+i);
            i--;
        }
    }
    return Box.empty();
}

bool Battle::checkDoor()
{
    return (DoorLeft.empty() || DoorRight.empty());
}


bool Battle::checkDeath()
{
    if(lives<=0)
        return true;
    return false;
}

void Battle::renderSFood()
{
    if(mode==1){
        for(std::pair<char,SnakeBody> x:this->sFood){
            mvwaddch(this->mWindows[1], x.second.getY(), x.second.getX(), x.first);
            wrefresh(this->mWindows[1]);
        }
    }
    if(mode==2){
        for(SnakeBody x:sFood_run){
            mvwaddch(this->mWindows[1], x.getY(), x.getX(), '#');
            wrefresh(this->mWindows[1]);
        }

    }
}

void Battle::renderSTerrain()
{
    if(mode==1){
        for(std::pair<char,SnakeBody> x:this->sTerrain){
            mvwaddch(this->mWindows[1], x.second.getY(), x.second.getX(), x.first);
            wrefresh(this->mWindows[1]);
        }
    }
    if(mode==2){
        for(std::pair<char,SnakeBody> x:this->sTerrain_run){
            mvwaddch(this->mWindows[1], x.second.getY(), x.second.getX(), x.first);
            wrefresh(this->mWindows[1]);
        }
    }
}

void Battle::renderbomb()
{
    if(time_warning>0){
        for(SnakeBody x:bomb){
            mvwaddch(this->mWindows[1], x.getY(), x.getX(), '!');
            wrefresh(this->mWindows[1]);
        }
    }
    else if(time_bomb>0){
        for(SnakeBody x:bomb){
            mvwaddch(this->mWindows[1], x.getY(), x.getX(), '*');
            wrefresh(this->mWindows[1]);
        }
    }
}

void Battle::renderammu()
{
    if(time_ammuition>0){
        for(pair<SnakeBody,SnakeBody> x:ammunition){
            mvwaddch(this->mWindows[1], x.second.getY(), x.second.getX(), '-');
            wrefresh(this->mWindows[1]);
        }
    }

}

void Battle::renderswamp()
{
    if(time_swamp>0){
        for(SnakeBody x:swamp){
            mvwaddch(this->mWindows[1], x.getY(), x.getX(), '~');
            wrefresh(this->mWindows[1]);
        }
    }
}

void Battle::rendersoldier()
{
    for(SnakeBody x:soldier){
        mvwaddch(this->mWindows[1], x.getY(), x.getX(), '%');
        wrefresh(this->mWindows[1]);
    }
}

Status Battle::runGame()
{
    refresh();
    bool choice;
    while (true)
    {
        if(mode!=3){
            this->renderBoards();
            //this->selectModes();
            this->initializeGame();
        }
        if(mode==1){
            this->runGame_main();
            this->updateLeaderBoard();
            this->writeLeaderBoard();

            choice = Game::renderMenu(END_OF_THE_GAME);
            if (!choice)
            {
                return QUIT;
            }
        }
        else if(mode==2){
            this->runGame_run();
            choice = this->renderResult();
            if (!choice)
            {
                return QUIT;
            }
            // Where is Render restart menu?
        }
        else{
            this->initializeGame();
            this->renderBoards();
            this->runGame_crucial();
            choice = this->renderResult();
            if (!choice)
            {
                return QUIT;
            }
        }
    }
}