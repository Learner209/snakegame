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
enum Status{END_OF_THE_GAME,NEW_GAME, RESUME_GAME, PAUSE_GAME,
        MAIN_MENU, QUIT, MODE, SETTINGS, ABNORMAL_EXIT, NULL_INPUT};

class Game
{
public:
    Game();
    ~Game();

    void createInformationBoard();
    virtual void renderInformationBoard() {return;};

    virtual void createGameBoard() {return;};
    virtual void renderGameBoard() const {return;};

    virtual void createInstructionBoard() {return;};
    virtual void renderInstructionBoard() {return;};

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
    virtual void renderDifficulty() const {return;};

    SnakeBody createRandomFood();
    virtual void renderFood() const {return;};
    virtual void renderSnake() const {return;};
    virtual bool controlSnake() const {return;};

    void startGame();

    Status renderMenu(Status status);
    void renderSettings();
    void renderMode() const;
    void adjustDelay();
    void adjustDifficulty(int);

    static void _(int a) {
        std::cout << a << std::endl;
    }

protected:
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
    //SnakeBody mFood;
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
    //Participants
    bool participants = true;
    //Auxiliary: Menu Select
    int menuSelect(WINDOW * menu, std::vector<std::string> lists, int axis_y, int axis_x, int whitespace, int init, bool direction = 1) ;
};

class Solo :public Game{
public:
    Solo();

    void createGameBoard() const;
    void renderGameBoard() const;

    void renderInformationBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;
    void renderDifficulty() const;

    void renderFood() const;
    void renderSnake() const;

    bool controlSnake() const;
protected:
};

class Double: public Game{
public:
    Double();

    void renderInformationBoard();

    void createGameBoard() const;
    void renderGameBoard() const;

    void renderBoards() const;

    void adjustDelay();

    void renderDifficulty() const;

    void renderFood() const;
    void renderSnake() const;

    bool controlSnake() const;
    void initializeGame();
    Status runGame() const;

protected:
    std::unique_ptr<Snake> aPtrSnake;
    std::unique_ptr<Snake> bPtrSnake;
    int aDifficulty = 0;
    int aDifficulty_init = 0;

    int bDifficulty = 0;
    int bDifficulty_init = 0;

    int aPoints = 0;
    int bPoints = 0;

    int aDelay = 0;
    int bDelay = 0;

};
#endif
