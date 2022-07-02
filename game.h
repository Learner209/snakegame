//
// Created by bitch on 6/29/22.
//

#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>

#include "snake.h"
#include <iostream>

//Game Status
enum Status{END_OF_THE_GAME,NEW_GAME, RESUME_GAME, PAUSE_GAME, MAIN_MENU, QUIT, MODE, SETTINGS, ABNORMAL_EXIT};

class Game
{
public:
    Game();
    ~Game();

    void createInformationBoard();
    void renderInformationBoard(int & choice);

    void createGameBoard();
    void renderGameBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;

    void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

    void renderBoards() const;

    void initializeGame();
    Status runGame();
    void renderPoints() const;
    void renderDifficulty() const;

    void createRandomFood();
    void renderFood() const;
    void renderSnake() const;
    bool controlSnake() const;

    void startGame();
    //void mainGame();
    Status renderMenu(Status status);
    void renderSettings();
    void renderMode() const;
    void adjustDelay();
    void adjustDifficulty(int);

    static void _(int a) {
        std::cout << a << std::endl;
    }

private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 3;
    const int mInstructionWidth = 20;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    const char mFoodSymbol = '#';
    int mPoints = 0;
    int mDifficulty = 0;
    int mDifficulty_init = 0;
    //Settings
    bool dynamic_difficulty = true;
    bool has_walls = true;
    int mBaseDelay = 100;
    int mDelay;
    const std::string mRecordBoardFilePath = "record.txt";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
    //Mouse
    MEVENT event;
    //Auxiliary: Menu Select
    int menuSelect(WINDOW * menu, std::vector<std::string> lists, int axis_y, int axis_x, int whitespace, int init, bool direction = 1) ;
};

#endif
