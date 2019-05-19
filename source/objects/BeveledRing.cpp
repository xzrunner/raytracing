#include "raytracing/objects/BeveledRing.h"
#include "raytracing/objects/Annulus.h"
#include "raytracing/objects/OpenCylinder.h"
#include "raytracing/objects/Instance.h"
#include "raytracing/objects/Torus.h"

namespace rt
{

BeveledRing::BeveledRing()
{
	auto bottom_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>(0.95, 0.05));
	bottom_torus_ptr->Translate(Vector3D(0, 0, 0));
	bottom_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(bottom_torus_ptr);

	auto top_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>(0.95, 0.05));
	top_torus_ptr->Translate(Vector3D(0, 1, 0));
	top_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(top_torus_ptr);

	m_parts.push_back(std::make_shared<OpenCylinder>(0, 1, 0.9));	// iwall
	m_parts.push_back(std::make_shared<OpenCylinder>(0, 1, 0.1));	// owall

	bbox.x0 = -1;
	bbox.y0 = -1.05;
	bbox.z0 = -1;
	bbox.x1 = 1;
	bbox.y1 = 1.05;
	bbox.z1 = 1;
}

BeveledRing::BeveledRing(double bottom, double top, double i_radius, double o_radius, double b_radius)
{
	m_parts.push_back(std::make_shared<Annulus>(Point3D(0, bottom, 0), Normal(0, -1, 0), i_radius + b_radius, o_radius - i_radius - 2 * b_radius));

	m_parts.push_back(std::make_shared<Annulus>(Point3D(0, top, 0), Normal(0, 1, 0), i_radius + b_radius, o_radius - i_radius - 2 * b_radius));

	auto outer_bottom_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>((o_radius - b_radius ), b_radius));
	outer_bottom_torus_ptr->Translate(Vector3D(0, 0 + b_radius, 0));
	outer_bottom_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(outer_bottom_torus_ptr);

	auto outer_top_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>((o_radius - b_radius ), b_radius));
	outer_top_torus_ptr->Translate(Vector3D(0, top - b_radius, 0));
	outer_top_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(outer_top_torus_ptr);

	auto inner_bottom_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>((i_radius + b_radius ), b_radius));
	inner_bottom_torus_ptr->Translate(Vector3D(0, 0 + b_radius, 0));
	inner_bottom_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(inner_bottom_torus_ptr);

	auto inner_top_torus_ptr = std::make_shared<Instance>(std::make_shared<Torus>((i_radius + b_radius), b_radius));
	inner_top_torus_ptr->Translate(Vector3D(0, top - b_radius, 0));
	inner_top_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(inner_top_torus_ptr);


	m_parts.push_back(std::make_shared<OpenCylinder>(bottom + b_radius, top - b_radius, i_radius));	// iwall

	m_parts.push_back(std::make_shared<OpenCylinder>(bottom + b_radius, top - b_radius, o_radius));	// owall

	bbox.x0 = -o_radius;
	bbox.y0 = bottom - ( o_radius - i_radius ) / 2;
	bbox.z0 = -o_radius;
	bbox.x1 = o_radius;
	bbox.y1 = top + ( o_radius - i_radius ) / 2;
	bbox.z1 = o_radius;
}

bool BeveledRing::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
    if (bbox.Hit(ray)) {
        return Compound::Hit(ray, tmin, sr);
    } else {
        return false;
    }
}

bool BeveledRing::ShadowHit(const Ray& ray, float& tmin) const
{
    if (bbox.Hit(ray)) {
        return Compound::ShadowHit(ray, tmin);
    } else {
        return false;
    }
}

void BeveledRing::SetMaterial(const std::shared_ptr<Material>& material) const
{
    m_material = material;
    for (auto& child : m_parts) {
        child->SetMaterial(material);
    }
}

}