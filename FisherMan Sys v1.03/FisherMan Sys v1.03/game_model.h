#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// game model class, which is divided from game UI
#include <vector>

// ---------------- Global Variable ---------------- //
// The situation of block
enum BlockState
{
    UN_DIG,
    DIGGED,
    MARKED,
    BOMB,
    WRONG_BOMB
};
// The block class
struct MineBlock
{
    BlockState curState; // Current State
    int valueFlag; // 0-8 are the number it should shows, -1 means mine
};

// The situation of a game
enum GameState
{
    PLAYING,
    FAULT,
    OVER,
    WIN
};

// Game difficulty level
enum GameLevel
{
    BASIC,
    MEDIUM,
    HARD
};

// some default variable of a game
const int kRow = 15;
const int kCol = 20;
const int kMineCount = 50;
const int kTime = 0;
// ----------------------------------------------- //

class GameModel
{
public:
    GameModel();
    virtual ~GameModel();
public:
    void digMine(int m, int n); //dig the mine, m is column, n is row
    void markMine(int m, int n); // mark a mine
    void createGame(int row = kRow, int col = kCol, int mineCount = kMineCount, GameLevel level = MEDIUM); // initialize
    void restartGame();  // restart the game according to current variable
    void checkGame(); // check the win or lose of a game
public:
    std::vector<std::vector<MineBlock>> gameMap; // map of a game
    int mRow; // row
    int mCol; // column
    int totalMineNumber; // total mines
    int curMineNumber;   // current mines
    int timerSeconds; // time


    GameState gameState; // current game state
    GameLevel gameLevel; // current game level
};

#endif // GAMEMODEL_H
