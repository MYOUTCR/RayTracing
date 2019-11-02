
#ifndef _MATERIAL_HEAD_
#define _MATERIAL_HEAD_

#include "ray.h"
#include "hittable.h"
#include "random.h"

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0*r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}


bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float fDis = 1.0 - ni_over_nt*ni_over_nt*(1.0 - dt*dt);
	if (fDis > 0)
	{
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(fDis);
		return true;
	}
	else
	{
		return false;
	}
}

vec3 reflect(const vec3 &v, const vec3 &n)
{
	return v - 2 * dot(v, n)*n;
}

class material
{
public:
	material(){}
	~material(){}

	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const = 0;
private:

};

class lambertian :public material
{
public:
	lambertian(vec3 albedo) :_albedo(albedo){}

	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = _albedo;
		return true;
	}

	vec3 _albedo;
};



class metal :public material
{
public:
	metal(vec3 albedo) :_albedo(albedo), _fuzz(0){}
	metal(vec3 albedo, float fz) :_albedo(albedo)
	{
		_fuzz = (fz < 1) ? fz : 1;
	}

	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + _fuzz*random_in_unit_sphere());
		attenuation = _albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 _albedo;
	float _fuzz;
};


class dielectric :public material
{
public:
	dielectric(float ri) :_ref_idx(ri){};

	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const
	{
		vec3 outWard_normal;
		vec3 relected = reflect(r_in.direction(), rec.normal);

		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;

		float refract_prob;
		float cosine;

		if (dot(r_in.direction(), rec.normal) > 0)
		{
			outWard_normal = -rec.normal;
			ni_over_nt = _ref_idx;
			//cosine = _ref_idx*dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = sqrt(1 - _ref_idx*_ref_idx*(1 - cosine*cosine));
		}
		else
		{
			outWard_normal = rec.normal;
			ni_over_nt = 1.0 / _ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		if (refract(r_in.direction(), outWard_normal, ni_over_nt, refracted))
		{
			refract_prob = schlick(cosine, _ref_idx);
		}
		else
		{
			refract_prob = 1.0;
		}

		if (random_double() < refract_prob)
		{
			scattered = ray(rec.p, relected);
		}
		else
		{
			scattered = ray(rec.p, refracted);
		}

		return true;
	}

	float _ref_idx;

};


#endif