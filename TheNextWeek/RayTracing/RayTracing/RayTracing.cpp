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
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"

#include "stb_image.h"

void example_1();
vec3 example_1_color(const ray &r, hittable *world, int depth);
hittable* random_scence();

void example_2();
hittable* random_scence_2();

void example_3();
hittable_list *tow_spheres();

void example_4();
hittable_list *tow_perlin_spheres();

void example_5();

void example_6();
hittable_list *tow_perlin_spheres_sc();

void example_7();
hittable *earth();

void example_8();
vec3 color2(const ray &r, hittable *world, int depth);

void example_9();
hittable *simple_light();

hittable *cornell_box();
void example_10();

hittable *cornell_box_2();
void example_11();

hittable *cornell_smoke();
void example_12();

hittable *final();
void example_13();

int _tmain(int argc, _TCHAR* argv[])
{
	example_13();
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
	texture *perTex = new noise_texture();

	hittable **list = new hittable *[2];
	list[0] = new sphere(vec3(0, -1000, 0.0), 1000, new lambertian(perTex));
	list[1] = new sphere(vec3(0, 2, 0.0), 2, new lambertian(perTex));

	return new hittable_list(list, 2);
}

void example_5()
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

	stbi_write_jpg("E:\\example_5.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

void example_6()
{
	int nx = 1200, ny = 800, ns = 10;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = tow_perlin_spheres_sc();

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

	stbi_write_jpg("E:\\example_6.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable_list *tow_perlin_spheres_sc()
{
	texture *perTex = new noise_texture(4);

	hittable **list = new hittable *[2];
	list[0] = new sphere(vec3(0, -1000, 0.0), 1000, new lambertian(perTex));
	list[1] = new sphere(vec3(0, 2, 0.0), 2, new lambertian(perTex));

	return new hittable_list(list, 2);
}

void example_7()
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

	stbi_write_jpg("E:\\example_7.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

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

vec3 color2(const ray &r, hittable *world, int depth)
{
	hit_record rec;
// 	if (world->hit(r, 0.001, FLT_MAX, rec))
// 	{
// 		ray scattered;
// 		vec3 attenuation;
// 		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
// 		if (depth < 50 && (rec.mat_ptr->scatter(r, rec, attenuation, scattered)))
// 		{
// 			return emitted + attenuation*color2(scattered, world, depth + 1);
// 		}
// 		else
// 		{	
// 			return emitted;
// 		}
// 	}
// 	else
// 	{
// 		return vec3(0, 0, 0);
// 	}

	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return emitted + attenuation*color2(scattered, world, depth + 1);
		else
			return emitted;
	}
	else
		return vec3(0, 0, 0);

}

void example_8()
{
	int nx = 1200, ny = 800, ns = 10;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = tow_perlin_spheres_sc();

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
				col += color2(r, world, 0);
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

	stbi_write_jpg("E:\\example_8.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable *simple_light()
{
	texture *perTex = new noise_texture(4);

	hittable **list = new hittable *[4];
	list[0] = new sphere(vec3(0, -1000, 0.0), 1000, new lambertian(perTex));
	list[1] = new sphere(vec3(0, 2, 0.0), 2, new lambertian(perTex));
	list[2] = new sphere(vec3(0, 7, 0.0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));

	return new hittable_list(list, 4);
};

void example_9()
{
	int nx = 1200, ny = 800, ns = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = simple_light();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 40, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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
				col += color2(r, world, 0);
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

	stbi_write_jpg("E:\\example_9.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable *cornell_box()
{
	hittable **list = new hittable *[5];

	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *gree = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));

	list[i++] = new yz_rect(0, 555, 0, 555, 555, gree);
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new xy_rect(0, 555, 0, 555, 555, white);

	return new hittable_list(list, 5);
}

void example_10()
{
	int nx = 1200, ny = 800, ns = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = cornell_box();

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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
				col += color2(r, world, 0);
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

	stbi_write_jpg("E:\\example_10.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable *cornell_box_2()
{
	hittable **list = new hittable *[8];

	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *gree = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));

	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, gree));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	//list[i++] = new box(vec3(130, 0, 65), vec3(295, 165, 230), white);
	//list[i++] = new box(vec3(265, 0, 295), vec3(430, 330, 460), white);

	list[i++] = new translate (new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),vec3(130,0,165));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), -18), vec3(265, 0, 295));

	return new hittable_list(list, i);
}

void example_11()
{
	int nx = 1200, ny = 800, ns = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = cornell_box_2();

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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
				col += color2(r, world, 0);
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

	stbi_write_jpg("E:\\example_11_1.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable *cornell_smoke()
{
	hittable **list = new hittable *[8];

	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *gree = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));

	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, gree));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	//list[i++] = new box(vec3(130, 0, 65), vec3(295, 165, 230), white);
	//list[i++] = new box(vec3(265, 0, 295), vec3(430, 330, 460), white);

	hittable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 165));
	hittable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), -18), vec3(265, 0, 295));

	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));


	return new hittable_list(list, i);
}

void example_12()
{
	int nx = 1200, ny = 800, ns = 100;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = cornell_smoke();

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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
				col += color2(r, world, 0);
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

	stbi_write_jpg("E:\\example_12.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

void example_13()
{
	int nx = 1200, ny = 800, ns = 10000;

	char *Ptr = NULL;
	Ptr = (char *)malloc(nx*ny * 3 * sizeof(char));

	hittable *world = final();

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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
				vec3 p = r.point_at_parameter(2.0);
				col += color2(r, world, 0);
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

	stbi_write_jpg("E:\\example_13.jpg", nx, ny, channels_num, Ptr, nx * channels_num);

	if (NULL != Ptr)
	{
		free(Ptr);
		Ptr = NULL;
	}
}

hittable *final()
{
	int nb = 20;
	hittable **list = new hittable *[30];

	hittable **boxlist = new hittable *[10000];
	hittable **boxlist2 = new hittable *[10000];

	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));

	int b = 0;
	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100;
			float x0 = -1000 + i*w;
			float z0 = -1000 + j*w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (random_double() + 0.01);
			float z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}

	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);
	material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[l++] = new xz_rect(123, 423, 147, 412, 554, light);

	vec3 center(400, 400, 200);
	list[l++] = new moving_sphere(center, center + vec3(30, 0.0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));

	list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
	list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));

	hittable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));

	boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
	list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));


	int nx = 0, ny = 0, nn = 0;
	unsigned char *tex_date = stbi_load("E:\\earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_date, nx, ny));
	list[l++] = new sphere(vec3(400, 200, 400), 100, mat);

	texture *pertext = new noise_texture(0.1);
	list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));

	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxlist2[j] = new sphere(vec3(165 * random_double(), 165 * random_double(), 165 * random_double()), 10, white);
	}

	list[l++] = new translate(new rotate_y(
		new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));

	return new hittable_list(list, l);

}