//
// Created by bitch on 6/29/22.
//
#include <cassert>
#include "game.h"

#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>

//Multi-thread
#include <atomic>


Game::Game()
{
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // cursor set
    curs_set(0);
    mousemask(BUTTON1_CLICKED | ALL_MOUSE_EVENTS, NULL);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

Solo::Solo()
{
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();
}

Double::Double()
{
    this->mGameBoardWidth = this->mScreenWidth / 2;
    this->createInformationBoard();
    this->createGameBoard();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Solo::renderInformationBoard() const
{
    /*box(mWindows[0], 0, 0);
    std::vector<std::string> information = {"Menu", "Mode", "Settings", "Customize", "Help", "Quit"};
    int whitespace = (mScreenWidth - 3) / information.size() ;
    for (int offset = 0; offset < information.size(); offset++)
    {
        mvwaddch(mWindows[0], 0, offset * whitespace, ACS_TTEE);
        mvwaddch(mWindows[0], 1,  offset * whitespace,  ACS_VLINE);
        mvwaddch(mWindows[0], 2, offset * whitespace, ACS_BTEE);
    }
    int choice = menuSelect(mWindows[0], information, 1, 1, whitespace, -1, false);
    wnoutrefresh(mWindows[0]);
    doupdate();

    switch (choice) {
        case -1: return NULL_INPUT;
        case 0: return MAIN_MENU;
        case 1: return MODE;
        case 2: return SETTINGS;
        case 3: return SETTINGS;
        case 4: return SETTINGS;
        case 5: return QUIT;
    }
    return NULL_INPUT;*/
    mvwprintw(mWindows[0], 1, 1, "Welcome to the snake game!");
    wnoutrefresh(mWindows[0]);
}


void Double::renderInformationBoard()
{
    mvwprintw(mWindows[0], 1, 1, "Time");
    wnoutrefresh(mWindows[0]);
}

void Solo::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Solo::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");

    wnoutrefresh(this->mWindows[2]);
}


void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 14, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i ++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 14 + (i + 1), 1, "%s", rank.c_str());
        mvwprintw(this->mWindows[2], 14 + (i + 1), 5, "%s", pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

Status Game::renderMenu(Status status)
{
    WINDOW * menu;
    float width, height, startX, startY;
    if (status != MAIN_MENU) {
        width = this->mGameBoardWidth * 0.5;
        height = this->mGameBoardHeight * 0.5;
        startX = this->mGameBoardWidth * 0.25;
        startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;
    }
    else{
        width = this->mScreenWidth * 0.618;
        height = this->mScreenHeight * 0.618;
        startX = this->mScreenWidth * (1 - 0.618) / 2;
        startY = this->mScreenHeight * (1 - 0.618) / 2;
    }

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"New Game", "Quit"};
    int index = 0, axis_x, axis_y, offset = 1;

    if (status == RESUME_GAME)
    {
        menuItems.insert(menuItems.begin() + 1, "Resume");
        menuItems.insert(menuItems.begin() + 2, "Main Menu");
        menuItems.insert(menuItems.begin() + 3, "Mode");
        menuItems.insert(menuItems.begin() + 4, "Settings");
        axis_x = (width - 8) / 2;
        axis_y = (height - 6) / 2;
    }

    if (status == MAIN_MENU)
    {
        for (int i = 0; i < mWindows.size(); i++){
            werase(mWindows[i]);
            wnoutrefresh(mWindows[i]);
        }
        doupdate();
        mvwprintw(menu, height * (1 - 0.8), (width - 24) / 2, "Welcome to the Snake Game!");
        menuItems.insert(menuItems.begin() + 1, "Mode");
        menuItems.insert(menuItems.begin() + 2, "Settings");
        axis_x = (width - 8) / 2;
        axis_y = (height - 4) / 2;
    }

    if (status == END_OF_THE_GAME)
    {
        axis_y = (height - 6) / 2 + 3;
        axis_x = (width - 16) / 2;
        menuItems.insert(menuItems.begin() + 1, "Main Menu");
        mvwprintw(menu, axis_y - 3, axis_x, "Your Final Score:");
        std::string pointString = std::to_string(this->mPoints);
        mvwprintw(menu, axis_y - 2, axis_x, "%s", pointString.c_str());
    }

    int size = menuItems.size();
    index = this->menuSelect(menu, menuItems, axis_y, axis_x, 1, 0);
    delwin(menu);

    if (status == RESUME_GAME)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    if (-1 < index && index < size && menuItems[index] == "New Game") {
        return NEW_GAME;
    } else if (-1 < index && index < size && menuItems[index] == "Resume") {
        return RESUME_GAME;
    } else if (index == -1){
        return ABNORMAL_EXIT;
    } else if (-1 < index && index < size && menuItems[index] == "Quit") {
        return QUIT;
    } else if (-1 < index && index < size && menuItems[index] == "Main Menu") {
        return MAIN_MENU;
    } else if (-1 < index && index < size && menuItems[index] == "Settings"){
        return SETTINGS;
    } else if (-1 < index && index < size && menuItems[index] == "Mode"){
        return MODE;
    }
    return ABNORMAL_EXIT;
}

void Game::renderSettings()
{

    const int size_of_settings = 3;
    float startX = this->mScreenWidth * (1 - 0.618) / 2;
    float startY = this->mScreenHeight * (1 - 0.618) / 2;
    WINDOW * setting = newwin(this->mScreenHeight * 0.618, this->mScreenWidth * 0.618, startY, startX);
    box(setting, 0, 0);

    std:std::vector<std::string> settings = {"Difficulty:", "Has Dynamic Difficulty:", "Has walls:"};

    while(true) {

        std::string difficulty = std::to_string(this->mDifficulty);
        mvwprintw(setting, startY + 1, startX, "%s", difficulty.c_str());
        std::string dynamic = (this->dynamic_difficulty) ? "On" : "Off";
        mvwprintw(setting, startY + 3, startX, "%s", dynamic.c_str());
        std::string has_walls = (this->has_walls) ? "On" : "Off";
        mvwprintw(setting, startY + 5, startX, "%s", has_walls.c_str());
        wnoutrefresh(setting);

        int index = this->menuSelect(setting, settings, startY, startX, 2, 0);


        int width = 6, height = 4;
        int choice;
        std::vector <std::string> tmp = {"On", "Off"};

        if (index == -1){
            werase(setting);
            wrefresh(setting);
            delwin(setting);
            return;
        }
        else if (index == 0) {
            WINDOW * setwin = newwin(3, mScreenWidth - 2, 0, 1);
            box(setwin, 0, 0);
            int whitespace = (mScreenWidth - 2) / 10;
            tmp = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
            choice = this->menuSelect(setwin, tmp, 1, 1, whitespace, 0, false);
            if (choice >= 0)
            {
                this->mDifficulty_init = choice;
                adjustDelay();
            }
            werase(setwin);
            wrefresh(setwin);
            delwin(setwin);
        }
        else if (index == 1)
        {
            WINDOW *setwin = newwin(height, width, 1, 1);
            box(setwin, 0, 0);
            if (this->dynamic_difficulty) {
                choice = this->menuSelect(setwin, tmp, 1, 1, 1, 0);
            } else {
                choice = this->menuSelect(setwin, tmp, 1, 1, 1, 1);
            }
            werase(setwin);
            wrefresh(setwin);
            delwin(setwin);
            if (choice) this->dynamic_difficulty = false;
            else this->dynamic_difficulty = true;
        }
        else if (index == 2)
        {
            WINDOW *setwin = newwin(height, width, 1, 1);
            box(setwin, 0, 0);
            if (this->has_walls) {
                choice = this->menuSelect(setwin, tmp, 1, 1, 1, 0);
            } else {
                choice = this->menuSelect(setwin, tmp, 1, 1, 1, 1);
            }
            werase(setwin);
            wrefresh(setwin);
            delwin(setwin);
            if (choice) this->has_walls = false;
            else this->has_walls = true;
        }
        werase(setting);
        box(setting, 0, 0);
        wrefresh(setting);
    }


}

void Game::renderMode() const
{
    std::vector<std::string> modes = {"Terrains", "Participants"};
    float startX = this->mScreenWidth * (1 - 0.618) / 2;
    float startY = this->mScreenHeight * (1 - 0.618) / 2;
    WINDOW * mode = newwin(this->mScreenHeight * 0.618, this->mScreenWidth * 0.618, startY, startX);
    box(mode, 0, 0);

    int axis_x = (this->mScreenWidth * 0.618 - 11) / 2;
    int axis_y = (this->mScreenHeight * 0.618 - modes.size()) / 2;

    int choice = menuSelect(mode, modes, axis_y, axis_x, 1, 0);
    if (choice == 0)
    {
        break;
    }
    else if (choice == 1)
    {
        int height = 4, width = 10;
        WINDOW *modewin = newwin(height, width, 1, 1);
        box(modewin, 0, 0);
        std::vector<std::string> participants = {"SOLO", "DOUBLE"};
        if (this->participants) {
            choice = this->menuSelect(modewin, participants, mScreenHeight/ 2, mScreenWidth / 2, 1, 0);
        } else {
            choice = this->menuSelect(modewin, participants, mScreenHeight / 2, mScreenWidth / 2, 1, 1);
        }
        werase(modewin);
        wrefresh(modewin);
        delwin(modewin);
        if (choice) this->participants = false;
        else this->participants = true;
    }

    werase(mode);
    wrefresh(mode);
    delwin(mode);


}
void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, "%s", pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Solo::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, "%s", difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

void Double::renderDifficulty() const
{
    std::string aDifficultyString = std::to_string(this->aDifficulty);
    std::string bDifficultyString = std::to_string(this->bDifficulty);
    mvwprintw(this->mWindows[0], 1, 1, "%s", "Difficulty: ");
    mvwprintw(this->mWindows[0], 1, 13, "%s", aDifficultyString.c_str());
    mvwprintw(this->mWindows[0], 1, mScreenWidth - 2 - 12, "%s", "Difficulty: ");
    mvwprintw(this->mWindows[0], 1, mScreenWidth - 2, "%s", bDifficultyString.c_str());
}

void Game::initializeGame()
{
    // allocate memory for a new snake
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));

    // TODO
    //1.initialize the game points as zero
    this->mPoints = 0;
    //2. create a food at random place
    //3.make the snake aware of the food
    this->mPtrSnake->senseFood(this->createRandomFood());
    //4.Adjust delay
    this->adjustDelay();
    //5.initialize the difficulty
    this->renderDifficulty();
    //6.Check if it has walls
    this->mPtrSnake->hasWalls(this->has_walls);
}

void Double::initializeGame()
{
    // allocate memory for a new snake
    this->aPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->bPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));

    //1.initialize the game points as zero
    this->aPoints = 0;
    this->bPoints = 0;
    //2. create a food at random place
    //3.make the snake aware of the food
    this->aPtrSnake->senseFood(this->createRandomFood());
    this->bPtrSnake->senseFood(this->createRandomFood());
    //4.Adjust delay
    this->adjustDelay();
    //5.initialize the difficulty
    this->renderDifficulty();
    //6.Check if it has walls
    this->aPtrSnake->hasWalls(this->has_walls);
    this->bPtrSnake->hasWalls(this->has_walls);
}
SnakeBody Game::createRandomFood()
{
    /* TODO
    * create a food at random places
    * make sure that the food doesn't overlap with the snake.
    */
    int width, height;
    do{
        width = rand() % (this->mGameBoardWidth - 2) + 1;
        height = rand() % (this->mGameBoardHeight - 2) + 1;
    }while(mPtrSnake->isPartOfSnake(width, height));
    SnakeBody food(width, height);
    return food;
}



void Solo::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mPtrSnake->mFood->getY(), this->mPtrSnake->mFood->getY(), ACS_DIAMOND);
    wnoutrefresh(this->mWindows[1]);
}

void Double::renderFood() const
{
    mvwaddch(this->mWindows[1], this->aPtrSnake->mFood->getY(), this->aPtrSnake->mFood->getY(), ACS_DIAMOND);
    wnoutrefresh(this->mWindows[1]);
    mvwaddch(this->mWindows[2],this->bPtrSnake->mFood->getY(), this->bPtrSnake->mFood->getY(), ACS_DIAMOND);
    wnoutrefresh(this->mWindows[2]);

}

void Solo::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), mSnakeSymbol);
    }
    wnoutrefresh(this->mWindows[1]);
}

void Double::renderSnake() const
{
    int aSnakeLength = this->aPtrSnake->getLength();
    std::vector<SnakeBody>& aSnake = this->aPtrSnake->getSnake();
    for (int i = 0; i < aSnakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], aSnake[i].getY(), aSnake[i].getX(), mSnakeSymbol);
    }
    wnoutrefresh(this->mWindows[1]);

    int bSnakeLength = this->bPtrSnake->getLength();
    std::vector<SnakeBody>& bSnake = this->bPtrSnake->getSnake();
    for (int i = 0; i < bSnakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], bSnake[i].getY(), bSnake[i].getX(), mSnakeSymbol);
    }
    wnoutrefresh(this->mWindows[2]);
}


bool Solo::controlSnake() const
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            // TODO change the direction of the snake.
            this->mPtrSnake->changeDirection(Direction::Up);
            return true;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            // TODO change the direction of the snake.

            this->mPtrSnake->changeDirection(Direction::Down);
            return true;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            // TODO change the direction of the snake.
            this->mPtrSnake->changeDirection(Direction::Left);
            return true;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            // TODO change the direction of the snake.
            this->mPtrSnake->changeDirection(Direction::Right);
            return true;
        }
        case ' ':
        case KEY_BACKSPACE:
        case 27:
        {
            return false;
        }
        default:
        {
            return true;
        }
    }
}

bool Double::controlSnake() const
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        {
            this->aPtrSnake->changeDirection(Direction::Up);
            return true;
        }
        case 'S':
        case 's':
        {
            this->aPtrSnake->changeDirection(Direction::Down);
            return true;
        }
        case 'A':
        case 'a':
        {
            this->aPtrSnake->changeDirection(Direction::Left);
            return true;
        }
        case 'D':
        case 'd':
        {
            this->aPtrSnake->changeDirection(Direction::Right);
            return true;
        }
        case KEY_UP:
        {
            this->bPtrSnake->changeDirection(Direction::Up);
            return true;
        }
        case KEY_DOWN:
        {
            this->bPtrSnake->changeDirection(Direction::Down);
            return true;
        }
        case KEY_LEFT:
        {
            this->bPtrSnake->changeDirection(Direction::Left);
            return true;
        }
        case KEY_RIGHT:
        {
            this->bPtrSnake->changeDirection(Direction::Right);
            return true;
        }
        case ' ':
        case KEY_BACKSPACE:
        case 27:
        {
            return false;
        }
        default:
        {
            return true;
        }

    }
}
void Game::renderBoards() const
{

    for (int i = 1; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 1; i < this->mWindows.size(); i ++)
    {
            box(this->mWindows[i], 0, 0);
            wnoutrefresh(mWindows[i]);
    }
    this->renderLeaderBoard();
    doupdate();
}

void Double::RenderBoards() const
{
    for (int i = 1; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderGameBoard();
    for (int i = 1; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wnoutrefresh(mWindows[i]);
    }
    this->renderLeaderBoard();
    doupdate();
}

void Game::adjustDelay()
{
    this->mDifficulty = this->mDifficulty_init + this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

void Double::adjustDelay()
{
    this->aDifficulty = this->aDifficulty_init + this->aPoints / 5;
    if (aPoints % 5 == 0)
    {
        this->aDelay = this->mBaseDelay * pow(0.75, this->aDifficulty);
    }
    this->bDifficulty = this->bDifficulty_init + this->bPoints / 5;
    if (bPoints % 5 == 0)
    {
        this->bDelay = this->mBaseDelay * pow(0.75, this->bDifficulty);
    }
}

Status Game::runGame()
{
    bool moveSuccess;
    int key = 0;
    while (true)
    {
         // 1. process your keyboard input
            if (!this->controlSnake()) return PAUSE_GAME;
         // 2. clear the window
            werase(mWindows[1]);
            box(mWindows[1], 0, 0);
         // 3. move the current snake forward
            moveSuccess = false;
            if (this->mPtrSnake->moveFoward()) moveSuccess = true;
         // 4. check if the snake has eaten the food after movement
         // 5. check if the snake dies after the movement
            if (this->mPtrSnake->checkCollision()) return END_OF_THE_GAME;
         // 6. make corresponding steps for the ``if conditions'' in 3 and 4.
            if (moveSuccess){
                this->mFood = this->createRandomFood();
                this->mPtrSnake->senseFood(this->mFood);
            }
         // 7. render the position of the food and snake in the new frame of window.
            this->renderFood();
            this->renderSnake();
            doupdate();
         // 8. update other game states and refresh the window
            this->renderPoints();
            this->renderDifficulty();
            if (moveSuccess) this->mPoints++;
            // refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
            if (dynamic_difficulty) this->adjustDelay();

    }
}


void Game::startGame() {

    refresh();
    int choice = this->renderMenu(MAIN_MENU);
    switch (choice) {
        case NEW_GAME:{
            if (this->participants)
            {
                Solo* solo;
                solo->runGame();
            }
            else
            {
                Double* battle;
                battle->runGame();
            }
            this->renderBoards();
            this->updateLeaderBoard();
            this->writeLeaderBoard();
            this->readLeaderBoard();
            this->initializeGame();
            choice = this->runGame();
            break;
        }
        case END_OF_THE_GAME: {
            choice = this->renderMenu(END_OF_THE_GAME);
            if (choice == ABNORMAL_EXIT) choice = MAIN_MENU;
            break;
        }
        case MODE: {
            break;
        }
        case SETTINGS: {
            this->renderSettings();
            choice = MAIN_MENU;
            break;
        }
        case MAIN_MENU: {
            choice = this->renderMenu(MAIN_MENU);
            if (choice == ABNORMAL_EXIT) choice = QUIT;
            continue;
        }
        case PAUSE_GAME:
        {
            choice = this->renderMenu(RESUME_GAME);
            if (choice == ABNORMAL_EXIT) choice = RESUME_GAME;
            continue;
        }
        case ABNORMAL_EXIT:{
            choice = QUIT;
            break;
        }
        case RESUME_GAME: {
            choice = this->runGame();
            continue;
        }
        case QUIT: {
            return;
        }
    }



}

int Game::menuSelect(WINDOW * menu, std::vector<std::string> lists, int axis_y, int axis_x, int whitespace, int init, bool direction)
{
    int key, size = lists.size(), index = 0, offset = 0;
    if (direction)
    {
        for (; offset < size; offset++) {
            if (offset != init) {
                mvwprintw(menu, axis_y + offset * whitespace, axis_x, "%s", lists[offset].c_str());
            } else {
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, axis_y + offset * whitespace, axis_x, "%s", lists[offset].c_str());
                wattroff(menu, A_STANDOUT);
            }
        }
    }
    else
    {
        for (; offset < size; offset++) {
            if (offset != init) {
                mvwprintw(menu, axis_y, axis_x + offset * whitespace, "%s", lists[offset].c_str());
            } else {
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, axis_y, axis_x + offset * whitespace, "%s", lists[offset].c_str());
                wattroff(menu, A_STANDOUT);
            }
        }
    }
    wattron(menu, A_STANDOUT);
    mvwaddch(menu, 1, getmaxx(menu) - 2, 'x');
    wattroff(menu, A_STANDOUT);


    wnoutrefresh(menu);

    while (true) {
        key = getch();
        switch (key) {
            case 'W':
            case 'w':
            case KEY_UP: {
                if (!direction) break;
                index--;
                index = (index < 0) ? size - 1 : index;
                for (offset = 0; offset < size; ++offset) {
                    if (offset != index) {
                        mvwprintw(menu, axis_y + offset * whitespace, axis_x, "%s", lists[offset].c_str());
                    } else {
                        wattron(menu, A_STANDOUT);
                        mvwprintw(menu, axis_y + offset * whitespace, axis_x, "%s", lists[offset].c_str());
                        wattroff(menu, A_STANDOUT);
                    }
                }
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN: {
                if (!direction) break;
                index++;
                index = (index > size - 1) ? 0 : index;
                for (offset = 0; offset < size; ++offset) {
                    if (offset != index) {
                        mvwprintw(menu, axis_y + offset * whitespace, axis_x, "%s", lists[offset].c_str());
                    } else {
                        wattron(menu, A_STANDOUT);
                        mvwprintw(menu, axis_y + offset * whitespace, axis_x, "%s", lists[offset].c_str());
                        wattroff(menu, A_STANDOUT);
                    }
                }
                break;
            }
            case 'A':
            case 'a':
            case KEY_LEFT: {
                if (direction) break;
                index--;
                index = (index < 0) ? size - 1 : index;
                for (offset = 0; offset < size; ++offset) {
                    if (offset != index) {
                        mvwprintw(menu, axis_y, axis_x + offset * whitespace, "%s", lists[offset].c_str());
                    } else {
                        wattron(menu, A_STANDOUT);
                        mvwprintw(menu, axis_y, axis_x + offset * whitespace, "%s", lists[offset].c_str());
                        wattroff(menu, A_STANDOUT);
                    }
                }
                break;
            }
            case 'D':
            case 'd':
            case KEY_RIGHT: {
                if (direction) break;
                index++;
                index = (index > size - 1) ? 0 : index;
                for (offset = 0; offset < size; ++offset) {
                    if (offset != index) {
                        mvwprintw(menu, axis_y, axis_x + offset * whitespace, "%s", lists[offset].c_str());
                    } else {
                        wattron(menu, A_STANDOUT);
                        mvwprintw(menu, axis_y, axis_x + offset * whitespace, "%s", lists[offset].c_str());
                        wattroff(menu, A_STANDOUT);
                    }
                }
                break;
            }
            case KEY_MOUSE: {
                if (getmouse(&event) == OK) {
                    int event_y = event.y, event_x = event.x;
                    if (event.bstate == BUTTON1_CLICKED && wmouse_trafo(menu, &event_y, &event_x, false)) {
                        if (event_x == (getmaxx(menu) - 2) && event_y == 1) {
                            return -1;
                        } else if (direction) {
                            if ((event_y - axis_y) % whitespace == 0) {
                                mvwprintw(menu, axis_y + index * whitespace, axis_x, "%s", lists[index].c_str());
                                index = (event_y - axis_y) / whitespace;
                                wattron(menu, A_STANDOUT);
                                mvwprintw(menu, event_y, axis_x, "%s", lists[index].c_str());
                                wattroff(menu, A_STANDOUT);
                                wnoutrefresh(menu);
                                doupdate();
                                return index;
                            }
                            break;
                        } else {
                            if ((event_x - axis_x) % whitespace == 0) {
                                mvwprintw(menu, axis_y, axis_x + index * whitespace, "%s", lists[index].c_str());
                                index = (event_x - axis_x) / whitespace;
                                wattron(menu, A_STANDOUT);
                                mvwprintw(menu, axis_y, event_x, "%s", lists[index].c_str());
                                wattroff(menu, A_STANDOUT);
                                wnoutrefresh(menu);
                                doupdate();
                                return index;
                            }
                        }
                    }
                    /*if (event.bstate == ALL_MOUSE_EVENTS && wmouse_trafo(menu, &event_y, &event_x, false))
                    {
                        if (direction)
                        {
                            if ((event_y - axis_y) % whitespace == 0)
                            {
                                mvwprintw(menu, axis_y + index * whitespace, axis_x, "%s", lists[index].c_str());
                                index = (event_y - axis_y) / whitespace;
                                wattron(menu, A_STANDOUT);
                                mvwprintw(menu, event_y, axis_x, "%s", lists[offset].c_str());
                                wattroff(menu, A_STANDOUT);
                                wrefresh(menu);

                            }
                            break;
                        }
                        else
                        {
                            if((event_x - axis_x) % whitespace == 0)
                            {
                                mvwprintw(menu, axis_y, axis_x + index * whitespace, "%s", lists[index].c_str());
                                index = (event_x - axis_x) / whitespace;
                                wattron(menu, A_STANDOUT);
                                mvwprintw(menu, event_y, axis_x, "%s", lists[index].c_str());
                                wattroff(menu, A_STANDOUT);
                            }
                        }
                    }*/
                }
            }
        }

        wnoutrefresh(menu);
        doupdate();

        if (key == 10) {
            break;
        } else if (key == KEY_BACKSPACE || key == 27 || key == ' ') {
            return -1;
        }
    }
    return index;
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open()){

        return false;

    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;

    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}

void Solo::createGameBoard() const
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
};

void Double::createGameBoard() const
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth / 2, startY, startX);
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth / 2, startY, startX);
}

void Solo::renderGameBoard() const
{
    wnoutrefresh(mWindows[1]);
}

void Double::renderGameBoard() const
{
    wnoutrefresh(mWindows[1]);
    wnoutrefresh(mWindows[2]);
}

