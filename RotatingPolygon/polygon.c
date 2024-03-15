#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_OF_VERTICES 7
#define ROTATION_ANGLE 0.0
#define ROTATION_SPEED 0.02
#define MIN_SCALE 0.2
#define ORIGINAL_RADIUS 150
#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)

typedef struct {
    int x;
    int y;
} Vertex;

typedef enum { INCREASING, DECREASING } grow_state;

Vertex calculateVertex(int centerX, int centerY, int currentRadius, int i, double angle, double rotationAngle) {
    Vertex vertex;
    vertex.x = centerX + (int)(currentRadius * cos(i * angle + rotationAngle));
    vertex.y = centerY + (int)(currentRadius * sin(i * angle + rotationAngle));
    return vertex;
}



void drawPolygon(int vertices, double currentRadius, int centerX, int centerY,
                 double rotationAngle)
{
    double angle = 2 * M_PI / vertices;

    for (int i = 0; i < vertices; ++i) {
        Vertex vertex1 = calculateVertex(centerX, centerY, currentRadius, i, angle, rotationAngle);
        Vertex vertex2 = calculateVertex(centerX, centerY, currentRadius, i + 1, angle, rotationAngle);

        gfx_line(vertex1.x, vertex1.y, vertex2.x, vertex2.y, WHITE);
    }
}


void changeGrowState(double scale, int originalRadius, grow_state* growth,
                     float scale_factor)
{
    if ((scale * scale_factor * originalRadius) >= CENTER_X ||
        (scale * scale_factor * originalRadius) >= CENTER_Y) {
        *growth = DECREASING;
    }
    else if (*growth == DECREASING && scale <= MIN_SCALE) {
        *growth = INCREASING;
    }
}

double scalePolygon(double scale, int originalRadius, grow_state growth,
                    float* scale_factor) 
{

    switch (growth) {
    case INCREASING:
        *scale_factor = 1.01;
        break;
    case DECREASING:
        *scale_factor = 0.99;
        break;
    }

    return scale * (*scale_factor);
}

double rotatePolygon(double rotationAngle, double rotationSpeed)
{
    rotationAngle += rotationSpeed;

    if (rotationAngle > 2 * M_PI) {
        rotationAngle -= 2 * M_PI;
    }
    else if (rotationAngle < 0) {
        rotationAngle += 2 * M_PI;
    }

    return rotationAngle;
}

double calculateCurrentRadius(double scale, int originalRadius)
{
    double currentRadius = originalRadius * scale;
    return currentRadius;
}

int main(int argc, char* argv[])
{
    if (gfx_init()) {
        exit(3);
    }

    double scale = 1.0;
    float scale_factor;
    double rotationAngle = ROTATION_ANGLE;
    double currentRadius;
    grow_state growth = INCREASING;

    while (1) {
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
        drawPolygon(NUM_OF_VERTICES, currentRadius, CENTER_X, CENTER_Y, rotationAngle);
        gfx_updateScreen();
        SDL_Delay(10);

        scale = scalePolygon(scale, ORIGINAL_RADIUS, growth, &scale_factor);
        currentRadius = calculateCurrentRadius(scale, ORIGINAL_RADIUS);
        changeGrowState(scale, ORIGINAL_RADIUS, &growth, scale_factor);
        rotationAngle = rotatePolygon(rotationAngle, ROTATION_SPEED);
        if (gfx_pollkey() == SDLK_ESCAPE)
            break;
    }
    return 0;
}