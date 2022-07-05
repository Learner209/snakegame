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

#include <cassert>

//Multi-thread
#include <atomic>
#include <mutex>
#include <condition_variable>

//Game Status
enum Status{END_OF_THE_GAME,aEND_OF_THE_GAME, bEND_OF_THE_GAME, NEW_GAME, RESUME_GAME, PAUSE_GAME,
    MAIN_MENU, QUIT, MODE, SETTINGS, ABNORMAL_EXIT, NULL_INPUT};


void init();

class Game {
public:
    Game();

    ~Game();


    void createInformationBoard();
    void renderInformationBoard() { return; };

    virtual void createGameBoard() { return; };
    virtual void renderGameBoard() const { return; };

    virtual void createInstructionBoard() { return; };
    virtual void renderInstructionBoard() { return; };

    void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

    virtual void renderBoards() { return; };

    virtual void initializeGame() { return; };
    virtual Status runGame() { return RESUME_GAME; };

    virtual void renderPoints() const { return; };
    virtual void renderDifficulty() const { return; };

    SnakeBody createRandomFood();

    virtual void renderFood() const { return; };
    virtual void renderSnake() const { return; };
    virtual bool controlSnake() const { return true; };

    void startGame();

    virtual Status renderMenu(Status status, int delay = 0);
    virtual void renderSettings();
    void renderMode();

    void adjustDelay();

protected:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    int mInformationHeight = 3;
    const int mInstructionWidth = 20;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr <Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    //SnakeBody mFood;
    const char mFoodSymbol = '#';
    int mPoints = 0;
    int mDifficulty = 0;
    int mDifficulty_init = 0;
    //Settings
    static bool dynamic_difficulty;
    static bool has_walls;
    int mBaseDelay = 100;
    int mDelay;
    const std::string mRecordBoardFilePath = "record.txt";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
    //Mouse
    MEVENT event;
    //Participants
    static bool participants;
    //Multi-thread
    std::mutex m;
    std::condition_variable cond;
    //Terrain
    std::vector <std::string> Terrains = {"Plain"};
    int indexTerrain = 0;
    //Auxiliary: Menu Select
    int menuSelect(WINDOW *menu, std::vector <std::string> lists, int axis_y, int axis_x, int whitespace, int init,
                   bool direction = 1);
    void numSelect(WINDOW *menu, std::vector<int> &original, std::vector<int> &positions, int axis_y, int axis_x);

};

class Solo :public Game{
public:
    Solo();
    ~Solo(){};

    void createGameBoard() ;
    void renderGameBoard() const;

    void createInformationBoard();
    void renderInformationBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;
    void renderDifficulty() const;
    
    void renderFood() const;
    void renderPoints() const;
    void renderSnake() const;
    void renderBoards();

    void initializeGame();
    Status runGame();

    bool controlSnake() const;
protected:
};

class Double: public Game{
public:
    Double();
    ~Double(){};

    void createInformationBoard();
    void renderInformationBoard();

    void createGameBoard() ;
    void renderGameBoard() const;

    void renderBoards();
    void renderSettings();
    Status renderMenu(Status status, int delay = 0);

    void adjustDelay();
    inline void renderCountdown(std::string * countdown, std::atomic_int * size, Status * signal);
    void renderDifficulty() const;

    SnakeBody createRandomFood();
    inline void renderFood(SnakeBody, WINDOW*) const;
    void renderPoints() const;
    inline void renderSnake(std::unique_ptr<Snake> & snake, WINDOW* win) const;
    inline void renderASnake() const;
    inline void renderBSnake() const;

    bool controlSnake() const;
    void initializeGame();
    Status runGame();


protected:
    std::unique_ptr<Snake> aPtrSnake;
    std::unique_ptr<Snake> bPtrSnake;

    static int Difficulty;

    int aDifficulty = 0;
    int aDifficulty_init = 0;

    int bDifficulty = 0;
    int bDifficulty_init = 0;

    int mInformationHeight = 5;

    int aPoints = 0;
    int bPoints = 0;

    int mCountdown = 20;

    SnakeBody aFood;
    SnakeBody bFood;

    int aDelay = 0;
    int bDelay = 0;

};
#endif
