#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define MAX_VERTICES 50
#define VERTICES 5
#define MAX_RADIUS 700
#define MIN_RADIUS 100
#define RADIUS_CHANGE_SPEED 10
#define MAX_ANGLE M_PI*2
#define MIN_ANGLE 0
#define ROTATION_SPEED M_PI/32

typedef struct{
    float x;
    float y;
    int vertices;
    int radius;
    float rotation;
    int radius_direction;
    float points[MAX_VERTICES][2];
}polygon;

void rotate_polygon(polygon *p);
void scale_polygon(polygon *p);
polygon generate_polygon(float x, float y, int vertices, int radius);
void update_polygon(polygon *p);
void render_polygon(polygon *p);

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

    polygon p = generate_polygon(gfx_screenWidth() / 2, gfx_screenHeight() / 2, VERTICES, MIN_RADIUS);

    do {
        rotate_polygon(&p);
        scale_polygon(&p);

        update_polygon(&p);

		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
        render_polygon(&p);
		gfx_updateScreen();
		SDL_Delay(16);
    } while (gfx_pollkey() == -1);

	return 0;
}

void rotate_polygon(polygon *p) {
    if (p->rotation > MAX_ANGLE)
        p->rotation = MIN_ANGLE;
    p->rotation += ROTATION_SPEED;
}

void scale_polygon(polygon *p) {
    if (p->radius > MAX_RADIUS || p->radius < MIN_RADIUS)
        p->radius_direction *= -1;
    p->radius += RADIUS_CHANGE_SPEED * p->radius_direction;
}

polygon generate_polygon(float x, float y, int vertices, int radius) {
    polygon p = {x, y, vertices, radius, 0, 1};

    float angle_of_one_vertex = M_PI * 2 / vertices;

    for (int i = 0; i < vertices; i++) {
        p.points[i][0] = sin(angle_of_one_vertex * i) * radius;
        p.points[i][1] = cos(angle_of_one_vertex * i) * radius;
    }
    return p;
}

void update_polygon(polygon *p) {
    float angle_of_one_vertex = M_PI * 2 / p->vertices;

    for (int i = 0; i < p->vertices; i++) {
        p->points[i][0] = sin(angle_of_one_vertex * i + p->rotation) * p->radius;
        p->points[i][1] = cos(angle_of_one_vertex * i + p->rotation) * p->radius;
    }
}

void render_polygon(polygon *p) {
    for (int i = 0; i < p->vertices; i++) {
        int next_index = i+1;
        if (next_index >= p->vertices)
            next_index = 0;
        gfx_line(p->points[i][0] + p->x, p->points[i][1] + p->y,
                p->points[next_index][0] + p->x, p->points[next_index][1] + p->y, YELLOW);
    }
}

