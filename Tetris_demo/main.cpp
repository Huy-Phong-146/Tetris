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

const char BLOCK_CHAR = (char)219;
const char BORDER_V   = (char)186;
const char BORDER_H   = (char)205;
const char BORDER_BL  = (char)200;
const char BORDER_BR  = (char)188;
const char BORDER_TL  = (char)201;
const char BORDER_TR  = (char)187;
const char SPACE_CHAR = (char)32;

//=============================
// Thêm màu cho các khối
//=========================
enum Color {
    WHITE = 7,
    CYAN = 11,    // Block I
    YELLOW = 14,  // Block O
    PURPLE = 13,  // Block T
    ORANGE = 12,  // Block L
    BLUE = 9,     // Block J
    GREEN = 10,   // Block S
    RED = 12      // Block Z
};


void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
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

//=============================
// BaseBlock Class and Concrete Classes
//=============================
class BaseBlock {
public:
    int x;
    int y;
    vector<vector<char>> shape;
    Color blockColor;

    BaseBlock(Color blockColor): blockColor(blockColor) {
        x = WIDTH / 2 - 2;
        y = 0;
        shape = vector<vector<char>>(BLOCK_SIZE, vector<char>(BLOCK_SIZE, ' '));
    }

    virtual ~BaseBlock() {}

    void rotate(const vector<vector<char>>& grid) {
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
};

class BlockI : public BaseBlock {
public:
    BlockI() : BaseBlock(CYAN) {
        shape[1][0] = shape[1][1] = shape[1][2] = shape[1][3] = BLOCK_CHAR;
    }
};

class BlockO : public BaseBlock {
public:
    BlockO() : BaseBlock(YELLOW) {
        shape[1][1] = shape[1][2] = BLOCK_CHAR;
        shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockT : public BaseBlock {
public:
    BlockT() : BaseBlock(PURPLE) {
                      shape[1][1] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockL : public BaseBlock {
public:
    BlockL() : BaseBlock(ORANGE) {
                                    shape[1][2] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockJ : public BaseBlock {
public:
    BlockJ() : BaseBlock(BLUE) {
        shape[1][0] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockS : public BaseBlock {
public:
    BlockS() : BaseBlock(GREEN) {
                      shape[1][1] = shape[1][2] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = BLOCK_CHAR;
    }
};

class BlockZ : public BaseBlock {
public:
    BlockZ() : BaseBlock(RED) {
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
    vector<vector<int>> colorGrid;

    Board() {
        grid      = vector<vector<char>>(HEIGHT - 1, vector<char>(WIDTH, ' '));
        colorGrid = vector<vector<int> >(HEIGHT, vector<int>(WIDTH, WHITE));

        for (vector<char>& row : grid)
            row.front() = row.back() = BORDER_V;

        grid.emplace_back(vector<char>(WIDTH, BORDER_H));
        grid.back().front() = BORDER_BL;
        grid.back().back()  = BORDER_BR;
    }

    void draw() {
        gotoxy(0,0);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        for (int i = 0; i < HEIGHT; i++, cout << endl) {
            SetConsoleTextAttribute (hConsole, WHITE);
            cout << grid[i][0];

            for (int j = 1; j < WIDTH - 1; j++) {
                SetConsoleTextAttribute(hConsole, colorGrid[i][j]);
                cout << grid[i][j] << grid[i][j];
            }

            SetConsoleTextAttribute (hConsole, WHITE);
            cout << grid[i][WIDTH - 1];
        }
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
                    int tx = currBlock->x + j;
                    int ty = currBlock->y + i;
                    grid[ty][tx] = currBlock->shape[i][j];
                    colorGrid[ty][tx] = currBlock->blockColor;
                }
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
            colorGrid [line][k] = 15;
        }

        draw();
        _sleep(100);
    }


    int removeLine(){
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

            Beep(1200, 50);
            Beep(1600, 50);

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
    int level;
    int highestScore;
    int comboCount;
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
        int yPos = 11;

        for(int i = 0; i < BLOCK_SIZE; i++) {
            gotoxy(xPos + 9, yPos + 2 + i);
            cout << "    ";
        }

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute (hConsole, nextBlock->blockColor);

        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                gotoxy(xPos + 8 + j * 2, yPos + 2 + i);
                cout << nextBlock->shape[i][j] << nextBlock->shape[i][j];
            }
        }

        SetConsoleTextAttribute (hConsole, WHITE);
    }


    void drawUI(bool isNewRecord = false) {
        int xPos = WIDTH * 2 + 5;
        int boxWidth = 22;

        drawFrame(xPos, 2, boxWidth, 4, "SCORE");
        gotoxy(xPos + 2, 4);
        cout << "        ";              
        gotoxy(xPos + 2, 4);
        cout << score;
        
        int comboBoxX = xPos + boxWidth + 2;  
        int comboBoxW = 16;

        drawFrame(comboBoxX, 2, comboBoxW, 4, "XCOMBO");
        gotoxy(comboBoxX + 2, 4);
        cout << "        ";              
        gotoxy(comboBoxX + 2, 4);

        if (comboCount > 0)
            cout << "x" << (comboCount + 1); 
        else
            cout << "x0";

        string highScoreTitle = isNewRecord ? "NEW RECORD!" : "HIGH SCORE";
        drawFrame(xPos, 7, boxWidth, 4, highScoreTitle);

        gotoxy(xPos + 2, 9);
        cout << highestScore;

        drawFrame(xPos, 12, boxWidth, 6, "NEXT BLOCK");
        drawNextBlock();

        int yControl = 19;

        drawFrame(xPos, yControl, boxWidth, 9, "CONTROLS");
        gotoxy(xPos + 2, yControl + 1);  cout << "A     : Move Left";
        gotoxy(xPos + 2, yControl + 2);  cout << "D     : Move Right";
        gotoxy(xPos + 2, yControl + 3);  cout << "S     : Soft Drop";
        gotoxy(xPos + 2, yControl + 4);  cout << "SPACE : HARD Drop";
        gotoxy(xPos + 2, yControl + 5);  cout << "W     : Rotate";
        gotoxy(xPos + 2, yControl + 6);  cout << "P     : Pause Game";
        gotoxy(xPos + 2, yControl + 7);  cout << "Q     : Quit Game";
    }

    void scoreCalculate(int linesCleared){
        int baseScore = 0;
        switch (linesCleared){
            case 1: baseScore = 100; break;
            case 2: baseScore = 300; break;
            case 3: baseScore = 500; break;
            case 4: baseScore = 800; break; 
            default: return;
        }
        int levelMultiplier = (level);
        int comboBonus = (comboCount * 50 * linesCleared);
        score += baseScore * levelMultiplier + comboBonus;
    }

    void gameOverEffect() {


        for (int i = HEIGHT - 2; i >= 0; i--) {
            for (int j = 1; j < WIDTH - 1; j++) {
                board.grid[i][j] = '*';
                board.colorGrid[i][j] = WHITE;
            }

            board.draw();
            _sleep(40);
        }

        _sleep(300);
        system("cls");

        int x = 10, y = 5, w = 40, h = 10;

        drawFrame(x, y, w, h, "GAME OVER");

        gotoxy(x + 8, y + 4); cout << "Your Score: " << score;
        gotoxy(x + 8, y + 6); cout << "Press any key to return...";

        getch();
    }

    int showPauseMenu() {
        system("cls");
        int x = 10, y = 5, w = 40, h = 10;

        drawFrame(x, y, w, h, "PAUSE");
        gotoxy(x + 4, y + 3); cout << "1. Resume";
        gotoxy(x + 4, y + 4); cout << "2. Restart";
        gotoxy(x + 4, y + 5); cout << "3. Quit";
        gotoxy(x + 4, y + 7); cout << "Enter your choice: ";

        char c;

        while (true) {
            c = _getch();

            if ('0' < c && c < '4')
                return c -'0';
        }
    }



public:
    TetrisGame(int mode = 1) {
        level = 0;

        if (mode == 1) {
            gameSpeed = DEFAULT_GAME_SPEED;
            level = 1;
        } 
        if (mode == 2) {
            gameSpeed = DEFAULT_GAME_SPEED - 40;
            level = 2;
        } 
        if (mode == 3) {
            gameSpeed = DEFAULT_GAME_SPEED - 80;
            level = 3;
        } 

        hideCursor();
        system("cls");

        currBlock = createRandomBlock();
        nextBlock = createRandomBlock();
        score = 0;
        comboCount = 0;
        loadHighestScore();
        drawUI();
    }

    ~TetrisGame() {
        delete currBlock;
        delete nextBlock;
    }

    bool run() {
        int timer = 0;

        while (1){
            board.boardDeleteBlock(currBlock);

            if (kbhit()){
                char c = getch();
                c = tolower(c);


                if (c == 'a' && board.canMove(-1,0, currBlock)) {
                    currBlock->x--;
                    Beep(400, 30);
                } else if (c == 'd' && board.canMove( 1,0, currBlock)) {
                    currBlock->x++;
                    Beep(400, 30);
                } else if (c == 's' && board.canMove( 0,1, currBlock)) {
                    currBlock->y++;
                    Beep(450, 30);
                    bool isNew = checkHighScore();
                    drawUI(isNew);
                } else if (c == SPACE_CHAR) {
                    while (board.canMove(0, 1, currBlock))
                        currBlock->y++;

                    timer = gameSpeed + 1;
                    Beep(800, 50);
                } else if (c == 'w') {
                    currBlock->rotate(board.grid);
                    Beep(600, 30);
                } else if (c == 'q') {
                    return false;
                } else if (c == 'p') {
                    int choice = showPauseMenu();

                    if (choice == 1) {
                        system("cls");
                        board.draw();
                        drawUI(checkHighScore());
                    } else if (choice == 2)
                        return true;
                    else if (choice == 3) {
                        return false;
                    }
                }
        }

            if (timer >= gameSpeed) {
                if (board.canMove(0,1, currBlock))
                    currBlock->y++;
                else {
                    Beep(200, 50);
                    board.blockToBoard(currBlock);

                    int linesCleared = board.removeLine();
                    if (linesCleared > 0) {
                        scoreCalculate(linesCleared);
                        comboCount++;
                        increaseSpeed();
                        bool isNew = checkHighScore();
                        drawUI(isNew);
                    }
                    else{
                        comboCount = 0;
                        drawUI(false);
                    }

                    delete currBlock;
                    currBlock = nextBlock;
                    nextBlock = createRandomBlock();

                    if (!board.canMove(0, 0, currBlock)) {
                        Beep(300, 800);
                        gameOverEffect();
                        return false;
                    }

                    drawNextBlock();
                }

                timer = 0;
            }

            board.blockToBoard(currBlock);

            board.draw();

            _sleep(50);
            timer += 50;
        }

        return false;
    }
};


class GameManager {
private:
    int menu() {
        while (true) {
            system("cls");

            int x = 10, y = 3, w = 40, h = 10;
            drawFrame(x, y, w, h, "TETRIS MASTER");

            gotoxy(x + 4, y + 3); cout << "1. Start Game";
            gotoxy(x + 4, y + 4); cout << "2. View High Score";
            gotoxy(x + 4, y + 5); cout << "3. Quit";

            gotoxy(x + 12, y + 7); cout << "Enter your choice";

            char c = _getch();
            if ('0' < c && c < '4') return c - '0';
        }
    }

    int chooseMode() {
        while (true) {
            system("cls");

            int x = 10, y = 5, w = 40, h = 10;
            drawFrame(x, y, w, h, "SELECT MODE");

            gotoxy(x + 4, y + 3); cout << "1. Normal Mode";
            gotoxy(x + 4, y + 4); cout << "2. Medium Mode";
            gotoxy(x + 4, y + 5); cout << "3. Hard Mode";
            gotoxy(x + 4, y + 6); cout << "4. Back";
            gotoxy(x + 4, y + 7); cout << "Enter your choice: ";

            char mode = _getch();

            if ('0' < mode && mode < '5')
                return mode - '0';
        }
    }

    void showHighScore() {
        system("cls");

        ifstream file("highest_score.txt");
        int hs = 0;

        if (file.is_open())
            file >> hs;

        int x = 10, y = 5, w = 40, h = 10;
        drawFrame(x, y, w, h, "HIGH SCORE");

        gotoxy(x + 4, y + 4); cout << "Highest Score: " << hs;
        gotoxy(x + 7, y + 7); cout << "Press any key to return...";
        getch();
    }

public:
    void runProgram() {
        hideCursor();

        while (true) {
            int option = menu();

            if (option == 1) {
                int mode = chooseMode();

                if (mode == 4)
                    continue;

                bool playAgain = false;

                do {
                    TetrisGame tetris(mode);
                    playAgain = tetris.run();
                } while (playAgain);
            } else if (option == 2) {
                showHighScore();
            } else if (option == 3)
                break;
        }

        gotoxy(0, HEIGHT);
    }
};


int main() {
    SetConsoleOutputCP(437);
    SetConsoleCP(437);
    srand(time(0));
    GameManager app;
    app.runProgram();

    return 0;
}
