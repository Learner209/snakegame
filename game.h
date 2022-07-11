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


//Game Status
enum Status{END_OF_THE_GAME,aEND_OF_THE_GAME, bEND_OF_THE_GAME, NEW_GAME, RESUME_GAME, PAUSE_GAME,
    MAIN_MENU, QUIT, MODE, SETTINGS, ABNORMAL_EXIT, NULL_INPUT};
enum Drowning{DROWNING, aDROWNING, bDROWNING, NORMAL};

void init();

class Game {
public:
    Game();

    ~Game();


    void createInformationBoard();
    virtual void renderInformationBoard() {};

    virtual void createGameBoard() {};
    virtual void renderGameBoard() const {};

    virtual void createInstructionBoard() {};
    virtual void renderInstructionBoard() {};

    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

    virtual void renderBoards() {};

    virtual void initializeGame() {};
    virtual Status runGame() { return RESUME_GAME; };

    virtual void renderPoints() const {};
    virtual void renderDifficulty() const {};

    SnakeBody createRandomFood();

    virtual void renderFood() const {};
    virtual void renderSnake() const { assert(false);};
    virtual void renderTerrain() const {};
    virtual bool controlSnake() const { return true; };

    void startGame();

    virtual Status renderMenu(Status status, int delay = 0);
    virtual void renderSettings();
    void renderMode();

    void calculateDelay();
    void adjustDelay();

    //Battle
    virtual void changeMode(int) {};

protected:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    int mInformationHeight = 4;
    int mInstructionWidth = 20;
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
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
    //Mouse
    MEVENT event;
    //Participants
    static int participants;
    //Multi-thread

    //Terrain
    std::vector <terrain> Terrains = {Plain, Water, Mountain, Forest, Maze};
    int indexTerrain = 2;
    const int mDrowningTime = 5;
    int mTerrainDifficulty = 0;
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
    void renderTerrain() const;
    void renderBoards();
    inline void calculateDrowning(std::string * countdown, std::atomic_int * size, Status * signal, Drowning* drowningSignal);

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
    inline void calculateDrowning(std::string * countdown, std::atomic_int * size, Status * signal, Drowning * drowningSignal, bool player);
    void renderDifficulty() const;

    SnakeBody createRandomFood(std::unique_ptr<Snake>& snake);
    inline void renderFood(SnakeBody, WINDOW*) const;
    void renderPoints() const;
    inline void renderSnake(std::unique_ptr<Snake> & snake, WINDOW* win) const;
    void renderTerrain(std::unique_ptr<Snake> & snake, WINDOW* win) const;

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

    int mInformationHeight = 7;

    int aPoints = 0;
    int bPoints = 0;

    int mCountdown = 60;

    SnakeBody aFood;
    SnakeBody bFood;

    int aDelay = 0;
    int bDelay = 0;
    //Man to Machine
    bool manToMachine = false;
    int machineSmartness = 3;
};
#endif
