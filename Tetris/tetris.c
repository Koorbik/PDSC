#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "pieces.inl"

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

void drawBoard()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			int x = CENTER_GAME_X + j * CELL_SIZE;
			int y = CENTER_GAME_Y + i * CELL_SIZE;

			if (gameBoard[i][j] == 0)
			{
				gfx_filledRect(x, y, x + CELL_SIZE, y + CELL_SIZE, WHITE);
			}
		}
	}
}

void drawGrid ()
{
	for (int i = 0; i <= BOARD_HEIGHT; i++)
	{
		gfx_line(CENTER_GAME_X, CENTER_GAME_Y + i * CELL_SIZE, CENTER_GAME_X + GAME_WIDTH, CENTER_GAME_Y + i * CELL_SIZE, RED);
	}
	for (int j = 0; j <= BOARD_WIDTH; j++)
	{
		gfx_line(CENTER_GAME_X + j * CELL_SIZE, CENTER_GAME_Y, CENTER_GAME_X + j * CELL_SIZE, CENTER_GAME_Y + GAME_HEIGHT, RED);
	}
}


int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	while (1) {
		gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
		drawBoard();
		drawGrid();
		gfx_updateScreen();
		SDL_Delay(10);
		if (gfx_pollkey() == SDLK_ESCAPE)
			break;
	}
	return 0;
}