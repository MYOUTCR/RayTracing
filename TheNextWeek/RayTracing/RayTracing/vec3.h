#pragma once

#ifndef _VEC3_HEAD_
#define _VEC3_HEAD_

#include <math.h>
#include <iostream>

class vec3
{
public:
	vec3(){ _v[0] = 0.0f; _v[1] = 0.0f; _v[2] = 0.0f; }
	vec3(float fv0, float fv1, float fv2){ _v[0] = fv0; _v[1] = fv1; _v[2] = fv2; }
	~vec3(){};

	inline float x()const { return _v[0]; }
	inline float y()const { return _v[1]; }
	inline float z()const { return _v[2]; }

	inline float r()const { return _v[0]; }
	inline float g()const { return _v[1]; }
	inline float b()const { return _v[2]; }

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-()const { return vec3(-_v[0], -_v[1], -_v[2]); }
	inline float operator[](int i)const { return _v[i]; }
	inline float& operator[](int i) { return _v[i]; }

	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator-=(const vec3 &v2);
	inline vec3& operator*=(const vec3 &v2);
	inline vec3& operator/=(const vec3 &v2);
	inline vec3& operator*=(const float ft);
	inline vec3& operator/=(const float ft);

	inline float length()const
	{
		return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
	}

	inline float squared_length()const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2];
	}

	inline void make_unit_vector();

	float _v[3];

};


inline std::istream& operator>>(std::istream &is, vec3 &t)
{
	is >> t[0] >> t[1] >> t[2];
	return is;
}

inline std::ostream& operator>>(std::ostream &os, vec3 &t)
{
	os << t[0] <<" "<< t[1] <<" "<< t[2];
	return os;
}

inline void vec3::make_unit_vector()
{
	float fk = 1.0 / sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
	_v[0] *= fk;
	_v[1] *= fk;
	_v[2] *= fk;

}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
	return vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline vec3 operator-(const vec3&v1, const vec3 &v2)
{
	return vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline vec3 operator*(const vec3&v1, const vec3 &v2)
{
	return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline vec3 operator/(const vec3&v1, const vec3 &v2)
{
	return vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

inline vec3 operator*(float t, const vec3 &v)
{
	return vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline vec3 operator/(const vec3 v,float t)
{
	return vec3(v[0] / t, v[1] / t, v[2] / t);
}

inline vec3 operator*(const vec3 &v, float t)
{
	return vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline float dot(const vec3&v1, const vec3 &v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline vec3 cross(const vec3&v1, const vec3 &v2)
{
	return vec3((v1[1] * v2[2] - v1[2] * v2[1]),
		(-(v1[0] * v2[2] - v1[2] * v2[0])),
		(v1[0] * v2[1] - v1[1] * v2[0]));
}

inline vec3& vec3::operator+=(const vec3 &v)
{
	_v[0] += v[0];
	_v[1] += v[1];
	_v[2] += v[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3 &v)
{
	_v[0] -= v[0];
	_v[1] -= v[1];
	_v[2] -= v[2];
	return *this;
}
inline vec3& vec3::operator*=(const vec3 &v)
{
	_v[0] *= v[0];
	_v[1] *= v[1];
	_v[2] *= v[2];
	return *this;
}
inline vec3&  vec3::operator/=(const vec3 &v)
{
	_v[0] /= v[0];
	_v[1] /= v[1];
	_v[2] /= v[2];
	return *this;
}
inline vec3& vec3::operator*=(const float t)
{
	_v[0] *= t;
	_v[1] *= t;
	_v[2] *= t;
	return *this;
}
inline vec3& vec3::operator/=(const float t)
{
	float fk = 1.0 / t;
	_v[0] *= fk;
	_v[1] *= fk;
	_v[2] *= fk;
	return *this;
}

inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}


#endif


