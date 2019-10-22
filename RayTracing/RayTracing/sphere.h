#pragma once
#include "hittable.h"

#ifndef _SPHERE_HEAD_
#define _SPHERE_HEAD_

class sphere :public hittable
{
public:
	sphere(){};
	sphere(vec3 center, float fRadius) :_center(center), _fRadius(fRadius){}
	~sphere(){};

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;

	vec3 _center;
	float _fRadius;
};

bool sphere::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{

}

#endif


