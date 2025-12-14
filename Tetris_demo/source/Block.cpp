#include "Block.h"

BaseBlock::BaseBlock(Color blockColor): blockColor(blockColor) {
    x = WIDTH / 2 - 2;
    y = 0;
    shape = vector<vector<char>>(BLOCK_SIZE, vector<char>(BLOCK_SIZE, ' '));
}

void BaseBlock::rotate(const vector<vector<char>>& grid) {
    vector<vector<char>> old = shape;
    vector<vector<char>> rot(BLOCK_SIZE, vector<char>(BLOCK_SIZE, ' '));

    for (int i = 0; i < BLOCK_SIZE; i++)
        for (int j = 0; j < BLOCK_SIZE; j++)
            rot[j][BLOCK_SIZE - i - 1] = old[i][j];

    int kickX[] = {0, -1, 1, -2, 2};
    int kickY[] = {0,  0, 0,  0, 0};

    for (int k = 0; k < 5; k++) {
        int nx = x + kickX[k];
        int ny = y + kickY[k];

        bool canRotate = true;

        for (int i = 0; i < BLOCK_SIZE && canRotate; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (rot[i][j] == ' ')
                    continue;

                int tx = nx + j;
                int ty = ny + i;

                if (tx < 1
                 || tx >= WIDTH - 1
                 || ty >= HEIGHT - 1
                 || grid[ty][tx] != ' ') {
                    canRotate = false;
                    break;
                }
            }
        }

        if (canRotate) {
            x = nx;
            y = ny;
            shape = rot;
            return;
        }
    }
}

BlockI::BlockI(): BaseBlock(CYAN) {
    shape[1][0] = shape[1][1] = shape[1][2] = shape[1][3] = BLOCK_CHAR;
}

BlockO::BlockO() : BaseBlock(YELLOW) {
    shape[1][1] = shape[1][2] = BLOCK_CHAR;
    shape[2][1] = shape[2][2] = BLOCK_CHAR;
}

BlockT::BlockT() : BaseBlock(PURPLE) {
                  shape[1][1] = BLOCK_CHAR;
    shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
}

BlockL::BlockL() : BaseBlock(RED) {
                                shape[1][2] = BLOCK_CHAR;
    shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
}

BlockJ::BlockJ() : BaseBlock(BLUE) {
    shape[1][0] = BLOCK_CHAR;
    shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
}

BlockS::BlockS() : BaseBlock(GREEN) {
                  shape[1][1] = shape[1][2] = BLOCK_CHAR;
    shape[2][0] = shape[2][1] = BLOCK_CHAR;
}

BlockZ::BlockZ() : BaseBlock(RED) {
    shape[1][0] = shape[1][1] = BLOCK_CHAR;
                  shape[2][1] = shape[2][2] = BLOCK_CHAR;
}
