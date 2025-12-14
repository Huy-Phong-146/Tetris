#pragma once

#include "TetrisGame.h"
#include "Scene.h"

class GameManager {
private:
    int menu();
    int chooseLevel();
    unordered_map<int, int> loadHighScore();
    void showHighScore();

public:
    void runProgram();
};
