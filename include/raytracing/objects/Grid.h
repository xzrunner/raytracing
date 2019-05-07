#pragma once

#include "raytracing/objects/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

//class Mesh;

class Grid : public Compound
{
public:
	Grid();
    virtual ~Grid();

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

	virtual AABB GetBoundingBox() const;

	void SetupCells(void);

private:
	Point3D FindMinBounds() const;
	Point3D FindMaxBounds() const;

private:
	std::vector<std::shared_ptr<Object>> cells;		    // grid of cells
	int	 nx, ny, nz;    	// number of cells in the x, y, and z directions
	AABB aabb;			    // bounding box
//	std::unique_ptr<Mesh> mesh = nullptr;			// holds triangle data
	bool reverse_normal;	// some PLY files have normals that point inwards

}; // Grid

}
