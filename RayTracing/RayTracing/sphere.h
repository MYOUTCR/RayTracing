#pragma once

#ifndef _SPHERE_HEAD_
#define _SPHERE_HEAD_

#include "hittable.h"
#include "lambertian.h"
class sphere :public hittable
{
public:
	sphere(){}
	sphere(vec3 center, float fRadius, material *m) :_center(center), _fRadius(fRadius), _mat_prt(m){}
	sphere(vec3 center, float fRadius) :_center(center), _fRadius(fRadius){}
	~sphere(){}

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;

	vec3 _center;
	float _fRadius;
	material *_mat_prt;
};

bool sphere::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	vec3 oc = r.origin() - _center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - _fRadius*_fRadius;
	float fDis = b*b - a*c;

	if (fDis > 0)
	{
		float temp = (-b - sqrt(fDis)) / a;
		if (temp<f_max&&temp>f_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - _center) / _fRadius;
			rec.mat_prt = _mat_prt;
			return true;
		}

		temp = (-b + sqrt(fDis)) / a;
		if (temp<f_max&&temp>f_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - _center) / _fRadius;
			rec.mat_prt = _mat_prt;
			return true;
		}
	}

	return false;
}

#endif


