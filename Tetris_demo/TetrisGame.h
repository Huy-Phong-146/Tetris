#pragma once

#include "Utilities.h"
#include "Block.h"
#include "Board.h"
#include <unordered_map>
#include <vector>

enum class GameMode {
    SOLO,
    PVP
};

// ====================
// PlayerState Class
// ====================
struct PlayerState {
    int id;
    Board board;
    int gameSpeed;
    BaseBlock* currBlock;
    BaseBlock* nextBlock;
    int timer;
    int score;
    int comboCount;
    bool isGameOver;

    char kLeft, kRight, kRotate, kSoftDrop, kHardDrop;

    PlayerState(int id, int _offsetX, int _gameSpeed,
                char _kLeft, char _kRight, char _kRotate, char _kSoftDrop, char _kHardDrop)
        : id(id), board(_offsetX, 5), gameSpeed(_gameSpeed), currBlock(nullptr), nextBlock(nullptr),
          timer(0), score(0), comboCount(0), isGameOver(false),
          kLeft(_kLeft), kRight(_kRight), kRotate(_kRotate), kSoftDrop(_kSoftDrop), kHardDrop(_kHardDrop) {}
};

// ====================
// TetrisGame Class
// ====================
class TetrisGame {
private:
    GameMode gameMode;
    int level;

    vector<PlayerState> players;
    unordered_map<int, int> highScores;

    BaseBlock* createRandomBlock();

    void loadHighScores();
    void saveHighScores();
    bool checkHighScore(int score);

    void increaseSpeed(PlayerState& p);
    void scoreCalculate(PlayerState& p, int linesCleared);

    void handleInput(PlayerState& p, int key);
    void updatePhysics(PlayerState& p);

    void drawNextBlock(const PlayerState& p);
    void drawUI(const PlayerState& p, bool isNewRecord = false);
    void gameOverEffect(PlayerState& p);
    int showPauseMenu();

public:
    TetrisGame(GameMode mode, int level);
    ~TetrisGame();

    bool run();
};
