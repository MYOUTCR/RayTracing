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

class perlin
{
public:
	float noise(const vec3 &p)const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		float c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
				{
				}

		return trilinear_interp(c, u, v, w);

	}

	static float *_s_ranfloat;
	static int *_s_perm_x;
	static int *_s_perm_y;
	static int *_s_perm_z;
};

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
int *perlin::_s_perm_x = perlin_generate_permute();
int *perlin::_s_perm_y = perlin_generate_permute();
int *perlin::_s_perm_z = perlin_generate_permute();


#endif
