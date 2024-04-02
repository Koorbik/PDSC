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
#define NUM_OF_POLES 10
#define POLE_WIDTH (SCREEN_WIDTH / (NUM_OF_POLES + 2))
#define DISK_WIDTH (POLE_WIDTH / 2)
#define POLE_HEIGHT (SCREEN_HEIGHT /2)
#define DISK_HEIGHT (POLE_HEIGHT / NUM_OF_DISKS)
#define NUM_OF_DISKS 40
#define EMPTY (-1)
#define EXTREME_CASE_INDEX 9
#define MAX_Y 50
#define ANIMATION_SPEED 5

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
    Disk currentDisks[NUM_OF_DISKS];
} Pole;

typedef struct {
    int startX;
    int startY;
    int endX;
    int endY;
    int speed;
} DiskMovement;

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
        newDisk.value = j + 1;
        push(poles, newDisk);
    }
}

void drawDisks(Pole poles[]) {
    for (int i = 0; i < NUM_OF_POLES; i++) {
        for (int j = 0; j <= poles[i].topValue; j++) {
            Disk *currentDisk = &poles[i].currentDisks[j];
            int diskWidth = POLE_WIDTH +
                (DISK_WIDTH - POLE_WIDTH) * (currentDisk->value - 1) / (NUM_OF_DISKS - 1);
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

bool isMoveLegal (Pole poles[], int sourcePole, int destinationPole) {
    if (sourcePole < 0 || sourcePole >= NUM_OF_POLES || destinationPole < 0 || destinationPole >= NUM_OF_POLES) {
        return false;
    }

    Disk disk = poles[sourcePole].currentDisks[poles[sourcePole].topValue];
    if (disk.value == EMPTY) {
        return false;
    }

    Disk topDisk = poles[destinationPole].currentDisks[poles[destinationPole].topValue];
    if (topDisk.value == EMPTY || disk.value > topDisk.value) {
        return true;
    }

    return false;
}

void drawScreen (Pole poles[]) {
    gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
    gfx_filledRect(0, SCREEN_HEIGHT - (FLOOR_HEIGHT - 1), SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, GREEN);
    drawPoles();
    drawDisks(poles);
}

void moveDiskHorizontally(Disk disk, Pole poles[], DiskMovement diskMovement) {
    for (int i = 0; i <= 100; i++) {
        drawScreen(poles);
        int currentX = diskMovement.startX + (diskMovement.endX - diskMovement.startX) * i / 100;
        int currentY = MAX_Y;
        gfx_filledRect(currentX, currentY, currentX + (disk.bottomX - disk.topX), currentY + (disk.bottomY - disk.topY), RED);
        gfx_updateScreen();
        SDL_Delay(diskMovement.speed);
    }
}

void moveDiskUpwards (Disk disk, Pole poles[], DiskMovement diskMovement) {
    for (int i = 0; i <= 100; i++) {
        drawScreen(poles);
        int currentX = diskMovement.startX;
        int currentY = diskMovement.startY - ((diskMovement.startY - MAX_Y) * i / 100);
        gfx_filledRect(currentX, currentY, currentX + (disk.bottomX - disk.topX), currentY + (disk.bottomY - disk.topY), RED);
        gfx_updateScreen();
        SDL_Delay(diskMovement.speed);
    }
}

void moveDiskDownwards (Disk disk, Pole poles[], DiskMovement diskMovement) {
    for (int i = 0; i <= 100; i++) {
        drawScreen(poles);
        int currentX = diskMovement.endX;
        int currentY = MAX_Y + ((diskMovement.endY - MAX_Y) * i / 100);
        gfx_filledRect(currentX, currentY, currentX + (disk.bottomX - disk.topX), currentY + (disk.bottomY - disk.topY), RED);
        gfx_updateScreen();
        SDL_Delay(diskMovement.speed);
    }
}

void animateDiskMovement(Pole poles[], int sourcePole, int destinationPole, Disk disk) {
    DiskMovement diskMovement;
    diskMovement.startX = disk.topX;
    diskMovement.endX = POLE_GAP * (destinationPole + 1) - (disk.bottomX - disk.topX) / 2;
    diskMovement.startY = disk.topY;
    diskMovement.endY = SCREEN_HEIGHT - FLOOR_HEIGHT - (DISK_HEIGHT * (poles[destinationPole].topValue + 2));
    diskMovement.speed = ANIMATION_SPEED;
    moveDiskUpwards(disk, poles, diskMovement);
    
    moveDiskHorizontally(disk, poles, diskMovement);

    moveDiskDownwards(disk, poles, diskMovement);
}

void moveDiskFromPoleToPole(Pole poles[], int sourcePole, int destinationPole) {
   if (!isMoveLegal(poles, sourcePole, destinationPole)) {
    printf("Invalid move. Cannot move disk from pole %d to pole %d.\n", sourcePole + 1, destinationPole + 1);
    return;
    }

    Disk disk = pop(&poles[sourcePole]);
    if (disk.value != EMPTY) {
        animateDiskMovement(poles, sourcePole, destinationPole, disk);
        push(&poles[destinationPole], disk);
        printf("Disk of value %d moved from pole %d to pole %d.\n", disk.value, sourcePole + 1, destinationPole + 1);
    } else {
        printf("No disk to move from pole %d.\n", sourcePole + 1);
    }
}

void handleExtremeCase(Pole poles[], int* sourcePole) {
    int poleIndex = EXTREME_CASE_INDEX;

    if (*sourcePole == EMPTY) {
        *sourcePole = poleIndex;
    }
    else {
        moveDiskFromPoleToPole(poles, *sourcePole, poleIndex);
        *sourcePole = EMPTY;
    }
}


void handleKey(int key, Pole poles[]) {
    static int sourcePole = EMPTY;

    if (key >= '1' && key <= ('0' + NUM_OF_POLES)) {
        int poleIndex = key - '1';

        if (sourcePole == EMPTY) {
            sourcePole = poleIndex;
        } else {
            moveDiskFromPoleToPole(poles, sourcePole, poleIndex);
            sourcePole = EMPTY;
        }
    }
    else if (key == '0' && NUM_OF_POLES == 10) {
        handleExtremeCase(poles, &sourcePole);
    }
}


int main(int argc, char *argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    Pole poles[NUM_OF_POLES];
    initializePoles(poles);

    while (1) {
        drawScreen(poles);
        gfx_updateScreen();
        int key = gfx_getkey();
        handleKey(key, poles);
        SDL_Delay(10);
        if (key == SDLK_ESCAPE) break;
    }

    return 0;
}