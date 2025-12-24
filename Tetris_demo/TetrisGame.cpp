#include "TetrisGame.h"

TetrisGame::TetrisGame(GameMode mode, int level) : gameMode(mode), level(level) {
    loadHighScores();
    int initGameSpeed = DEFAULT_GAME_SPEED - 40 * (level - 1);

    system("cls");
    int offsetXPlayer1 = mode == GameMode::PVP ? 4 : 35;

    PlayerState p1(1, offsetXPlayer1, initGameSpeed, 'a', 'd', 'w', 's', SPACE_CHAR);
    p1.currBlock = createRandomBlock();
    p1.nextBlock = createRandomBlock();
    players.push_back(p1);

    if (mode == GameMode::PVP) {
        PlayerState p2(2, 65, initGameSpeed, 75, 77, 72, 80, 13);
        p2.currBlock = createRandomBlock();
        p2.nextBlock = createRandomBlock();
        players.push_back(p2);
    }

    for (auto& p : players) {
        p.board.draw();
        drawUI(p);
    }
}

TetrisGame::~TetrisGame() {
    for (auto& p : players) {
        if (p.currBlock) delete p.currBlock;
        if (p.nextBlock) delete p.nextBlock;
    }
}

bool TetrisGame::run() {
    const int FRAME_TICK = 30;

    while (true) {
        while (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) key = _getch();
            else key = tolower(key);

            if (key == 'p') {
                playSound(800, AUDIO_LENGTH);
                int choice = showPauseMenu();

                if (choice == 1) {
                    system("cls");

                    for (auto& p : players) {
                        p.board.draw();
                        drawUI(p);
                    }
                } else if (choice == 2) {
                    return true;
                } else if (choice == 3)
                    return false;
            }

            if (key == 'q') {
                playSound(800, AUDIO_LENGTH);
                return false;
            }

            for (auto& player : players)
                handleInput(player, key);
        }

        for (auto& p : players) {
            if (p.isGameOver) continue;

            p.board.boardDeleteBlock(p.currBlock);
            p.timer += FRAME_TICK;

            if (p.timer >= p.gameSpeed) {
                updatePhysics(p);
                p.timer = 0;
            }

            p.board.blockToBoard(p.currBlock);
            p.board.draw();
            p.timer += FRAME_TICK;
        }

        if (gameMode == GameMode::SOLO) {
            if (players[0].isGameOver) {
                system("cls");
                int x = X_POS_FRAME, y = Y_POS_FRAME, w = 40, h = 10;
                drawFrame(x, y, w, h, "GAME OVER");
                gotoxy(x + 8, y + 4); cout << "Your Score: " << players[0].score;
                gotoxy(x + 8, y + 6); cout << "Press any key to return...";
                getch();

                return false;
            }
        } else {
            if (players[0].isGameOver || players[1].isGameOver) {
                system("cls");
                int x = X_POS_FRAME, y = Y_POS_FRAME, w = 40, h = 10;
                int id_winner = players[0].isGameOver ? 2 : 1;
                string title = "PLAYER " + to_string(id_winner) + " WIN";

                drawFrame(x, y, w, h, title);
                gotoxy(x + 8, y + 4); cout << "Score: " << players[id_winner - 1].score;
                gotoxy(x + 8, y + 6); cout << "Press any key to return...";
                getch();

                return false;
            }
        }

        Sleep(FRAME_TICK);
    }

    return false;
}

void TetrisGame::handleInput(PlayerState& p, int key) {
    if (p.isGameOver) return;

    p.board.boardDeleteBlock(p.currBlock);

    if (key == p.kLeft && p.board.canMove(-1, 0, p.currBlock)) {
        p.currBlock->x--;
        playSound(400, AUDIO_LENGTH);
    } else if (key == p.kRight && p.board.canMove(1, 0, p.currBlock)) {
        p.currBlock->x++;
        playSound(400, AUDIO_LENGTH);
    } else if (key == p.kSoftDrop && p.board.canMove(0, 1, p.currBlock)) {
        p.currBlock->y++;
        p.score++;
         drawUI(p);
    } else if (key == p.kHardDrop) {
        while (p.board.canMove(0, 1, p.currBlock)) {
            p.currBlock->y++;
            p.score += 2;
        }

        p.timer = p.gameSpeed + 1;
        playSound(800, AUDIO_LENGTH);
    } else if (key == p.kRotate) {
        p.currBlock->rotate(p.board.grid);
        playSound(600, AUDIO_LENGTH);
    }
}

void TetrisGame::updatePhysics(PlayerState& p) {
    if (p.isGameOver) return;

    p.board.boardDeleteBlock(p.currBlock);

    if (p.board.canMove(0, 1, p.currBlock)) {
        p.currBlock->y++;
    } else {
        playSound(200, AUDIO_LENGTH);
        p.board.blockToBoard(p.currBlock);

        int linesCleared = p.board.removeLine();

        if (linesCleared > 0) {
            p.comboCount++;
            scoreCalculate(p, linesCleared);
            increaseSpeed(p);
            bool isNew = checkHighScore(p.score);
            drawUI(p);
        } else {
            p.comboCount = 0;
            drawUI(p);
        }

        delete p.currBlock;
        p.currBlock = p.nextBlock;
        p.nextBlock = createRandomBlock();

        if (!p.board.canMove(0, 0, p.currBlock)) {
            playSound(300, AUDIO_LENGTH + 100);
            p.isGameOver = true;
            gameOverEffect(p);
            return;
        }

        drawNextBlock(p);
    }
}

void TetrisGame::scoreCalculate(PlayerState& p, int linesCleared) {
    int baseScore = 0;
    
    if (linesCleared >= 4) baseScore = 800;
    else if (linesCleared == 3) baseScore = 500;
    else if (linesCleared == 2) baseScore = 300;
    else if (linesCleared == 1) baseScore = 100;
    else return;

    int comboBonus = max(0, p.comboCount - 1) * 50 * linesCleared;
    int diff = max(1, level);
    p.score += baseScore * diff + comboBonus;
}

void TetrisGame::increaseSpeed(PlayerState& p) {
    if (p.gameSpeed > 50) p.gameSpeed -= D_SPEED_DECREASE;
}

void TetrisGame::drawUI(const PlayerState& p, bool isNewRecord) {
    int xPos = p.board.offsetX + WIDTH * 2 + 2;
    int yPos = p.board.offsetY + 1;
    int boxWidth = 22;

    gotoxy(xPos, yPos - 2); cout << "PLAYER " << p.id;

    drawFrame(xPos, yPos, boxWidth, 4, "SCORE");
    gotoxy(xPos + 2, yPos + 2); cout << p.score;

    drawFrame(xPos, yPos + 5, boxWidth, 4, "XCOMBO");
    gotoxy(xPos + 2, yPos + 7); printf("x%d", p.comboCount ? p.comboCount + 1 : 0);

    drawFrame(xPos, yPos + 10, boxWidth, 6, "NEXT BLOCK");
    drawNextBlock(p);
}

void TetrisGame::drawNextBlock(const PlayerState& p) {
    int xPos = p.board.offsetX + WIDTH * 2 + 5;
    int yPos = p.board.offsetY + 9;

    for(int i = 0; i < 4; i++) {
        gotoxy(xPos + 4, yPos + 3 + i); cout << "        ";
    }

    setColor(p.nextBlock->blockColor);

    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            gotoxy(xPos + 4 + j * 2, yPos + 3 + i);
            cout << p.nextBlock->shape[i][j] << p.nextBlock->shape[i][j];
        }
    }

    setColor(WHITE);
}

void TetrisGame::gameOverEffect(PlayerState& p) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            if (p.board.grid[i][j] != ' ')
                p.board.colorGrid[i][j] = DARK_GRAY;
        }
    }

    playSound(300, 500);
    Sleep(1000);

    for (int i = HEIGHT - 2; i >= 0; i--) {
        for (int j = 1; j < WIDTH - 1; j++) {
            p.board.grid[i][j] = BLOCK_CHAR;
            p.board.colorGrid[i][j] = RED;
        }

        if (i > 4)
            playSound(600 - (i * 20), 140);

        p.board.draw();
        Sleep(60);
    }

    Sleep(700);
    system("cls");

    setColor(WHITE);
}

BaseBlock* TetrisGame::createRandomBlock() {
    int r = rand() % 7;
    switch (r) {
        case 0: return new BlockI(); case 1: return new BlockO();
        case 2: return new BlockT(); case 3: return new BlockL();
        case 4: return new BlockJ(); case 5: return new BlockS();
        default: return new BlockZ();
    }
}

void TetrisGame::loadHighScores() {
    ifstream file("high_scores.dat");
    int k, v;
    while(file >> k >> v) highScores[k] = v;
    int curr_mode = level + (gameMode == GameMode::PVP ? 1 : 0) * 3;
    if(!highScores.count(curr_mode)) highScores[curr_mode] = 0;
}

void TetrisGame::saveHighScores() {
    ofstream file("high_scores.dat");
    for(auto& [k, v] : highScores) file << k << ' ' << v << '\n';
}

bool TetrisGame::checkHighScore(int score) {
    int curr_mode = level + (gameMode == GameMode::PVP ? 1 : 0) * 3;

    if (score <= highScores[curr_mode])
        return false;

    highScores[curr_mode] = score;
    saveHighScores();
    return true;
}

int TetrisGame::showPauseMenu() {
    system("cls");
    int x = X_POS_FRAME, y = Y_POS_FRAME, w = 40, h = 7;
    drawFrame(x, y, w, h, "PAUSE");
    gotoxy(x + 4, y + 2); cout << "1. Resume";
    gotoxy(x + 4, y + 3); cout << "2. Restart";
    gotoxy(x + 4, y + 4); cout << "3. Quit";

    char c;
    while (true) {
        c = _getch();
        if ('0' < c && c < '4') return c - '0';
    }
}
