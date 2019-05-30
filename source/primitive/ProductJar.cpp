#include "raytracing/primitive/ProductJar.h"
#include "raytracing/primitive/OpenCylinder.h"
#include "raytracing/primitive/GeoInstance.h"
#include "raytracing/primitive/Torus.h"
#include "raytracing/primitive/Disk.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

ProductJar::ProductJar(double bottom, double body_top, double cap_top, double body_radius,
                       double bottom_bevel_radius, double top_bevel_radius, double cap_bevel_radius)
{
	/*
		bottom
		body_top
		cap_top
		body_radius
		bottom_bevel_radius
		top_bevel_radius
		cap_bevel_radius
		product_jar_ptr->set_material(matte_ptr);
		product_jar_ptr->set_body_material(body_matte_ptr);		// renders vertical body surface red
		*/
	this->body_top = body_top - top_bevel_radius;
	this->body_bottom = bottom + bottom_bevel_radius;

	m_parts.push_back(std::make_shared<OpenCylinder>(bottom+bottom_bevel_radius, body_top-top_bevel_radius, body_radius));	// wall

	auto top_torus_ptr = std::make_shared<GeoInstance>(std::make_shared<Torus>(body_radius - top_bevel_radius, top_bevel_radius));
	top_torus_ptr->Translate(rt::Vector3D(0, body_top - top_bevel_radius, 0));
	top_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(top_torus_ptr);

	auto bottom_torus_ptr = std::make_shared<GeoInstance>(std::make_shared<Torus>(body_radius - bottom_bevel_radius, bottom_bevel_radius));
	bottom_torus_ptr->Translate(rt::Vector3D(0, bottom_bevel_radius, 0));
	bottom_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(bottom_torus_ptr);

	m_parts.push_back(std::make_shared<OpenCylinder>(bottom+body_top, cap_top-cap_bevel_radius, body_radius-top_bevel_radius));	// wall

	auto cap_torus_ptr = std::make_shared<GeoInstance>(std::make_shared<Torus>(body_radius-top_bevel_radius-cap_bevel_radius, cap_bevel_radius));
	cap_torus_ptr->Translate(rt::Vector3D(0, cap_top-cap_bevel_radius, 0));
	cap_torus_ptr->SetTransformTexture(false);
	m_parts.push_back(cap_torus_ptr);

	m_parts.push_back(std::make_shared<Disk>(
        Point3D(0, bottom, 0), 			// bottom
        Normal(0, -1, 0),
        body_radius - bottom_bevel_radius
    ));

	m_parts.push_back(std::make_shared<Disk>(
        Point3D(0, cap_top, 0), 			// bottom
        Normal(0, 1, 0),
        body_radius-top_bevel_radius-cap_bevel_radius
    ));

	bbox.x0 = -body_radius;
	bbox.y0 = bottom;
	bbox.z0 = -body_radius;
	bbox.x1 = body_radius;
	bbox.y1 = cap_top;
	bbox.z1 = body_radius;
}

bool ProductJar::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (bbox.Hit(ray))
	{
		bool b;
		b= Compound::Hit(ray, tmin, sr);
		if ( b && sr.local_hit_point.y >= body_bottom && sr.local_hit_point.y <= body_top )
		{
			sr.local_hit_point.y = (sr.local_hit_point.y - body_bottom) / ( body_top - body_bottom );
		}
		return b;
	}
	else
		return (false);
}

bool ProductJar::ShadowHit(const Ray& ray, float& tmin) const
{
	if (bbox.Hit(ray))
		return (Compound::ShadowHit(ray, tmin));
	else
		return (false);
}

void ProductJar::SetMaterial(const std::shared_ptr<Material>& material) const
{
    for (int i = 0, n = m_parts.size(); i < n; ++i) {
        m_parts[i]->SetMaterial(material);
    }
}

}