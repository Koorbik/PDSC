#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "pieces.inl"
#include <time.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define CELL_SIZE 32
#define GAME_WIDTH (CELL_SIZE * BOARD_WIDTH)
#define GAME_HEIGHT (CELL_SIZE * BOARD_HEIGHT)
#define CENTER_GAME_X ((SCREEN_WIDTH - GAME_WIDTH) / 2)
#define CENTER_GAME_Y (SCREEN_HEIGHT - GAME_HEIGHT - 1)
#define SIDEBAR_WIDTH 200

int gameBoard[BOARD_HEIGHT][BOARD_WIDTH] = {0};

typedef struct {
    int shape[4][4];
    int x;
    int y;
    int kind;
    int rotation;
} TetrisPiece;

TetrisPiece currentPiece;

int randomizeKind() {
    return rand() % 7; 
}

void addToGameBoard(TetrisPiece piece) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece.y + i >= 0 && piece.y + i < BOARD_HEIGHT && piece.x + j >= 0 && piece.x + j < BOARD_WIDTH) {
                if (piece.shape[i][j] != 0) {
                    gameBoard[piece.y + i][piece.x + j] = piece.shape[i][j];
                }
            }
        }
    }
}

void initializePiece(TetrisPiece *piece, int kind) {
    piece->kind = kind;
    piece->rotation = 0;
    piece->x = BOARD_WIDTH / 2 - 1; // Adjusted to shift the piece to the left
    piece->y = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            piece->shape[i][j] = pieces[kind][0][i][j];
        }
    }
}

int checkCollision(TetrisPiece *piece, int xOffset) {
    for (int i = 0; i < 4; i++) { 
        for (int j = 0; j < 4; j++) {
            if (piece->shape[i][j] != 0) {
                int newX = piece->x + j + xOffset;
                int newY = piece->y + i + 1; 
                if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT) {
                    return 1; 
                }
                if (newY >= 0 && gameBoard[newY][newX] != 0) {
                    return 1; 
                }
            }
        }
    }
    return 0; 
}

void movePieceSidewise(TetrisPiece *piece, int key) {
    if (key == SDLK_LEFT && piece->x > 0 && !checkCollision(piece, -1)) {
        piece->x--;
    } else if (key == SDLK_RIGHT && piece->x + 2 < BOARD_WIDTH && !checkCollision(piece, 1)) {
        piece->x++;
    }
}


void fallingPiece(TetrisPiece *piece) {
    static int counter = 0;
    counter++;
    if (counter % 20 == 0) {
        if (checkCollision(piece, 0)) {
            addToGameBoard(*piece);
            int kind = randomizeKind();
            initializePiece(piece, kind);
        } else {
            piece->y++;
        }
    }
}


void drawPiece(TetrisPiece piece) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece.shape[i][j] != 0) {
                int x = CENTER_GAME_X + (piece.x + j) * CELL_SIZE;
                int y = CENTER_GAME_Y + (piece.y + i) * CELL_SIZE;
                if (piece.shape[i][j] == 2) {
                    gfx_filledRect(x, y, x + CELL_SIZE, y + CELL_SIZE, GREEN);
                } else {
                    gfx_filledRect(x, y, x + CELL_SIZE, y + CELL_SIZE, BLUE);
                }
            }
        }
    }
}

void drawBoard() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            int x = CENTER_GAME_X + j * CELL_SIZE;
            int y = CENTER_GAME_Y + i * CELL_SIZE;
            if (gameBoard[i][j] == 0) {
                gfx_filledRect(x, y, x + CELL_SIZE, y + CELL_SIZE, WHITE);
            }
        }
    }
}

void drawGrid () {
    for (int i = 0; i <= BOARD_HEIGHT; i++) {
        gfx_line(CENTER_GAME_X, CENTER_GAME_Y + i * CELL_SIZE, CENTER_GAME_X + GAME_WIDTH, CENTER_GAME_Y + i * CELL_SIZE, RED);
    }
    for (int j = 0; j <= BOARD_WIDTH; j++) {
        gfx_line(CENTER_GAME_X + j * CELL_SIZE, CENTER_GAME_Y, CENTER_GAME_X + j * CELL_SIZE, CENTER_GAME_Y + GAME_HEIGHT, RED);
    }
}

int main(int argc, char* argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    srand(time(NULL));

    int kind = randomizeKind();
    initializePiece(&currentPiece, kind);

    while (1) {
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
        drawBoard();
        
        int key = gfx_pollkey();
        movePieceSidewise(&currentPiece, key);
        
        drawPiece(currentPiece);
        fallingPiece(&currentPiece);
        drawGrid();
        gfx_updateScreen();
        SDL_Delay(10);
        if (key == SDLK_ESCAPE)
            break;
    }
    return 0;
}