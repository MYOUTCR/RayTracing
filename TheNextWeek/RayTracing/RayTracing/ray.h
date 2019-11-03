#pragma once

#ifndef _RAY_HRAD_
#define _RAY_HRAD_

#include "vec3.h"

class ray
{
public:
	ray(){};
	//ray(const vec3 &origin, const vec3 &direction){ _origin = origin, _direction = direction; }
	ray(const vec3 &origin, const vec3 &direction, float ti = 0){ _origin = origin, _direction = direction; _time = ti; }

	~ray(){};
	vec3 origin() const { return _origin; }
	vec3 direction() const { return _direction; }
	vec3 point_at_parameter(float t) const { return _origin + _direction*t; }

	float time() const { return _time; }
private:
	vec3 _origin;
	vec3 _direction;
	float _time;
};

#endif

