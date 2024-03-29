

#ifndef _RANDOM_HEAD_
#define _RANDOM_HEAD_

#include <functional>
#include <random>

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator = std::bind(distribution, generator);

	return rand_generator();
}

vec3 random_in_unit_sphere()
{
	vec3 p, unitP(1.0, 1.0, 1.0);
	do
	{
		p = 2.0*vec3(random_double(), random_double(), random_double()) - unitP;
	} while (p.squared_length() >= 1.0);

	return p;
}

#endif