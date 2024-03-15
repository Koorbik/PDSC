#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define CENTER_X SCREEN_WIDTH / 2
#define CENTER_Y SCREEN_HEIGHT / 2
#define FLOOR_HEIGHT 30
#define NUM_OF_POLES 10
#define POLE_HEIGHT 200
#define POLE_WIDTH 20

typedef struct {
	int x;
	int y;
} Point;

void drawPoles(int poles, int screenWidth, int screenHeight)
{
    int poleGap = screenWidth / (poles + 1);

    for (int i = 1; i <= poles; ++i)
    {
        int poleX = poleGap * i;
        Point point1 = {poleX, screenHeight};
        Point point2 = {poleX + POLE_WIDTH, screenHeight - POLE_HEIGHT};

        gfx_filledRect(point1.x, point1.y, point2.x, point2.y, BLUE);
    }
}
int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	while (1) {
		gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
		gfx_filledRect(0, SCREEN_HEIGHT - FLOOR_HEIGHT, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, GREEN);
		drawPoles(NUM_OF_POLES, SCREEN_WIDTH, SCREEN_HEIGHT - (FLOOR_HEIGHT+1));
		gfx_updateScreen();
		SDL_Delay(10);
		if (gfx_pollkey() == SDLK_ESCAPE)
			break;
	}
	return 0;
}
