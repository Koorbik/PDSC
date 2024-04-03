#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "pieces.inl"

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define CELL_SIZE 40
#define GAME_WIDTH (CELL_SIZE * BOARD_WIDTH)
#define GAME_HEIGHT (CELL_SIZE * BOARD_HEIGHT)
#define SIDEBAR_WIDTH 100

int gameBoard[BOARD_HEIGHT][BOARD_WIDTH] = {0};

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	for (int i = -99; i < 100; ++i) {
		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
					   BLACK);
		gfx_filledCircle(gfx_screenWidth() / 2 + i, gfx_screenHeight() / 2, 100,
						 YELLOW);
		gfx_textout(gfx_screenWidth() / 2 - i, gfx_screenHeight() / 2,
					"This is a text", RED);
		gfx_updateScreen();
		SDL_Delay(10);
	}

	return 0;
}