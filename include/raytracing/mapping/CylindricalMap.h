#pragma once

#include "raytracing/mapping/Mapping.h"

namespace rt
{

class CylindricalMap : public Mapping
{
public:

	virtual void GetTexelCoordinates(const Point3D& hit_point,
		int xres, int yres, int& row, int& column) const;

}; // CylindricalMap

}
