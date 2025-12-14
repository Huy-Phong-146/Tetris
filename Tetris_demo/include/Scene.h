#pragma once

#include "Utilities.h"

// ====================
// Scene Class
// ====================
class Scene {
private:
    static const vector<string> TETRIS_LOGO;
    static const vector<string> S_THREE;
    static const vector<string> S_TWO;
    static const vector<string> S_ONE;
    static const vector<string> S_READY;

    void drawLogo(int x, int y, int color);
    void drawLoadingBar(int x, int y, int width, int percent);
    void drawCountDownFrame(const vector<string>& frame, int startX, int startY, bool blink = false);

public:
    void runIntro();
    void runCountDown();
};
