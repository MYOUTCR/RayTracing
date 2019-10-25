
#ifndef _MATERIAL_HEAD_
#define _MATERIAL_HEAD_

#include "ray.h"
#include "hittable.h"

class material
{
public:
	material(){}
	~material(){}

	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray& scattered) const = 0;
private:

};



#endif