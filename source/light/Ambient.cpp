#include "raytracing/light/Ambient.h"

namespace rt
{

Ambient::Ambient()
	: m_ls(1)
	, m_color(1, 1, 1)
{
}

RGBColor Ambient::L(const ShadeRec& sr) const
{
	return m_ls * m_color;
}

Vector3D Ambient::GetDirection(const ShadeRec& sr) const
{
	return Vector3D(0, 0, 0);
}

bool Ambient::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	// not implented yet
	return true;
}

void Ambient::ScaleRadiance(const float b)
{
	m_ls = b;
}

}