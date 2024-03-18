#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define CENTER_X SCREEN_WIDTH / 2
#define CENTER_Y SCREEN_HEIGHT / 2
#define FLOOR_HEIGHT 30
#define NUM_OF_POLES 3
#define POLE_HEIGHT 200
#define POLE_WIDTH 20
#define DISK_HEIGHT 10
#define NUM_OF_DISKS 5
#define EMPTY (-1)



typedef struct {
    int top;
    int bottom;
    int value;
} Disk;

typedef struct {
	int top; // its for drawing a pole think of better name ffs
	int bottom;
    int topValue;
    int poleNumber;
    Disk currentDisks[NUM_OF_DISKS];
} Pole;

bool push(Pole *stack, Disk value) {
    if (stack->topValue >= NUM_OF_DISKS - 1) return false;
    stack->topValue++;
    stack->currentDisks[stack->topValue] = value;
    return true;
}

Disk pop(Pole *stack) {
    Disk poppedDisk;
    poppedDisk.value = EMPTY;
    if (stack->topValue == EMPTY) return poppedDisk;
    poppedDisk = stack->currentDisks[stack->topValue];
    stack->topValue--;
    return poppedDisk;
}



void drawPoles(int poles, int screenWidth, int screenHeight)
{
    int poleGap = screenWidth / (poles + 1);

    for (int i = 1; i <= poles; ++i)
    {
        int poleX = poleGap * i;
        Pole pole = {poleX, screenHeight};

        gfx_filledRect(pole.top, pole.bottom, pole.top + POLE_WIDTH, pole.bottom - POLE_HEIGHT, BLUE);
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