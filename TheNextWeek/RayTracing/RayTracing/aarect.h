

#ifndef _AARECT_HEAD_
#define _AARECT_HEAD_
#include "hittable.h"

class xy_rect :public hittable
{
public:
	xy_rect(){}
	xy_rect(float x0,float x1,float y0,float y1,float k,material *mat):
		_x0(x0), _x1(x1), _y0(y0), _y1(y1), _k(k), _mp(mat){}
	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{
		box = aabb(vec3(_x0, _y0, _k - 0.0001), vec3(_x1, _y1, _k + 0.0001));
		return true;
	}

private:
	material *_mp;
	float _x0, _x1, _y0, _y1, _k;
};

bool xy_rect::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	float t = (_k - r.origin().z()) / r.direction().z();

	if (t<f_min || t>f_max)
		return false;

	float x = r.origin().x() + t*r.direction().x();
	float y = r.origin().y() + t*r.direction().y();

	if (x < _x0 || x>_x1 || y<_y0 || y>_y1)
		return false;
	rec.u = (x - _x0) / (_x1 - _x0);
	rec.v = (y - _y0) / (_y1 - _y0);
	rec.t = t;
	rec.mat_ptr = _mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 0, 1);

	return true;
}

class xz_rect :public hittable
{
public:
	xz_rect(){}
	xz_rect(float x0, float x1, float z0, float z1, float k, material *mat) :
		_x0(x0), _x1(x1), _z0(z0), _z1(z1), _k(k), _mp(mat){}
	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{
		box = aabb(vec3(_x0, _k - 0.0001, _z0), vec3(_x1, _k + 0.0001, _z1));
		return true;
	}

private:
	material *_mp;
	float _x0, _x1, _z0, _z1, _k;
};

bool xz_rect::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	float t = (_k - r.origin().y()) / r.direction().y();

	if (t<f_min || t>f_max)
		return false;

	float x = r.origin().x() + t*r.direction().x();
	float z = r.origin().z() + t*r.direction().z();

	if (x < _x0 || x>_x1 || z<_z0 || z>_z1)
		return false;
	rec.u = (x - _x0) / (_x1 - _x0);
	rec.v = (z - _z0) / (_z1 - _z0);
	rec.t = t;
	rec.mat_ptr = _mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 1, 0);

	return true;
}

class yz_rect :public hittable
{
public:
	yz_rect(){}
	yz_rect(float y0, float y1, float z0, float z1, float k, material *mat) :
		_y0(y0), _y1(y1), _z0(z0), _z1(z1), _k(k), _mp(mat){}
	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{
		box = aabb(vec3(_k - 0.0001, _y0, _z0), vec3(_k + 0.0001, _y1, _z1));
		return true;
	}

private:
	material *_mp;
	float _y0, _y1, _z0, _z1, _k;
};

bool yz_rect::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	float t = (_k - r.origin().x()) / r.direction().x();

	if (t<f_min || t>f_max)
		return false;

	float y = r.origin().y() + t*r.direction().y();
	float z = r.origin().z() + t*r.direction().z();

	if (y < _y0 || y>_y1 || z<_z0 || z>_z1)
		return false;
	rec.u = (y - _y0) / (_y1 - _y0);
	rec.v = (z - _z0) / (_z1 - _z0);
	rec.t = t;
	rec.mat_ptr = _mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(1, 0, 0);

	return true;
}

class flip_normals :public hittable
{
public:
	flip_normals(hittable *p) :_ptr(p){}
	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const
	{
		if (_ptr->hit(r, f_min, f_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{
		return _ptr->bounding_box(t0, t1, box);
	}

private:
	hittable *_ptr;
};


class translate :public hittable
{
public:
	translate(hittable *p, const vec3 &displacement)
		:_ptr(p),
		_offset(displacement){}
	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const
	{
		ray moved_r(r.origin() - _offset, r.direction(), r.time());
		if (_ptr->hit(moved_r, f_min, f_max, rec))
		{
			rec.p += _offset;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{
		if (_ptr->bounding_box(t0, t1, box))
		{
			box = aabb(box.min() + _offset, box.max() + _offset);
			
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	hittable *_ptr;
	vec3 _offset;

};

class rotate_y :public hittable
{
public:
	rotate_y(hittable *p, float angle)
		:_ptr(p)
	{
		float radians = (_M_PI_ / 180.0)*angle;
		_sin_theta = sin(radians);
		_cos_theta = cos(radians);
		_hasBox = _ptr->bounding_box(0, 1, _bbox);

		vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
		vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
				{
					float x = i*_bbox.max().x() + (1 - i)*_bbox.min().x();
					float y = j*_bbox.max().y() + (1 - j)*_bbox.min().y();
					float z = k*_bbox.max().z() + (1 - k)*_bbox.min().z();

					float newx = _cos_theta*x + _sin_theta*z;
					float newz = -_sin_theta*x + _cos_theta*z;

					vec3 tester(newx, y, newz);

					for (int c = 0; c < 3; c++)
					{
						if (tester[c]>max[c])
							max[c] = tester[c];
						if (tester[c]<min[c])
							min[c] = tester[c];
					}
				}

		_bbox = aabb(min, max);
	}

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const
	{
		vec3 origin = r.origin();
		vec3 direction = r.direction();

		origin[0] = _cos_theta*origin[0] - _sin_theta*origin[2];
		origin[2] = _sin_theta*origin[0] + _cos_theta*origin[2];

		direction[0] = _cos_theta*direction[0] - _sin_theta*direction[2];
		direction[2] = _sin_theta*direction[0] + _cos_theta*direction[2];

		ray rotated_r(origin, direction, r.time());

		if (_ptr->hit(rotated_r, f_min, f_max, rec))
		{
			vec3 p = rec.p;
			vec3 normal = rec.normal;
			p[0] = _cos_theta*p[0] + _sin_theta*p[2];
			p[2] = -_sin_theta*p[0] + _cos_theta*p[2];

			normal[0] = _cos_theta*normal[0] + _sin_theta*normal[2];
			normal[2] = -_sin_theta*normal[0] + _cos_theta*normal[2];

			rec.p = p;
			rec.normal = normal;

			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{
		box = _bbox;
		return _hasBox;
	}
private:
	hittable *_ptr;
	float _sin_theta;
	float _cos_theta;
	bool _hasBox;
	aabb _bbox;
};

#endif