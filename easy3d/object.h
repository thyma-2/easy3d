#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct triangle
{
	float x1,y1,z1,x2,y2,z2,x3,y3,z3;
	float tx1,tx2,tx3,ty1,ty2,ty3,tz1,tz2,tz3;
	unsigned int color;
};

struct object{
	float x,y,z,rx,ry,rz;
	float spx, spy, spz, sprx, spry, sprz;
	int nt;
	struct triangle **triangles; // = np * 3
	struct object *sun; 	
	struct object *brother;
};

struct triangle * create_triangle(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3, int color);
void add_triangle_into_object(struct object *obj, struct triangle *t);
struct object *create_object(struct object *father, float x, float y,float z,float rx,float ry,float rz);
void fill_object_with_obj(struct object *obj, char *string);
int partition(struct triangle *arr[], int start, int end);
void quickSort(struct triangle *arr[], int start, int end);
int cuboid_to_obj(struct object *obj, char *string); 	