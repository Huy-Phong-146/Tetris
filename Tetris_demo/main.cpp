#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <unordered_map>
#include <thread>
using namespace std;

#define HEIGHT 20
#define WIDTH 15
#define BLOCK_SIZE 4
#define DEFAULT_GAME_SPEED 200
#define MAX_GAME_SPEED 50
#define D_SPEED_DECREASE 10
#define AUDIO_LENGTH 170

constexpr char BLOCK_CHAR = (char)219;
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
    BLACK = 0,
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    DARK_CYAN = 3,
    DARK_RED = 4,
    DARK_MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    BLUE = 9,
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    PURPLE = 13,
    YELLOW = 14,
    WHITE = 15
};

void playSound(int freq, int duration) {
    thread([=]() {
        Beep(freq, duration);
    }).detach();
}


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

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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
    BlockI() : BaseBlock(Color::CYAN) {
        shape[1][0] = shape[1][1] = shape[1][2] = shape[1][3] = BLOCK_CHAR;
    }
};

class BlockO : public BaseBlock {
public:
    BlockO() : BaseBlock(Color::YELLOW) {
        shape[1][1] = shape[1][2] = BLOCK_CHAR;
        shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockT : public BaseBlock {
public:
    BlockT() : BaseBlock(Color::PURPLE) {
                      shape[1][1] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockL : public BaseBlock {
public:
    BlockL() : BaseBlock(Color::RED) {
                                    shape[1][2] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockJ : public BaseBlock {
public:
    BlockJ() : BaseBlock(Color::BLUE) {
        shape[1][0] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = shape[2][2] = BLOCK_CHAR;
    }
};

class BlockS : public BaseBlock {
public:
    BlockS() : BaseBlock(Color::GREEN) {
                      shape[1][1] = shape[1][2] = BLOCK_CHAR;
        shape[2][0] = shape[2][1] = BLOCK_CHAR;
    }
};

class BlockZ : public BaseBlock {
public:
    BlockZ() : BaseBlock(Color::RED) {
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
    int offsetX;
    int offsetY;

    Board() : offsetX(3), offsetY(5) {
    grid      = vector<vector<char>>(HEIGHT - 1, vector<char>(WIDTH, ' '));
    colorGrid = vector<vector<int> >(HEIGHT, vector<int>(WIDTH, LIGHT_GRAY));

    for (vector<char>& row : grid)
        row.front() = row.back() = BORDER_V;

    grid.emplace_back(vector<char>(WIDTH, BORDER_H));
    grid.back().front() = BORDER_BL;
    grid.back().back()  = BORDER_BR;
}

    Board(int offsetX, int offsetY) : offsetX(offsetX), offsetY(offsetY) {
        grid      = vector<vector<char>>(HEIGHT - 1, vector<char>(WIDTH, ' '));
        colorGrid = vector<vector<int> >(HEIGHT, vector<int>(WIDTH, LIGHT_GRAY));

        for (vector<char>& row : grid)
            row.front() = row.back() = BORDER_V;

        grid.emplace_back(vector<char>(WIDTH, BORDER_H));
        grid.back().front() = BORDER_BL;
        grid.back().back()  = BORDER_BR;
    }

    void draw() {
        gotoxy(0,0);

        for (int i = 0; i < HEIGHT; i++, cout << endl) {
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

    // Sửa hiệu ứng particle
    void animateLineClear(int line) {
        for (int k = 1; k < WIDTH - 1; k++){
            grid[line][k] = BLOCK_CHAR;
            colorGrid [line][k] = 15;
        }

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

            Beep(1200, 50);
            Beep(1600, 50);

            animateLineClear(i);
            hasLineClear = true;

            for (int ii = i; ii > 0; ii--)
                for (int k = 0; k < WIDTH - 1; k++){
                    grid[ii][k] = grid[ii - 1][k];
                    colorGrid[ii][k] = colorGrid[ii - 1][k];
                }

            i++;
            draw();
            _sleep(200);
        }

        return hasLineClear;
    }
};

enum class GameMode {
    SOLO,
    PVP
};

// ====================
// PlayerState Class
// ====================
struct PlayerState {
    int id;
    Board board;
    int gameSpeed;
    BaseBlock* currBlock;
    BaseBlock* nextBlock;
    int timer;
    int score;
    int comboCount;
    bool isGameOver;

    char kLeft, kRight, kRotate, kSoftDrop, kHardDrop;

    PlayerState(int id, int _offsetX, int _gameSpeed,
                char _kLeft, char _kRight, char _kRotate, char _kSoftDrop, char _kHardDrop)
        : id(id), board(_offsetX, 3), gameSpeed(_gameSpeed), currBlock(nullptr), nextBlock(nullptr),
          timer(0), score(0), comboCount(0), isGameOver(false),
          kLeft(_kLeft), kRight(_kRight), kRotate(_kRotate), kSoftDrop(_kSoftDrop), kHardDrop(_kHardDrop) {}
};

// ====================
// TetrisGame Class
// ====================
class TetrisGame {
    private:
        GameMode gameMode;
        int level;

        vector<PlayerState> players;
        unordered_map<int, int> highScores;

        void loadHighScores() {
            ifstream file("high_scores.dat");
            int k, v;
            while(file >> k >> v) highScores[k] = v;
            if(!highScores.count(level)) highScores[level] = 0;
        }

        void saveHighScores() {
            ofstream file("high_scores.dat");
            for(auto& [k, v] : highScores) file << k << ' ' << v << '\n';
        }

        bool checkHighScore(int score) {
            if (score <= highScores[level])
                return false;

            highScores[level] = score;
            saveHighScores();
            return true;
        }
        void increaseSpeed(PlayerState& p) {
            if (p.gameSpeed > 50) p.gameSpeed -= D_SPEED_DECREASE;
        }

        void scoreCalculate(PlayerState& p, int linesCleared) {
            int baseScore = 0;
            switch (linesCleared){
                case 1: baseScore = 100; break;
                case 2: baseScore = 300; break;
                case 3: baseScore = 500; break;
                case 4: baseScore = 800; break;
                default: return;
            }
        }

        void handleInput(PlayerState& p, int key) {
            if (p.isGameOver) return;

            p.board.boardDeleteBlock(p.currBlock);

            if (key == p.kLeft && p.board.canMove(-1, 0, p.currBlock)) {
                p.currBlock->x--;
                playSound(400, AUDIO_LENGTH);
            } else if (key == p.kRight && p.board.canMove(1, 0, p.currBlock)) {
                p.currBlock->x++;
                playSound(400, AUDIO_LENGTH);
            } else if (key == p.kSoftDrop && p.board.canMove(0, 1, p.currBlock)) {
                p.currBlock->y++;
                p.score += 1;
                drawUI(p);
            } else if (key == p.kHardDrop) {
                while (p.board.canMove(0, 1, p.currBlock))
                    p.currBlock->y++;

                p.timer = p.gameSpeed + 1;
                playSound(800, AUDIO_LENGTH);
            } else if (key == p.kRotate) {
                p.currBlock->rotate(p.board.grid);
                playSound(600, AUDIO_LENGTH);
            }
        }

        void updatePhysics(PlayerState& p) {
            if (p.isGameOver) return;

            p.board.boardDeleteBlock(p.currBlock);

            if (p.board.canMove(0, 1, p.currBlock)) {
                p.currBlock->y++;
            } else {
                playSound(200, AUDIO_LENGTH);
                p.board.blockToBoard(p.currBlock);

                int linesCleared = p.board.removeLine();

                if (linesCleared > 0) {
                    p.comboCount++;
                    scoreCalculate(p, linesCleared);
                    increaseSpeed(p);

                    bool isNew = checkHighScore(p.score);
                    drawUI(p);
                } else {
                    p.comboCount = 0;
                    drawUI(p); // Cập nhật lại UI để xóa combo text
                }

                delete p.currBlock;
                p.currBlock = p.nextBlock;
                p.nextBlock = createRandomBlock();

                if (!p.board.canMove(0, 0, p.currBlock)) {
                    playSound(300, AUDIO_LENGTH + 100);
                    p.isGameOver = true;
                    gameOverEffect(p);
                    return;
                }

                drawNextBlock(p);
            }
        }

        void drawNextBlock(const PlayerState& p) {
            int xPos = p.board.offsetX + WIDTH * 2 + 5;
            int yPos = 11;

            for(int i = 0; i < 4; i++) {
                gotoxy(xPos + 4, yPos + 2 + i); cout << "        ";
            }

            setColor(p.nextBlock->blockColor);

            for (int i = 0; i < BLOCK_SIZE; i++) {
                for (int j = 0; j < BLOCK_SIZE; j++) {
                    gotoxy(xPos + 4 + j * 2, yPos + 2 + i);
                    cout << p.nextBlock->shape[i][j] << p.nextBlock->shape[i][j];
                }
            }

            setColor(WHITE);
        }

        void drawUI(const PlayerState& p, bool isNewRecord = false) {
            int xPos = p.board.offsetX + WIDTH * 2 + 2;
            int boxWidth = 22;

            gotoxy(xPos, 1);
            cout << "PLAYER " << p.id;

            drawFrame(xPos, 2, boxWidth, 4, "SCORE");
            gotoxy(xPos + 2, 4); cout << p.score;

            drawFrame(xPos, 7, boxWidth, 4, "XCOMBO");
            gotoxy(xPos + 2, 9); printf("x%d", p.comboCount ? p.comboCount + 1 : 0);

            drawFrame(xPos, 12, boxWidth, 6, "NEXT BLOCK");
            drawNextBlock(p);
        }

        void gameOverEffect(PlayerState& p) {
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 1; j < WIDTH - 1; j++) {
                    if (p.board.grid[i][j] != ' ')
                        p.board.colorGrid[i][j] = DARK_GRAY;
                }
            }

            Sleep(1000);

            for (int i = HEIGHT - 2; i >= 0; i--) {
                for (int j = 1; j < WIDTH - 1; j++) {
                    p.board.grid[i][j] = BLOCK_CHAR;
                    p.board.colorGrid[i][j] = RED;
                }

                if (i > 4)
                    playSound(600 - (i * 20), 125);

                p.board.draw();
                Sleep(60);
            }

            Sleep(500);
            system("cls");

            setColor(WHITE);
        }

        int showPauseMenu() {
            system("cls");
            int x = 10, y = 5, w = 40, h = 10;
            drawFrame(x, y, w, h, "PAUSE");
            gotoxy(x + 4, y + 3); cout << "1. Resume";
            gotoxy(x + 4, y + 4); cout << "2. Restart";
            gotoxy(x + 4, y + 5); cout << "3. Quit";

            char c;
            while (true) {
                c = _getch();
                if ('0' < c && c < '4') return c - '0';
            }
        }

        


        BaseBlock* createRandomBlock() {
            int r = rand() % 7;
            switch (r) {
                case 0: return new BlockI(); case 1: return new BlockO();
                case 2: return new BlockT(); case 3: return new BlockL();
                case 4: return new BlockJ(); case 5: return new BlockS();
                default: return new BlockZ();
            }
        }

    public:
        TetrisGame(GameMode mode, int level) : gameMode(mode), level(level) {
            loadHighScores();
            int initGameSpeed = DEFAULT_GAME_SPEED - 40 * (level - 1);

            system("cls");

            PlayerState p1(1, 4, initGameSpeed, 'a', 'd', 'w', 's', SPACE_CHAR);
            p1.currBlock = createRandomBlock();
            p1.nextBlock = createRandomBlock();
            players.push_back(p1);

            if (mode == GameMode::PVP) {
                PlayerState p2(2, 65, initGameSpeed, 75, 77, 72, 80, 13);
                p2.currBlock = createRandomBlock();
                p2.nextBlock = createRandomBlock();
                players.push_back(p2);
            }

            for (auto& p : players) {
                p.board.draw();
                drawUI(p);
            }
        }

        ~TetrisGame() {
            for (auto& p : players) {
                if (p.currBlock) delete p.currBlock;
                if (p.nextBlock) delete p.nextBlock;
            }
        }

        bool run() {
            const int FRAME_TICK = 30;

            while (true) {
                if (_kbhit()) {
                    int key = _getch();
                    if (key == 0 || key == 224) key = _getch();
                    else key = tolower(key);

                    if (key == 'p') {
                        playSound(800, AUDIO_LENGTH);
                        int choice = showPauseMenu();

                        if (choice == 1) {
                            system("cls");

                            for (auto& p : players) {
                                p.board.draw();
                                drawUI(p);
                            }
                        } else if (choice == 2) {
                            return true;
                        } else if (choice == 3)
                            return false;
                    }

                    if (key == 'q') {
                        playSound(800, AUDIO_LENGTH);
                        return false;
                    }

                    for (auto& player : players)
                        handleInput(player, key);
                }

                for (auto& p : players) {
                    if (p.isGameOver) continue;

                    p.board.boardDeleteBlock(p.currBlock);
                    p.timer += FRAME_TICK;

                    if (p.timer >= p.gameSpeed) {
                        updatePhysics(p);
                        p.timer = 0;
                    }

                    p.board.blockToBoard(p.currBlock);
                    p.board.draw();
                    p.timer += FRAME_TICK;
                }

                if (gameMode == GameMode::SOLO) {
                    if (players[0].isGameOver) return false;
                } else {
                    if (players[0].isGameOver || players[1].isGameOver) return false;
                }

                Sleep(FRAME_TICK);
            }
            return false;
        }

};

class Scene {
private:
    const vector<string>TETRIS_LOGO = {
        "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",
        "\xC8\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC\xC8\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC",
        "   \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xBB     \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",
        "   \xDB\xDB\xBA   \xDB\xDB\xC9\xCD\xCD\xBC     \xDB\xDB\xBA   \xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xBA\xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA",
        "   \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB   \xDB\xDB\xBA   \xDB\xDB\xBA  \xDB\xDB\xBA\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA",
        "   \xC8\xCD\xBC   \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC   \xC8\xCD\xBC   \xC8\xCD\xBC  \xC8\xCD\xBC\xC8\xCD\xBC\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC"
    };
    const vector<string> S_THREE = {
        " \xDB\xDB\xDB\xDB\xDB\xDB\xBB",     // ██████╗
        " \xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA", // ╚════██║
        "  \xDB\xDB\xDB\xDB\xDB\xC9\xBC",    //  █████╔╝
        "  \xC8\xCD\xCD\xCD\xDB\xDB\xBB",    //  ╚═══██╗
        " \xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC", // ██████╔╝
        " \xC8\xCD\xCD\xCD\xCD\xCD\xBC "     // ╚═════╝
    };
    const vector<string> S_TWO = {
        " \xDB\xDB\xDB\xDB\xDB\xDB\xBB",     // ██████╗
        " \xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA", // ╚════██║
        "  \xDB\xDB\xDB\xDB\xDB\xC9\xBC",    //  █████╔╝
        " \xDB\xDB\xC9\xCD\xCD\xCD\xBC ",    // ██╔═══╝
        " \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB", // ███████╗
        " \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC"  // ╚══════╝
    };

    const vector<string> S_ONE = {
        "   \xDC\xDB\xBB",                   //    ▄█╗
        " \xDB\xDB\xDB\xDB\xBA",             //  ████║
        " \xC8\xCD\xDB\xDB\xBA",             //  ╚═██║
        "   \xDB\xDB\xBA",                   //    ██║
        "   \xDB\xDB\xBA",                   //    ██║
        " \xDB\xDB\xDB\xDB\xDB\xDB\xBB",     //  ██████╗
        " \xC8\xCD\xCD\xCD\xCD\xCD\xBC"      //  ╚═════╝
    };
    const vector<string> S_READY = {
        " \xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xBB",
        " \xDB\xDB\xC9\xCD\xCD\xCD\xBC\xC8\xCD\xDB\xDB\xC9\xCD\xBC\xDB\xDB\xC9\xCD\xDB\xDB\xBB\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xC8\xCD\xDB\xDB\xC9\xCD\xBC\xDB\xDB\xBA",
        " \xDB\xDB\xDB\xDB\xDB\xDB\xBB  \xDB\xDB\xBA  \xDB\xDB\xDB\xDB\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC  \xDB\xDB\xBA  \xDB\xDB\xBA",
        " \xC8\xCD\xCD\xCD\xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xC9\xCD\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB  \xDB\xDB\xBA  \xC8\xCD\xBC",
        " \xDB\xDB\xDB\xDB\xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xBA \xDB\xDB\xBA\xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xBB",
        " \xC8\xCD\xCD\xCD\xCD\xCD\xBC  \xC8\xCD\xBC  \xC8\xCD\xBC \xC8\xCD\xBC\xC8\xCD\xBC  \xC8\xCD\xBC  \xC8\xCD\xBC  \xC8\xCD\xBC"
    };

    void drawLogo(int x, int y, int color) {
        setColor(color);

        for (int i = 0; i < TETRIS_LOGO.size(); i++) {
            gotoxy(x, y + i);
            cout << TETRIS_LOGO[i];
        }
    }
    void drawLoadingBar(int x, int y, int width, int percent) {
        gotoxy(x, y);

        setColor(WHITE);
        cout << "[";

        int bars = (width * percent) / 100;

        setColor(GREEN);
        for (int i = 0; i < bars; i++) cout << BLOCK_CHAR;

        setColor(WHITE);
        for (int i = bars; i < width; i++) cout << " ";

        cout << "] " << percent << "%";
    }
    void drawCountDownFrame(const vector<string>& frame, int startX, int startY, bool blink) {
        int tMax = blink ? 2 : 1;
        for (int t = 0; t < tMax; t++) {
            if (blink) {
                setColor((t % 2 == 0) ?
                        FOREGROUND_RED | FOREGROUND_INTENSITY :
                        FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }

            for (int i = 0; i < frame.size(); i++) {
                gotoxy(startX, startY + i);
                cout << frame[i];
            }

            if (blink) Sleep(300);
        }

        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

public:
    void runIntro(){
        system("cls");
        int startX = 10;
        int startY = 5;

        vector<int> sequence = {
            CYAN, YELLOW, PURPLE, GREEN, RED, BLUE, RED
        };
        int idx = 0;
        int frame = 0;

        while (!_kbhit()) {
            int currentColor = sequence[idx];
            drawLogo(startX, startY, currentColor);

            if (frame % 2 == 0)
                setColor(WHITE);
            else
                setColor(DARK_GRAY);

            gotoxy(startX + 9, startY + 8);
            cout << ">> PRESS ANY KEY TO START <<";

            idx = (idx + 1) % sequence.size();

            frame++;
            Sleep(400);
        }

        _getch();
        playSound(1500, 200);

        system("cls");
        drawLogo(startX, startY, WHITE);

        for (int i = 0; i <= 100; i += 4) {
            drawLoadingBar(startX + 6, startY + 8, 30, i);
            Sleep(30);
        }

        Sleep(500);
        setColor(LIGHT_GRAY);
        system("cls");
    }
    void runCountDown(){
        int x = 14, y = 7;
        const vector<vector<string>> sCountDown = {S_THREE, S_TWO, S_ONE, S_READY};

        for (int i = 0; i < sCountDown.size(); i++) {
            system("cls");
            bool blink = (i < 3);

            if (i < 3) playSound(600, AUDIO_LENGTH);
            else playSound(1200, 600);

            drawCountDownFrame(sCountDown[i], x, y, blink);
            _sleep(800);
        }

        system("cls");
    }
    

};


// ====================
// Intro Class
// ====================
// class IntroScene {
// private:
//     const vector<string> BIG_LOGO = {
//         // Dòng 1: "████████╗███████╗████████╗██████╗ ██╗███████╗"
//         "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",

//         // Dòng 2: "╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝"
//         "\xC8\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC\xC8\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC",

//         // Dòng 3: "   ██║   █████╗     ██║   ██████╔╝██║███████╗"
//         "   \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xBB     \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",

//         // Dòng 4: "   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║"
//         "   \xDB\xDB\xBA   \xDB\xDB\xC9\xCD\xCD\xBC     \xDB\xDB\xBA   \xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xBA\xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA",

//         // Dòng 5: "   ██║   ███████╗   ██║   ██║  ██║██║███████║"
//         "   \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB   \xDB\xDB\xBA   \xDB\xDB\xBA  \xDB\xDB\xBA\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA",

//         // Dòng 6: "   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝"
//         "   \xC8\xCD\xBC   \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC   \xC8\xCD\xBC   \xC8\xCD\xBC  \xC8\xCD\xBC\xC8\xCD\xBC\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC"
//     };

//     void drawLogo(int x, int y, int color) {
//         setColor(color);

//         for (int i = 0; i < BIG_LOGO.size(); i++) {
//             gotoxy(x, y + i);
//             cout << BIG_LOGO[i];
//         }
//     }

//     void drawLoadingBar(int x, int y, int width, int percent) {
//         gotoxy(x, y);

//         setColor(WHITE);
//         cout << "[";

//         int bars = (width * percent) / 100;

//         setColor(GREEN);
//         for (int i = 0; i < bars; i++) cout << BLOCK_CHAR;

//         setColor(WHITE);
//         for (int i = bars; i < width; i++) cout << " ";

//         cout << "] " << percent << "%";
//     }

// public:
//     void run() {
//         system("cls");
//         int startX = 10;
//         int startY = 5;

//         vector<int> sequence = {
//             CYAN, YELLOW, PURPLE, GREEN, RED, BLUE, RED
//         };

//         int idx = 0;
//         int frame = 0;

//         while (!_kbhit()) {
//             int currentColor = sequence[idx];
//             drawLogo(startX, startY, currentColor);

//             if (frame % 2 == 0)
//                 setColor(WHITE);
//             else
//                 setColor(DARK_GRAY);

//             gotoxy(startX + 9, startY + 8);
//             cout << ">> PRESS ANY KEY TO START <<";

//             idx = (idx + 1) % sequence.size();

//             frame++;
//             Sleep(400);
//         }

//         _getch();

//         system("cls");
//         drawLogo(startX, startY, WHITE);

//         for (int i = 0; i <= 100; i += 4) {
//             drawLoadingBar(startX + 7, startY + 8, 30, i);
//             Sleep(30);
//         }

//         Sleep(500);
//         setColor(LIGHT_GRAY);
//         system("cls");
//     }
// };

class GameManager {
private:
    int menu() {
        while (true) {
            system("cls");

            int x = 10, y = 5, w = 40, h = 12;
            drawFrame(x, y, w, h, "TETRIS MASTER");

            gotoxy(x + 4, y + 3);  cout << "1. Single Mode";
            gotoxy(x + 4, y + 4);  cout << "2. PvP Mode";
            gotoxy(x + 4, y + 5);  cout << "3. View High Score";
            gotoxy(x + 4, y + 6);  cout << "4. Quit";
            gotoxy(x + 12, y + 9); cout << "Enter your choice";

            char c = _getch();
            playSound(800, AUDIO_LENGTH);

            if ('0' < c && c < '5') return c - '0';
        }
    }

    int chooseLevel() {
        while (true) {
            system("cls");

            int x = 10, y = 5, w = 40, h = 10;
            drawFrame(x, y, w, h, "SELECT LEVEL");

            gotoxy(x + 4, y + 3);  cout << "1. Normal Level";
            gotoxy(x + 4, y + 4);  cout << "2. Medium Level";
            gotoxy(x + 4, y + 5);  cout << "3. Hard Level";
            gotoxy(x + 4, y + 6);  cout << "4. Back";
            gotoxy(x + 12, y + 7); cout << "Enter your choice";

            char mode = _getch();
            playSound(800, AUDIO_LENGTH);

            if ('0' < mode && mode < '5')
                return mode - '0';
        }
    }

    unordered_map<int, int> loadHighScore() {
        unordered_map<int, int> scores;
        ifstream file("high_scores.dat");
        int key;
        int value;

        while (file >> key >> value)
            scores[key] = value;

        return scores;
    }

    void showHighScore() {
        system("cls");
        unordered_map<int, int> scores = loadHighScore();

        int hsNormal = scores[1];
        int hsMedium = scores[2];
        int hsHard   = scores[3];

        int x = 10, y = 5, w = 40, h = 12;
        drawFrame(x, y, w, h, "HIGH SCORES");

        gotoxy(x + 6, y + 4); cout << "NORMAL : " << hsNormal;
        gotoxy(x + 6, y + 5); cout << "MEDIUM : " << hsMedium;
        gotoxy(x + 6, y + 6); cout << "HARD   : " << hsHard;

        gotoxy(x + 9, y + 9); cout << "Press any key to return";
        getch();
        playSound(600, AUDIO_LENGTH);
    }

public:
    void runProgram() {
        hideCursor();

        Scene scene;
        scene.runIntro();

        while (true) {
            int option = menu();

            if (option == 1 || option == 2) {
                GameMode mode = option == 1 ? GameMode::SOLO : GameMode::PVP;
                int level = chooseLevel();

                if (level == 4)
                    continue;

                bool playAgain = false;

                do {
                    scene.runCountDown();
                    TetrisGame tetris(mode, level);
                    playAgain = tetris.run();
                } while (playAgain);
            } else if (option == 3) {
                showHighScore();
            } else if (option == 4)
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
