#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)
#define FLOOR_HEIGHT 30
#define NUM_OF_POLES 3
#define POLE_HEIGHT 200
#define POLE_WIDTH 20
#define DISK_HEIGHT 20  
#define DISK_WIDTH 60
#define NUM_OF_DISKS 10
#define EMPTY (-1)

typedef struct {
    int top;
    int bottom;
    int value;
} Disk;

typedef struct {
    int top;
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

void initializePoles(Pole poles[], Disk disks[]) {
    for (int i = 0; i < NUM_OF_POLES; i++) {
        poles[i].top = SCREEN_HEIGHT - FLOOR_HEIGHT;
        poles[i].bottom = poles[i].top - POLE_HEIGHT;
        poles[i].topValue = (i == 0) ? NUM_OF_DISKS - 1 : EMPTY; // Initialize first pole with disks
        poles[i].poleNumber = i;
        if (i == 0) {
            for (int j = 0; j < NUM_OF_DISKS; j++) {
                poles[i].currentDisks[j] = disks[j];
            }
        }
    }
}

void drawDisks(Pole poles[]) {
    int poleGap = SCREEN_WIDTH / (NUM_OF_POLES + 1);
    int maxDiskWidth = DISK_WIDTH;
    for (int i = 0; i < NUM_OF_POLES; i++) {
        Pole currentPole = poles[i];
        int diskWidth = maxDiskWidth;
        for (int j = 0; j <= currentPole.topValue; j++) {
            int poleX = poleGap * (i + 1);
            int diskY = currentPole.top - (j + 1) * DISK_HEIGHT;
            int diskX = poleX - diskWidth / 2;
            gfx_filledRect(diskX, diskY, diskX + diskWidth, diskY + DISK_HEIGHT, RED);
            diskWidth -= 5; 
        }
    }
}


void drawPoles(int numPoles) {
    int poleGap = SCREEN_WIDTH / (numPoles + 1);
    for (int i = 1; i <= numPoles; ++i) {
        int poleX = poleGap * i;
        gfx_filledRect(poleX - POLE_WIDTH / 2, SCREEN_HEIGHT - FLOOR_HEIGHT, poleX + POLE_WIDTH / 2, SCREEN_HEIGHT - FLOOR_HEIGHT - POLE_HEIGHT, BLUE);
    }
}

int main(int argc, char* argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    Pole poles[NUM_OF_POLES];
    Disk disks[NUM_OF_DISKS];

    while (1) {
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
        gfx_filledRect(0, SCREEN_HEIGHT - (FLOOR_HEIGHT-1), SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, GREEN);
        initializePoles(poles, disks);
        drawPoles(NUM_OF_POLES);
        drawDisks(poles);
        gfx_updateScreen();
        SDL_Delay(10);
        if (gfx_pollkey() == SDLK_ESCAPE) break;
    }
    return 0;
}
