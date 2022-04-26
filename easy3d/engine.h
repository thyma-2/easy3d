#define client_width 1400
#define client_height 700
#include <math.h>
#include "object.h"
#include <stdint.h> 

struct camera {
	float x, y, z;
	float rx, ry, rz;
	float crx,srx,cry,sry,crz,srz;
};

struct camera *init_cam(void);
void clear_screen(uint32_t color);
void draw_scene(struct object *obj, float x, float y, float z, float rx, float ry, float rz, float n);