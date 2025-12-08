#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>

using namespace std;

#define HEIGHT 20
#define WIDTH 15
#define BLOCK_SIZE 4

char board[HEIGHT][WIDTH] = {} ;

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

    virtual void rotate() {
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
                 || board[ty][tx] != ' ')
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

int gameSpeed = 200;
char blocks[][4][4] = {
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},

        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {' ','T',' ',' '},
         {'T','T','T',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {' ',' ','L',' '},
         {'L','L','L',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {'J',' ',' ',' '},
         {'J','J','J',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {' ','S','S',' '},
         {'S','S',' ',' '},
         {' ',' ',' ',' '}},

        {{' ',' ',' ',' '},
         {'Z','Z',' ',' '},
         {' ','Z','Z',' '},
         {' ',' ',' ',' '}}
};

int x = 4;
int y = 0;
int b = 1;

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' && y+j < HEIGHT)
                board[y+i][x+j] = ' ';
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' )
                board[y+i][x+j] = blocks[b][i][j];
}

void initBoard(){
    for (int i = 0 ; i < HEIGHT ; i++)
        for (int j = 0 ; j < WIDTH ; j++)
            if ((i==HEIGHT-1) || (j==0) || (j == WIDTH-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw(){
    gotoxy(0,0);
    for (int i = 0 ; i < HEIGHT ; i++, cout<<endl)
        for (int j = 0 ; j < WIDTH ; j++)
            cout<<board[i][j];
}

bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= WIDTH-1 || ty >= HEIGHT-1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

void animateLineClear(int line) {
    // Thay đổi các ô trong hàng thành ký tự particle
    for (int k = 1; k < WIDTH-1 ; k++ ) {
        board[line][k] = '*'; // Ký tự nhấp nháy/particle
    }
    draw();
    _sleep(100); // Dừng ngắn để người dùng thấy hiệu ứng
}

void removeLine(){
    int j;

    for (int i = HEIGHT - 2; i > 0 ; i-- ){
        for (j = 0; j < WIDTH - 1 ; j++)
            if (board[i][j] == ' ')
                break;

        if (j == WIDTH - 1){
            animateLineClear(i);

            for (int ii = i; ii >0 ; ii-- )
                for (int j = 0; j < WIDTH-1 ; j++ ) board[ii][j] = board[ii-1][j];

            i++;
            draw();
            _sleep(200);
        }
    }
}

void increaseSpeed() {
    if(gameSpeed > 50) {
        gameSpeed -= 5;
    }
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

int main() {
    srand(time(0));
    b = rand() % 2;

    hideCursor();
    system("cls");

    initBoard();
    int timer = 0;

    while (1){
        boardDelBlock();

        if (kbhit()){
            char c = getch();
            if (c=='a' && canMove(-1,0)) x--;
            if (c=='d' && canMove( 1,0)) x++;
            if (c=='x' && canMove( 0,1)) y++;
            if (c=='w') rotateBlock();
            if (c=='q') break;
        }

        if (timer > gameSpeed) {
            if (canMove(0,1)) y++;
            else {
                block2Board();
                removeLine();
                increaseSpeed();
                x = 5; y = 0; b = rand() % 2;
            }

            timer = 0;
        }

        block2Board();
        draw();

        _sleep(50);
        timer += 50;
    }

    return 0;
}
