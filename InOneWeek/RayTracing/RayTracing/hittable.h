#pragma once

#ifndef _HITTABLE_HEAD_
#define _HITTABLE_HEAD_

#include "ray.h"

class material;

struct hit_record
{
	float t;
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
};



#endif


