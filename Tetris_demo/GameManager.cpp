#include "GameManager.h"

int GameManager::menu() {
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

int GameManager::chooseLevel() {
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
