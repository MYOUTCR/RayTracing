#pragma once

#ifndef _HITTABLE_HEAD_
#define _HITTABLE_HEAD_

#include "ray.h"
#include "aabb.h"
#include "camera.h"


void get_shpere_uv(const vec3 &p, float &u, float &v)
{
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + _M_PI_) / (2 * _M_PI_);
	v = (theta + _M_PI_ / 2) / _M_PI_;
}

class material;

struct hit_record
{
	float t;
	float u;
	float v;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};

class hittable
{
public:
	hittable(){};
	~hittable(){};

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const = 0;
	virtual bool bounding_box(float t0, float t1, aabb &box)const = 0;
};



#endif


