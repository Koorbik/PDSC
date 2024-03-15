#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_OF_VERTICES 6
#define ROTATION_ANGLE 0.0
#define ROTATION_SPEED 0.02 
#define MIN_SCALE 0.2
#define ORIGINAL_RADIUS 150
#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()

typedef struct {
	int x;
	int y;
} Vertex;

typedef enum { INCREASING, DECREASING } grow_state;

void drawPolygon(int vertices, double scale, int centerX, int centerY,
				 int originalRadius, double rotationAngle)
{
	double angle = 2 * M_PI / vertices;

	for (int i = 0; i < vertices; ++i) {
		// conversion from polar to cartesian coordinates
		Vertex vertex1 = {centerX + (int)(originalRadius * scale * cos(i * angle + rotationAngle)),
						  centerY + (int)(originalRadius * scale * sin(i * angle + rotationAngle))};
		Vertex vertex2 = {centerX + (int)(originalRadius * scale * cos((i + 1) * angle + rotationAngle)),
						  centerY + (int)(originalRadius * scale * sin((i + 1) * angle + rotationAngle))};

		gfx_line(vertex1.x, vertex1.y, vertex2.x, vertex2.y, WHITE);
	}
}

double scalePolygon(
	double scale, int originalRadius,
	grow_state* growth) // pass growth by reference to modify the original value
{
	float scale_factor;

	switch (*growth) {
	case INCREASING:
		scale_factor = 1.01;
		break;
	case DECREASING:
		scale_factor = 0.99;
		break;
	}

	if ((scale * scale_factor * originalRadius) >= SCREEN_WIDTH / 2 ||
		(scale * scale_factor * originalRadius) >= SCREEN_HEIGHT / 2) {
		*growth = DECREASING;
	}
	else if (*growth == DECREASING && scale <= MIN_SCALE) {
		*growth = INCREASING;
	}

	return scale * scale_factor;
}

double rotatePolygon(double rotationAngle, double rotationSpeed)
{
	rotationAngle += rotationSpeed;

	if (rotationAngle > 2 * M_PI) {
		rotationAngle -= 2 * M_PI; // Ensure the angle stays within [0, 2π]
	}
	else if (rotationAngle < 0) {
		rotationAngle += 2 * M_PI;
	}

	return rotationAngle;
}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	int centerX = SCREEN_WIDTH / 2;
	int centerY = SCREEN_HEIGHT / 2;

	double scale = 1.0;
	double rotationAngle = ROTATION_ANGLE;

	grow_state growth = INCREASING;

	while (1) {
		gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
		drawPolygon(NUM_OF_VERTICES, scale, centerX, centerY, ORIGINAL_RADIUS,
					rotationAngle);
		gfx_updateScreen();
		SDL_Delay(10);

		scale = scalePolygon(scale, ORIGINAL_RADIUS, &growth);
		rotationAngle = rotatePolygon(rotationAngle, ROTATION_SPEED);
		if (gfx_pollkey() == SDLK_ESCAPE)
			break;
	}
	return 0;
}
