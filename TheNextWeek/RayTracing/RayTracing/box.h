
#ifndef _BOX_HEAD_
#define _BOX_HEAD_

#include "hittable_list.h"
#include "hittable.h"
#include "aarect.h"
class box :public hittable
{
public:
	box(){}
	box(const vec3&p0, const vec3 &p1, material *ptr);
	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box)const
	{
		box = aabb(_pMin, _pMax);
		return true;
	}
private:
	vec3 _pMin, _pMax;
	hittable *_list_ptr;
};

box::box(const vec3&p0, const vec3 &p1, material *ptr)
{
	_pMin = p0;
	_pMax = p1;

	hittable **list = new hittable*[6];
	list[0] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
	list[1] = new flip_normals(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
	list[2] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
	list[3] = new flip_normals(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
	list[4] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
	list[5] = new flip_normals(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));

	_list_ptr = new hittable_list(list, 6);
}

bool box::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	return _list_ptr->hit(r, f_min, f_max, rec);
}

#endif