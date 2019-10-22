#pragma once

#ifndef _HITTABLE_HEAD_
#define _HITTABLE_HEAD_

#include "ray.h"

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
};

class hittable
{
public:
	hittable(){};
	~hittable(){};

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const = 0;
};



#endif


