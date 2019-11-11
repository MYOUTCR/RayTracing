
#ifndef _CONSTANT_MEDIUM_
#define _CONSTANT_MEDIUM_

#include "hittable.h"
#include "texture.h"
#include "material.h"

class isotropic :public material
{
public:
	isotropic(texture *tx) :_albedo(tx){}
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const
	{
		scattered = ray(rec.p, random_in_unit_sphere());
		attenuation = _albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
private:
	texture *_albedo;
};

class constant_medium :public hittable
{
public:
	constant_medium(hittable *b, float d, texture *tx)
		:_boundary(b)
		, _density(d)
	{
		_phase_function = new isotropic(tx);
	}

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;

	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{	
		return _boundary->bounding_box(t0, t1, box);
	}

private:
	hittable *_boundary;
	float _density;
	material *_phase_function;
};

bool constant_medium::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	const bool enableDebug = false;
	bool debugin = enableDebug&&random_double() < 0.00001;

	hit_record rec1, rec2;
	if (_boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if (_boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2))
		{
			if (debugin) std::cerr << "\nf_min f_max " << rec1.t << " " << rec2.t << '\n';
		
			if (rec1.t < f_min)
				rec1.t = f_min;

			if (rec2.t > f_max)
				rec2.t = f_max;

			if (rec1.t >= rec2.t)
				return false;

			if (rec1.t < 0)
				rec1.t = 0;

			float fd_in_b = (rec2.t - rec1.t)*r.direction().length();
			float hit_distance = -(1 / _density)*log(random_double());

			if (hit_distance < fd_in_b)
			{
				rec.t = rec1.t + hit_distance / r.direction().length();
				rec.p = r.point_at_parameter(rec.t);

				if (debugin)
				{
					std::cerr << "hit_distance=" << hit_distance << '\n'
						<< "rec.t=" << rec.t << '\n';
						//<< "rec.p=" << rec.p << '\n'
				}

				rec.normal = vec3(1, 0, 0);
				rec.mat_ptr = _phase_function;

				return true;
			}
		}
	}

	return false;

}

#endif
