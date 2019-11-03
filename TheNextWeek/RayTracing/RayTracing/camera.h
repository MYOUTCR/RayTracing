

#ifndef _CAMERA_HEAD_
#define _CAMERA_HEAD_

#include "ray.h"
#include <math.h>
#include "random.h"
#define _M_PI_ 3.14159265358979323846

vec3 random_in_unit_disk()
{
	vec3 p;
	do 
	{
		p = 2.0*vec3(random_double(), random_double(), 0) - vec3(1.0, 1.0, 0.0);
	} while (dot(p, p) >= 1.0);

	return p;
}

class camera
{
public:
	camera()
	{
		_lower_left_corner=vec3(-2.0, -1.0, -1.0);
		_horizontal = vec3(4.0, 0.0, 0.0);
		_vertical = vec3(0.0, 2.0, 0.0);
		_origin = vec3(0.0, 0.0, 0.0);
	}

	camera(float vfov, float aspect)
	{
		float theta = vfov*_M_PI_ / 180.0;
		float half_height = tan(theta*0.5);
		float half_width = aspect*half_height;
		_lower_left_corner = vec3(-half_width, -half_height, -1.0);
		_horizontal = vec3(2 * half_width, 0.0, 0.0);
		_vertical = vec3(0.0, 2.0*half_height, 0.0);
		_origin = vec3(0.0, 0.0, 0.0);
	}

	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture,float focus_dist)
	{
		_lens_radius = aperture*0.5;
		float theta = vfov*_M_PI_ / 180.0;
		float half_height = tan(theta*0.5);
		float half_width = aspect*half_height;
		_origin = lookfrom;

		_w = unit_vector(lookfrom - lookat);
		_u = unit_vector(cross(vup, _w));
		_v = cross(_w, _u);

		_lower_left_corner = _origin - half_width*focus_dist*_u - half_height*focus_dist*_v - focus_dist*_w;

		_horizontal = 2 * half_width*focus_dist*_u;
		_vertical = 2 * half_height*focus_dist*_v;
	}

	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float ft0, float ft1)
	{
		_lens_radius = aperture*0.5;
		float theta = vfov*_M_PI_ / 180.0;
		float half_height = tan(theta*0.5);
		float half_width = aspect*half_height;
		_origin = lookfrom;

		_time0 = ft0;
		_time1 = ft1;

		_w = unit_vector(lookfrom - lookat);
		_u = unit_vector(cross(vup, _w));
		_v = cross(_w, _u);

		_lower_left_corner = _origin - half_width*focus_dist*_u - half_height*focus_dist*_v - focus_dist*_w;

		_horizontal = 2 * half_width*focus_dist*_u;
		_vertical = 2 * half_height*focus_dist*_v;
	}

	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect)
	{
		vec3 u, v, w;
		float theta = vfov*_M_PI_ / 180.0;
		float half_height = tan(theta*0.5);
		float half_width = aspect*half_height;
		_origin = lookfrom;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		_lower_left_corner = _origin - half_width*u - half_height*v - w;

		_horizontal = 2 * half_width*u;
		_vertical = 2 * half_height*v;
	}

	~camera(){}

// 	ray getRay(float u, float v)
// 	{
// 		return ray(_origin, _lower_left_corner + u*_horizontal + v*_vertical - _origin);
// 	}

	ray getRay(float s, float t)
	{
		vec3 rd = _lens_radius*random_in_unit_disk();
		vec3 offset = _u*rd.x() + _v*rd.y();
		float ftime = _time0 + random_double()*(_time1 - _time0);
		return ray(_origin + offset, _lower_left_corner + s*_horizontal + t*_vertical - _origin - offset);
	}

	vec3 _origin;
	vec3 _lower_left_corner;
	vec3 _horizontal;
	vec3 _vertical;
	vec3 _u, _v, _w;
	float _lens_radius;
	float _time0, _time1;
};




#endif