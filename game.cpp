#include <cassert>
#include "game.h"

#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>


bool Game::participants = true;
bool Game::dynamic_difficulty = true;
bool Game::has_walls = true;
int Double::Difficulty = 0;

void init(){
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // cursor set
    curs_set(0);
    mousemask(BUTTON1_CLICKED | REPORT_MOUSE_POSITION, NULL);
}

Game::Game()
{
    this->mWindows.resize(3);
    
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
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


Solo::Solo():Game()
{
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    Game::createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();
}

Double::Double():Game()
{
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;
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

void Double::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}
void Solo::renderInformationBoard() const
{
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

void Solo::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
};

void Double::createGameBoard()
{

    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth / 2, startY, startX);
    if (mScreenWidth % 2 == 0)
    {
        this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth / 2, startY, this->mScreenWidth / 2);
    }
    else
    {
        this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth / 2, startY, this->mScreenWidth / 2 + 1);
    }
}

void Solo::renderGameBoard() const
{
    wnoutrefresh(mWindows[1]);
}

void Double::renderGameBoard() const
{
    wnoutrefresh(mWindows[1]);
    wnoutrefresh(mWindows[2]);
    doupdate();
}

void Solo::renderBoards()
{

    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderGameBoard();
    this->renderInstructionBoard();
    this->renderInformationBoard();

    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wnoutrefresh(mWindows[i]);
    }
    this->renderLeaderBoard();
    doupdate();
}

void Double::renderBoards()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderGameBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wnoutrefresh(mWindows[i]);
    }
    doupdate();

}

Status Game::renderMenu(Status status, int delay)
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

    werase(menu);
    wrefresh(menu);
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

Status Double::renderMenu(Status status, int delay)
{
    WINDOW * menu;
    float width, height, startX, startY;

    if (status == aEND_OF_THE_GAME) {
        width = this->mGameBoardWidth * 0.8;
        height = this->mGameBoardHeight * 0.5;
        startX = this->mGameBoardWidth * 0.1;
        startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;
    }
    else if(status == bEND_OF_THE_GAME)
    {
        width = this->mGameBoardWidth * 0.8;
        height = this->mGameBoardHeight * 0.5;
        startX = this->mGameBoardWidth * 0.1 + this->mScreenWidth / 2;
        startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;
    }
    else
    {
        width = this->mScreenWidth * 0.618;
        height = this->mScreenHeight * 0.618;
        startX = this->mScreenWidth * (1 - 0.618) / 2;
        startY = this->mScreenHeight * (1 - 0.618) / 2;
    }


    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);

    std::vector<std::string> menuItems = {"New Game", "Quit"};
    int index = 0, axis_x, axis_y, offset = 1;

    switch (status) {
        case RESUME_GAME:
        {
            menuItems.insert(menuItems.begin() + 1, "Resume");
            menuItems.insert(menuItems.begin() + 2, "Main Menu");
            menuItems.insert(menuItems.begin() + 3, "Mode");
            menuItems.insert(menuItems.begin() + 4, "Settings");
            axis_x = (width - 8) / 2;
            axis_y = (height - 6) / 2;
            break;
        }
        case MAIN_MENU:
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
            break;
        }
        case END_OF_THE_GAME:
        {
            axis_y = (height - 6) / 2 + 3;
            axis_x = (width / 2 - 16) / 2 + 16;
            menuItems.insert(menuItems.begin() + 1, "Main Menu");
            std::string aPoint = std::to_string(this->aPoints);
            std::string bPoint = std::to_string(this->bPoints);

            mvwprintw(menu, axis_y - 3, axis_x - 16, (this->manToMachine) ? "Player's Score": "Player A's Score:");
            mvwprintw(menu, axis_y - 3, axis_x + width / 2 - 1 - 16, (this->manToMachine) ? "Computer's Score": "Player B's Score:");
            mvwprintw(menu, axis_y - 2, axis_x - 16, "%s", aPoint.c_str());
            mvwprintw(menu, axis_y - 2, axis_x + width / 2 - 1 - 16, "%s", bPoint.c_str());
            break;
        }
        case aEND_OF_THE_GAME:
        {
            axis_y = (height - 5) / 2 + 3;
            mvwprintw(menu, axis_y - 3, 3, (this->manToMachine) ? "Player's Score": "Player A's Score:");
            std::string aPoint = std::to_string(this->aPoints);
            mvwprintw(menu, axis_y - 2, 3, "%s", aPoint.c_str());
            mvwprintw(menu, axis_y, 3, "%s", "Do me a favor...");
            mvwprintw(menu, axis_y + 1, 3, "%s", "Perform better next time?");
            wrefresh(menu);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            break;
        }
        case bEND_OF_THE_GAME:
        {
            axis_y = (height - 5) / 2 + 3;
            mvwprintw(menu, axis_y - 3, 3, (this->manToMachine) ? "Computer's Score": "Player B's Score:");
            std::string bPoint = std::to_string(this->bPoints);
            mvwprintw(menu, axis_y - 2, 3, "%s", bPoint.c_str());
            mvwprintw(menu, axis_y, 3, "%s", (this->manToMachine) ? "Oops...": "Do me a favor...");
            mvwprintw(menu, axis_y + 1, 3, "%s", (this->manToMachine) ? "Man beat Computer? Unbelievable..." : "Perform better next time?");
            wrefresh(menu);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            break;
        }
    }

    if (status == aEND_OF_THE_GAME || status == bEND_OF_THE_GAME)
    {
        return ABNORMAL_EXIT;
    }

    index = this->menuSelect(menu, menuItems, axis_y, axis_x, 1, 0);

    werase(menu);
    wrefresh(menu);
    delwin(menu);

    int size = menuItems.size();
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
    float startX = this->mScreenWidth * (1 - 0.618) / 2;
    float startY = this->mScreenHeight * (1 - 0.618) / 2;
    WINDOW * setting = newwin(this->mScreenHeight * 0.618, this->mScreenWidth * 0.618, startY, startX);
    box(setting, 0, 0);

    std:std::vector<std::string> settings = {"Difficulty:", "Has Dynamic Difficulty:", "Has walls:"};

    while(true) {

        std::string difficulty = std::to_string(this->mDifficulty);
        mvwprintw(setting, startY + 1, startX, "%s", difficulty.c_str());
        std::string dynamic = (Game::dynamic_difficulty) ? "On" : "Off";
        mvwprintw(setting, startY + 3, startX, "%s", dynamic.c_str());
        std::string has_walls = (Game::has_walls) ? "On" : "Off";
        mvwprintw(setting, startY + 5, startX, "%s", has_walls.c_str());
        wnoutrefresh(setting);

        int index = this->menuSelect(setting, settings, startY, startX, 2, 0);

        int choice;
        std::vector <std::string> tmp = {"On", "Off"};

        if (index == -1){
            werase(setting);
            wrefresh(setting);
            delwin(setting);
            return;
        }
        else if (index == 0) {
            WINDOW * setWin = newwin(3, mScreenWidth - 2, 0, 1);
            box(setWin, 0, 0);
            int whitespace = (mScreenWidth - 2) / 10;
            tmp = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
            choice = this->menuSelect(setWin, tmp, 1, 1, whitespace, 0, false);
            if (choice >= 0)
            {
                this->mDifficulty_init = choice;
                adjustDelay();
            }
            werase(setWin);
            wrefresh(setWin);
            delwin(setWin);
        }
        else if (index == 1)
        {
            WINDOW *setWin = newwin(7, 10, (mScreenHeight - 7) / 2, (mScreenWidth - 10) / 2);
            box(setWin, 0, 0);
            if (Game::dynamic_difficulty) {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 0);
            } else {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 1);
            }
            werase(setWin);
            wrefresh(setWin);
            delwin(setWin);
            if (choice) Game::dynamic_difficulty = false;
            else Game::dynamic_difficulty = true;
        }
        else if (index == 2)
        {
            WINDOW *setWin = newwin(7, 10, (mScreenHeight - 7) / 2, (mScreenWidth - 10) / 2);
            box(setWin, 0, 0);
            if (Game::has_walls) {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 0);
            } else {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 1);
            }
            werase(setWin);
            wrefresh(setWin);
            delwin(setWin);
            if (choice) Game::has_walls = false;
            else Game::has_walls = true;
        }
        werase(setting);
        box(setting, 0, 0);
        wrefresh(setting);
    }


}

void Double::renderSettings()
{
    float startX = this->mScreenWidth * (1 - 0.618) / 2;
    float startY = this->mScreenHeight * (1 - 0.618) / 2;
    WINDOW * setting = newwin(this->mScreenHeight * 0.618, this->mScreenWidth * 0.618, startY, startX);
    box(setting, 0, 0);

    float axis_y = (this->manToMachine) ? (this->mScreenHeight * 0.618 - 10) / 2 : (this->mScreenHeight * 0.618 - 8) / 2;
    float axis_x = (this->mScreenWidth * 0.618 - 12) / 2;
    std:std::vector<std::string> settings = {"Speed:", "Countdown", "Has walls:", "Man to ?:"};

    while(true) {

        std::string mDifficulty = std::to_string(Double::Difficulty);
        std::string countdown = std::to_string(mCountdown / 60) + ": ";
        countdown += ((this->mCountdown - this->mCountdown / 60 * 60) < 10) ? "0" + std::to_string(this->mCountdown - this->mCountdown / 60 * 60):
                     std::to_string(this->mCountdown - this->mCountdown / 60 * 60);
        std::string has_walls = (Game::has_walls) ? "ON" : "OFF";
        std::string manToMachine = (this->manToMachine) ? "Man to Computer": "Man to Man";

        if (this->manToMachine)
        {
            settings.push_back("Difficulty:");
        }

        if (this->manToMachine)
        {
            std::string smartness;
            switch ((this->machineSmartness)) {
                case 0:{
                    smartness = "Hard";
                    break;
                }
                case 1:{
                    smartness = "Medium";
                    break;
                }
                case 2:{
                    smartness = "Normal";
                    break;
                }
                case 3:{
                    smartness = "Easy";
                    break;
                }
            }
            mvwprintw(setting, axis_y + 9, axis_x, "%s", smartness.c_str());
        }

        mvwprintw(setting, axis_y + 1, axis_x, "%s", mDifficulty.c_str());
        mvwprintw(setting, axis_y + 3, axis_x, "%s", countdown.c_str());
        mvwprintw(setting, axis_y + 5, axis_x, "%s", has_walls.c_str());
        mvwprintw(setting, axis_y + 7, axis_x, "%s", manToMachine.c_str());
        wnoutrefresh(setting);

        int index = this->menuSelect(setting, settings, axis_y, axis_x, 2, 0);

        int choice;
        std::vector <std::string> tmp = {"On", "Off"};

        if (index == -1)
        {
            werase(setting);
            wrefresh(setting);
            delwin(setting);
            return;
        }
        else if (index == 0)
        {
            WINDOW * setWin = newwin(3, mScreenWidth - 2, 0, 1);
            box(setWin, 0, 0);
            int whitespace = (mScreenWidth - 2) / 10;
            tmp = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
            choice = this->menuSelect(setWin, tmp, 1, 1, whitespace, 0, false);
            if (choice >= 0)
            {
                Double::Difficulty = choice;
            }
            werase(setWin);
            wrefresh(setWin);
            delwin(setWin);
        }
        else if (index == 1)
        {

            WINDOW * countdownWin = newwin(5, 10, (mScreenHeight - 5) / 2, (mScreenWidth - 8) / 2);
            box(countdownWin, 0, 0);

            int minutes = this->mCountdown / 60, tenths = (this->mCountdown - this->mCountdown / 60 * 60) / 10;
            int digit = this->mCountdown - 60 * minutes - 10 * tenths;
            std::vector<int> original = {minutes, tenths, digit};
            std::vector<int> positions = {3, 5, 6};

            mvwaddch(countdownWin, 2, 4, ':');
            this->numSelect(countdownWin, original, positions, 2, 0);
            this->mCountdown = original[0] * 60 + original[1] * 10 + original[2];

            werase(countdownWin);
            wrefresh(countdownWin);
            delwin(countdownWin);
        }
        else if (index == 2)
        {
            WINDOW *setWin = newwin(7, 10, (mScreenHeight - 7) / 2, (mScreenWidth - 10) / 2);
            box(setWin, 0, 0);
            if (Game::has_walls) {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 0);
            } else {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 1);
            }
            werase(setWin);
            wrefresh(setWin);
            delwin(setWin);
            if (choice) Game::has_walls = false;
            else Game::has_walls = true;
        }
        else if (index == 3)
        {
            tmp = {"Man to Man", "Man to Computer"};
            WINDOW *setWin = newwin(7, 20, (mScreenHeight - 7) / 2, (mScreenWidth - 15) / 2);
            box(setWin, 0, 0);
            if (!this->manToMachine) {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 0);
            } else {
                choice = this->menuSelect(setWin, tmp, 2, 3, 2, 1);
            }
            werase(setWin);
            wrefresh(setWin);
            delwin(setWin);
            if (choice) this->manToMachine = true;
            else this->manToMachine = false;
        }
        else if (index == 4)
        {
            tmp = {"Easy", "Normal", "Medium", "Hard"};
            WINDOW *setWin = newwin(8, 20, (mScreenHeight - 7) / 2, (mScreenWidth - 6) / 2);
            box(setWin, 0, 0);
            switch (this->machineSmartness) {
                case 0:
                {
                    choice = this->menuSelect(setWin, tmp, 2, 7, 1, 3);
                    break;
                }
                case 1:
                {
                    choice = this->menuSelect(setWin, tmp, 2, 7, 1, 2);
                    break;
                }
                case 2:
                {
                    choice = this->menuSelect(setWin, tmp, 2, 7, 1, 1);
                    break;
                }
                case 3:
                {
                    choice = this->menuSelect(setWin, tmp, 2, 7, 1, 0);
                    break;
                }
            }
            werase(setWin);
            wrefresh(setWin);
            delwin(setWin);
            switch (choice) {
                case 0:
                {
                  this->machineSmartness = 3;
                  break;
                }
                case 1:
                {
                    this->machineSmartness = 2;
                    break;
                }
                case 2:
                {
                    this->machineSmartness = 1;
                    break;
                }
                case 3:
                {
                    this->machineSmartness = 0;
                    break;
                }
            }
        }
        werase(setting);
        box(setting, 0, 0);
        wrefresh(setting);
    }


}

void Game::renderMode()
{
    int choice = 0;

    std::vector<std::string> modes = {"Terrains:", "Participants:"};
    float startX = this->mScreenWidth * (1 - 0.618) / 2;
    float startY = this->mScreenHeight * (1 - 0.618) / 2;
    WINDOW * mode = newwin(this->mScreenHeight * 0.618, this->mScreenWidth * 0.618, startY, startX);

    box(mode, 0, 0);

    int axis_x = (this->mScreenWidth * 0.618 - 11) / 2;
    int axis_y = (this->mScreenHeight * 0.618 - 4) / 2;

    while(true) {

        terrain type = Terrains[indexTerrain];
        std::string terrain;
        switch(type)
        {
            case terrain::Plain:
            {
                terrain = "Plain";
                break;
            }
            case terrain::Water:
            {
                terrain = "Water";
                break;
            }
            case terrain::Mountain:
            {
                terrain = "Mountain";
                break;
            }
            case terrain::Forest:
            {
                terrain = "Forest";
                break;
            }
            case terrain::Maze:
            {
                terrain = "Maze";
                break;
            }
        }
        mvwprintw(mode, axis_y + 1, axis_x, "%s", terrain.c_str());
        std::string participants = (Game::participants) ? "SOLO" : "DOUBLE";
        mvwprintw(mode, axis_y + 3, axis_x, "%s", participants.c_str());
        wnoutrefresh(mode);

        choice = this->menuSelect(mode, modes, axis_y, axis_x, 2, 0);

        if (choice == -1){
            werase(mode);
            wrefresh(mode);
            delwin(mode);
            return;
        }
        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            int height = (mScreenHeight - 5) / 2, width = (mScreenWidth - 20) / 2;
            WINDOW *modeWin = newwin(5, 20, height, width);
            box(modeWin, 0, 0);
            std::vector<std::string> participants = {"SOLO", "DOUBLE"};
            if (this->participants) {
                choice = this->menuSelect(modeWin, participants, 2, 2, 8, 0, false);
            } else {
                choice = this->menuSelect(modeWin, participants, 2, 2, 8, 1, false);
            }
            werase(modeWin);
            wrefresh(modeWin);
            delwin(modeWin);

            if (choice) Game::participants = false;
            else Game::participants = true;
        }

    }
    werase(mode);
    wrefresh(mode);
    delwin(mode);
}
void Solo::renderPoints() const
{
    std::string pointsString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, "%s", pointsString.c_str());
    wrefresh(this->mWindows[2]);
}

inline void Double::renderPoints() const
{
    mvwprintw(this->mWindows[0], 2, 1, "%s", "Points: ");
    mvwprintw(this->mWindows[0], 2, 9, "%s", std::to_string(this->aPoints).c_str());
    mvwprintw(this->mWindows[0], 2, mScreenWidth - 2 - 9, "%s", "Points: ");
    mvwprintw(this->mWindows[0], 2, mScreenWidth - 3, "%s", std::to_string(this->bPoints).c_str());
}

void Solo::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, "%s", difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

inline void Double::renderDifficulty() const
{
    mvwprintw(this->mWindows[0], 1, 1, "%s", "Speed: ");
    mvwprintw(this->mWindows[0], 1, 8, "%s", std::to_string(Double::Difficulty).c_str());
}


SnakeBody Game::createRandomFood()
{
    // create a food at random places
    // make sure that the food doesn't overlap with the snake.
    int width, height;
    do{
        width = rand() % (this->mGameBoardWidth - 2) + 1;
        height = rand() % (this->mGameBoardHeight - 2) + 1;
        if (mPtrSnake->isPartOfSnake(width, height) != -1)
        {
            continue;
        }
        if (this->mPtrSnake->mTerrain->getattr() == Mountain)
        {
            auto block = std::make_pair(width, height);
            if (this->mPtrSnake->mTerrain->inVector(this->mPtrSnake->mTerrain->getTerrains(), block))
            {
                continue;
            }
        }
        break;
    }while(true);
    SnakeBody food(width, height);
    return food;
}

SnakeBody Double::createRandomFood()
{
    // create a food at random places
    // make sure that the food doesn't overlap with the snake.
    int width, height;
    do{
        width = rand() % (this->mGameBoardWidth - 2) + 1;
        height = rand() % (this->mGameBoardHeight - 2) + 1;
    }while(aPtrSnake->isPartOfSnake(width, height) != -1);
    SnakeBody food(width, height);
    return food;
}

inline void Solo::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), ACS_DIAMOND);
    wnoutrefresh(this->mWindows[1]);
}

inline void Double::renderFood(SnakeBody food, WINDOW* win) const
{
    mvwaddch(win, food.getY(), food.getX(), ACS_DIAMOND);
    wnoutrefresh(win);
}

inline void Solo::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), mSnakeSymbol);
    }
    wnoutrefresh(this->mWindows[1]);
}

inline void Double::renderSnake(std::unique_ptr<Snake> & snake, WINDOW* win) const
{
    int SnakeLength = snake->getLength();
    std::vector<SnakeBody>& Snake = snake->getSnake();
    for (int i = 0; i < SnakeLength; i ++)
    {
        mvwaddch(win, Snake[i].getY(), Snake[i].getX(), mSnakeSymbol);
    }
    wnoutrefresh(win);
}

void Solo::renderTerrain() const
{
    std::vector<Block> Terrains = this->mPtrSnake->mTerrain->getTerrains();
    int l = Terrains.size();
    terrain map = this->mPtrSnake->mTerrain->getattr();
    char symbol;
    switch (map) {
        case terrain::Water:
        {
            symbol = '~';
            break;
        }
        case terrain::Mountain:
        {
            symbol = '^';
            break;
        }
        case terrain::Forest:
        {
            symbol = '#';
            break;
        }
        case terrain::Maze:
        {
            symbol = 'o';
            break;
        }
    }
    for (int i = 0; i < l; i++)
    {
        mvwaddch(this->mWindows[1], Terrains[i].second, Terrains[i].first, symbol);
    }
    wnoutrefresh(this->mWindows[1]);
}

void Double::renderTerrain(std::unique_ptr<Snake> & snake, WINDOW* win) const
{
    std::vector<Block> Terrains = snake->mTerrain->getTerrains();
    int l = Terrains.size();
    for (int i = 0; i < l; i++)
    {
        mvwaddch(win, Terrains[i].first, Terrains[i].second, '~');
    }
    wnoutrefresh(win);
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
            this->mPtrSnake->changeDirection(Direction::Up);
            return true;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            return true;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            return true;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
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

    if (manToMachine) {
        this->bPtrSnake->simulateDirection(this->machineSmartness, Double::Difficulty);
        switch (key) {
            case 'W':
            case 'w':
            case KEY_UP: {
                this->aPtrSnake->changeDirection(Direction::Up);
                return true;
            }
            case 'S':
            case 's':
            case KEY_DOWN: {
                this->aPtrSnake->changeDirection(Direction::Down);
                return true;
            }
            case 'A':
            case 'a':
            case KEY_LEFT: {
                this->aPtrSnake->changeDirection(Direction::Left);
                return true;
            }
            case 'D':
            case 'd':
            case KEY_RIGHT: {
                this->aPtrSnake->changeDirection(Direction::Right);
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
    else {
        switch (key) {
            case 'W':
            case 'w': {
                this->aPtrSnake->changeDirection(Direction::Up);
                return true;
            }
            case 'S':
            case 's': {
                this->aPtrSnake->changeDirection(Direction::Down);
                return true;
            }
            case 'A':
            case 'a': {
                this->aPtrSnake->changeDirection(Direction::Left);
                return true;
            }
            case 'D':
            case 'd': {
                this->aPtrSnake->changeDirection(Direction::Right);
                return true;
            }
            case KEY_UP: {
                this->bPtrSnake->changeDirection(Direction::Up);
                return true;
            }
            case KEY_DOWN: {
                this->bPtrSnake->changeDirection(Direction::Down);
                return true;
            }
            case KEY_LEFT: {
                this->bPtrSnake->changeDirection(Direction::Left);
                return true;
            }
            case KEY_RIGHT: {
                this->bPtrSnake->changeDirection(Direction::Right);
                return true;
            }
            case ' ':
            case KEY_BACKSPACE:
            case 27: {
                return false;
            }
            default: {
                return true;
            }
        }
    }
    return true;

}

void Game::adjustDelay()
{
    this->mDifficulty = this->mDifficulty_init + this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
    if (this->mPtrSnake->mTerrain->getattr() == Forest)
    {
        SnakeBody head = this->mPtrSnake->getSnake()[0];
        Block block = std::make_pair(head.getX(), head.getY());
        if (this->mPtrSnake->mTerrain->inVector(this->mPtrSnake->mTerrain->getTerrains(), block))
        {
            this->mDelay *= 2;
        }
        else
        {
            this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
        }
    }
}

void Double::adjustDelay()
{
    this->mDelay = this->mBaseDelay * pow(0.8, Double::Difficulty);
    if (this->mPtrSnake->mTerrain->getattr() == Forest)
    {
        SnakeBody head = this->mPtrSnake->getSnake()[0];
        Block block = std::make_pair(head.getX(), head.getY());
        if (this->mPtrSnake->mTerrain->inVector(this->mPtrSnake->mTerrain->getTerrains(), block))
        {
            this->mDelay *= 2;
        }
        else
        {
            this->mDelay = this->mBaseDelay * pow(0.8, Double::Difficulty);
        }
    }
}

void Solo::initializeGame()
{
    //0.allocate memory for a new snake
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength, this->Terrains[indexTerrain]));
    //1.initialize the game points as zero
    this->mPoints = 0;
    //2. create a food at random place
    //3.make the snake aware of the food
    this->mFood = this->createRandomFood();
    this->mPtrSnake->senseFood(this->mFood);
    //4.Adjust delay
    this->adjustDelay();
    //5.initialize the difficulty
    this->renderDifficulty();
    //6.Check if it has walls
    this->mPtrSnake->hasWalls(Game::has_walls);
}

void Double::initializeGame()
{
    //0.allocate memory for a new snake
    this->aPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->bPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    //1.initialize the game points as zero
    this->aPoints = 0;
    this->bPoints = 0;
    //2. create a food at random place
    //3.make the snake aware of the food
    this->aFood = this->createRandomFood();
    this->bFood = this->createRandomFood();
    this->aPtrSnake->senseFood(this->aFood);
    this->bPtrSnake->senseFood(this->bFood);
    //4.Adjust delay
    this->adjustDelay();
    //5.initialize the difficulty
    //this->renderDifficulty();
    //6.Check if it has walls
    this->aPtrSnake->hasWalls(Game::has_walls);
    this->bPtrSnake->hasWalls(Game::has_walls);
    //7.Check manual or machine
    this->bPtrSnake->manualOrMachine(this->manToMachine);

}

Status Solo::runGame()
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
            if (this->mPtrSnake->moveFoward())
            {
                moveSuccess = true;
            }
         // 4. check if the snake has eaten the food after movement
         // 5. check if the snake dies after the movement
            if (this->mPtrSnake->checkCollision())
            {
                return END_OF_THE_GAME;
            }
         // 6. make corresponding steps for the ``if conditions'' in 3 and 4.
            if (moveSuccess){
                this->mFood = this->createRandomFood();
                this->mPtrSnake->senseFood(this->mFood);
                this->mPoints++;
            }
         // 7. render the position of the food and snake in the new frame of window.
            this->renderTerrain();
            this->renderFood();
            this->renderSnake();
            doupdate();
         // 8. update other game states and refresh the window
            this->renderPoints();
            this->renderDifficulty();
            std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
            if (Game::dynamic_difficulty) this->adjustDelay();

    }
}

inline void Double::renderCountdown(std::string * countdown, std::atomic_int * size, Status * signal)
{
    long double tmp = this->mCountdown * 1000;
    long double interval = (this->mCountdown * 1000) / (mScreenWidth - 2);
    *size = mScreenWidth - 2;
    int duration = interval;

    while(tmp > 0)
    {
        if (*signal != PAUSE_GAME)
        {
            *countdown = std::to_string(int(tmp / 1000 / 60)) + ": ";
            *countdown += ((tmp / 1000 - int(tmp / 60  / 1000) * 60) < 10) ? "0" + std::to_string(int(tmp / 1000 - int(tmp / 60 / 1000) * 60)):
                          std::to_string(int(tmp / 1000 - int(tmp / 60 / 1000) * 60));
            tmp -= interval;
            *size -= 1;
        }
        if (*signal == END_OF_THE_GAME)
        {
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }
    *signal = END_OF_THE_GAME;
}

Status Double::runGame()
{
    auto countdown = new std::string;
    auto size = new std::atomic_int;
    auto signal = new Status;
    auto res = new Status;
    //Initialize
    *signal = NEW_GAME;
    *res = END_OF_THE_GAME;

    auto execute = [this](auto countdown, auto size, auto signal, auto res){
        while (true)
        {
            // 1. process your keyboard input
            if (!this->controlSnake())
            {
                *res = *signal;
                *signal = PAUSE_GAME; // To hinder renderCountDown
                Status choice = this->renderMenu(RESUME_GAME);
                if (choice == ABNORMAL_EXIT || choice == RESUME_GAME)
                {
                    *signal = *res;
                }
                else
                {
                    *signal = END_OF_THE_GAME;
                    *res = choice;
                    return;
                }
            }
            bool aMoveSuccess = false, bMoveSuccess = false;
            // 2. clear the window
            if (*signal == aEND_OF_THE_GAME)
            {
                werase(this->mWindows[2]);
                box(this->mWindows[2], 0, 0);
            }
            else if (*signal == bEND_OF_THE_GAME)
            {
                werase(this->mWindows[1]);
                box(this->mWindows[1], 0, 0);
            }
            else
            {
                werase(this->mWindows[1]);
                box(this->mWindows[1], 0, 0);
                werase(this->mWindows[2]);
                box(this->mWindows[2], 0, 0);
            }
            werase(this->mWindows[0]);
            box(this->mWindows[0], 0, 0);

            // 3. move the current snake forward
            if (*signal != aEND_OF_THE_GAME && this->aPtrSnake->moveFoward()) aMoveSuccess = true;
            if (*signal != bEND_OF_THE_GAME && this->bPtrSnake->moveFoward()) bMoveSuccess = true;
            // 4. check if the snake has eaten the food after movement
            // 5. check if the snake dies after the movement
            if (this->aPtrSnake->checkCollision())
            {
                if (*signal == bEND_OF_THE_GAME)
                {
                    *signal = END_OF_THE_GAME;
                    return;
                }
                else
                {
                    *signal = aEND_OF_THE_GAME;
                    float width, height, startX, startY;
                    width = this->mGameBoardWidth * 0.8;
                    height = this->mGameBoardHeight * 0.5;
                    startX = this->mGameBoardWidth * 0.1;
                    startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;
                    WINDOW * win = newwin(height, width, startY, startX);
                    box(win, 0, 0);
                    int axis_y = (this->mGameBoardHeight * 0.5 - 5) / 2 + 3;
                    mvwprintw(win, axis_y - 3, 3, (this->manToMachine)? "Player's Score": "Player A's Score:");
                    std::string aPoint = std::to_string(this->aPoints);
                    mvwprintw(win, axis_y - 2, 3, "%s", aPoint.c_str());
                    mvwprintw(win, axis_y, 3, "%s", "Do me a favor...");
                    mvwprintw(win, axis_y + 1, 3, "%s", "Perform better next time?");
                    wrefresh(win);
                }

            }
            if (this->bPtrSnake->checkCollision())
            {
                if (*signal == aEND_OF_THE_GAME)
                {
                    *signal = END_OF_THE_GAME;
                    return;
                }
                else
                {
                    *signal = bEND_OF_THE_GAME;
                    float width, height, startX, startY;
                    width = (this->manToMachine)? this->mGameBoardWidth * 0.9: this->mGameBoardWidth * 0.8;
                    height = this->mGameBoardHeight * 0.5;
                    startX = (this->manToMachine)? this->mGameBoardWidth * 0.05 + this->mScreenWidth / 2:
                                                   this->mGameBoardWidth * 0.1 + this->mScreenWidth / 2;
                    startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;
                    WINDOW * win = newwin(height, width, startY, startX);
                    int axis_y = (this->mGameBoardHeight * 0.5  - 5) / 2 + 3;
                    mvwprintw(win, axis_y - 3, 3, (this->manToMachine)? "Computer's Score" :"Player B's Score:");
                    std::string bPoint = std::to_string(this->bPoints);
                    mvwprintw(win, axis_y - 2, 3, "%s", bPoint.c_str());
                    mvwprintw(win, axis_y, 3, (this->manToMachine)? "Oops..." :"Do me a favor...");
                    mvwprintw(win, axis_y + 1, 3, (this->manToMachine)? "The game must have been rigged..": "Perform better next time?");
                    box(win, 0, 0);
                    wrefresh(win);
                }
            }

            // 6. make corresponding steps for the ``if conditions'' in 3 and 4.
            if (aMoveSuccess){
                this->aFood = this->createRandomFood();
                this->aPtrSnake->senseFood(this->aFood);
            }
            if (bMoveSuccess){
                this->bFood = this->createRandomFood();
                this->bPtrSnake->senseFood(this->bFood);
            }
            // 8. render the position of the food and snake in the new frame of window.
            if (*signal == END_OF_THE_GAME)
            {
                return;
            }
            else if (*signal != aEND_OF_THE_GAME && *signal != bEND_OF_THE_GAME)
            {
                this->renderFood(this->bFood, this->mWindows[2]);
                this->renderSnake(this->bPtrSnake, this->mWindows[2]);
                this->renderFood(this->aFood, this->mWindows[1]);
                this->renderSnake(this->aPtrSnake, this->mWindows[1]);
                doupdate();
            }
            else if (*signal == aEND_OF_THE_GAME)
            {
                this->renderFood(this->bFood, this->mWindows[2]);
                this->renderSnake(this->bPtrSnake, this->mWindows[2]);
                doupdate();
            }
            else if (*signal == bEND_OF_THE_GAME)
            {
                this->renderFood(this->aFood, this->mWindows[1]);
                this->renderSnake(this->aPtrSnake, this->mWindows[1]);
                doupdate();
            }

            //  render the countdown
            //  update other game states and refresh the window
            mvwprintw(this->mWindows[0], 1, (mScreenWidth - 4) / 2, "TIME");
            mvwprintw(this->mWindows[0], 2, (mScreenWidth - 4) / 2, "%s", countdown->c_str());
            for (int i = 0; i < *size; i++)
            {
                mvwaddch(this->mWindows[0], 3, i + 1, ACS_CKBOARD);
            }
            this->renderPoints();
            this->renderDifficulty();
            wnoutrefresh(this->mWindows[0]);
            doupdate();

            if (aMoveSuccess) this->aPoints++;
            if (bMoveSuccess) this->bPoints++;
            std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        }

    };

    std::thread th1(execute, countdown, size, signal, res);
    std::thread th2(&Double::renderCountdown, this, countdown, size, signal);
    th2.join();
    th1.join();
    this->renderPoints();
    this->renderDifficulty();

    Status ans = *res;

    delete signal, countdown, size, res;

    return ans;

}

void Game::startGame() {

    refresh();
    int choice = this->renderMenu(MAIN_MENU);

    Game* solo = new Solo;
    Game* battle = new Double;
    Game* play;

    while(true) {
        switch (choice) {
            case NEW_GAME: {
                if (participants)
                {
                    play = solo;
                    play->readLeaderBoard();
                }
                else
                {
                    play = battle;
                }
                play->renderBoards();
                play->initializeGame();
                choice = play->runGame();
                if (participants) {
                    play->updateLeaderBoard();
                    play->writeLeaderBoard();
                }
                break;
            }
            case END_OF_THE_GAME: {
                choice = play->renderMenu(END_OF_THE_GAME);
                if (choice == ABNORMAL_EXIT) choice = MAIN_MENU;
                break;
            }
            case MODE: {
                if (participants)
                {
                    play = solo;
                }
                else
                {
                    play = battle;
                }
                play->renderMode();
                choice = MAIN_MENU;
                break;
            }
            case SETTINGS: {
                if (participants)
                {
                    play = solo;
                }
                else
                {
                    play = battle;
                }
                play->renderSettings();
                choice = MAIN_MENU;
                break;
            }
            case MAIN_MENU: {
                choice = play->renderMenu(MAIN_MENU);
                if (choice == ABNORMAL_EXIT) choice = QUIT;
                continue;
            }
            case PAUSE_GAME: {
                choice = play->renderMenu(RESUME_GAME);
                if (choice == ABNORMAL_EXIT) choice = RESUME_GAME;
                continue;
            }
            case ABNORMAL_EXIT: {
                choice = QUIT;
                break;
            }
            case RESUME_GAME: {
                choice = play->runGame();
                continue;
            }
            case QUIT: {
                delete solo;
                delete battle;
                return;
            }
        }

    }

}

int Game::menuSelect(WINDOW * menu, std::vector<std::string> lists, int axis_y, int axis_x, int whitespace, int init, bool direction)
{
    int key, size = lists.size(), index = 0, offset = 0;
    if (size < 2) return -1;
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
    doupdate();


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

                    if (event.bstate == BUTTON1_CLICKED && wmouse_trafo(menu, &event_y, &event_x, false))
                    {
                        if (event_x == (getmaxx(menu) - 2) && event_y == 1)
                        {
                            return -1;
                        }
                        else if (direction)
                        {
                            if ((event_y - axis_y) % whitespace == 0 && (event_y - axis_y) >= 0)
                            {
                                if (index < 0 || index >= size) break;

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
                        }
                        else
                        {
                            if ((event_x - axis_x) < size * whitespace && (event_x - axis_x) >= 0)
                            {
                                if (index < 0 || index >= size) break;

                                mvwprintw(menu, axis_y, axis_x + index * whitespace, "%s", lists[index].c_str());
                                index = (event_x - axis_x) / whitespace;
                                wattron(menu, A_STANDOUT);
                                mvwprintw(menu, axis_y, event_x, "%s", lists[index].c_str());
                                wattroff(menu, A_STANDOUT);
                                wnoutrefresh(menu);
                                doupdate();
                                return index;
                            }
                            break;
                        }
                    }
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

void Game::numSelect(WINDOW * menu, std::vector<int> &original, std::vector<int> &positions, int axis_y, int axis_x)
{
    assert(original.size() == positions.size());
    int size = original.size();
    int key, index = 0;
    for(;index < size; index++)
    {
        if (index != 0)
        {
            mvwprintw(menu, axis_y, axis_x + positions[index], "%s", std::to_string(original[index]).c_str());
        }
        else
        {
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, axis_y, axis_x + positions[index], "%s", std::to_string(original[index]).c_str());
            wattroff(menu, A_STANDOUT);
        }
    }

    wattron(menu, A_STANDOUT);
    mvwaddch(menu, 1, getmaxx(menu) - 2, 'x');
    wattroff(menu, A_STANDOUT);

    wnoutrefresh(menu);
    doupdate();

    index = 0;
    while(true)
    {
        key = getch();
        if (key == KEY_MOUSE)
        {
            if (getmouse(&event) == OK)
            {
                int event_y = event.y, event_x = event.x;
                if (event.bstate == BUTTON1_CLICKED && wmouse_trafo(menu, &event_y, &event_x, false))
                {
                    if (event_x == (getmaxx(menu) - 2) && event_y == 1)
                    {
                        return;
                    }
                    if (event_x == axis_x + positions[0] && event_y == axis_y)
                    {
                        index = 0;
                        wattron(menu, A_STANDOUT);
                        mvwprintw(menu, axis_y, axis_x + positions[0], "%s", std::to_string(original[0]).c_str());
                        wattroff(menu, A_STANDOUT);
                        wnoutrefresh(menu);
                        doupdate();
                    }
                }
            }
        }
        else if (key >= '0' && key <= '9' && index < size)
        {
            if (index == 1 && key > '5')
            {
                WINDOW *warnWin = newwin(3, 40, (mScreenHeight) * 0.06, (mScreenWidth - 40) / 2) ;
                box(warnWin, 0, 0);
                mvwprintw(warnWin, 1, 3, "%s", "Please enter an valid number !");
                wrefresh(warnWin);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                werase(warnWin);
                wrefresh(warnWin);
                delwin(warnWin);
                continue;
            }
            mvwprintw(menu, axis_y, axis_x + positions[index], "%s", std::to_string(key - '0').c_str());
            wattron(menu, A_STANDOUT);
            if (index != size - 1)
            {
                mvwprintw(menu, axis_y, axis_x + positions[index + 1], "%s", std::to_string(original[index + 1]).c_str());
            }
            wattroff(menu, A_STANDOUT);
            original[index] = key - '0';
            wnoutrefresh(menu);
            doupdate();
            index++;
            index = (index >= size) ? 0: index;
        }
        else if (key == KEY_BACKSPACE && index > 0)
        {
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, axis_y, axis_x + positions[index - 1] , "%s", std::to_string(original[index - 1]).c_str());
            wattroff(menu, A_STANDOUT);
            mvwprintw(menu, axis_y, axis_x + positions[index], "%s", std::to_string(original[index]).c_str());
            wnoutrefresh(menu);
            doupdate();
            index--;
            index = (index < 0) ? 0:index;
        }
        else if (key == KEY_BACKSPACE && index == 0)
        {
            continue;
        }
        else if (key == KEY_ENTER || key == 27)
        {
            return;
        }
    }
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
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



