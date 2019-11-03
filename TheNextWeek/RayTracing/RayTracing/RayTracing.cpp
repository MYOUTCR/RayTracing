// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "stb_image_write.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include "material.h"
#include "texture.h"

#include "stb_image.h"

void example_1();
vec3 example_1_color(const ray &r, hittable *world, int depth);
hittable* random_scence();

void example_2();
hittable* random_scence_2();

void example_3();
hittable_list *tow_spheres();

void example_5();
hittable *earth();


void example_4();///**
hittable_list *tow_perlin_spheres();

int _tmain(int argc, _TCHAR* argv[])
{
	example_5();

	return 0;
}


void example_1()
{
	int nx = 1200, ny = 800, ns = 10;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = random_scence();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(random_double() + i) / float(nx);
				float v = float(random_double() + j) / float(ny);
				ray r = cam.getRay(u, v);
				col += example_1_color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			//std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
		std::cout << ny - j << "行" << "\n";
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example_1.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

vec3 example_1_color(const ray &r, hittable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && (rec.mat_ptr->scatter(r, rec, attenuation, scattered)))
		{
			return attenuation*example_1_color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}

hittable* random_scence()
{
	int n = 500;
	hittable **list = new hittable *[n + 1];
	//list[0] = new sphere(vec3(0, -1000, 0.0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	list[0] = new sphere(vec3(0, -1000, 0.0), 1000, new lambertian(new constant_texture(vec3(0.5, 0.5, 0.5))));

	int i = 1;

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = random_double();
			vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					list[i++] = new sphere(center, 0.2, new lambertian(new constant_texture(vec3(random_double()*random_double(), random_double()*random_double(), random_double()*random_double()))));
				}
				else if (choose_mat < 0.95)
				{
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1.0 + random_double()), 0.5*(1.0 + random_double()), 0.5*(1.0 + random_double())), 0.5* random_double()));
				}
				else
				{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hittable_list(list, i);
}


void example_2()
{
	int nx = 1200, ny = 800, ns = 10;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = random_scence_2();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(random_double() + i) / float(nx);
				float v = float(random_double() + j) / float(ny);
				ray r = cam.getRay(u, v);
				col += example_1_color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			//std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
		std::cout << ny - j << "行" << "\n";
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example_2.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable* random_scence_2()
{
	int n = 500;
	hittable **list = new hittable *[n + 1];
	
	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));

	list[0] = new sphere(vec3(0, -1000, 0.0), 1000, new lambertian(checker));

	int i = 1;

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = random_double();
			vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					list[i++] = new sphere(center, 0.2, new lambertian(new constant_texture(vec3(random_double()*random_double(), random_double()*random_double(), random_double()*random_double()))));
				}
				else if (choose_mat < 0.95)
				{
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1.0 + random_double()), 0.5*(1.0 + random_double()), 0.5*(1.0 + random_double())), 0.5* random_double()));
				}
				else
				{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hittable_list(list, i);
}


void example_3()
{
	int nx = 1200, ny = 800, ns = 10;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = tow_spheres();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(random_double() + i) / float(nx);
				float v = float(random_double() + j) / float(ny);
				ray r = cam.getRay(u, v);
				col += example_1_color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			//std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
		std::cout << ny - j << "行" << "\n";
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example_3.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable_list *tow_spheres()
{
	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));

	int n = 50;
	hittable **list = new hittable *[n + 1];
	list[0] = new sphere(vec3(0, -10, 0.0), 10, new lambertian(checker));
	list[1] = new sphere(vec3(0, 10, 0.0), 10, new lambertian(checker));

	return new hittable_list(list, 2);
}


void example_4()
{
	int nx = 1200, ny = 800, ns = 10;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = tow_perlin_spheres();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(random_double() + i) / float(nx);
				float v = float(random_double() + j) / float(ny);
				ray r = cam.getRay(u, v);
				col += example_1_color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			//std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
		std::cout << ny - j << "行" << "\n";
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example_4.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable_list *tow_perlin_spheres()
{
	texture *perTex = new moise_texture();

	int n = 50;
	hittable **list = new hittable *[n + 1];
	list[0] = new sphere(vec3(0, -10, 0.0), 10, new lambertian(perTex));
	list[1] = new sphere(vec3(0, 10, 0.0), 10, new lambertian(perTex));

	return new hittable_list(list, 2);
}

void example_5()
{
	int nx = 1200, ny = 800, ns = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = earth();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	int nIndex = 0;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(random_double() + i) / float(nx);
				float v = float(random_double() + j) / float(ny);
				
				ray r = cam.getRay(u, v);
				col += example_1_color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			Ptr[nIndex++] = unsigned char(255.99*col[0]);
			Ptr[nIndex++] = unsigned char(255.99*col[1]);
			Ptr[nIndex++] = unsigned char(255.99*col[2]);
			//std::cout << 255.99*col[0] << " " << 255.99*col[1] << " " << 255.99*col[2] << "\n";
		}
		std::cout << ny - j << "行" << "\n";
	}

	unsigned int channels_num = 3;

	stbi_write_jpg("E:\\example_5.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable *earth()
{
	int nx = 0, ny = 0, nn = 0;
	unsigned char *tex_date = stbi_load("E:\\earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_date, nx, ny));

	return new sphere(vec3(0, 0, 0), 2, mat);
}