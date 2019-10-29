

#ifndef _CAMERA_HEAD_
#define _CAMERA_HEAD_

#include "ray.h"
#include <math.h>

#define _M_PI_ 3.14159265358979323846

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

	ray getRay(float u, float v)
	{
		return ray(_origin, _lower_left_corner + u*_horizontal + v*_vertical - _origin);
	}

	vec3 _origin;
	vec3 _lower_left_corner;
	vec3 _horizontal;
	vec3 _vertical;

};




#endif