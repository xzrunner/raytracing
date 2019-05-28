#include "raytracing/primitive/BeveledBox.h"
#include "raytracing/primitive/GeoInstance.h"
#include "raytracing/primitive/OpenCylinder.h"
#include "raytracing/primitive/Rectangle.h"
#include "raytracing/primitive/Sphere.h"
#include "raytracing/material/Reflective.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/maths/maths.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

BeveledBox::BeveledBox(const Point3D& bottom, const Point3D& top, float bevel_radius)
    : p0(bottom)
    , p1(top)
    , br(bevel_radius)
{
	double width = p1.z - p0.z;
	double length = p1.x - p0.x;
	double height = p1.y - p0.y;

	//BeveledCylinder *ocd = new BeveledCylinder(p0.y, p1.y, br, br);
	auto ocd = std::make_shared<OpenCylinder>(p0.y + br, p1.y - br, br);

	// Here we must know that there is another box except previous twelve axis
	m_parts.reserve(26);
	m_parts.assign(26, 0);

	m_parts[0] = std::make_shared<GeoInstance>(ocd);
	m_parts[1] = std::make_shared<GeoInstance>(ocd);
	m_parts[2] = std::make_shared<GeoInstance>(ocd);
	m_parts[3] = std::make_shared<GeoInstance>(ocd);

	auto ocd1 = std::make_shared<OpenCylinder>(p0.x + br, p1.x - br, br);
	//BeveledCylinder *ocd1 = new BeveledCylinder(p0.x,p1.x,br,br);

	m_parts[4] = std::make_shared<GeoInstance>(ocd1);
	m_parts[5] = std::make_shared<GeoInstance>(ocd1);
	m_parts[6] = std::make_shared<GeoInstance>(ocd1);
	m_parts[7] = std::make_shared<GeoInstance>(ocd1);

	//BeveledCylinder *ocd2 = new BeveledCylinder(p0.z,p1.z,br,br);
	auto ocd2 = std::make_shared<OpenCylinder>(p0.z + br, p1.z - br, br);
	m_parts[8] = std::make_shared<GeoInstance>(ocd2);
	m_parts[9] = std::make_shared<GeoInstance>(ocd2);
	m_parts[10] = std::make_shared<GeoInstance>(ocd2);
	m_parts[11] = std::make_shared<GeoInstance>(ocd2);

	//Here add a box
	m_parts[12] = std::make_shared<GeoInstance>(std::make_shared<Rectangle>( Point3D( -length/2+br,-height/2+br, width/2 ), Vector3D( length - 2 * br, 0, 0 ), Vector3D(  0, height - 2 * br, 0 ), Normal(0, 0, 1) ));
	m_parts[13] = std::make_shared<GeoInstance>(std::make_shared<Rectangle>( Point3D( length/2,-height/2+br, width/2-br), Vector3D( 0, 0, - width + 2 * br ), Vector3D(  0, height - 2 * br, 0 ), Normal(1, 0, 0) ));
	m_parts[14] = std::make_shared<GeoInstance>(std::make_shared<Rectangle>( Point3D( length/2-br,-height/2+br, -width/2), Vector3D( - length + 2 * br, 0, 0 ), Vector3D( 0, height - 2 * br, 0 ), Normal(0, 0, -1) ));
	m_parts[15] = std::make_shared<GeoInstance>(std::make_shared<Rectangle>( Point3D( -length/2, -height/2+br, -width/2+br), Vector3D( 0, 0, width - 2 * br ), Vector3D(  0, height - 2 * br, 0 ), Normal(-1, 0, 0) ));
	m_parts[16] = std::make_shared<GeoInstance>(std::make_shared<Rectangle>( Point3D( -length/2+br,height/2, width/2-br), Vector3D( length - 2 * br, 0, 0 ), Vector3D(  0, 0, -width + 2 * br ), Normal(0, 1, 0) ));
	m_parts[17] = std::make_shared<GeoInstance>(std::make_shared<Rectangle>( Point3D( -length/2+br,-height/2, -width/2+br), Vector3D( length - 2 * br, 0, 0 ), Vector3D(  0, 0, width - 2 * br ), Normal(0, -1, 0) ));

	m_parts[18] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( -length/2+br,height/2-br, width/2-br ), br ));
	m_parts[19] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( length/2-br,height/2-br, width/2-br ), br ));
	m_parts[20] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( length/2-br,height/2-br, -width/2+br ), br ));
	m_parts[21] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( -length/2+br,height/2-br, -width/2+br ), br ));
	m_parts[22] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( -length/2+br,-height/2+br, width/2-br ), br ));
	m_parts[23] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( length/2-br,-height/2+br, width/2-br ), br ));
	m_parts[24] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( length/2-br,-height/2+br, -width/2+br ), br ));
	m_parts[25] = std::make_shared<GeoInstance>(std::make_shared<Sphere>( Point3D( -length/2+br,-height/2+br, -width/2+br ), br ));

	std::static_pointer_cast<GeoInstance>(m_parts[0])->Translate(Vector3D(-length / 2 + br, 0, -width / 2 + br));
	std::static_pointer_cast<GeoInstance>(m_parts[1])->Translate(Vector3D(-length / 2 + br, 0, width / 2 - br));
	std::static_pointer_cast<GeoInstance>(m_parts[2])->Translate(Vector3D(length / 2 - br, 0, width / 2 - br));
	std::static_pointer_cast<GeoInstance>(m_parts[3])->Translate(Vector3D(length / 2 - br, 0, -width / 2 + br));

	std::static_pointer_cast<GeoInstance>(m_parts[4])->Translate(Vector3D(0, -(p0.x + p1.x) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[4])->RotateZ(90);
	std::static_pointer_cast<GeoInstance>(m_parts[4])->Translate(Vector3D(0, height / 2 - br, width / 2 - br));
	std::static_pointer_cast<GeoInstance>(m_parts[5])->Translate(Vector3D(0, -(p0.x + p1.x) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[5])->RotateZ(90);
	std::static_pointer_cast<GeoInstance>(m_parts[5])->Translate(Vector3D(0, -height / 2 + br, width / 2 - br));
	std::static_pointer_cast<GeoInstance>(m_parts[6])->Translate(Vector3D(0, -(p0.x + p1.x) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[6])->RotateZ(90);
	std::static_pointer_cast<GeoInstance>(m_parts[6])->Translate(Vector3D(0, -height / 2 + br, -width / 2 + br));
	std::static_pointer_cast<GeoInstance>(m_parts[7])->Translate(Vector3D(0, -(p0.x + p1.x) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[7])->RotateZ(90);
	std::static_pointer_cast<GeoInstance>(m_parts[7])->Translate(Vector3D(0, height / 2 - br, -width / 2 + br));

	std::static_pointer_cast<GeoInstance>(m_parts[8])->Translate(Vector3D(0, -(p0.z + p1.z) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[8])->RotateX(90);
	std::static_pointer_cast<GeoInstance>(m_parts[8])->Translate(Vector3D(-length / 2 + br, height / 2 - br, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[9])->Translate(Vector3D(0, -(p0.z + p1.z) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[9])->RotateX(90);
	std::static_pointer_cast<GeoInstance>(m_parts[9])->Translate(Vector3D(-length / 2 + br, -height / 2 + br, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[10])->Translate(Vector3D(0, -(p0.z + p1.z) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[10])->RotateX(90);
	std::static_pointer_cast<GeoInstance>(m_parts[10])->Translate(Vector3D(length / 2 - br, -height / 2 + br, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[11])->Translate(Vector3D(0, -(p0.z + p1.z) / 2, 0));
	std::static_pointer_cast<GeoInstance>(m_parts[11])->RotateX(90);
	std::static_pointer_cast<GeoInstance>(m_parts[11])->Translate(Vector3D(length / 2 - br, height / 2 - br, 0));

	bbox.x0 = p0.x - bevel_radius;
	bbox.y0 = p0.y - bevel_radius;
	bbox.z0 = p0.z - bevel_radius;
	bbox.x1 = p1.x + bevel_radius;
	bbox.y1 = p1.y + bevel_radius;
	bbox.z1 = p1.z + bevel_radius;

	// The code below only works for 24.29d, which can replace the outer material set by others with random reflective materials
	set_rand_seed(1000);
	float c = 0.25;

	for (int j = 0; j < m_parts.size(); j++)
    {
		auto reflective_ptr = std::make_shared<Reflective>();
		reflective_ptr->SetKa(0.0);
		reflective_ptr->SetKd(0.0);
		reflective_ptr->SetKs(0.0);
		reflective_ptr->SetCd(RGBColor(0, 0, 0));
		reflective_ptr->SetKr(0.9);
		reflective_ptr->SetCr(RGBColor(1.0, 0.75 + c * (-1.0 + 2.0 * rand_float()) , 0.25 + c * (-1.0 + 2.0 * rand_float())));
		m_parts[j]->SetMaterial(reflective_ptr);
	}
}

bool BeveledBox::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (bbox.Hit(ray))
	{
		double		t;
		Normal		normal;
		Point3D		local_hit_point;
		bool		hit 		= false;
					tmin 		= HUGE_VALUE;
		int 		num_objects	= m_parts.size();

		for (int j = 0; j < num_objects; j++)
			if (m_parts[j]->Hit(ray, t, sr) && (t < tmin)) {
				hit				= true;
				tmin 			= t;
//				material	= m_parts[j]->get_material();	// lhs is GeometricObject::material_ptr
				normal			= sr.normal;
				local_hit_point	= sr.local_hit_point;
			}

		if (hit) {
			sr.t				= tmin;
			sr.normal 			= normal;
			sr.local_hit_point 	= local_hit_point;
		}
		return (hit);
	}
	else
		return (false);
}

bool BeveledBox::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows || !bbox.Hit(ray)) {
        return (false);
    }

	float		t = 100000;		// may be important too
	Normal		normal;
	Point3D		local_hit_point;
	bool		hit 		= false;
	int 		num_objects	= m_parts.size();

	for (int j = 0; j < num_objects; j++)
		if (m_parts[j]->ShadowHit(ray, t) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
		}
	return (hit);
}

}