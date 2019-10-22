// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "stb_image_write.h"

void example01();
void example02();
void example03();
vec3 example03_color(const ray &r);

void example04();
bool example04_hit_Sphere(const vec3 &center, float fRadius, const ray &r);
vec3 example04_color(const ray &r);

void example05();
vec3 example05_color(const ray &r);
float example05_hit_Sphere(const vec3 &center, float fRadius, const ray &r);

int _tmain(int argc, _TCHAR* argv[])
{
	example05();

	return 0;
}


void example01()
{
	int nx = 200, ny = 100;
	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float fr = float(i) / nx;
			float fg = float(j) / ny;
			float fb = 0.2;
			int ir = int(255.99*fr);
			int ig = int(255.99*fg);
			int ib = int(255.99*fb);
			Ptr[nIndex++] = unsigned char(255.99*fr);
			Ptr[nIndex++] = unsigned char(255.99*fg);
			Ptr[nIndex++] = unsigned char(255.99*fb);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("example01.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

void example02()
{
	int nx = 200, ny = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(float(i) / nx, float(j) / ny, 0.2);
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example02.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

void example03()
{
	int nx = 200, ny = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / nx;
			float v = float(j) / ny;

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = example03_color(r);
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example03.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

vec3 example03_color(const ray &r)
{
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

void example04()
{
	int nx = 200, ny = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / nx;
			float v = float(j) / ny;

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = example04_color(r);
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example04.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

bool example04_hit_Sphere(const vec3 &center, float fRadius, const ray &r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0*dot(oc, r.direction());
	float c = dot(oc, oc) - fRadius*fRadius;
	float fDis = b*b - 4 * a*c;
	return (fDis > 0);
}

vec3 example04_color(const ray &r)
{
	if (example04_hit_Sphere(vec3(0, 0, -1.0), 0.7, r))
	{
		return vec3(1.0, 0.0, 0.0);
	}

	return example03_color(r);
}


void example05()
{
	int nx = 200, ny = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / nx;
			float v = float(j) / ny;

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = example05_color(r);
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example05.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

float example05_hit_Sphere(const vec3 &center, float fRadius, const ray &r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0*dot(oc, r.direction());
	float c = dot(oc, oc) - fRadius*fRadius;
	float fDis = b*b - 4 * a*c;
	if (fDis < 0)
	{
		return -1;
	}
	else
	{
		return (-b - sqrt(fDis)) / (2.0*a);
	}
}

vec3 example05_color(const ray &r)
{
	float t = example04_hit_Sphere(vec3(0, 0, -1.0), 0.5, r);

	if (t > 0.0)
	{
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
		return 0.5*vec3(N.x() + 1, N.y() + 1.0, N.z() + 1);
	}

	return example03_color(r);
}