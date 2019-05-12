#include "raytracing/mapping/LightProbe.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

void
LightProbe::GetTexelCoordinates(const Point3D& hit_point, int xres, int yres, int& row, int& column) const
{
	float alpha;

	// compute
	float x = hit_point.x, y = hit_point.y, z = hit_point.z;

    if (m_map_type == MapType::Panoramic) {
        alpha = acos(-z);
    } else if (m_map_type == MapType::Regular) {
        alpha = acos(z);
    }

	float d = sqrt(x * x + y * y);
	float sin_beta = y / d;
	float cos_beta = x / d;

	// map theta and phi to u, v (range is 0 to 1)

	float r = static_cast<float>(alpha * INV_PI);
	float u = (1.0f + r * cos_beta) * 0.5f;
	float v = (1.0f + r * sin_beta) * 0.5f;

	// map u and v to texel coordinates

	column = (int) ((xres - 1) * u);
	row = (int) ((yres - 1) * v);
}

}