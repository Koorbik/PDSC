#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "pieces.inl"
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define CELL_SIZE 32
#define GAME_WIDTH (CELL_SIZE * BOARD_WIDTH)
#define GAME_HEIGHT (CELL_SIZE * BOARD_HEIGHT)
#define CENTER_GAME_X ((SCREEN_WIDTH - GAME_WIDTH) / 2)
#define CENTER_GAME_Y (SCREEN_HEIGHT - GAME_HEIGHT - 1)
#define NO_FULL_ROW -1

int gameBoard[BOARD_WIDTH][BOARD_HEIGHT] = {0};

bool GameOver = false;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point position;
    int kind;
    int rotation;
    int nextKind;
} TetrisPiece;

TetrisPiece currentPiece;

int randomizeKind() {
    return rand() % 7; 
}

void initializePiece() {
    currentPiece.position.x = BOARD_WIDTH / 2 - 1;
    currentPiece.position.y = 0;
    currentPiece.kind = currentPiece.nextKind;
    currentPiece.rotation = 0;
    currentPiece.nextKind = randomizeKind();
}

int checkCollision(Point direction) {
    int newX, newY;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if(pieces[currentPiece.kind][currentPiece.rotation][i][j] != 0) {
                newX = currentPiece.position.x + i + direction.x;
                newY = currentPiece.position.y + j + direction.y;
                if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT) {
                    return 1;
                }
                if (newY >= 0 && gameBoard[newX][newY] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void drawPieceInBoard(int x, int y, enum color col, int filled) {
    int xCoord = CENTER_GAME_X + x * CELL_SIZE;
    int yCoord = CENTER_GAME_Y + y * CELL_SIZE;
    if (filled) {
        gfx_filledRect(xCoord, yCoord, xCoord + CELL_SIZE, yCoord + CELL_SIZE, col);
    } else {
        gfx_rect(xCoord, yCoord, xCoord + CELL_SIZE, yCoord + CELL_SIZE, col);
    }
}

void drawBoard() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (gameBoard[i][j] != 0) {
                drawPieceInBoard(i, j, BLUE, 1);
            } else {
                drawPieceInBoard(i, j, WHITE, 0);
            }
        }
    }
}

void drawPiece() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (pieces[currentPiece.kind][currentPiece.rotation][i][j] == 1) {
                drawPieceInBoard(currentPiece.position.x + i, currentPiece.position.y + j, BLUE, 1);
            } else if (pieces[currentPiece.kind][currentPiece.rotation][i][j] == 2) {
                drawPieceInBoard(currentPiece.position.x + i, currentPiece.position.y + j, GREEN, 1);
            }
        }
    }

}

void addPieceToBoard() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //printf("%d ", pieces[currentPiece.kind][currentPiece.rotation][i][j]);
            if (pieces[currentPiece.kind][currentPiece.rotation][i][j] != 0) {
                gameBoard[currentPiece.position.x + i][currentPiece.position.y + j] = 1;
            }
        }
    }
}

void drawGrid() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            drawPieceInBoard(i, j, WHITE, 0);
        }
    }
}

Point getCenterOffset() {
    Point offset = {0, 0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (pieces[currentPiece.kind][currentPiece.rotation][i][j] == 2) {
                offset.x = i;
                offset.y = j;
                return offset;
            }
        }
    }
    return offset;
}

int rotatePiece() {
    int oldRotation = currentPiece.rotation;
    Point oldPosition = currentPiece.position;

    Point oldOffset = getCenterOffset();

    int newRotation = (currentPiece.rotation + 1) % 4;
    currentPiece.rotation = newRotation;

    Point newOffset = getCenterOffset();
    Point move = {oldOffset.x - newOffset.x, oldOffset.y - newOffset.y};
    currentPiece.position.x += move.x;
    currentPiece.position.y += move.y;

    if (checkCollision((Point){0, 0})) { //add the condition that prevents the piece from rotating when being to far up
        currentPiece.rotation = oldRotation;
        currentPiece.position = oldPosition;
        return 0;
    }
    return 1;
}

void gameOverCondition() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        if (gameBoard[i][0] != 0) {
            GameOver = true;
            break;
        }
    }
}

void drawGameOver() {
    gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
    gfx_textout(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "GAME OVER", RED);
}

void movePieceDown() {
    Point direction = {0, 1};
    static int counter = 0;
    counter++;
    if (counter % 30 == 0)
    {
    if (!checkCollision(direction)) {
        currentPiece.position.y += 1;
    } 
    else {
        addPieceToBoard();
        initializePiece();
    }
    }
}

int isFull() {
    for (int j = 0; j < BOARD_HEIGHT; j++) {
        bool Full = true;
        for (int i = 0; i < BOARD_WIDTH; i++) {
            if (gameBoard[i][j] == 0) {
                Full = false;
                break;
            }
        }
        if (Full) {
            return j; 
        }
    }
    return NO_FULL_ROW; 
}

void removeRow(int row) {
    // Shift all rows above the specified row down by one
    for (int j = row; j > 0; j--) {
        for (int i = 0; i < BOARD_WIDTH; i++) {
            gameBoard[i][j] = gameBoard[i][j - 1];
        }
    }
    // Clear the top row
    for (int i = 0; i < BOARD_WIDTH; i++) {
        gameBoard[i][0] = 0;
    }
}

void moveDownFast() {
    while (!checkCollision((Point){0, 1})) {
        currentPiece.position.y += 1;
    }
}

void drawSidebar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (pieces[currentPiece.nextKind][0][i][j] == 1) {
                drawPieceInBoard(BOARD_WIDTH + i + 1, j, BLUE, 1);
            } else if (pieces[currentPiece.nextKind][0][i][j] == 2) {
                drawPieceInBoard(BOARD_WIDTH + i+ 1, j, GREEN, 1);
            }
        }
    }
} 

void handleMovement (int key)
{
    switch(key) {
        case SDLK_LEFT:
            if (!checkCollision((Point){-1, 0})) {
                currentPiece.position.x -= 1;
            }
            break;
        case SDLK_RIGHT:
            if (!checkCollision((Point){1, 0})) {
                currentPiece.position.x += 1;
            }
            break;
        case SDLK_DOWN:
            moveDownFast();
            break;
        case SDLK_SPACE:
            rotatePiece();
            break;
        case SDLK_ESCAPE:
            exit(0);
            break;
    }
}

int main(int argc, char* argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    srand(time(NULL));
    initializePiece();

    while (1) {
        int key = gfx_pollkey();
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
        drawBoard();
        drawPiece();
        drawGrid();
        drawSidebar();
        movePieceDown(); // Move the position down
        handleMovement(key);
        gameOverCondition();
        int fullRow = isFull();
        if (fullRow != NO_FULL_ROW) {
            removeRow(fullRow);
        }
        if (GameOver) {
            drawGameOver();
        }
        gfx_updateScreen();
        SDL_Delay(10);
    }
    return 0;
}