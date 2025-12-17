#include "GameManager.h"

int GameManager::menu() {
    while (true) {
        system("cls");

        int x = X_POS_FRAME, y = Y_POS_FRAME, w = 40, h = 12;
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

int GameManager::chooseLevel() {
    while (true) {
        system("cls");

        int x = X_POS_FRAME, y = Y_POS_FRAME, w = 40, h = 11;
        drawFrame(x, y, w, h, "SELECT LEVEL");

        gotoxy(x + 4, y + 3);  cout << "1. Normal Level";
        gotoxy(x + 4, y + 4);  cout << "2. Medium Level";
        gotoxy(x + 4, y + 5);  cout << "3. Hard Level";
        gotoxy(x + 4, y + 6);  cout << "4. Back";
        gotoxy(x + 12, y + 8); cout << "Enter your choice";

        char mode = _getch();
        playSound(800, AUDIO_LENGTH);

        if ('0' < mode && mode < '5')
            return mode - '0';
    }
}

unordered_map<int, int> GameManager::loadHighScore() {
    unordered_map<int, int> scores;
    ifstream file("high_scores.dat");
    int key;
    int value;

    while (file >> key >> value)
        scores[key] = value;

    return scores;
}

void GameManager::showHighScore() {
    system("cls");
    unordered_map<int, int> scores = loadHighScore();

    int hsNormalSingle = scores[1];
    int hsMediumSingle = scores[2];
    int hsHardSingle   = scores[3];
    int hsNormalPvP    = scores[4];
    int hsMediumPvP    = scores[5];
    int hsHardPvP      = scores[6];

    int x = X_POS_FRAME, y = Y_POS_FRAME, w = 40, h = 19;
    drawFrame(x, y, w, h, "HIGH SCORES");

    gotoxy(x + 14, y + 3); cout << "SINGLE MODE";
    gotoxy(x + 6, y + 5);  cout << "NORMAL : " << hsNormalSingle;
    gotoxy(x + 6, y + 6);  cout << "MEDIUM : " << hsMediumSingle;
    gotoxy(x + 6, y + 7);  cout << "HARD   : " << hsHardSingle;

    gotoxy(x + 15, y + 9); cout << "PVP MODE";
    gotoxy(x + 6, y + 11); cout << "NORMAL : " << hsNormalPvP;
    gotoxy(x + 6, y + 12); cout << "MEDIUM : " << hsMediumPvP;
    gotoxy(x + 6, y + 13); cout << "HARD   : " << hsHardPvP;

    gotoxy(x + 9, y + 16); cout << "Press any key to return";
    getch();
    playSound(600, AUDIO_LENGTH);
}

void GameManager::runProgram() {
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
