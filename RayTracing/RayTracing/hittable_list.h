

#ifndef _HITTABLE_LIST_HEAD_
#define _HITTABLE_LIST_HEAD_

#include "hittable.h"

class hittable_list:public hittable
{
public:
	hittable_list(hittable **list, int n){ _list = list; _list_size = n; }
	~hittable_list(){};

	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;

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



#endif
