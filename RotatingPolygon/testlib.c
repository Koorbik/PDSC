#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_OF_VERTICES 15
#define ANGLE_OF_ROTATION (M_PI / 2) // in radians at least for now

void drawPolygon(int vertices)
{
	int centerX = gfx_screenWidth() / 2;
	int centerY = gfx_screenHeight() / 2;
	int radius = 150;
	double angle = 2 * M_PI / vertices;

	for (int i = 0; i < vertices; ++i) {
		// conversion from polar coordinates to cartesian ones
		int x1 = centerX + radius * cos(i * angle);
		int y1 = centerY + radius * sin(i * angle);

		int x2 = centerX + radius * cos((i + 1) * angle);
		int y2 = centerY + radius * sin((i + 1) * angle);

		gfx_line(x1, y1, x2, y2, WHITE);
	}
}
int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}
	while (1) {

		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
					   BLACK);
		drawPolygon(NUM_OF_VERTICES);
		gfx_updateScreen();
		SDL_Delay(10);
	}
	gfx_getkey();
	return 0;
}
