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

//=============================
// Thêm màu cho các khối
//=========================
enum BlockColor {
    CYAN = 11,    // Block I
    YELLOW = 14,  // Block O
    PURPLE = 13,  // Block T
    ORANGE = 12,  // Block L
    BLUE = 9,     // Block J
    GREEN = 10,   // Block S
    RED = 12      // Block Z
};
//=============================
// BaseBlock Class and Concrete Classes
//=============================
class BaseBlock {
public:
    int x;
    int y;
    vector<vector<char>> shape;
    char color;

    BaseBlock() {
        x = WIDTH / 2 - 2;
        y = 0;
        shape = vector<vector<char>>(BLOCK_SIZE, vector<char>(BLOCK_SIZE, ' '));
    }

    virtual ~BaseBlock() {}

    void rotate(const vector<vector<char>>& grid) {
        vector<vector<char>> tmp = shape;
        vector<vector<char>> rot = vector<vector<char>>(BLOCK_SIZE, vector<char>(BLOCK_SIZE, ' '));

        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++)
                rot[j][BLOCK_SIZE - i - 1] = tmp[i][j];

        for (int i = 0; i < BLOCK_SIZE; i++)
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (rot[i][j] == ' ')
                    continue;

                int tx = x + j;
                int ty = y + i;

                if (tx < 1
                 || tx >= WIDTH - 1
                 || ty >= HEIGHT - 1
                 || grid[ty][tx] != ' ')
                    return;
        }

        shape = rot;
    }
};

class BlockI : public BaseBlock {
public:
    BlockI() {
        char c = 219;
        color = CYAN;
        shape[1][0] = c; shape[1][1] = c; shape[1][2] = c; shape[1][3] = c;
    }
};

class BlockO : public BaseBlock {
public:
    BlockO() {
        char c = 219;
        color = YELLOW;
        shape[1][1] = c; shape[1][2] = c;
        shape[2][1] = c; shape[2][2] = c;
    }
};

class BlockT : public BaseBlock {
public:
    BlockT() {
        char c = 219;
        color = PURPLE;
                           shape[1][1] = c;
        shape[2][0] = c; shape[2][1] = c; shape[2][2] = c;
    }
};

class BlockL : public BaseBlock {
public:
    BlockL() {
        char c = 219;
        color = ORANGE;
                                              shape[1][2] = c;
        shape[2][0] = c; shape[2][1] = c; shape[2][2] = c;
    }
};

class BlockJ : public BaseBlock {
public:
    BlockJ() {
        char c = 219;
        color = BLUE;
        shape[1][0] = c;
        shape[2][0] = c; shape[2][1] = c; shape[2][2] = c;
    }
};

class BlockS : public BaseBlock {
public:
    BlockS() {
        char c = 219;
        color = GREEN;
                           shape[1][1] = c; shape[1][2] = c;
        shape[2][0] = c; shape[2][1] = c;
    }
};

class BlockZ : public BaseBlock {
public:
    BlockZ() {
        char c = 219;
        color = RED;
        shape[1][0] = c; shape[1][1] = c;
                           shape[2][1] = c; shape[2][2] = c;
    }
};

// =====================
// Board Class
// =====================
class Board {
public:
    vector<vector<char>> grid;
    vector<vector<int>> colorGrid; // Thêm lưới màu
    Board() {
        grid = vector<vector<char>>(HEIGHT - 1, vector<char>(WIDTH, ' '));
         colorGrid = vector<vector<int>>(HEIGHT - 1, vector<int>(WIDTH, 7)); // mặc định là màu trắng
        grid.emplace_back(vector<char>(WIDTH, '#'));
        colorGrid.emplace_back(vector<int>(WIDTH, 7));

       for (int i = 0; i < HEIGHT; i++) {
            grid[i][0] = grid[i][WIDTH-1] = '#';
            colorGrid[i][0] = colorGrid[i][WIDTH-1] = 7;
        }
    }

    void gotoxy(int x, int y) {
        COORD c = {x, y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    void draw() {
        gotoxy(0, 0);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int i = 0; i < HEIGHT; i++, cout << endl)
            for (int j = 0; j < WIDTH; j++){
                SetConsoleTextAttribute(hConsole, colorGrid[i][j]); // set màu khi vẽ
                cout << grid[i][j];
            }
            SetConsoleTextAttribute (hConsole, 7); // reset lại màu trắng
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
                if (currBlock->shape[i][j] != ' '){
                    grid[currBlock->y + i][currBlock->x + j] = currBlock->shape[i][j];
                    // Gắn màu từ lưới vào các ô
                     int tx = currBlock->x + j;
                     int ty = currBlock->y + i;
                    colorGrid[ty][tx] = currBlock->color;}
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
        for (int k = 1; k < WIDTH - 1; k++){
            grid[line][k] = '*';
            colorGrid [line][k] =15;}
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

            animateLineClear(i);
            hasLineClear = true;

            for (int ii = i; ii > 0; ii--)
                for (int k = 0; k < WIDTH - 1; k++){
                    grid[ii][k] = grid[ii - 1][k];
                    colorGrid[ii][k] = colorGrid[ii - 1][k]; // dịch màu
                }

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

                if (c == 'a' && board.canMove(-1,0, currBlock)) currBlock->x--;
                else if (c == 'd' && board.canMove( 1,0, currBlock)) currBlock->x++;
                else if (c == 'x' && board.canMove( 0,1, currBlock)) currBlock->y++;
                else if (c == 'w') currBlock->rotate(board.grid);
                else if (c == 'q') break;
            }

            if (timer > gameSpeed) {
                if (board.canMove(0,1, currBlock))
                    currBlock->y++;
                else {
                    board.blockToBoard(currBlock);

                    if (board.removeLine())
                        increaseSpeed();

                    delete currBlock;
                    currBlock = createRandomBlock();
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
     SetConsoleOutputCP(437);
    SetConsoleCP(437);
    srand(time(0));
    TetrisGame tetris;
    tetris.run();

    return 0;
}
