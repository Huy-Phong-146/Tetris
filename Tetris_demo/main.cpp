# include <iostream>
# include <conio.h>
# include <windows.h>
# include <ctime>

using namespace std;

# define H 20
# define W 15

int gameSpeed = 200;
char board[H][W] = {} ;
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
            if (blocks[b][i][j] != ' ' && y+j < H)
                board[y+i][x+j] = ' ';
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' )
                board[y+i][x+j] = blocks[b][i][j];
}

void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw(){
    gotoxy(0,0);
    for (int i = 0 ; i < H ; i++, cout<<endl)
        for (int j = 0 ; j < W ; j++)
            cout<<board[i][j];
}

bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= W-1 || ty >= H-1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

void animateLineClear(int line) {
    // Thay đổi các ô trong hàng thành ký tự particle
    for (int k = 1; k < W-1 ; k++ ) {
        board[line][k] = '*'; // Ký tự nhấp nháy/particle
    }
    draw();
    _sleep(100); // Dừng ngắn để người dùng thấy hiệu ứng
}

void removeLine(){
    int j;

    for (int i = H - 2; i > 0 ; i-- ){
        for (j = 0; j < W - 1 ; j++)
            if (board[i][j] == ' ')
                break;

        if (j == W - 1){
            animateLineClear(i);

            for (int ii = i; ii >0 ; ii-- )
                for (int j = 0; j < W-1 ; j++ ) board[ii][j] = board[ii-1][j];

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

void rotateBlock() {
    char rot[4][4], tmp[4][4];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[i][j] = blocks[b][i][j];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            rot[j][3 - i] = tmp[i][j];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (rot[i][j] != ' ') {
                int tx = x + j;
                int ty = y + i;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1) return;
                if (board[ty][tx] != ' ') return;
            }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            blocks[b][i][j] = rot[i][j];
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
