#include "primlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)
#define POLE_GAP (SCREEN_WIDTH / (NUM_OF_POLES + 1))
#define FLOOR_HEIGHT 30
#define NUM_OF_POLES 3
#define POLE_WIDTH 20
#define DISK_WIDTH 40
#define POLE_HEIGHT 200
#define DISK_HEIGHT 20
#define NUM_OF_DISKS 5
#define EMPTY (-1)

typedef struct {
    int topX;
    int topY;
    int bottomX;
    int bottomY;
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

void initializePoles(Pole poles[]) {
    for (int i = 0; i < NUM_OF_POLES; i++) {
        Pole newPole;
        newPole.topValue = EMPTY;
        poles[i] = newPole;
    }

    for (int j = 0; j < NUM_OF_DISKS; j++) {
        Disk newDisk;
        newDisk.value = j;
        push(poles, newDisk);
    }
}

void drawDisks(Pole poles[]) {
    for (int i = 0; i < NUM_OF_POLES; i++) {
        for (int j = 0; j <= poles[i].topValue; j++) {
            Disk *currentDisk = &poles[i].currentDisks[j];
            int diskWidth = POLE_WIDTH +
                            (DISK_WIDTH - POLE_WIDTH) * (NUM_OF_DISKS - 1 - currentDisk->value) / (NUM_OF_DISKS - 1);
            currentDisk->topX = POLE_GAP * (i + 1) - (diskWidth / 2);
            currentDisk->bottomX = currentDisk->topX + diskWidth;
            currentDisk->topY = SCREEN_HEIGHT - FLOOR_HEIGHT - (DISK_HEIGHT * (j + 1));
            currentDisk->bottomY = SCREEN_HEIGHT - FLOOR_HEIGHT - (DISK_HEIGHT * j);
            gfx_filledRect(currentDisk->topX, currentDisk->topY, currentDisk->bottomX, currentDisk->bottomY, RED);
        }
    }
}

void drawPoles() {
    int poleWidth = DISK_WIDTH / NUM_OF_DISKS + 1; // Ensure poleWidth is always greater than the smallest disk width
    for (int i = 1; i <= NUM_OF_POLES; ++i) {
        int poleX = POLE_GAP * i;
        gfx_filledRect(poleX - poleWidth / 2, SCREEN_HEIGHT - FLOOR_HEIGHT, poleX + poleWidth / 2,
                       SCREEN_HEIGHT - FLOOR_HEIGHT - POLE_HEIGHT, BLUE);
    }
}

void moveDiskFromPoleToPole(Pole poles[], int sourcePole, int destinationPole) {
    if (sourcePole < 0 || sourcePole >= NUM_OF_POLES || destinationPole < 0 || destinationPole >= NUM_OF_POLES) {
        printf("Invalid pole number.\n");
        return;
    }

    Disk disk = pop(&poles[sourcePole]);
    if (disk.value != EMPTY) {
        if (!push(&poles[destinationPole], disk)) {
            printf("Cannot move disk of value %d from pole %d to pole %d. Destination pole is full.\n", disk.value,
                   sourcePole + 1, destinationPole + 1);
            push(&poles[sourcePole], disk);
        } else {
            printf("Disk of value %d moved from pole %d to pole %d.\n", disk.value, sourcePole + 1,
                   destinationPole + 1);
        }
    } else {
        printf("No disk to move from pole %d.\n", sourcePole + 1);
    }
}

int main(int argc, char *argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    Pole poles[NUM_OF_POLES];
    initializePoles(poles);

    while (1) {
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
        gfx_filledRect(0, SCREEN_HEIGHT - (FLOOR_HEIGHT - 1), SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, GREEN);
        drawPoles();
        drawDisks(poles);
        gfx_updateScreen();
        int key = gfx_pollkey();
        if (key == '1') {
            moveDiskFromPoleToPole(poles, 0, 1);
        }
        SDL_Delay(10);
        if (gfx_pollkey() == SDLK_ESCAPE) break;
    }

    return 0;
}