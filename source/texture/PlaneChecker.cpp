#include "raytracing/texture/PlaneChecker.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

PlaneChecker::PlaneChecker()
    : m_outline_width(0)
    , m_color1(1.0f, 1.0f, 1.0f)
    , m_color2(0.5f, 0.5f, 0.5f)
    , m_outline_color(0, 0, 0)
    , m_size(0)
{
}

RGBColor PlaneChecker::GetColor(const ShadeRec& sr) const
{
	float x = sr.local_hit_point.x;
	float z = sr.local_hit_point.z;
	int ix = static_cast<int>(floor(x / m_size));
	int iz = static_cast<int>(floor(z / m_size));
	float fx = x / m_size - ix;
	float fz = z / m_size - iz;
	float width = static_cast<float>(0.5 * m_outline_width / m_size);
	bool in_outline = (fx < width || fx > 1.0 - width) || (fz < width || fz > 1.0 - width);

	if ((ix + iz) % 2 == 0) {
		if (!in_outline)
			return m_color1;
	}
	else {
		if (!in_outline)
			return m_color2;
	}

	return m_outline_color;
}

}