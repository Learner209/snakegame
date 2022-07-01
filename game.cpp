//
// Created by bitch on 6/29/22.
//

#include "game.h"

#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>


Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

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

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, " MingHao Liu");
    mvwprintw(this->mWindows[0], 3, 1, " ");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wnoutrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wnoutrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
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

Status Game::renderMenu(Status status) const
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
        mvwprintw(menu, 6, (width - 24) / 2, "Welcome to the Snake Game!");
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

    if (index < size && menuItems[index] == "New Game") {
        return NEW_GAME;
    } else if (index < size && menuItems[index] == "Resume") {
        return RESUME_GAME;
    } else if (index < size && menuItems[index] == "Quit") {
        return QUIT;
    } else if (index < size && menuItems[index] == "Main Menu") {
        return MAIN_MENU;
    } else if (index < size && menuItems[index] == "Settings"){
        return SETTINGS;
    } else if (index < size && menuItems[index] == "Mode"){
        return MODE;
    }
    return QUIT;
}

void Game::renderSettings()
{

    const int size_of_settings = 3;
    float startX = this->mScreenWidth * (1 - 0.618) / 2;
    float startY = this->mScreenHeight * (1 - 0.618) / 2;
    WINDOW * setting = newwin(this->mScreenHeight * 0.618, this->mScreenWidth * 0.618, startY, startX);
    box(setting, 0, 0);

    std:std::vector<std::string> settings = {"Difficulty:", "Dynamic Difficulty:", "Has walls:"};

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

    }


}
void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, "%s", pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, "%s", difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::initializeGame()
{
    // allocate memory for a new snake
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));


    // TODO
    //1.initialize the game points as zero
    this->mPoints = 0;
    //2. create a food at random place
    this->createRandomFood();
    //3.make the snake aware of the food
    this->mPtrSnake->senseFood(this->mFood);
    //4.initialize the difficulty
    this->renderDifficulty();
    //5.Adjust delay
    this->adjustDelay();
    //6.Check if it has walls
    this->mPtrSnake->hasWalls(this->has_walls);
}

void Game::createRandomFood()
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
    this->mFood = food;
}

void Game::renderFood() const
{
    mvwaddch(this->mWindows[1],mFood.getY(), mFood.getX(), ACS_DIAMOND);
    wnoutrefresh(this->mWindows[1]);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), mSnakeSymbol);
    }
    wnoutrefresh(this->mWindows[1]);
}

bool Game::controlSnake() const
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

void Game::renderBoards() const
{

    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    doupdate();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
    }
    this->renderLeaderBoard();
}


void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

void Game::adjustDifficulty(int difficulty)
{
    this->mDifficulty = difficulty;
}

Status Game::runGame()
{
    bool moveSuccess;
    int key = 0;
    while (true)
    {
        /* TODO
         * this is the main control loop of the game.
         * it keeps running a while loop, and does the following things:*/
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
                this->createRandomFood();
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
    while (true) {
        switch (choice) {
            case NEW_GAME:{
                this->updateLeaderBoard();
                this->writeLeaderBoard();
                this->readLeaderBoard();
                this->renderBoards();
                this->initializeGame();
                choice = this->runGame();
                break;
            }
            case END_OF_THE_GAME: {
                choice = this->renderMenu(END_OF_THE_GAME);
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
                continue;
            }
            case PAUSE_GAME:
            {
                choice = this->renderMenu(RESUME_GAME);
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
}
int Game::menuSelect(WINDOW * menu, std::vector<std::string> lists, int axis_y, int axis_x, int line, int init) const
{
    int key, size = lists.size(), index = 0, offset = 0;
    for (;offset < lists.size(); offset++){
        if (offset != init)
        {
            mvwprintw(menu, axis_y + offset * line, axis_x, "%s", lists[offset].c_str());
        }
        else
        {
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, axis_y + offset * line, axis_x, "%s", lists[offset].c_str());
            wattroff(menu, A_STANDOUT);
        }
    }

    for (;offset < lists.size(); offset++){
        mvwprintw(menu, axis_y + offset, axis_x, "%s", lists[offset].c_str());
    }
    wnoutrefresh(menu);


    while (true) {
        key = getch();
        switch (key) {
            case 'W':
            case 'w':
            case KEY_UP: {
                index--;
                index = (index < 0) ? size - 1 : index;
                for (offset = 0; offset < size; ++offset) {
                    if (offset != index) {
                        mvwprintw(menu, axis_y + offset * line, axis_x, "%s", lists[offset].c_str());
                    } else {
                        wattron(menu, A_STANDOUT);
                        mvwprintw(menu, axis_y + offset * line, axis_x, "%s", lists[offset].c_str());
                        wattroff(menu, A_STANDOUT);
                    }
                }
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN: {
                index++;
                index = (index > size - 1) ? 0 : index;
                for (offset = 0; offset < size; ++offset) {
                    if (offset != index) {
                        mvwprintw(menu, axis_y + offset * line, axis_x, "%s", lists[offset].c_str());
                    } else {
                        wattron(menu, A_STANDOUT);
                        mvwprintw(menu, axis_y + offset * line, axis_x, "%s", lists[offset].c_str());
                        wattroff(menu, A_STANDOUT);
                    }
                }
                break;
            }
        }

        wnoutrefresh(menu);
        doupdate();

        if (key == 10) {
            break;
        }
        if (key == KEY_BACKSPACE || key == 27 || key == ' ')
        {
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
