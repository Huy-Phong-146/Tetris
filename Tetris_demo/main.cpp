#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>

using namespace std;

#define HEIGHT 20
#define WIDTH 15
#define BLOCK_SIZE 4
#define DEFAULT_GAME_SPEED 200
#define MAX_GAME_SPEED 50
#define D_SPEED_DECREASE 10

const char BLOCK_CHAR = (char)219;
const char BORDER_V   = (char)186;
const char BORDER_H   = (char)205;
const char BORDER_BL  = (char)200;
const char BORDER_BR  = (char)188;

//=============================
// BaseBlock Class and Concrete Classes
//=============================
class BaseBlock {
public:
    int x;
    int y;
    vector<vector<char>> shape;

    BaseBlock() {
        x = WIDTH / 2 - 2;
        y = 0;
        shape = vector<vector<char>>(BLOCK_SIZE, vector<char>(BLOCK_SIZE, ' '));
    }

    virtual ~BaseBlock() {}

    void rotate(const vector<vector<char>>& grid) {
    vector<vector<char>> old = shape;
    vector<vector<char>> rot(BLOCK_SIZE, vector<char>(BLOCK_SIZE, ' '));

    // Xoay 90 độ
    for (int i = 0; i < BLOCK_SIZE; i++)
        for (int j = 0; j < BLOCK_SIZE; j++)
            rot[j][BLOCK_SIZE - i - 1] = old[i][j];

    // Các offset wall-kick (X, Y)
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
};

class BlockI : public BaseBlock {
public:
    BlockI() {
        shape[1][0] = shape[1][1] = shape[1][2] = shape[1][3] = BLOCK_CHAR;
    }
};

class BlockO : public BaseBlock {
public:
    BlockO() {
        shape[1][1] = shape[1][2] = BLOCK_CHAR;
        shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockT : public BaseBlock {
public:
    BlockT() {
                      shape[1][1] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockL : public BaseBlock {
public:
    BlockL() {
                                    shape[1][2] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockJ : public BaseBlock {
public:
    BlockJ() {
        shape[1][0] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockS : public BaseBlock {
public:
    BlockS() {
                      shape[1][1] = shape[1][2] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = BLOCK_CHAR;
    }
};

class BlockZ : public BaseBlock {
public:
    BlockZ() {
        shape[1][0] = shape[1][1] = BLOCK_CHAR;
                      shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

// =====================
// Board Class
// =====================
class Board {
public:
    vector<vector<char>> grid;

    Board() {
        grid = vector<vector<char>>(HEIGHT - 1, vector<char>(WIDTH, ' '));

        for (vector<char>& row : grid)
            row.front() = row.back() = BORDER_V;

        grid.emplace_back(vector<char>(WIDTH, BORDER_H));
        grid.back().front() = BORDER_BL;
        grid.back().back()  = BORDER_BR;
    }

    void gotoxy(int x, int y) {
        COORD c = {x, y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    void draw() {
        gotoxy(0,0);

        for (int i = 0; i < HEIGHT; i++, cout << endl)
            for (int j = 0; j < WIDTH; j++)
                cout << grid[i][j] << grid[i][j];
    }

    void boardDeleteBlock(BaseBlock* currBlock) {
        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++)
                if (currBlock->shape[i][j] != ' ' && currBlock->y + j < HEIGHT)
                    grid[currBlock->y + i][currBlock->x + j] = ' ';
    }

    void blockToBoard(BaseBlock* currBlock){
        for (int i = 0 ; i < BLOCK_SIZE; i++)
            for (int j = 0 ; j < BLOCK_SIZE; j++)
                if (currBlock->shape[i][j] != ' ')
                    grid[currBlock->y + i][currBlock->x + j] = currBlock->shape[i][j];
    }

    bool canMove(int dx, int dy, BaseBlock* currBlock) {
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

    void animateLineClear(int line) {
        for (int k = 1; k < WIDTH - 1; k++)
            grid[line][k] = '*';

        draw();
        _sleep(100);
    }


    bool removeLine(){
        bool hasLineClear = false;

        for (int i = HEIGHT - 2; i > 0; i--){
            int j;

            for (j = 0; j < WIDTH - 1; j++)
                if (grid[i][j] == ' ')
                    break;

            if (j != WIDTH - 1)
                continue;

            // Am thanh khi Tang diem
            Beep(1200, 50);
            Beep(1600, 50);

            animateLineClear(i);
            hasLineClear = true;

            for (int ii = i; ii > 0; ii--)
                for (int k = 0; k < WIDTH - 1; k++)
                    grid[ii][k] = grid[ii - 1][k];

            i++;
            draw();
            _sleep(200);
        }

        return hasLineClear;
    }
};

// ====================
// TetrisGame Class
// ====================
class TetrisGame {
private:
    Board board;
    BaseBlock* currBlock;
    int gameSpeed;

    void hideCursor() {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    BaseBlock* createRandomBlock() {
        int r = rand() % 7;

        switch (r) {
            case 0: return new BlockI();
            case 1: return new BlockO();
            case 2: return new BlockT();
            case 3: return new BlockL();
            case 4: return new BlockJ();
            case 5: return new BlockS();
            default : return new BlockZ();
        }
    }

    void increaseSpeed() {
        if (gameSpeed > MAX_GAME_SPEED)
            gameSpeed -= D_SPEED_DECREASE;
    }

public:
    TetrisGame() {
        gameSpeed = DEFAULT_GAME_SPEED;
        hideCursor();
        system("cls");
        currBlock = createRandomBlock();
    }

    void run() {
        int timer = 0;

        while (1){
            board.boardDeleteBlock(currBlock);

            if (kbhit()){
                char c = getch();

                // Qua trai
                if (c == 'a' && board.canMove(-1,0, currBlock)) {
                    currBlock->x--;
                    Beep(400, 30);
                }
                // Qua phai
                else if (c == 'd' && board.canMove( 1,0, currBlock)) {
                    currBlock->x++;
                    Beep(400, 30);
                }
                // Di chuyen xuong nhanh
                else if (c == 'x' && board.canMove( 0,1, currBlock)) {
                    currBlock->y++;
                    Beep(450, 30);
                }
                // Xoay
                else if (c == 'w') {
                    currBlock->rotate(board.grid);
                    Beep(600, 30);
                }
                else if (c == ' ') { // HARD DROP
    while (board.canMove(0, 1, currBlock)) {
        currBlock->y++;
    }

    // Ép timer để khối được đặt ngay lập tức
    timer = gameSpeed + 1;
    Beep(800, 50);
}
                else if (c == 'q') break;
            }

            if (timer > gameSpeed) {
                if (board.canMove(0,1, currBlock))
                    currBlock->y++;
                else {

                    Beep(200, 50); // Them am thanh khi dap dat
                    board.blockToBoard(currBlock);

                    if (board.removeLine())
                        increaseSpeed();

                    delete currBlock;
                    currBlock = createRandomBlock();

                    // Them am thanh, thoat vong lap game khi Game Over
                    if (!board.canMove(0, 0, currBlock)) {
                        Beep(300, 800);
                        break;
                    }
                }

                timer = 0;
            }

            board.blockToBoard(currBlock);
            board.draw();

            _sleep(50);
            timer += 50;
        }
    }
};

int main() {
    srand(time(0));
    TetrisGame tetris;
    tetris.run();

    return 0;
}
