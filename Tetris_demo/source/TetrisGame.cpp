#include "TetrisGame.h"

BaseBlock* TetrisGame::createRandomBlock() {
    int r = rand() % 7;

    switch (r) {
        case 0: return new BlockI();
        case 1: return new BlockO();
        case 2: return new BlockT();
        case 3: return new BlockL();
        case 4: return new BlockJ();
        case 5: return new BlockS();
        default : return new BlockZ();
    }
}

void TetrisGame::increaseSpeed() {
    if (gameSpeed > MAX_GAME_SPEED)
        gameSpeed -= D_SPEED_DECREASE;
}

void TetrisGame::loadHighestScore() {
    ifstream file("highest_score.txt");

    if (file.is_open()) {
        file >> highestScore;
        file.close();
    } else
        highestScore = 0;
}

void TetrisGame::saveHighestScore() {
    ofstream file("highest_score.txt");

    if (file.is_open()) {
        file << highestScore;
        file.close();
    }
}

bool TetrisGame::checkHighScore() {
    if (score <= highestScore)
        return false;

    highestScore = score;
    saveHighestScore();
    return true;
}

void TetrisGame::drawNextBlock() {
    int xPos = WIDTH * 2 + 5;
    int yPos = 11;

    for(int i = 0; i < BLOCK_SIZE; i++) {
        gotoxy(xPos + 8, yPos + 2 + i);
        cout << "        ";
    }

    setColor(nextBlock->blockColor);

    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            gotoxy(xPos + 8 + j * 2, yPos + 2 + i);
            cout << nextBlock->shape[i][j] << nextBlock->shape[i][j];
        }
    }

    setColor(WHITE);
}

void TetrisGame::drawUI(bool isNewRecord) {
    int xPos = WIDTH * 2 + 5;
    int boxWidth = 22;

    drawFrame(xPos, 2, boxWidth, 4, "SCORE");
    gotoxy(xPos + 2, 4);
    cout << "        ";
    gotoxy(xPos + 2, 4);
    cout << score;

    int comboBoxX = xPos + boxWidth + 2;
    int comboBoxW = 16;

    drawFrame(comboBoxX, 2, comboBoxW, 4, "XCOMBO");
    gotoxy(comboBoxX + 2, 4);
    cout << "        ";
    gotoxy(comboBoxX + 2, 4);

    if (comboCount > 0)
        cout << "x" << (comboCount + 1);
    else
        cout << "x0";

    string highScoreTitle = isNewRecord ? "NEW RECORD!" : "HIGH SCORE";
    drawFrame(xPos, 7, boxWidth, 4, highScoreTitle);

    gotoxy(xPos + 2, 9);
    cout << highestScore;

    drawFrame(xPos, 12, boxWidth, 6, "NEXT BLOCK");
    drawNextBlock();

    int yControl = 19;

    drawFrame(xPos, yControl, boxWidth, 9, "CONTROLS");
    gotoxy(xPos + 2, yControl + 1);  cout << "A     : Move Left";
    gotoxy(xPos + 2, yControl + 2);  cout << "D     : Move Right";
    gotoxy(xPos + 2, yControl + 3);  cout << "S     : Soft Drop";
    gotoxy(xPos + 2, yControl + 4);  cout << "SPACE : Hard Drop";
    gotoxy(xPos + 2, yControl + 5);  cout << "W     : Rotate";
    gotoxy(xPos + 2, yControl + 6);  cout << "P     : Pause Game";
    gotoxy(xPos + 2, yControl + 7);  cout << "Q     : Quit Game";
}

void TetrisGame::scoreCalculate(int linesCleared){
    int baseScore = 0;
    switch (linesCleared){
        case 1: baseScore = 100; break;
        case 2: baseScore = 300; break;
        case 3: baseScore = 500; break;
        case 4: baseScore = 800; break;
        default: return;
    }

    int levelMultiplier = (level);
    int comboBonus = max(0, comboCount - 1) * 50 * linesCleared;
    score += baseScore * levelMultiplier + comboBonus;
}

void TetrisGame::gameOverEffect() {
    for (int i = 0; i < HEIGHT - 1; i++)
        for (int j = 1; j < WIDTH - 1; j++)
            if (board.grid[i][j] != ' ')
                board.colorGrid[i][j] = DARK_GRAY;

    _sleep(1000);

    for (int i = HEIGHT - 2; i >= 0; i--) {
        for (int j = 1; j < WIDTH - 1; j++) {
            board.grid[i][j] = BLOCK_CHAR;
            board.colorGrid[i][j] = RED;
        }

        if (i > 4)
            playSound(600 - (i * 20), 125);

        board.draw();
        _sleep(60);
    }

    _sleep(500);
    system("cls");

    int x = 10, y = 5, w = 40, h = 10;

    drawFrame(x, y, w, h, "GAME OVER");

    gotoxy(x + 8, y + 4); cout << "Your Score: " << score;
    gotoxy(x + 8, y + 6); cout << "Press any key to return...";

    getch();
}

int TetrisGame::showPauseMenu() {
    system("cls");
    int x = 10, y = 5, w = 40, h = 10;

    drawFrame(x, y, w, h, "PAUSE");
    gotoxy(x + 4, y + 3); cout << "1. Resume";
    gotoxy(x + 4, y + 4); cout << "2. Restart";
    gotoxy(x + 4, y + 5); cout << "3. Quit";
    gotoxy(x + 12, y + 7); cout << "Enter your choice";

    char c;

    while (true) {
        c = _getch();
        playSound(800, AUDIO_LENGTH);

        if ('0' < c && c < '4')
            return c -'0';
    }
}

TetrisGame::TetrisGame(int mode) {
    level = 0;

    if (mode == 1) {
        gameSpeed = DEFAULT_GAME_SPEED;
        level = 1;
    } else if (mode == 2) {
        gameSpeed = DEFAULT_GAME_SPEED - 40;
        level = 2;
    } else if (mode == 3) {
        gameSpeed = DEFAULT_GAME_SPEED - 80;
        level = 3;
    }

    hideCursor();
    system("cls");

    currBlock = createRandomBlock();
    nextBlock = createRandomBlock();
    score = 0;
    comboCount = 0;
    loadHighestScore();
    drawUI();
}

TetrisGame::~TetrisGame() {
    delete currBlock;
    delete nextBlock;
}

bool TetrisGame::run() {
    int timer = 0;

    while (1){
        board.boardDeleteBlock(currBlock);

        if (kbhit()){
            char c = getch();
            c = tolower(c);

            if (c == 'a' && board.canMove(-1,0, currBlock)) {
                currBlock->x--;
                playSound(400, AUDIO_LENGTH);
            } else if (c == 'd' && board.canMove( 1,0, currBlock)) {
                currBlock->x++;
                playSound(400, AUDIO_LENGTH);
            } else if (c == 's' && board.canMove( 0,1, currBlock)) {
                currBlock->y++;
                playSound(450, AUDIO_LENGTH);
                bool isNew = checkHighScore();
                drawUI(isNew);
            } else if (c == SPACE_CHAR) {
                while (board.canMove(0, 1, currBlock))
                    currBlock->y++;

                timer = gameSpeed + 1;
                playSound(800, AUDIO_LENGTH);
            } else if (c == 'w') {
                currBlock->rotate(board.grid);
                playSound(600, AUDIO_LENGTH);
            } else if (c == 'q') {
                playSound(800, AUDIO_LENGTH);
                return false;
            } else if (c == 'p') {
                playSound(800, AUDIO_LENGTH);
                int choice = showPauseMenu();

                if (choice == 1) {
                    system("cls");
                    board.draw();
                    drawUI(checkHighScore());
                } else if (choice == 2)
                    return true;
                else if (choice == 3) {
                    return false;
                }
            }
    }

        if (timer >= gameSpeed) {
            if (board.canMove(0,1, currBlock))
                currBlock->y++;
            else {
                playSound(200, AUDIO_LENGTH);
                board.blockToBoard(currBlock);

                int linesCleared = board.removeLine();

                if (linesCleared > 0) {;
                    comboCount = (comboCount == 0) ? 1 : comboCount + 1;
                    scoreCalculate(linesCleared);

                    increaseSpeed();
                    bool isNew = checkHighScore();
                    drawUI(isNew);
                }
                else{
                    comboCount = 0;
                    drawUI(false);
                }

                delete currBlock;
                currBlock = nextBlock;
                nextBlock = createRandomBlock();

                if (!board.canMove(0, 0, currBlock)) {
                    playSound(300, AUDIO_LENGTH + 100);
                    gameOverEffect();
                    return false;
                }

                drawNextBlock();
            }

            timer = 0;
        }

        board.blockToBoard(currBlock);

        board.draw();

        _sleep(30);
        timer += 30;
    }

    return false;
}
