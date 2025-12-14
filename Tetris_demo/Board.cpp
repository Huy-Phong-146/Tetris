#include "Board.h"
#include "Block.h"

Board::Board() : offsetX(3), offsetY(5) {
    grid      = vector<vector<char>>(HEIGHT - 1, vector<char>(WIDTH, ' '));
    colorGrid = vector<vector<int> >(HEIGHT, vector<int>(WIDTH, LIGHT_GRAY));

    for (vector<char>& row : grid)
        row.front() = row.back() = BORDER_V;

    grid.emplace_back(vector<char>(WIDTH, BORDER_H));
    grid.back().front() = BORDER_BL;
    grid.back().back()  = BORDER_BR;
}

Board::Board(int offsetX, int offsetY) : offsetX(offsetX), offsetY(offsetY) {
    grid      = vector<vector<char>>(HEIGHT - 1, vector<char>(WIDTH, ' '));
    colorGrid = vector<vector<int> >(HEIGHT, vector<int>(WIDTH, LIGHT_GRAY));

    for (vector<char>& row : grid)
        row.front() = row.back() = BORDER_V;

    grid.emplace_back(vector<char>(WIDTH, BORDER_H));
    grid.back().front() = BORDER_BL;
    grid.back().back()  = BORDER_BR;
}

void Board::draw() {
    for (int i = 0; i < HEIGHT; i++, cout << endl) {
        gotoxy(offsetX, i + offsetY);
        setColor(WHITE);
        cout << grid[i][0];

        for (int j = 1; j < WIDTH - 1; j++) {
            setColor(colorGrid[i][j]);
            cout << grid[i][j] << grid[i][j];
        }

        setColor(WHITE);
        cout << grid[i][WIDTH - 1];
    }
}

void Board::boardDeleteBlock(BaseBlock* currBlock) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        for (int j = 0; j < BLOCK_SIZE; j++)
            if (currBlock->shape[i][j] != ' ' && currBlock->y + j < HEIGHT)
                grid[currBlock->y + i][currBlock->x + j] = ' ';
}

void Board::blockToBoard(BaseBlock* currBlock){
    for (int i = 0 ; i < BLOCK_SIZE; i++)
        for (int j = 0 ; j < BLOCK_SIZE; j++)
            if (currBlock->shape[i][j] != ' '){
                int tx = currBlock->x + j;
                int ty = currBlock->y + i;
                grid[ty][tx] = currBlock->shape[i][j];
                colorGrid[ty][tx] = currBlock->blockColor;
            }
}

bool Board::canMove(int dx, int dy, BaseBlock* currBlock) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        for (int j = 0; j < BLOCK_SIZE; j++)
            if (currBlock->shape[i][j] != ' ') {
                int tx = currBlock->x + j + dx;
                int ty = currBlock->y + i + dy;

                if (tx < 1
                ||  tx >= WIDTH - 1
                ||  ty >= HEIGHT - 1
                ||  grid[ty][tx] != ' ')
                    return false;
            }

    return true;
}

void Board::animateLineClear(int line) {
    char frames[] = {(char)219, (char)178, (char)177, (char) 176};

    for (int k = 1; k < WIDTH - 1; k++){
        grid[line][k] = frames[0];
        colorGrid [line][k] = WHITE;
    }

    draw();
    _sleep(50);
    playSound(1000, 700);

    for (int i = 1; i < 4; i++) {
        for (int k = 1; k < WIDTH - 1; k++) {
            grid[line][k] = frames[i];
            colorGrid[line][k] = DARK_GRAY;
        }

        draw();
        _sleep(40);
    }
}


int Board::removeLine(){
    int linesCleared = 0;

    for (int i = HEIGHT - 2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < WIDTH - 1; j++) {
            if (grid[i][j] == ' ') {
                full = false;
                break;
            }
        }

        if (!full) continue;

        linesCleared++;

        playSound(1200, AUDIO_LENGTH);
        playSound(1600, AUDIO_LENGTH);

        animateLineClear(i);

        for (int ii = i; ii > 0; ii--)
            for (int k = 0; k < WIDTH - 1; k++){
                grid[ii][k] = grid[ii - 1][k];
                colorGrid[ii][k] = colorGrid[ii - 1][k];
            }

        for (int k = 1; k < WIDTH - 1; k++) {
            grid[0][k] = ' ';
            colorGrid[0][k] = WHITE;
        }

        i++;
        draw();
        _sleep(200);
    }

    return linesCleared;
}
