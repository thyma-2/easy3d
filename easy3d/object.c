#include "object.h"

int nt = 0;
int size_array = 0;
struct triangle **triangles;
struct object *objects = NULL;

struct triangle *create_triangle(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3, int color)
{
	if (nt == size_array)
	{
		size_array = size_array * 2 + 1;
		triangles = realloc(triangles, sizeof(struct triangle*) * size_array);
	}
	triangles[nt] = malloc(sizeof(struct triangle));
	triangles[nt]->x1 = x1;
	triangles[nt]->x2 = x2;
	triangles[nt]->x3 = x3;
	triangles[nt]->y1 = y1;
	triangles[nt]->y2 = y2;
	triangles[nt]->y3 = y3;
	triangles[nt]->z1 = z1;
	triangles[nt]->z2 = z2;
	triangles[nt]->z3 = z3;
	triangles[nt]->color = color;
	nt += 1;
	return triangles[nt-1];
}

void add_triangle_into_object(struct object *obj, struct triangle *t)
{
	obj->nt += 1;
	obj->triangles = realloc(obj->triangles, sizeof(struct triangle*) * obj->nt);
	obj->triangles[obj->nt - 1] = t;
}

struct object *create_object(struct object *father, float x, float y,float z,float rx,float ry,float rz)
{
	struct object *new = malloc(sizeof(struct object));
	new->sun = NULL;
	new->brother = NULL;
	new->x = x;
	new->y = y;
	new->z = z;
	new->rx = rx;
	new->ry = ry;
	new->rz = rz;
	new->nt = 0;
	if (father == NULL)
	{
		new->triangles = NULL;
		if (objects == NULL)
			objects = new;
		else
		{
			struct object *p = objects;
			while (p->brother != NULL)
				p = p->brother;
			p->brother = new;
		}
	}
	else
	{
		if (father->sun == NULL)
			father->sun = new;
		else
		{
			struct object *p = father->sun;
			while (p->brother != NULL)
				p = p->brother;
			p->brother = new;
		}
	}
	return new;
}

void fill_object_with_obj(struct object *obj, char *string)
{
	printf ("%s\n", string);
	int i = 0;
	int np = 0;
	int startpt = 0;
	int endpt = 0;
	
	while (strncmp(string + i, "\nv", 2) != 0)
		i++;
	i++;
	startpt = i;
	while (string[i + 1] != 't' && string[i + 1] != '\n')
	{
		np ++;
		while(string[i] != '\n')
			i++;
		i++;
	}
	
	endpt = i;
	float *pts = malloc(sizeof(float) * np * 3);

	for (int i = 0; i < np; i++)
	{
		sscanf(string + startpt, "v %f %f %f", pts + i * 3,pts + i * 3 + 1, pts + i * 3 + 2);
		while (string[startpt] != '\n')
		{
			startpt ++;
		}
		startpt++;
	}

	while (strncmp(string + i, "\nf", 2) != 0)
		i++;
	i++;
	int nshlash = 0;
	for (int b = i + 1; string[b] != '\n';b++)
		if (string[b] == '/')
			nshlash++;
	if (nshlash == 6) // triangles
	{
		int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9;
		while (1 == 1)
		{
			printf ("%s\n", string + i);
			sscanf(string + i, "f %d/%d/%d %d/%d/%d %d/%d/%d", &tmp1, &tmp2, &tmp3, &tmp4, &tmp5, &tmp6, &tmp7, &tmp8, &tmp9);
			printf ("%d %d %d\n", tmp1, tmp4, tmp7);
			struct triangle *t = create_triangle(
					pts[3 * (tmp1 - 1)], pts[3 * (tmp4 - 1)], pts[3 * (tmp7 - 1)], 
					pts[3 * (tmp1 - 1) + 1], pts[3 * (tmp4 - 1) + 1], pts[3 * (tmp7 - 1) + 1], 
					pts[3 * (tmp1 - 1) + 2], pts[3 * (tmp4 - 1) + 2	], pts[3 * (tmp7 - 1) + 2], rand());
			printf ("test\n");
			add_triangle_into_object(obj, t);
			i++;
			while (string[i] != '\n' && string[i] != 0)
				i++;
			i++;
			if (string[i] == 0)
				return;
		}
	}
	else if (nshlash == 8)
	{

		int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10, tmp11,tmp12;
		while (1 == 1)
		{
			sscanf(string + i, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &tmp1, &tmp2, &tmp3, &tmp4, &tmp5, &tmp6, &tmp7, &tmp8, &tmp9, &tmp10, &tmp11, &tmp12);
			//printf ("%d %d %d %d\n", tmp1, tmp4, tmp7, tmp10);
			struct triangle *t = create_triangle(
					pts[3 * (tmp1 - 1)], pts[3 * (tmp4 - 1)], pts[3 * (tmp7 - 1)], 
					pts[3 * (tmp1 - 1) + 1], pts[3 * (tmp4 - 1) + 1], pts[3 * (tmp7 - 1) + 1], 
					pts[3 * (tmp1 - 1) + 2], pts[3 * (tmp4 - 1) + 2	], pts[3 * (tmp7 - 1) + 2], rand());
			add_triangle_into_object(obj, t);
			t = create_triangle(
					pts[3 * (tmp7 - 1)], pts[3 * (tmp10 - 1)], pts[3 * (tmp1 - 1)], 
					pts[3 * (tmp7 - 1) + 1], pts[3 * (tmp10 - 1) + 1], pts[3 * (tmp1 - 1) + 1], 
					pts[3 * (tmp7 - 1) + 2], pts[3 * (tmp10 - 1) + 2	], pts[3 * (tmp1 - 1) + 2], rand());
			add_triangle_into_object(obj, t);
			
			i++;
			while (string[i] != '\n' && string[i] != 0)
				i++;
			i++;
			if (string[i] == 0)
				return;
		}
	}
}

int cuboid_to_obj(struct object *obj, char *string)
{
	int i = 0;
	while (string[i] != 0)
	{
		if (string[i] == '>')
		{
			i++;
			float x,y,z,rx,ry,rz;
			sscanf(string + i, "%f %f %f %f %f %f", &x,&y,&z,&rx, &ry, &rz);
			while (string[i] != '\n')
				i++;
			i++;
			struct object *sun = create_object(obj, x,y,z,rx,ry,rz);
			i += cuboid_to_obj(sun, string + i);
			
		}
		else if (string[i] == '<')
		{
			i += 2;
			return i;
		}
		else
		{
			float x,y,z,sx,sy,sz,rx,ry,rz;
			int color;
			sscanf(string + i, "%f %f %f %f %f %f %f %f %f %x", &x,&y,&z,&sx,&sy,&sz,&rx, &ry, &rz, &color);
			
			float a = cos(rx);
			float b = sin(rx);
			float c = cos(ry);
			float d = sin(ry);
			float e = cos(rz);
			float f = sin(rz);
			
			int x1 = - sx/2;
			int y1 = - sy/2;
			int z1 = - sz/2;
			int x2 = - sx/2;
			int y2 = sy/2;
			int z2 = - sz/2;
			int x3 = sx/2;
			int y3 = sy/2;
			int z3 = - sz/2;
			int x4 = sx/2;
			int y4 = - sy/2;
			int z4 = - sz/2;
			int x5 = - sx/2;
			int y5 = - sy/2;
			int z5 = sz/2;
			int x6 = - sx/2;
			int y6 = sy/2;
			int z6 = sz/2;
			int x7 = sx/2;
			int y7 = sy/2;
			int z7 = sz/2;
			int x8 = sx/2;
			int y8 = - sy/2;
			int z8 = sz/2;
			
			
			float newx1 = c*e*x1 - c*f*y1 + z1*d + x;
			float newy1 = (b*d*e + a*f)*x1 + (a*e-b*d*f)*y1 - b*c*z1 + y;
			float newz1 = (b*f-a*d*e)*x1 + (a*d*f + b*e)*y1 + a*c*z1 + z;
			float newx2 = c*e*x2 - c*f*y2 + z2*d + x;
			float newy2 = (b*d*e + a*f)*x2 + (a*e-b*d*f)*y2 - b*c*z2 + y;
			float newz2 = (b*f-a*d*e)*x2 + (a*d*f + b*e)*y2 + a*c*z2 + z;
			float newx3 = c*e*x3 - c*f*y3 + z3*d + x;
			float newy3 = (b*d*e + a*f)*x3 + (a*e-b*d*f)*y3 - b*c*z3 + y;
			float newz3 = (b*f-a*d*e)*x3 + (a*d*f + b*e)*y3 + a*c*z3 + z;
			float newx4 = c*e*x4 - c*f*y4 + z4*d + x;
			float newy4 = (b*d*e + a*f)*x4 + (a*e-b*d*f)*y4 - b*c*z4 + y;
			float newz4 = (b*f-a*d*e)*x4 + (a*d*f + b*e)*y4 + a*c*z4 + z;
			float newx5 = c*e*x5 - c*f*y5 + z5*d + x;
			float newy5 = (b*d*e + a*f)*x5 + (a*e-b*d*f)*y5 - b*c*z5 + y;
			float newz5 = (b*f-a*d*e)*x5 + (a*d*f + b*e)*y5 + a*c*z5 + z;
			float newx6 = c*e*x6 - c*f*y6 + z6*d + x;
			float newy6 = (b*d*e + a*f)*x6 + (a*e-b*d*f)*y6 - b*c*z6 + y;
			float newz6 = (b*f-a*d*e)*x6 + (a*d*f + b*e)*y6 + a*c*z6 + z;
			float newx7 = c*e*x7 - c*f*y7 + z7*d + x;
			float newy7 = (b*d*e + a*f)*x7 + (a*e-b*d*f)*y7 - b*c*z7 + y;
			float newz7 = (b*f-a*d*e)*x7 + (a*d*f + b*e)*y7 + a*c*z7 + z;
			float newx8 = c*e*x8 - c*f*y8 + z8*d + x;
			float newy8 = (b*d*e + a*f)*x8 + (a*e-b*d*f)*y8 - b*c*z8 + y;
			float newz8 = (b*f-a*d*e)*x8 + (a*d*f + b*e)*y8 + a*c*z8 + z;
			
			struct triangle *t = create_triangle(newx1, newx2, newx4, newy1,newy2, newy4, newz1, newz2, newz4, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx2, newx3, newx4, newy2,newy3, newy4, newz2, newz3, newz4, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx3, newx4, newx8, newy3, newy4, newy8, newz3, newz4, newz8, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx3, newx7, newx8, newy3, newy7, newy8, newz3, newz7, newz8, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx1, newx2, newx6, newy1, newy2, newy6, newz1, newz2, newz6, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx1, newx5, newx6, newy1, newy5, newy6, newz1, newz5, newz6, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx2, newx3, newx7, newy2, newy3, newy7, newz2, newz3, newz7, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx2, newx6, newx7, newy2, newy6, newy7, newz2, newz6, newz7, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx1, newx4, newx8, newy1, newy4, newy8, newz1, newz4, newz8, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx1, newx5, newx8, newy1, newy5, newy8, newz1, newz5, newz8, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx6, newx7, newx8, newy6, newy7, newy8, newz6, newz7, newz8, color);
			add_triangle_into_object(obj, t);
			t = create_triangle(newx5, newx6, newx8, newy5, newy6, newy8, newz5, newz6, newz8, color);
			add_triangle_into_object(obj, t);
			while (string[i] != '\n')
				i++;
			i++;
			obj->sprx = 0;
			obj->spry = 0;
			obj->sprz = 0;
			obj->spx = 0;
			obj->spy = 0;
			obj->spz = 0;
		}
	}
	return i;
}


/*void quickSort(struct triangle *arr[], int start, int end)
{
    if(start < end)
    {
        int pIndex = partition(arr, start, end);
        quickSort(arr, start, pIndex-1);
        quickSort(arr, pIndex+1, end);
    }
}

int partition(struct triangle *arr[], int start, int end)
{
    int pIndex = start;
    int pivot = arr[end]->tz1 + arr[end]->tz2 + arr[end]->tz3;
    int i;
    for(i = start; i < end; i++)
    {
        if(arr[i]->tz1 + arr[i]->tz2 + arr[i]->tz3 > pivot)
        {
            struct triangle *tmp = arr[pIndex];
			arr[pIndex] = arr[i];
			arr[i] = tmp;
            pIndex++;
        }
    }
    struct triangle *tmp = arr[pIndex];
	arr[pIndex] = arr[i];
	arr[i] = tmp;
    return pIndex;
}*/