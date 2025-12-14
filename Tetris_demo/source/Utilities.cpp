#include "Utilities.h"

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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
        int titleLen = title.length() + 2;
        gotoxy(x + (w - titleLen) / 2, y);
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

void playSound(int freq, int duration) {
    thread([=]() {
        Beep(freq, duration);
    }).detach();
}
