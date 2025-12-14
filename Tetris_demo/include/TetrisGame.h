#pragma once

#include "Utilities.h"
#include "Block.h"
#include "Board.h"

// ====================
// TetrisGame Class
// ====================
class TetrisGame {
private:
    Board board;
    BaseBlock* currBlock;
    BaseBlock* nextBlock;
    int gameSpeed;
    int score;
    int level;
    int highestScore;
    int comboCount;

    BaseBlock* createRandomBlock();
    void increaseSpeed();
    void loadHighestScore();
    void saveHighestScore();
    bool checkHighScore();
    void drawNextBlock();
    void drawUI(bool isNewRecord = false);
    void scoreCalculate(int linesCleared);
    void gameOverEffect();
    int showPauseMenu();

public:
    TetrisGame(int mode = 1);
    ~TetrisGame();

    bool run();
};
