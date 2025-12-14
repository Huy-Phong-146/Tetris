#pragma once

#include "Utilities.h"
#include "Block.h"
// =====================
// Board Class
// =====================
class Board {
public:
    vector<vector<char>> grid;
    vector<vector<int>> colorGrid;

    Board();

    void draw();
    void boardDeleteBlock(BaseBlock* currBlock);
    void blockToBoard(BaseBlock* currBlock);
    bool canMove(int dx, int dy, BaseBlock* currBlock);
    void animateLineClear(int line);
    int removeLine();
};
