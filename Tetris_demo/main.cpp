#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

#define HEIGHT 20
#define WIDTH 15
#define BLOCK_SIZE 4
#define DEFAULT_GAME_SPEED 200
#define MAX_GAME_SPEED 50
#define D_SPEED_DECREASE 10


void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

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
        shape[1][0] = 'I'; shape[1][1] = 'I'; shape[1][2] = 'I'; shape[1][3] = 'I';
    }
};

class BlockO : public BaseBlock {
public:
    BlockO() {
        shape[1][1] = 'O'; shape[1][2] = 'O';
        shape[2][1] = 'O'; shape[2][2] = 'O';
    }
};

class BlockT : public BaseBlock {
public:
    BlockT() {
                           shape[1][1] = 'T';
        shape[2][0] = 'T'; shape[2][1] = 'T'; shape[2][2] = 'T';
    }
};

class BlockL : public BaseBlock {
public:
    BlockL() {
                                              shape[1][2] = 'L';
        shape[2][0] = 'L'; shape[2][1] = 'L'; shape[2][2] = 'L';
    }
};

class BlockJ : public BaseBlock {
public:
    BlockJ() {
        shape[1][0] = 'J';
        shape[2][0] = 'J'; shape[2][1] = 'J'; shape[2][2] = 'J';
    }
};

class BlockS : public BaseBlock {
public:
    BlockS() {
                           shape[1][1] = 'S'; shape[1][2] = 'S';
        shape[2][0] = 'S'; shape[2][1] = 'S';
    }
};

class BlockZ : public BaseBlock {
public:
    BlockZ() {
        shape[1][0] = 'Z'; shape[1][1] = 'Z';
                           shape[2][1] = 'Z'; shape[2][2] = 'Z';
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
        grid.emplace_back(vector<char>(WIDTH, '#'));

        for (vector<char>& row : grid)
            row.front() = row.back() = '#';
    }

    void draw() {
        gotoxy(0, 0);

        for (int i = 0; i < HEIGHT; i++, cout << endl)
            for (int j = 0; j < WIDTH; j++)
                cout << grid[i][j];
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
    BaseBlock* nextBlock;
    int gameSpeed;
    int score;
    int highestScore;

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

    void loadHighestScore() {
        ifstream file("highest_score.txt");

        if (file.is_open()) {
            file >> highestScore;
            file.close();
        } else
            highestScore = 0;
    }

    void saveHighestScore() {
        ofstream file("highest_score.txt");

        if (file.is_open()) {
            file << highestScore;
            file.close();
        }
    }

    bool checkHighScore() {
        if (score <= highestScore)
            return false;

        highestScore = score;
        saveHighestScore();
        return true;
    }

    void drawNextBlock() {
        int xPos = WIDTH * 2 + 5;
        int yPos = 17;

        gotoxy(xPos, yPos);     cout << "== NEXT BLOCK =========";

        for(int i = 0; i < 4; i++) {
            gotoxy(xPos + 5, yPos + 2 + i);
            cout << "    ";
        }

        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (nextBlock->shape[i][j] != ' ') {
                    gotoxy(xPos + 5 + j, yPos + 2 + i);
                    cout << nextBlock->shape[i][j];
                }
            }
        }

        gotoxy(xPos, yPos + 7); cout << "=======================";
    }

    void drawUI(bool isNewRecord = false) {
        int xPos = WIDTH * 2 + 5;
        int yPos = 2;

        gotoxy(xPos, yPos);     cout << "== SCORE ==============";
        gotoxy(xPos, yPos + 1); cout << "|                     |";
        gotoxy(xPos, yPos + 2); cout << "=======================";
        gotoxy(xPos + 2, yPos + 1); cout << score;

        if (isNewRecord) {
            gotoxy(xPos, yPos + 5); cout << "== HIGH SCORE (NEW!) ==";
        } else {
            gotoxy(xPos, yPos + 5); cout << "== HIGH SCORE =========";
        }

        gotoxy(xPos, yPos + 6); cout << "|                     |";
        gotoxy(xPos, yPos + 7); cout << "=======================";
        gotoxy(xPos + 2, yPos + 6); cout << highestScore;

        gotoxy(xPos, yPos + 10); cout << "Controls:";
        gotoxy(xPos, yPos + 11); cout << " A/D : Move";
        gotoxy(xPos, yPos + 12); cout << " W   : Rotate";
        gotoxy(xPos, yPos + 13); cout << " X   : Soft Drop";
        gotoxy(xPos, yPos + 14); cout << " Q   : Quit Game";

        drawNextBlock();
    }
public:
    TetrisGame() {
        gameSpeed = DEFAULT_GAME_SPEED;
        hideCursor();
        system("cls");

        currBlock = createRandomBlock();
        nextBlock = createRandomBlock();
        score = 0;
        loadHighestScore();
        drawUI();
    }

    void run() {
        int timer = 0;

        while (1){
            board.boardDeleteBlock(currBlock);

            if (kbhit()){
                char c = getch();

                if (c == 'a' && board.canMove(-1,0, currBlock)) currBlock->x--;
                else if (c == 'd' && board.canMove( 1,0, currBlock)) currBlock->x++;
                else if (c == 'x' && board.canMove( 0,1, currBlock)) {
                    currBlock->y++;
                    score++;
                    bool isNew = checkHighScore();
                    drawUI(isNew);
                } else if (c == 'w') currBlock->rotate(board.grid);
                else if (c == 'q') break;
            }

            if (timer > gameSpeed) {
                if (board.canMove(0,1, currBlock))
                    currBlock->y++;
                else {
                    board.blockToBoard(currBlock);

                    if (board.removeLine()) {
                        score += 30;
                        increaseSpeed();
                        bool isNew = checkHighScore();
                        drawUI(isNew);
                    }

                    delete currBlock;
                    currBlock = nextBlock;
                    nextBlock = createRandomBlock();

                    drawNextBlock();
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
