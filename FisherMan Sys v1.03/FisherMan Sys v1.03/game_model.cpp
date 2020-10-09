#include <time.h>
#include <stdlib.h>
#include "game_model.h"

GameModel::GameModel()
    : mRow(kRow),
      mCol(kCol),
      totalMineNumber(kMineCount),
      timerSeconds(kTime),
      gameState(PLAYING)
{
    // Since we cannot determine when the constrcutor will be used, initialization will not be included here.
}

GameModel::~GameModel()
{

}

void GameModel::createGame(int row, int col, int mineCount, GameLevel level)
{
    // Clear the map first
    gameMap.clear();
    // Set the member variable
    mRow = row;
    mCol = col;
    totalMineNumber = mineCount;
    curMineNumber = mineCount;
    gameState = PLAYING;
    gameLevel = level;
    timerSeconds = 0;

    // Initialize blocks
    for(int i = 0; i < mRow; i++)
    {
        //Add blocks for each column
        std::vector<MineBlock> lineBlocks;
        for(int j = 0; j < mCol; j++)
        {
            MineBlock mineBlock;
            mineBlock.curState = UN_DIG; // Be unearthed in the beginning
            mineBlock.valueFlag = 0; // No value flag
            lineBlocks.push_back(mineBlock);
        }
        gameMap.push_back(lineBlocks);
    }

    // Put the mines
    srand((unsigned int)time(0));
    int k = totalMineNumber;
    while(k > 0)
    {
        // Avoid choosing the same place
        int pRow = rand() % mRow;
        int pCol = rand() % mCol;
        if(gameMap[pRow][pCol].valueFlag != -1)
        {
             gameMap[pRow][pCol].valueFlag = -1;
             k--; // Loop when choosing a chosen place
        }
    }

    // Calculate the number that needs to be shown
    for(int i = 0; i < mRow; i++)
    {
        for(int j = 0; j < mCol; j++)
        {
            // The sum of mines in the eight blocks around itself
            // The block itself should not have a mine
            if(gameMap[i][j].valueFlag != -1)
            {
                for(int y = -1; y <= 1; y++)
                {
                    for(int x = -1; x <= 1; x++)
                    {
                        if(i + y >= 0
                        && i + y < mRow
                        && j + x >= 0
                        && j + x < mCol
                        && gameMap[i + y][j + x].valueFlag == -1
                        && !(x == 0 && y == 0))
                        {
                            gameMap[i][j].valueFlag++;
                        }
                    }
                }
            }
        }
    }
}

void GameModel::restartGame()
{
    createGame(mRow, mCol, totalMineNumber, gameLevel);
}

void GameModel::digMine(int m, int n)
{
    // Mark an block to be unearthed
    //It cannot be unearthed at first
    if(gameMap[m][n].valueFlag > 0
     && gameMap[m][n].curState == UN_DIG)
    {
        gameMap[m][n].curState = DIGGED;
    }

    // Use BFS to unearthed all the empty blocks, gameover when a mine was unearthed.
    if(gameMap[m][n].valueFlag == 0
     && gameMap[m][n].curState == UN_DIG)
    {
        gameMap[m][n].curState = DIGGED;
        for(int y = -1; y <= 1; y++)
        {
            for(int x = -1; x <= 1; x++)
            {
                if(m + y >= 0
                && m + y < mRow
                && n + x >= 0
                && n + x < mCol
                && !(x == 0 && y == 0))
                {
                    digMine(m + y, n + x);
                }
            }
        }
    }

    if(gameMap[m][n].valueFlag == -1)
    {
        gameState = OVER;
        gameMap[m][n].curState = BOMB;
    }

    // check whether the player wins
    checkGame();
}

void GameModel::markMine(int m, int n)
{
    // If the mark was incorrect, show it when the game is over
    // Remark the block will make it become unmarked.
    if(gameMap[m][n].curState == UN_DIG)
    {
        if(gameMap[m][n].valueFlag == -1)
        {
            gameMap[m][n].curState = MARKED;
        }
        else
        {
            gameState = FAULT;
            gameMap[m][n].curState = WRONG_BOMB;
        }
        curMineNumber--; // If the mark was correct,mine number-1.
    }
    else if(gameMap[m][n].curState == MARKED || gameMap[m][n].curState == WRONG_BOMB)
    {
        gameMap[m][n].curState = UN_DIG;
        gameState = PLAYING;
        curMineNumber++; // If the mark was incorrect,mine number+1.
    }
    // Check whether the player wins
    checkGame();
}

void GameModel::checkGame()
{
    if(gameState == OVER)
    {
        // Show all mines and all incorrect flags if the player fail.
        for(int i = 0; i < mRow; i++)
        {
            for(int j = 0; j < mCol; j++)
            {
                if(gameMap[i][j].valueFlag == -1)
                {
                    gameMap[i][j].curState = BOMB;
                }
            }
        }
        return;
    }
    // Win the game if all blocks without mine are unearthed
    if(gameState != FAULT)
    {
        for(int i = 0; i < mRow; i++)
        {
            for(int j = 0; j < mCol; j++)
            {
                if(gameMap[i][j].curState == UN_DIG)
                {
                    gameState = PLAYING;
                    return;
                }
            }
        }
        gameState = WIN;
    }
}
