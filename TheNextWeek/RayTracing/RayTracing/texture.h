

#ifndef _TEXTUEW_HEAD_
#define _TEXTUEW_HEAD_
#include "vec3.h"
#include "perlin.h"

class texture
{
public:
	virtual vec3 value(float u, float v, const vec3 &p) const=0;
};

 class constant_texture :public texture
 {
 public:
 	constant_texture(){}
 	constant_texture(vec3 c) :_color(c){}
 	virtual vec3 value(float u, float v, const vec3 &p) const
 	{
 		return _color;
 	}
 private:
 	vec3 _color;
 };

class checker_texture :public texture
{
public:
	checker_texture(){}
	checker_texture(texture *t0, texture *t1) :even(t0), _odd(t1){}

	virtual vec3 value(float u, float v, const vec3 &p) const
	{
		float sines = sin(10 * p.x())* sin(10 * p.y())* sin(10 * p.z());
		if (sines < 0)
		{
			return _odd->value(u, v, p);
		}
		else
		{
			return even->value(u, v, p);
		}
	}

	texture *_odd;
	texture *even;
};

class noise_texture :public texture
{
public:
	noise_texture(){}
	noise_texture(float fs) :_fScale(fs){}
	virtual vec3 value(float u, float v, const vec3 &p) const
	{
		//return vec3(1.0, 1.0, 1.0)*_noise.noise(p);
		//return vec3(1.0, 1.0, 1.0)*_noise.noise(_fScale*p);
		//return vec3(1.0, 1.0, 1.0)*0.5*(1 + sin(_fScale*p.z()+10*_noise.turb(p)));
		return vec3(1, 1, 1)*0.5*(1 + sin(_fScale*p.x() + 5 * _noise.turb(_fScale*p)));
	}

private:
	perlin _noise;
	float _fScale;
};

class image_texture :public texture
{
public:
	image_texture(){}
	image_texture(unsigned char * pixels, int a, int b) :_data(pixels), _nx(a), _ny(b){}
	virtual vec3 value(float u, float v, const vec3 &p) const;
private:
	unsigned char *_data;
	int _nx, _ny;
};

vec3 image_texture::value(float u, float v, const vec3 &p) const
{
	int i = u*_nx;
	int j = (1 - v)*_ny - 0.001;
	if (i < 0)i = 0;
	if (j < 0)j = 0;
	if (i > _nx - 1)i = _nx - 1;
	if (j > _ny - 1)j = _ny - 1;

	float r = int(_data[3 * i + 3 * _nx*j]) / 255.0;
	float g = int(_data[3 * i + 3 * _nx*j+1]) / 255.0;
	float b = int(_data[3 * i + 3 * _nx*j+2]) / 255.0;

	return vec3(r, g, b);
}

#endif
