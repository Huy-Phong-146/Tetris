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

const char BORDER_V   = (char)186;
const char BORDER_H   = (char)205;
const char BORDER_BL  = (char)200;
const char BORDER_BR  = (char)188;
const char BORDER_TL  = (char)201;
const char BORDER_TR  = (char)187;

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//=============================
// Thêm menu bao gồm :
// 1.Bắt đầu game
// 2.Xem highscore
// 3.Thoát
void drawMenuFrame(int x, int y, int w, int h, string title = "") {
    gotoxy(x, y);
    cout << (char)201;    
    for (int i = 0; i < w - 2; i++) cout << (char)205;
    cout << (char)187;

    if (!title.empty()) {
        gotoxy(x + (w - title.length()) / 2, y);
        cout << " " << title << " ";
    }

    for (int i = 1; i < h - 1; i++) {
        gotoxy(x, y + i);         cout << (char)186;
        gotoxy(x + w - 1, y + i); cout << (char)186;
    }

    gotoxy(x, y + h - 1);
    cout << (char)200;
    for (int i = 0; i < w - 2; i++) cout << (char)205;
    cout << (char)188;
}// dùng lại hàm vẽ khung trong Class Tetris game


int menu() {
    while (true) {
        system("cls");

        int x = 10, y = 3, w = 40, h = 10;

        drawMenuFrame(x, y, w, h, "MENU");

        gotoxy(x + 4, y + 3); cout << "1. Start Game";
        gotoxy(x + 4, y + 4); cout << "2. View High Score";
        gotoxy(x + 4, y + 5); cout << "3. Quit";

        gotoxy(x + 4, y + 7); cout << "Enter your choice: ";

        int c;
        cin >> c;
        if (c >= 1 && c <= 3) return c;
    }
}

//Chọn chế độ chơi sau khi chọn option Start Game
int chooseMode() {
    while (true) {
        system("cls");

        int x = 10, y = 5, w = 40, h = 10;

        drawMenuFrame(x, y, w, h, "SELECT MODE");

        gotoxy(x + 4, y + 3); cout << "1. Normal Mode";
        gotoxy(x + 4, y + 4); cout << "2. Hard Mode";

        gotoxy(x + 4, y + 7); cout << "Enter your choice: ";

        int mode;
        cin >> mode;
        if (mode == 1 || mode == 2) return mode;
    }
}

// Show điểm cao khi chọn option View High Score
void showHighScore() {
    system("cls");

    ifstream file("highest_score.txt");
    int hs = 0;
    if (file.is_open()) file >> hs;

    int x = 10, y = 5, w = 40, h = 10;

    drawMenuFrame(x, y, w, h, "HIGH SCORE");

    gotoxy(x + 4, y + 4);
    cout << "Highest Score: " << hs;

    gotoxy(x + 4, y + 7);
    cout << "Press any key to return...";
    getch();  
}
//=============================


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

    void drawFrame(int x, int y, int w, int h, string title) {
        gotoxy(x, y);
        cout << BORDER_TL;
        for (int i = 0; i < w - 2; i++) cout << BORDER_H;
        cout << BORDER_TR;

        if (!title.empty()) {
            gotoxy(x + (w - title.length()) / 2, y); // Căn giữa tiêu đề
            cout << " " << title << " ";
        }

        for (int i = 1; i < h - 1; i++) {
            gotoxy(x, y + i);         cout << BORDER_V;
            gotoxy(x + w - 1, y + i); cout << BORDER_V;
        }

        gotoxy(x, y + h - 1);
        cout << BORDER_BL;
        for (int i = 0; i < w - 2; i++) cout << BORDER_H;
        cout << BORDER_BR;
    }

    void drawNextBlock() {
        int xPos = WIDTH * 2 + 5;
        int yPos = 11;

        for(int i = 0; i < BLOCK_SIZE; i++) {
            gotoxy(xPos + 9, yPos + 2 + i);
            cout << "    ";
        }

        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (nextBlock->shape[i][j] != ' ') {
                    gotoxy(xPos + 9 + j, yPos + 2 + i);
                    cout << nextBlock->shape[i][j];
                }
            }
        }
    }


    void drawUI(bool isNewRecord = false) {
        int xPos = WIDTH * 2 + 5;
        int boxWidth = 22;

        drawFrame(xPos, 2, boxWidth, 4, "SCORE");
        gotoxy(xPos + 2, 4);
        cout << score;

        string highScoreTitle = isNewRecord ? "NEW RECORD!" : "HIGH SCORE";
        drawFrame(xPos, 7, boxWidth, 4, highScoreTitle);

        gotoxy(xPos + 2, 9);
        cout << highestScore;

        drawFrame(xPos, 12, boxWidth, 6, "NEXT BLOCK");
        drawNextBlock();

        int yControl = 19;

        drawFrame(xPos, yControl, boxWidth, 7, "CONTROLS");
        gotoxy(xPos + 2, yControl + 1);  cout << " A : Move Left";
        gotoxy(xPos + 2, yControl + 2);  cout << " D : Move Right";
        gotoxy(xPos + 2, yControl + 3);  cout << " S : Soft Drop";
        gotoxy(xPos + 2, yControl + 4);  cout << " W : Rotate";
        gotoxy(xPos + 2, yControl + 5);  cout << " Q : Quit Game";
    }

public:
    //Sửa constructor truyền vào tham số để chọn chế độ chơi
    TetrisGame(int mode = 1) {
        if (mode == 1) gameSpeed = DEFAULT_GAME_SPEED;   // chế độ thường
        else gameSpeed = 120;                            // chế độ khó

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

void runProgram() {
    while (true) {
        int option = menu();

        if (option == 1) {
            int mode = chooseMode();
            TetrisGame tetris(mode);
            tetris.run();
        }
        else if (option == 2) {
            showHighScore();
        }
        else if (option == 3) {
            break;
        }
    }
}

int main() {
    srand(time(0));
    runProgram();

    return 0;
}
