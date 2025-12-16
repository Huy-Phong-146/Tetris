#pragma once

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <thread>

using namespace std;

#define HEIGHT 20
#define WIDTH 15
#define BLOCK_SIZE 4
#define DEFAULT_GAME_SPEED 200
#define MAX_GAME_SPEED 50
#define D_SPEED_DECREASE 10
#define AUDIO_LENGTH 170
#define X_POS_FRAME 39
#define Y_POS_FRAME 8

const char SPACE_CHAR = (char)32;
const char BLOCK_CHAR = (char)219;
const char BORDER_V   = (char)186;
const char BORDER_H   = (char)205;
const char BORDER_BL  = (char)200;
const char BORDER_BR  = (char)188;
const char BORDER_TL  = (char)201;
const char BORDER_TR  = (char)187;


//=============================
// Thêm màu cho các khối
//=============================
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


void gotoxy(int x, int y);
void setColor(int color);
void hideCursor();
void drawFrame(int x, int y, int w, int h, string title);
void playSound(int freq, int duration);
