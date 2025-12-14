#pragma once

#include "Utilities.h"
#include "Block.h"
#include "Board.h"
#include <unordered_map>

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
    unordered_map<int, int> highScores;
    int comboCount;

    BaseBlock* createRandomBlock();
    void increaseSpeed();
    void loadHighScores();
    void saveHighScores();
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
