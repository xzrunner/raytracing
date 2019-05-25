#include "raytracing/noise/LinearNoise.h"
#include "raytracing/maths/Point3D.h"

namespace rt
{

LinearNoise::LinearNoise(int octaves, float lacunarity, float gain)
    : LatticeNoise(octaves, lacunarity, gain)
{
}

float LinearNoise::value_noise(const Point3D& p) const
{
    int 	ix, iy, iz;
    float 	fx, fy, fz;
    float 	d[2][2][2];
    float 	x0, x1, x2, x3, y0, y1, z0;

    ix = static_cast<int>(floor(p.x));
    fx = p.x - ix;

    iy = static_cast<int>(floor(p.y));
    fy = p.y - iy;

	iz = static_cast<int>(floor(p.z));
    fz = p.z - iz;

    for (int k = 0; k <= 1; k++)
        for (int j = 0; j <= 1; j++)
            for (int i = 0; i <= 1; i++)
                d[k][j][i] = value_table[INDEX(ix + i, iy + j, iz + k)];

    x0 = lerp(fx, d[0][0][0], d[0][0][1]);
    x1 = lerp(fx, d[0][1][0], d[0][1][1]);
    x2 = lerp(fx, d[1][0][0], d[1][0][1]);
    x3 = lerp(fx, d[1][1][0], d[1][1][1]);
    y0 = lerp(fy, x0, x1);
    y1 = lerp(fy, x2, x3);
    z0 = lerp(fz, y0, y1);

    return (z0);
}

Vector3D LinearNoise::vector_noise(const Point3D& p) const
{
	int 		ix, iy, iz;
    float 		fx, fy, fz;
    Vector3D	d[2][2][2];
    Vector3D 	x0, x1, x2, x3, y0, y1, z0;

    ix = static_cast<int>(floor(p.x));
    fx = p.x - ix;

    iy = static_cast<int>(floor(p.y));
    fy = p.y - iy;

	iz = static_cast<int>(floor(p.z));
    fz = p.z - iz;

    for (int k = 0; k <= 1; k++)
        for (int j = 0; j <= 1; j++)
            for (int i = 0; i <= 1; i++)
                d[k][j][i] = vector_table[INDEX(ix + i, iy + j, iz + k)];

    x0 = lerp(fx, d[0][0][0], d[0][0][1]);
    x1 = lerp(fx, d[0][1][0], d[0][1][1]);
    x2 = lerp(fx, d[1][0][0], d[1][0][1]);
    x3 = lerp(fx, d[1][1][0], d[1][1][1]);
    y0 = lerp(fy, x0, x1);
    y1 = lerp(fy, x2, x3);
    z0 = lerp(fz, y0, y1);

    return (z0);
}

}