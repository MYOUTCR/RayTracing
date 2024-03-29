#ifndef _BVH_HEAD_
#define _BVH_HEAD_

#include "hittable.h"
#include "aabb.h"
#include "random.h"

int box_x_compare(const void *a, const void *b)
{
	aabb box_left, box_right;

	hittable *ah = *(hittable**)a;
	hittable *bh = *(hittable**)b;

	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";

	if (box_left.min().x() - box_right.min().x() < 0.0)
		return -1;
	else
		return 1;

}

int box_y_compare(const void *a, const void *b)
{
	aabb box_left, box_right;

	hittable *ah = *(hittable**)a;
	hittable *bh = *(hittable**)b;

	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";

	if (box_left.min().y() - box_right.min().y() < 0.0)
		return -1;
	else
		return 1;
}

int box_z_compare(const void *a, const void *b)
{
	aabb box_left, box_right;

	hittable *ah = *(hittable**)a;
	hittable *bh = *(hittable**)b;

	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";

	if (box_left.min().z() - box_right.min().z() < 0.0)
		return -1;
	else
		return 1;
}

class bvh_node:public hittable
{
public:
	bvh_node(){}
	bvh_node(hittable **list, int n, float time0, float time1);
	virtual bool hit(const ray &r, float f_min, float f_max, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box)const;
private:
	hittable *_left;
	hittable *_right;
	aabb _box;
};

bvh_node::bvh_node(hittable **list, int n, float time0, float time1)
{
	int axis = int(3 * random_double());

	if (axis == 0)
	{
		qsort(list, n, sizeof(hittable*), box_x_compare);
	}
	else if (axis == 1)
	{
		qsort(list, n, sizeof(hittable*), box_y_compare);
	}
	else
	{
		qsort(list, n, sizeof(hittable*), box_z_compare);
	}

	if (1 == n)
	{
		_left = _right = list[0];
	}
	else if (n == 2)
	{
		_left = list[0];
		_right = list[1];
	}
	else
	{
		_left = new bvh_node(list, n / 2, time0, time1);
		_right = new bvh_node(list + n / 2, n - n / 2, time0, time1);

	}

	aabb  box_left, box_right;
	if (!_left->bounding_box(time0, time1, box_left) || !_right->bounding_box(time0, time1, box_right))
	{
		std::cerr << "no bounding box in bvh_node constructor\n";
	}

	_box = surrounding_box(box_left, box_right);

}

bool bvh_node::hit(const ray &r, float t_min, float t_max, hit_record &rec)const
{
	if (_box.hit(r, t_min, t_max))
	{
		hit_record left_rec, right_rec;
		bool hit_left = _left->hit(r, t_min, t_max, left_rec);
		bool hit_right = _right->hit(r, t_min, t_max, right_rec);
		if (hit_left&&hit_right)
		{
			if (left_rec.t < right_rec.t)
				rec = left_rec;
			else
				rec = right_rec;

			return true;
		}
		else if (hit_left)
		{
			rec = left_rec;
			return true;
		}
		else if (hit_right)
		{
			rec = right_rec;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb &box)const
{
	box = _box;
	return true;
}

#endif
