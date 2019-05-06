#pragma once

namespace rt
{

class Point3D;

class Mapping
{
public:
	virtual ~Mapping() {}
	virtual void GetTexelCoordinates(const Point3D& hit_point,
		int xres, int yres, int& row, int& column) const = 0;

}; // Mapping

}
