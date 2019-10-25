

#ifndef _LAMBERTIAN_HEAD_
#define _LAMBERTIAN_HEAD_

#include "material.h"
#include "random.h"

class lambertian :public material
{
public:
	lambertian(vec3 albedo) :_albedo(albedo){}

	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const 
	{
		vec3 target = rec.p + rec.normal + random_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = _albedo;
		return true;
	}

	vec3 _albedo;
};

vec3 reflect(const vec3 &v,const vec3 &n)
{
	return v - 2 * dot(v, n)*n;
}

class metal :public material
{
public:
	metal(vec3 albedo) :_albedo(albedo){}

	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = _albedo;

		return (dot(scattered.direction(), rec.normal)>0);
	}

	vec3 _albedo;
};


#endif