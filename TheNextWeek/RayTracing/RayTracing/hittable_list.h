

#ifndef _HITTABLE_LIST_HEAD_
#define _HITTABLE_LIST_HEAD_

#include "aabb.h"
#include "hittable.h"

class hittable_list:public hittable
{
public:
	hittable_list(hittable **list, int n){ _list = list; _list_size = n; }
	~hittable_list(){};

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box)const;
	 
	hittable **_list;
	int _list_size;
};

bool hittable_list::hit(const ray &r, float f_min, float f_max, hit_record &rec)const
{
	hit_record temp_rec;
	bool hit_anyting = false;
	double closest_so_fa = f_max;

	for (int i = 0; i < _list_size; i++)
	{
		if (_list[i]->hit(r, f_min, closest_so_fa, temp_rec))
		{
			hit_anyting = true;
			closest_so_fa = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anyting;
}

bool hittable_list::bounding_box(float t0, float t1, aabb &box) const
{
	if (_list_size < 1)return false;

	aabb temp_box;
	bool first_ture = _list[0]->bounding_box(t0, t1, temp_box);
	if (!first_ture)
		return false;
	else
		box = temp_box;

	for (int i = 1; i < _list_size; i++)
	{
		if (_list[i]->bounding_box(t0, t1, temp_box))
		{
			box = surrounding_box(temp_box, box);
		}
		else
			return false;
	}

	return true;
}



#endif
