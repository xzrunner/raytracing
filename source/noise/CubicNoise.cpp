#include "raytracing/noise/CubicNoise.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/maths.h"

namespace rt
{

CubicNoise::CubicNoise(int octaves)
    : LatticeNoise(octaves)
{
}

CubicNoise::CubicNoise(int octaves, float lacunarity, float gain)
    : LatticeNoise(octaves, lacunarity, gain)
{
}

float CubicNoise::value_noise(const Point3D& p) const
{
	int 	ix, iy, iz;
    float 	fx, fy, fz;
    float 	xknots[4], yknots[4], zknots[4];

    ix = static_cast<int>(floor(p.x));
    fx = p.x - ix;

    iy = static_cast<int>(floor(p.y));
    fy = p.y - iy;

	iz = static_cast<int>(floor(p.z));
    fz = p.z - iz;

    for (int k = -1; k <= 2; k++) {
        for (int j = -1; j <= 2; j++) {
            for (int i = -1; i <= 2; i++) {
                xknots[i+1] = value_table[INDEX(ix + i, iy + j, iz + k)];
            }
            yknots[j+1] = four_knot_spline(fx, xknots);
        }
        zknots[k+1] = four_knot_spline(fy, yknots);
    }

    return static_cast<float>(clamp(four_knot_spline(fz, zknots), -1.0, 1.0));
}

Vector3D CubicNoise::vector_noise(const Point3D& p) const
{
	int 		ix, iy, iz;
    float 		fx, fy, fz;
    Vector3D 	xknots[4], yknots[4], zknots[4];

    ix = static_cast<int>(floor(p.x));
    fx = p.x - ix;

    iy = static_cast<int>(floor(p.y));
    fy = p.y - iy;

	iz = static_cast<int>(floor(p.z));
    fz = p.z - iz;

    for (int k = -1; k <= 2; k++) {
        for (int j = -1; j <= 2; j++) {
            for (int i = -1; i <= 2; i++) {
                xknots[i+1] = vector_table[INDEX(ix + i, iy + j, iz + k)];
            }
             yknots[j+1] = four_knot_spline(fx, xknots);
        }
     	zknots[k+1] = four_knot_spline(fy, yknots);
    }

	return (four_knot_spline(fz, zknots));
}

}