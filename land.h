#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "easy3d\object.h"
#include <math.h>

struct surface
{
	int y00,y01,y11,y10;
};

float **generate_land(int nvx, int nvy, int ecart, int s);