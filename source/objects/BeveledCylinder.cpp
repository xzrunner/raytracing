#include "raytracing/objects/BeveledCylinder.h"
#include "raytracing/objects/Disk.h"
#include "raytracing/objects/OpenCylinder.h"
#include "raytracing/objects/Instance.h"
#include "raytracing/objects/Torus.h"

namespace rt
{

//BeveledCylinder::BeveledCylinder()
//{
//	//This is new function for chapter 30 for any diffault para solid cylinders. Ex 30.12
//    // bottom
//	m_parts.push_back(std::make_shared<Disk>(Point3D(0, -1, 0), Normal(0, -1, 0), 1));
//    // top
//    m_parts.push_back(std::make_shared<Disk>(Point3D(0, 1, 0), Normal(0, 1, 0), 1));
//    // wall
//    m_parts.push_back(std::make_shared<OpenCylinder>(-1, 1, 1));
//
//	bbox.x0 = -1;
//	bbox.y0 = -1;
//	bbox.z0 = -1;
//	bbox.x1 = 1;
//	bbox.y1 = 1;
//	bbox.z1 = 1;
//}

BeveledCylinder::BeveledCylinder(float bottom, float top, float r, float br)
    : bottom(bottom)
    , top(top)
    , radius(r)
    , bevel_radius(br)
{
	// construct the bottom, top, and curved surface

	m_parts.push_back(std::make_shared<Disk>(Point3D(0, bottom, 0), Normal(0, -1, 0), r - br)); // bottom
	m_parts.push_back(std::make_shared<Disk>(Point3D(0, top, 0), Normal(0, 1, 0), r - br));   	// top
	m_parts.push_back(std::make_shared<OpenCylinder>(bottom + br, top - br, r));   		  	    // curved surface

	// have to use instances for the tori because they have to be translated

	auto bottom_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>(r - br, br));
	bottom_torus_ptr->Translate(Vector3D(0, bottom + br, 0));
	bottom_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(bottom_torus_ptr);

	auto top_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>(r - br, br));
	top_torus_ptr->Translate(Vector3D(0, top  - br, 0));
	top_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(top_torus_ptr);

	bbox.x0 = -radius;
	bbox.y0 = bottom;
	bbox.z0 = -radius;
	bbox.x1 = radius;
	bbox.y1 = top;
	bbox.z1 = radius;
}

bool BeveledCylinder::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
    if (bbox.Hit(ray)) {
        return Compound::Hit(ray, tmin, sr);
    } else {
        return false;
    }
}

bool BeveledCylinder::ShadowHit(const Ray& ray, float& tmin) const
{
    if (m_shadows && bbox.Hit(ray)) {
        return Compound::ShadowHit(ray, tmin);
    } else {
        return false;
    }
}

}