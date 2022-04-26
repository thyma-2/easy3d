#include "land.h"

extern struct triangle **triangles;
extern struct object *objects;

void not_under_zero(struct surface *s)
{
	if (s->y00 <= 0)
		s->y00 = 0;
	if (s->y01 <= 0)
		s->y01 = 0;
	if (s->y11 <= 0)
		s->y11 = 0;
	if (s->y10 <= 0)
		s->y10 = 0;
}

void to_zerro(struct surface *s)
{
	s->y00 = 0;
	s->y01 = 0;
	s->y11 = 0;
	s->y10 = 0;
}

float **generate_land(int nvx, int nvy, int ecart, int s)
{
	srand(time(NULL));   // Initialization, should only be called once.
	
	int sx = ecart*nvx - ecart + 1;
	int sy = ecart*nvy - ecart + 1;
	
	float **land = (float**)calloc(sy*sizeof(float*), 1);
	for (int i = 0; i < sy; i++)
		land[i] = (float*)calloc(sx*sizeof(float), 1);
	
	
	for (int i = 0; i < sy; i+=ecart)
		for (int j = 0; j < sx; j+=ecart)
			land[i][j] = rand()%100*s - 50*s;
		
	for (int i = 0; i < sy; i+=ecart)
	{
		for (int j = 0; j < sx-ecart; j+=ecart)
		{
			float to_add = (land[i][j + ecart] - land[i][j]) / ecart;
			for (int k = j + 1; k < j + ecart; k+=1)
				land[i][k] = land[i][k-1] + to_add;
		}
	}
	for (int j = 0; j < sx; j+=1)
	{
		for (int i = 0; i < sy-ecart; i+=ecart)
		{
			float to_add = (land[i + ecart][j] - land[i][j]) / ecart;
			for (int k = i + 1; k < i + ecart; k+=1)
				land[k][j] = land[k-1][j] + to_add;
		}
	}
	
	struct surface **land2 = (struct surface**)malloc(sy*sizeof(struct surface*));
	for (int i = 0; i < sy; i++)
		land2[i] = (struct surface*)malloc(sx*sizeof(struct surface));
	
	land2[0][0].y00 = land[0][0];
	land2[0][0].y01 = land[0][0];
	land2[0][0].y11 = land[0][0];
	land2[0][0].y10 = land[0][0];
	for (int i = 1; i < sx; i++)
	{	
		land2[0][i].y00 = land2[0][i - 1].y10;
		land2[0][i].y01 = land2[0][i - 1].y11;
		land2[0][i].y10 = land[0][i];
		land2[0][i].y11 = land[0][i];
	}
	for (int i = 1; i < sy; i++)
	{
		land2[i][0].y00 = land2[i - 1][0].y01;
		land2[i][0].y10 = land2[i - 1][0].y11;
		land2[i][0].y11 = land[i][0];
		land2[i][0].y01 = land[i][0];
		for (int j = 1; j < sx; j++)
		{
			land2[i][j].y00 = land2[i - 1][j].y01;
			land2[i][j].y10 = land2[i - 1][j].y11;
			land2[i][j].y01 = land2[i][j - 1].y11;
			land2[i][j].y11 = land[i][j];
		}
	}
	
	struct object *obj = create_object(NULL,0,0, 0,0,0,0);
	for (int i = 0; i < sy; i++)
	{
		for (int j = 0; j < sx; j++)
		{
			int color;
			if (land[i][j] <= 0)
				color = 0x0000ff;
			else if (land[i][j] <= 2*s)
				color = 0xffff00;
			else if (land[i][j] < 25*s)
			{
				if (rand()%2 == 0)
					color = 0x00ff00;
				else
					color = 0xe0a000;
			}
			else
			{
				if (rand()%2 == 0)
					color = 0xffffff;
				else
					color = 0xd0e0f0;
			}
			not_under_zero(&land2[i][j]);
			
			struct triangle *t = create_triangle(-s + j*2*s, -s+ j*2*s, s+ j*2*s, land2[i][j].y00, land2[i][j].y01, land2[i][j].y10, -s + i*2*s, s+ i*2*s, -s+ i*2*s, color);
			add_triangle_into_object(obj,t);
			t = create_triangle(s+ j*2*s, s+ j*2*s, -s+ j*2*s, land2[i][j].y10, land2[i][j].y11, land2[i][j].y01 , -s+ i*2*s, s+ i*2*s, s+ i*2*s, color);
			add_triangle_into_object(obj,t);
		}
	}
	for (int i = 0; i < sy; i++)
		free(land2[i]);
	free(land2);
	return land;
}