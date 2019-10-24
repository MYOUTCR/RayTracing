

#ifndef _CAMERA_HEAD_
#define _CAMERA_HEAD_

#include "ray.h"

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