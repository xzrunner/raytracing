#pragma once

#include "raytracing/primitive/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class Mesh;

class Grid : public Compound
{
public:
	Grid();
    virtual ~Grid();

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

	virtual AABB GetBoundingBox() const override;

    virtual void SetMaterial(const std::shared_ptr<Material>& material) const override;

	void SetupCells(void);

    void ReadFlatTriangles(const std::string& filename);
    void ReadSmoothTriangles(const std::string& filename);

    void TessellateFlatSphere(int horizontal_steps, int vertical_steps);
    void TessellateSmoothSphere(int horizontal_steps, int vertical_steps);

private:
	Point3D FindMinBounds() const;
	Point3D FindMaxBounds() const;

    void ReadPlyFile(const std::string& filename, int triangle_type);
    void ComputeMeshNormals();

private:
	std::vector<std::shared_ptr<GeoPrimitive>> cells;		    // grid of cells
	int	 nx, ny, nz;    	// number of cells in the x, y, and z directions
	AABB aabb;			    // bounding box
	std::shared_ptr<Mesh> mesh = nullptr;			// holds triangle data
	bool reverse_normal;	// some PLY files have normals that point inwards

}; // Grid

}
