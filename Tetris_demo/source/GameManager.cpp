#include "GameManager.h"

int GameManager::menu() {
    while (true) {
        system("cls");

        int x = 10, y = 5, w = 40, h = 10;
        drawFrame(x, y, w, h, "TETRIS MASTER");

        gotoxy(x + 4, y + 3);  cout << "1. Start Game";
        gotoxy(x + 4, y + 4);  cout << "2. View High Score";
        gotoxy(x + 4, y + 5);  cout << "3. Quit";
        gotoxy(x + 12, y + 7); cout << "Enter your choice";

        char c = _getch();
        playSound(800, AUDIO_LENGTH);

        if ('0' < c && c < '4') return c - '0';
    }
}

int GameManager::chooseMode() {
    while (true) {
        system("cls");

        int x = 10, y = 5, w = 40, h = 10;
        drawFrame(x, y, w, h, "SELECT MODE");

        gotoxy(x + 4, y + 3);  cout << "1. Normal Mode";
        gotoxy(x + 4, y + 4);  cout << "2. Medium Mode";
        gotoxy(x + 4, y + 5);  cout << "3. Hard Mode";
        gotoxy(x + 4, y + 6);  cout << "4. Back";
        gotoxy(x + 12, y + 7); cout << "Enter your choice";

        char mode = _getch();
        playSound(800, AUDIO_LENGTH);

        if ('0' < mode && mode < '5')
            return mode - '0';
    }
}

void GameManager::showHighScore() {
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
    playSound(600, AUDIO_LENGTH);
}

void GameManager::runProgram() {
    hideCursor();

    Scene scene;
    scene.runIntro();

    while (true) {
        int option = menu();

        if (option == 1) {
            int mode = chooseMode();

            if (mode == 4)
                continue;

            bool playAgain = false;

            do {
                scene.runCountDown();
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
