#include "Scene.h"

const vector<string> Scene::TETRIS_LOGO = {
    "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",
    "\xC8\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC\xC8\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xBC\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xCD\xCD\xBC",
    "   \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xBB     \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB",
    "   \xDB\xDB\xBA   \xDB\xDB\xC9\xCD\xCD\xBC     \xDB\xDB\xBA   \xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xDB\xDB\xBA\xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA",
    "   \xDB\xDB\xBA   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB   \xDB\xDB\xBA   \xDB\xDB\xBA  \xDB\xDB\xBA\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBA",
    "   \xC8\xCD\xBC   \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC   \xC8\xCD\xBC   \xC8\xCD\xBC  \xC8\xCD\xBC\xC8\xCD\xBC\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC"
};

const vector<string> Scene::S_THREE = {
    " \xDB\xDB\xDB\xDB\xDB\xDB\xBB",     // ██████╗
    " \xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA", // ╚════██║
    "  \xDB\xDB\xDB\xDB\xDB\xC9\xBC",    //  █████╔╝
    "  \xC8\xCD\xCD\xCD\xDB\xDB\xBB",    //  ╚═══██╗
    " \xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC", // ██████╔╝
    " \xC8\xCD\xCD\xCD\xCD\xCD\xBC "     // ╚═════╝
};

const vector<string> Scene::S_TWO = {
    " \xDB\xDB\xDB\xDB\xDB\xDB\xBB",     // ██████╗
    " \xC8\xCD\xCD\xCD\xCD\xDB\xDB\xBA", // ╚════██║
    "  \xDB\xDB\xDB\xDB\xDB\xC9\xBC",    //  █████╔╝
    " \xDB\xDB\xC9\xCD\xCD\xCD\xBC ",    // ██╔═══╝
    " \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xBB", // ███████╗
    " \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xBC"  // ╚══════╝
};

const vector<string> Scene::S_ONE = {
    "   \xDC\xDB\xBB",                   //    ▄█╗
    " \xDB\xDB\xDB\xDB\xBA",             //  ████║
    " \xC8\xCD\xDB\xDB\xBA",             //  ╚═██║
    "   \xDB\xDB\xBA",                   //    ██║
    "   \xDB\xDB\xBA",                   //    ██║
    " \xDB\xDB\xDB\xDB\xDB\xDB\xBB",     //  ██████╗
    " \xC8\xCD\xCD\xCD\xCD\xCD\xBC"      //  ╚═════╝
};

const vector<string> Scene::S_READY = {
    " \xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xDB\xDB\xBB \xDB\xDB\xDB\xDB\xDB\xDB\xBB\xDB\xDB\xBB",
    " \xDB\xDB\xC9\xCD\xCD\xCD\xBC\xC8\xCD\xDB\xDB\xC9\xCD\xBC\xDB\xDB\xC9\xCD\xDB\xDB\xBB\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xC8\xCD\xDB\xDB\xC9\xCD\xBC\xDB\xDB\xBA",
    " \xDB\xDB\xDB\xDB\xDB\xDB\xBB  \xDB\xDB\xBA  \xDB\xDB\xDB\xDB\xDB\xDB\xBA\xDB\xDB\xDB\xDB\xDB\xDB\xC9\xBC  \xDB\xDB\xBA  \xDB\xDB\xBA",
    " \xC8\xCD\xCD\xCD\xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xC9\xCD\xDB\xDB\xBA\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB  \xDB\xDB\xBA  \xC8\xCD\xBC",
    " \xDB\xDB\xDB\xDB\xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xBA \xDB\xDB\xBA\xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xBA  \xDB\xDB\xBB",
    " \xC8\xCD\xCD\xCD\xCD\xCD\xBC  \xC8\xCD\xBC  \xC8\xCD\xBC \xC8\xCD\xBC\xC8\xCD\xBC  \xC8\xCD\xBC  \xC8\xCD\xBC  \xC8\xCD\xBC"
};


void Scene::drawLogo(int x, int y, int color) {
    setColor(color);

    for (int i = 0; i < TETRIS_LOGO.size(); i++) {
        gotoxy(x, y + i);
        cout << TETRIS_LOGO[i];
    }
}

void Scene::drawLoadingBar(int x, int y, int width, int percent) {
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

void Scene::drawCountDownFrame(const vector<string>& frame, int startX, int startY, bool blink) {
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

void Scene::runIntro() {
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

void Scene::runCountDown() {
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