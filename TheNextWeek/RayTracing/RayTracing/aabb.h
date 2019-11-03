
#ifndef _AABB_HEAD_
#define _AABB_HEAD_

#include "vec3.h"
#include "hittable_list.h"


inline float ffmin(float a, float b){ return a < b ? a : b; }
inline float ffmax(float a, float b){ return a > b ? a : b; }

class aabb
{
public:
	aabb(){};
	aabb(const vec3 &min, const vec3 &max){ _min = min, _max = max; }

	vec3 min()const { return _min; }
	vec3 max()const { return _max; }

	bool hit(const ray &r, float tmin, float tmax)const
	{
// 		for (int a = 0; a < 3; a++)
// 		{
// 			float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
// 			float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
// 			tmin = ffmax(t0, tmin);
// 			tmax = ffmax(t1, tmax);
// 			if (tmax <= tmin)
// 				return false;
// 		}
// 
// 		return true;

		for (int a = 0; a < 3; a++)
		{
			float invD = 1.0f / r.direction()[a];
			float t0 = (_min[a] - r.origin()[a])*invD;
			float t1 = (_max[a] - r.origin()[a])*invD;

			if (invD < 0)
				std::swap(t0, t1);

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin)
				return false;
		}

		return true;
	}

	~aabb(){};

private:
	vec3 _min;
	vec3 _max;
};

aabb surrounding_box(const aabb &box0, const aabb &box1)
{
	vec3 small, big;
	small[0] = ffmin(box0.min().x(), box1.min().x());
	small[1] = ffmin(box0.min().y(), box1.min().y());
	small[2] = ffmin(box0.min().z(), box1.min().z());

	big[0] = ffmax(box0.min().x(), box1.min().x());
	big[1] = ffmax(box0.min().y(), box1.min().y());
	big[2] = ffmax(box0.min().z(), box1.min().z());

	return aabb(small, big);
}



#endif