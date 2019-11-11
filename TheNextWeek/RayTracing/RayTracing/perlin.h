#ifndef _PERLINE_HEAD_
#define _PERLINE_HEAD_

inline float trilinear_interp(float c[2][2][2], float u, float v, float w)
{
	float accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
		accum += (i*u + (1 - i)*(1 - u))*
			(j*v + (1 - j)*(1 - v))*
			(k*w + (1 - k)*(1 - w))*c[i][j][k];
			}

	return accum;
}

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w)
{
	float uu = u*u*(3 - 2 * u);
	float vv = v*v*(3 - 2 * v);
	float ww = w*w*(3 - 2 * w);

	float accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
		vec3 width_v(u - i, v - j, w - k);
		accum += (i*uu + (1 - i)*(1 - uu))*
			(j*vv + (1 - j)*(1 - vv))*
			(k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k], width_v);
			}

	return accum;
}

class perlin
{
public:
	float noise1(const vec3 &p)const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		if (i < 0)i = 0;
		if (j < 0)j = 0;
		if (k < 0)k = 0;

		int nix = _s_perm_x[i] ^ _s_perm_y[j] ^ _s_perm_z[k];

		return _s_ranfloat[nix];
	}

	float noise2(const vec3 &p)const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		if (i < 0)i = 0;
		if (j < 0)j = 0;
		if (k < 0)k = 0;

		float c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
				{
					int nix = _s_perm_x[(i + di) & 255] ^ _s_perm_y[(j + dj) & 255] ^ _s_perm_z[(k + dk) & 255];
					c[di][dj][dk]=_s_ranfloat[nix];
				}

		return trilinear_interp(c, u, v, w);
	}

	float noise3(const vec3 &p)const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		u = u*u*(3 - 2 * u);
		v = v*v*(3 - 2 * v);
		w = w*w*(3 - 2 * w);

		if (i < 0)i = 0;
		if (j < 0)j = 0;
		if (k < 0)k = 0;

		float c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
				{
			int nix = _s_perm_x[(i + di) & 255] ^ _s_perm_y[(j + dj) & 255] ^ _s_perm_z[(k + dk) & 255];
			c[di][dj][dk] = _s_ranfloat[nix];
				}

		return trilinear_interp(c, u, v, w);
	}

	float noise(const vec3 &p)const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		if (i < 0)i = 0;
		if (j < 0)j = 0;
		if (k < 0)k = 0;

		vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
				{
			int nix = _s_perm_x[(i + di) & 255] ^ _s_perm_y[(j + dj) & 255] ^ _s_perm_z[(k + dk) & 255];
			c[di][dj][dk] = _s_ranVec[nix];
				}

		return perlin_interp(c, u, v, w);
	}

	float turb(const vec3 &p, int depth = 7) const
	{
		float accum = 0;
		vec3 temp_p = p;
		float weight = 1.0;

		for (int i = 0; i < depth; i++)
		{
			accum += weight*noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}

		return fabs(accum);
	}

public:
	static float *_s_ranfloat;
	static vec3 *_s_ranVec;
	static int *_s_perm_x;
	static int *_s_perm_y;
	static int *_s_perm_z;
};


static vec3 *perlin_generatev3()
{
	vec3 *p = new vec3[256];
	for (int i = 0; i < 256; ++i)
	{
		double dx = 2 * random_double() - 1;
		double dy = 2 * random_double() - 1;
		double dz = 2 * random_double() - 1;
		p[i] = unit_vector(vec3(dx, dy, dz));
	}
	return p;
}

static float *perlin_generate()
{
	float *p = new float[256];
	for (int i = 0; i < 256; ++i)
	{
		p[i] = random_double();
	}
	return p;
}

void permute(int *p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int target = int(random_double()*(i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}

	return;
}

static int* perlin_generate_permute()
{
	int *p = new int[256];
	for (int i = 0; i < 256; i++)
	{
		p[i] = i;
	}

	permute(p, 256);
	return p;
}

float *perlin::_s_ranfloat = perlin_generate();
vec3 *perlin::_s_ranVec = perlin_generatev3();
int *perlin::_s_perm_x = perlin_generate_permute();
int *perlin::_s_perm_y = perlin_generate_permute();
int *perlin::_s_perm_z = perlin_generate_permute();

#endif
