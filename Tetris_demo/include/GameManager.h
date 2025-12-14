#pragma once

#include "TetrisGame.h"
#include "Scene.h"

class GameManager {
private:
    int menu();
    int chooseMode();
    void showHighScore();

public:
    void runProgram();
};
