#pragma once

#ifndef _SPHERE_HEAD_
#define _SPHERE_HEAD_

#include "hittable.h"
#include "material.h"
#include "aabb.h"

class sphere :public hittable
{
public:
	sphere(){}
	sphere(vec3 center, float fRadius, material *m) :_center(center), _fRadius(fRadius), _mat_ptr(m){}
	sphere(vec3 center, float fRadius) :_center(center), _fRadius(fRadius){}
	~sphere(){}

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb&box)const;

	vec3 _center;
	float _fRadius;
	material *_mat_ptr;
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
			get_shpere_uv((rec.p - _center) / _fRadius, rec.u, rec.v);

			rec.normal = (rec.p - _center) / _fRadius;		
			rec.mat_ptr = _mat_ptr;
			return true;
		}

		temp = (-b + sqrt(fDis)) / a;
		if (temp<f_max&&temp>f_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			get_shpere_uv((rec.p - _center) / _fRadius, rec.u, rec.v);
			rec.normal = (rec.p - _center) / _fRadius;
			rec.mat_ptr = _mat_ptr;
			return true;
		}
	}

	return false;
}

bool sphere::bounding_box(float t0, float t1, aabb&box)const
{
	box = aabb(_center - vec3(_fRadius, _fRadius, _fRadius), _center + vec3(_fRadius, _fRadius, _fRadius));
	return true;
}

class moving_sphere:public hittable
{
public:
	moving_sphere(){}
	moving_sphere(const vec3 &c0,const vec3 &c1, float t0,float t1, material *m)
		:_center0(c0), _center1(c1), _time0(t0), _time1(t1), _mat_ptr(m){}
	moving_sphere(const vec3 &c0, const vec3 &c1, float t0, float t1,float r, material *m)
		:_center0(c0), _center1(c1), _time0(t0), _time1(t1),_fRadius(r), _mat_ptr(m){}
	~moving_sphere(){}

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb&box)const;
	vec3 center(float time) const;
private:
	vec3 _center0, _center1;
	float _fRadius;
	float _time0, _time1;
	material *_mat_ptr;

};

vec3 moving_sphere::center(float time) const
{
	return _center0 + ((time - _time0) / (_time1 - _time0))*(_center1 - _center0);
}

bool moving_sphere::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	vec3 oc = r.origin() - center(r.time());
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
			rec.normal = (rec.p - center(r.time())) / _fRadius;
			rec.mat_ptr = _mat_ptr;
			return true;
		}

		temp = (-b + sqrt(fDis)) / a;
		if (temp<f_max&&temp>f_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / _fRadius;
			rec.mat_ptr = _mat_ptr;
			return true;
		}
	}

	return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb&box)const
{
	vec3 c0 = center(t0);
	vec3 c1 = center(t1);

	aabb box0 = aabb(c0 - vec3(_fRadius, _fRadius, _fRadius), c0 + vec3(_fRadius, _fRadius, _fRadius));
	aabb box1 = aabb(c1 - vec3(_fRadius, _fRadius, _fRadius), c1 + vec3(_fRadius, _fRadius, _fRadius));

	box = surrounding_box(box0, box1);

	return true;
}

#endif


