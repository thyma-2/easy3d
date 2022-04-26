#include "engine.h"


extern struct triangle **triangles;
extern struct object *objects;
extern int nt;

uint32_t* memory;
float *zbuffer;

struct camera *cam;

struct camera *init_cam(void)
{
	cam = calloc(sizeof(struct camera), 1);
	cam->crx = cos(-cam->rx);
	cam->srx = sin(-cam->rx);
	cam->cry = cos(-cam->ry);
	cam->sry = sin(-cam->ry);
	cam->crz = cos(-cam->rz);
	cam->srz = sin(-cam->rz);
	return cam;
}

void draw_line(int X0, int X1, int Y0, int Y1, float Z0, float Z1, uint32_t color)
{
    // calculate dx & dy
    int dx = X1 - X0;
    int dy = Y1 - Y0;
	int dz = Z1 - Z0;
 
    // calculate steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	
 
    // calculate increment in x & y for each steps
    float Xinc = dx / (float) steps;
    float Yinc = dy / (float) steps;
	float Zinc = dz / (float) steps;
 
    // Put pixel for each step
    float X = X0;
    float Y = Y0;
	float Z = Z0;
    for (int i = 0; i <= steps; i++)
    {
		if (Z < *((float *)zbuffer + (int)X + (int)Y * client_width) && Z > 0)
		{
			*((uint32_t *)memory + (int)X + (int)Y * client_width) = color;
			*((float *)zbuffer + (int)X + (int)Y * client_width) = Z;
		}
        X += Xinc;           // increment in x at each step
        Y += Yinc;           // increment in y at each step
		Z += Zinc;
    }
}

void draw_triangle(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3, uint32_t color)
{
	/*
	draw_line(x1, x2, y1,y2, z1,z2, color);
	draw_line(x1, x3, y1,y3,z1,z3, color);	
	draw_line(x3, x2, y3,y2, z3,z2, color);
	*/
	
	float dx = x3 - x2;
	float dy = y3 - y2;
	float dz = z3 - z2;
	float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	float Xinc = dx / steps;
    float Yinc = dy / steps;
	float Zinc = dz / steps;
	float x = x2;
	float y = y2;
	float z = z2;
	for (int i = 0; i <= steps; i++)
	{
		draw_line(x, x1, y, y1, z, z1, color);
		x += Xinc;
		y += Yinc;
		z += Zinc;
	}
	dx = x1 - x2;
	dy = y1 - y2;
	dz = z1 - z2;
	steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	Xinc = dx / steps;
    Yinc = dy / steps;
	Zinc = dz / steps;
	x = x2;
	y = y2;
	z = z2;
	for (int i = 0; i <= steps; i++)
	{
		draw_line(x, x3, y, y3, z, z3, color);
		x += Xinc;
		y += Yinc;
		z += Zinc;
	}
	
	dx = x1 - x3;
	dy = y1 - y3;
	dz = z1 - z3;
	steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	Xinc = dx / steps;
    Yinc = dy / steps;
	Zinc = dz / steps;
	x = x3;
	y = y3;
	z = z3;
	for (int i = 0; i <= steps; i++)
	{
		draw_line(x, x2, y, y2,z,z2, color);
		x += Xinc;
		y += Yinc;
		z += Zinc;
	}
}

void transform(struct object *obj)
{
	while (obj != NULL)
	{
		obj->x += obj->spx;
		obj->y += obj->spy;
		obj->z += obj->spz;
		obj->rx += obj->sprx;
		obj->ry += obj->spry;
		obj->rz += obj->sprz;
		transform(obj->sun);
		obj = obj->brother;
	}
}

void draw_scene(struct object *obj, float x, float y, float z, float rx, float ry, float rz, float n)
{	
	while (obj != NULL)
	{
		float crx = cos(obj->rx+rx);
		float srx = sin(obj->rx+rx);
		float cry = cos(obj->ry+ry);
		float sry = sin(obj->ry+ry);
		float crz = cos(obj->rz+rz);
		float srz = sin(obj->rz+rz);
		
		float crx2 = cos(rx);
		float srx2 = sin(rx);
		float cry2 = cos(ry);
		float sry2 = sin(ry);
		float crz2 = cos(rz);
		float srz2 = sin(rz);
		
		float offsetx;
		float offsety;
		float offsetz;
		
		if (n == 0)
		{
			offsetx = obj->x - cam->x;
			offsety = obj->y - cam->y;
			offsetz = obj->z - cam->z;
		}
		else
		{
			offsetx = obj->x;
			offsety = obj->y;
			offsetz = obj->z;
			float newx = crz2*cry2*offsetx + (crz2*sry2*srx2-srz2*crx2)*offsety + (crz2*sry2*crx2+srz2*srx2)*offsetz;
			float newy = srz2*cry2*offsetx + (srz2*sry2*srx2+crz2*crx2)*offsety + (srz2*sry2*crx2-crz2*srx2)*offsetz;
			float newz = -sry2*offsetx + cry2*srx2*offsety + cry2*crx2*offsetz;
			offsetx = newx - cam->x + x;
			offsety = newy - cam->y + y;
			offsetz = newz - cam->z + z;
		}
		
		float newoffsetx = cam->cry*cam->crz*offsetx - cam->cry*cam->srz*offsety + offsetz*cam->sry;
		float newoffsety = (cam->srx*cam->sry*cam->crz + cam->crx*cam->srz)*offsetx + (cam->crx*cam->crz-cam->srx*cam->sry*cam->srz)*offsety - cam->srx*cam->cry*offsetz;
		float newoffsetz = (cam->srx*cam->srz-cam->crx*cam->sry*cam->crz)*offsetx + (cam->crx*cam->sry*cam->srz + cam->srx*cam->crz)*offsety + cam->crx*cam->cry*offsetz;
		
		if (obj->sun != NULL)
			draw_scene(obj->sun, offsetx+cam->x, offsety+cam->y, offsetz+cam->z	, obj->rx+rx, obj->ry+ry, obj->rz+rz, n+1);
		
		for (int j = 0; j < obj->nt; j++)
		{				

			float newx1 = crz*cry*obj->triangles[j]->x1 + (crz*sry*srx-srz*crx)*obj->triangles[j]->y1 + (crz*sry*crx+srz*srx)*obj->triangles[j]->z1;
			float newy1 = srz*cry*obj->triangles[j]->x1 + (srz*sry*srx+crz*crx)*obj->triangles[j]->y1 + (srz*sry*crx-crz*srx)*obj->triangles[j]->z1;
			float newz1 = -sry*obj->triangles[j]->x1 + cry*srx*obj->triangles[j]->y1 + cry*crx*obj->triangles[j]->z1;
			float newx2 = crz*cry*obj->triangles[j]->x2 + (crz*sry*srx-srz*crx)*obj->triangles[j]->y2 + (crz*sry*crx+srz*srx)*obj->triangles[j]->z2;
			float newy2 = srz*cry*obj->triangles[j]->x2 + (srz*sry*srx+crz*crx)*obj->triangles[j]->y2 + (srz*sry*crx-crz*srx)*obj->triangles[j]->z2;
			float newz2 = -sry*obj->triangles[j]->x2 + cry*srx*obj->triangles[j]->y2 + cry*crx*obj->triangles[j]->z2;
			float newx3 = crz*cry*obj->triangles[j]->x3 + (crz*sry*srx-srz*crx)*obj->triangles[j]->y3 + (crz*sry*crx+srz*srx)*obj->triangles[j]->z3;
			float newy3 = srz*cry*obj->triangles[j]->x3 + (srz*sry*srx+crz*crx)*obj->triangles[j]->y3 + (srz*sry*crx-crz*srx)*obj->triangles[j]->z3;
			float newz3 = -sry*obj->triangles[j]->x3 + cry*srx*obj->triangles[j]->y3 + cry*crx*obj->triangles[j]->z3	;
			
			obj->triangles[j]->tx1 = cam->cry*cam->crz*newx1 - cam->cry*cam->srz*newy1 + newz1*cam->sry + newoffsetx;
			obj->triangles[j]->ty1 = (cam->srx*cam->sry*cam->crz + cam->crx*cam->srz)*newx1 + (cam->crx*cam->crz-cam->srx*cam->sry*cam->srz)*newy1 - cam->srx*cam->cry*newz1 + newoffsety;
			obj->triangles[j]->tz1 = (cam->srx*cam->srz-cam->crx*cam->sry*cam->crz)*newx1 + (cam->crx*cam->sry*cam->srz + cam->srx*cam->crz)*newy1 + cam->crx*cam->cry*newz1 + newoffsetz;
			obj->triangles[j]->tx2 = cam->cry*cam->crz*newx2 - cam->cry*cam->srz*newy2 + newz2*cam->sry + newoffsetx;
			obj->triangles[j]->ty2 = (cam->srx*cam->sry*cam->crz + cam->crx*cam->srz)*newx2 + (cam->crx*cam->crz-cam->srx*cam->sry*cam->srz)*newy2 - cam->srx*cam->cry*newz2 + newoffsety;
			obj->triangles[j]->tz2 = (cam->srx*cam->srz-cam->crx*cam->sry*cam->crz)*newx2 + (cam->crx*cam->sry*cam->srz + cam->srx*cam->crz)*newy2 + cam->crx*cam->cry*newz2 + newoffsetz;
			obj->triangles[j]->tx3 = cam->cry*cam->crz*newx3 - cam->cry*cam->srz*newy3 + newz3*cam->sry + newoffsetx;
			obj->triangles[j]->ty3 = (cam->srx*cam->sry*cam->crz + cam->crx*cam->srz)*newx3 + (cam->crx*cam->crz-cam->srx*cam->sry*cam->srz)*newy3 - cam->srx*cam->cry*newz3 + newoffsety;
			obj->triangles[j]->tz3 = (cam->srx*cam->srz-cam->crx*cam->sry*cam->crz)*newx3 + (cam->crx*cam->sry*cam->srz + cam->srx*cam->crz)*newy3 + cam->crx*cam->cry*newz3 + newoffsetz;
			
		}
		obj = obj->brother;
	}
	for (int i = 0; i < nt; i++)
	{
		struct triangle *t = triangles[i];
		float x1 = t->tx1 * client_height/t->tz1/2 + client_width/2;
		float y1 = t->ty1 * client_height/t->tz1/2 + client_height/2;
		float x2 = t->tx2 * client_height/t->tz2/2 + client_width/2;
		float y2 = t->ty2 * client_height/t->tz2/2 + client_height/2;
		float x3 = t->tx3 * client_height/t->tz3/2 + client_width/2;
		float y3 = t->ty3 * client_height/t->tz3/2 + client_height/2;
		if (x1 > 0 && x1 < client_width && y1 > 0 && y1 < client_height && 
			x2 > 0 && x2 < client_width && y2 > 0 && y2 < client_height &&
			x3 > 0 && x3 < client_width && y3 > 0 && y3 < client_height &&
			t->tz1 >= 0 && t->tz2 >= 0 && t->tz3 >= 0)
			draw_triangle(x1,x2,x3,y1,y2,y3,t->tz1,t->tz2,t->tz3, t->color);
	}
}
	
void draw_rectangle(int centerx, int centery, int sizex, int sizey, uint32_t color)
{
	draw_line(centerx - sizex/2, centerx + sizex/2, centery - sizey/2, centery - sizey/2,0,0, color);
	draw_line(centerx - sizex/2, centerx + sizex/2, centery + sizey/2, centery + sizey/2,0,0, color);
	draw_line(centerx - sizex/2, centerx - sizex/2, centery + sizey/2, centery - sizey/2,0,0, color);
	draw_line(centerx + sizex/2, centerx + sizex/2, centery - sizey/2, centery + sizey/2,0,0, color);
}

void clear_screen(uint32_t color)
{
    uint32_t *pixel = (uint32_t *)memory;
    for(int i = 0; i < client_width * client_height; ++i)
        *pixel++ = color;
	float *z = (float*)zbuffer;
	for(int i = 0; i < client_width * client_height; ++i)
        *z++ = 9999999;
	
}

