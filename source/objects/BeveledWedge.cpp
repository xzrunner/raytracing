#include "raytracing/objects/BeveledWedge.h"
#include "raytracing/objects/Sphere.h"
#include "raytracing/objects/Rectangle.h"
#include "raytracing/objects/Instance.h"
#include "raytracing/objects/OpenCylinder.h"
#include "raytracing/objects/ConvexPartCylinder.h"
#include "raytracing/objects/ConvexPartTorus.h"
#include "raytracing/objects/ConcavePartCylinder.h"
#include "raytracing/objects/PartAnnulus.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

BeveledWedge::BeveledWedge(double y0, double y1, double r0, double r1, double rb, double phi0, double phi1)
    : y0(y0)
    , y1(y1)
    , r0(r0)
    , r1(r1)
    , rb(rb)
    , phi0(phi0)
    , phi1(phi1)
{
	double sin_phi0 = sin(phi0 * PI_ON_180);  // in radians
	double cos_phi0 = cos(phi0 * PI_ON_180);  // in radians
	double sin_phi1 = sin(phi1 * PI_ON_180);  // in radians
	double cos_phi1 = cos(phi1 * PI_ON_180);  // in radians

	double sin_alpha = rb / (r0 + rb);
	double cos_alpha = sqrt(r0 * r0 + 2.0 * r0 * rb) / (r0 + rb);
	double sin_beta = rb / (r1 - rb);
	double cos_beta = sqrt(r1 * r1 - 2.0 * r1 * rb) / (r1 - rb);

	double xc1 = (r0 + rb) * (sin_phi0 * cos_alpha + cos_phi0 * sin_alpha);
	double zc1 = (r0 + rb) * (cos_phi0 * cos_alpha - sin_phi0 * sin_alpha);

	double xc2 = (r1 - rb) * (sin_phi0 * cos_beta + cos_phi0 * sin_beta);
	double zc2 = (r1 - rb) * (cos_phi0 * cos_beta - sin_phi0 * sin_beta);

	double xc3 = (r0 + rb) * (sin_phi1 * cos_alpha - cos_phi1 * sin_alpha);
	double zc3 = (r0 + rb) * (cos_phi1 * cos_alpha + sin_phi1 * sin_alpha);

	double xc4 = (r1 - rb) * (sin_phi1 * cos_beta - cos_phi1 * sin_beta);
	double zc4 = (r1 - rb) * (cos_phi1 * cos_beta + sin_phi1 * sin_beta);


	// corner spheres -------------------------------------------------------------------------------

	// bottom spheres

	auto bottom_c1 = std::make_shared<Sphere>(Point3D(xc1, y0 + rb, zc1), rb);
	m_parts.push_back(bottom_c1);

	auto bottom_c2 = std::make_shared<Sphere>(Point3D(xc2, y0 + rb, zc2), rb);
	m_parts.push_back(bottom_c2);

	auto bottom_c3 = std::make_shared<Sphere>(Point3D(xc3, y0 + rb, zc3), rb);
	m_parts.push_back(bottom_c3);

	auto bottom_c4 = std::make_shared<Sphere>(Point3D(xc4, y0 + rb, zc4), rb);
	m_parts.push_back(bottom_c4);


	// top spheres

	auto top_c1 = std::make_shared<Sphere>(Point3D(xc1, y1 - rb, zc1), rb);
	m_parts.push_back(top_c1);

	auto top_c2 = std::make_shared<Sphere>(Point3D(xc2, y1 - rb, zc2), rb);
	m_parts.push_back(top_c2);

	auto top_c3 = std::make_shared<Sphere>(Point3D(xc3, y1 - rb, zc3), rb);
	m_parts.push_back(top_c3);

	auto top_c4 = std::make_shared<Sphere>(Point3D(xc4, y1 - rb, zc4), rb);
	m_parts.push_back(top_c4);


	// vertical cylinders ------------------------------------------------------------------------------

	auto bottom_c1_cylinder = std::make_shared<Instance>(std::make_shared<OpenCylinder>(y0 + rb, y1 - rb, rb));
	bottom_c1_cylinder->Translate(Vector3D(xc1, 0.0, zc1));
	bottom_c1_cylinder->SetTransformTexture(false);
	m_parts.push_back(bottom_c1_cylinder);

	auto bottom_c2_cylinder = std::make_shared<Instance>(std::make_shared<OpenCylinder>(y0 + rb, y1 - rb, rb));
	bottom_c2_cylinder->Translate(Vector3D(xc2, 0.0, zc2));
	bottom_c2_cylinder->SetTransformTexture(false);
	m_parts.push_back(bottom_c2_cylinder);

	auto bottom_c3_cylinder = std::make_shared<Instance>(std::make_shared<OpenCylinder>(y0 + rb, y1 - rb, rb));
	bottom_c3_cylinder->Translate(Vector3D(xc3, 0.0, zc3));
	bottom_c3_cylinder->SetTransformTexture(false);
	m_parts.push_back(bottom_c3_cylinder);

	auto bottom_c4_cylinder = std::make_shared<Instance>(std::make_shared<OpenCylinder>(y0 + rb, y1 - rb, rb));
	bottom_c4_cylinder->Translate(Vector3D(xc4, 0.0, zc4));
	bottom_c4_cylinder->SetTransformTexture(false);
	m_parts.push_back(bottom_c4_cylinder);


	// inner curved surface ---------------------------------------------------------------------------------

	// the azimuth angle range has to be specified in degrees

	double alpha = acos(cos_alpha);  // radians
	double phi_min = phi0 + alpha * 180.0 / PI;
	double phi_max = phi1 - alpha * 180.0 / PI;

	auto inner_cylinder_ptr = std::make_shared<ConcavePartCylinder>(y0 + rb, y1 - rb, r0, phi_min, phi_max);
	m_parts.push_back(inner_cylinder_ptr);


	// outer curved surface -----------------------------------------------------------------------------------

	// the azimuth angle range has to be specified in degrees

	double beta = acos(cos_beta);  // radians
	phi_min = phi0 + beta * 180.0 / PI;
	phi_max = phi1 - beta * 180.0 / PI;

	auto outer_cylinder_ptr = std::make_shared<ConvexPartCylinder>(y0 + rb, y1 - rb, r1, phi_min, phi_max);
	m_parts.push_back(outer_cylinder_ptr);


	// phi0 vertical rectangle

	double s1 = sqrt(r0 * r0 + 2.0 * r0 * rb);
	double s2 = sqrt(r1 * r1 - 2.0 * r1 * rb);
	Point3D p1(s1 * sin_phi0, y0 + rb, s1 * cos_phi0);
	Point3D p2(s2 * sin_phi0, y0 + rb, s2 * cos_phi0);
	Vector3D a = p2 - p1;
	Vector3D b(0, y1 - y0 - 2.0 * rb, 0);

	auto phi0_rectangle_ptr = std::make_shared<Rectangle>(p1, a, b);
	m_parts.push_back(phi0_rectangle_ptr);


	// phi1 vertical rectangle

	Point3D p3(s1 * sin_phi1, y0 + rb, s1 * cos_phi1);
	Point3D p4(s2 * sin_phi1, y0 + rb, s2 * cos_phi1);
	a = p3 - p4;

	auto phi1_rectangle_ptr = std::make_shared<Rectangle>(p4, a, b);
	m_parts.push_back(phi1_rectangle_ptr);



	// the tori --------------------------------------------------------------------------------------------

	// inner bottom

	phi_min = phi0 + alpha * 180.0 / PI;
	phi_max = phi1 - alpha * 180.0 / PI;

	auto inner_bottom_torus = std::make_shared<Instance>(std::make_shared<ConvexPartTorus>(r0 + rb, rb, phi_min, phi_max, 0, 360));
	inner_bottom_torus->Translate(Vector3D(0.0, y0 + rb, 0.0));
	inner_bottom_torus->SetTransformTexture(false);
	m_parts.push_back(inner_bottom_torus);


	// inner top

	auto inner_top_torus = std::make_shared<Instance>(std::make_shared<ConvexPartTorus>(r0 + rb, rb, phi_min, phi_max, 0, 360));
	inner_top_torus->Translate(Vector3D(0.0, y1 - rb, 0.0));
	inner_top_torus->SetTransformTexture(false);
	m_parts.push_back(inner_top_torus);


	// outer bottom

	phi_min = phi0 + beta * 180.0 / PI;
	phi_max = phi1 - beta * 180.0 / PI;

	auto outer_bottom_torus = std::make_shared<Instance>(std::make_shared<ConvexPartTorus>(r1 - rb, rb, phi_min, phi_max, 0, 360));
	outer_bottom_torus->Translate(Vector3D(0.0, y0 + rb, 0.0));
	outer_bottom_torus->SetTransformTexture(false);
	m_parts.push_back(outer_bottom_torus);


	// outer top

	auto outer_top_torus = std::make_shared<Instance>(std::make_shared<ConvexPartTorus>(r1 - rb, rb, phi_min, phi_max, 0, 360));
	outer_top_torus->Translate(Vector3D(0.0, y1 - rb, 0.0));
	outer_top_torus->SetTransformTexture(false);
	m_parts.push_back(outer_top_torus);


	// horizontal cylinders ----------------------------------------------------------------------------------

	// phi0 bottom cylinder

	auto phi0_bottom_cylinder_ptr = std::make_shared<Instance>(std::make_shared<OpenCylinder>(0, s2 - s1, rb));
	phi0_bottom_cylinder_ptr->RotateX(90);
	phi0_bottom_cylinder_ptr->RotateY(phi0);
	phi0_bottom_cylinder_ptr->Translate(Vector3D(xc1, y0 + rb, zc1));
	phi0_bottom_cylinder_ptr->SetTransformTexture(false);
	m_parts.push_back(phi0_bottom_cylinder_ptr);


	// phi0 top cylinder

	auto phi0_top_cylinder_ptr = std::make_shared<Instance>(std::make_shared<OpenCylinder>(0, s2 - s1, rb));
	phi0_top_cylinder_ptr->RotateX(90);
	phi0_top_cylinder_ptr->RotateY(phi0);
	phi0_top_cylinder_ptr->Translate(Vector3D(xc1, y1 - rb, zc1));
	phi0_top_cylinder_ptr->SetTransformTexture(false);
	m_parts.push_back(phi0_top_cylinder_ptr);


	// phi1 bottom cylinder

	auto phi1_bottom_cylinder_ptr = std::make_shared<Instance>(std::make_shared<OpenCylinder>(0, s2 - s1, rb));
	phi1_bottom_cylinder_ptr->RotateX(90);
	phi1_bottom_cylinder_ptr->RotateY(phi1);
	phi1_bottom_cylinder_ptr->Translate(Vector3D(xc3, y0 + rb, zc3));
	phi1_bottom_cylinder_ptr->SetTransformTexture(false);
	m_parts.push_back(phi1_bottom_cylinder_ptr);


	// phi1 top cylinder

	auto phi1_top_cylinder_ptr = std::make_shared<Instance>(std::make_shared<OpenCylinder>(0, s2 - s1, rb));
	phi1_top_cylinder_ptr->RotateX(90);
	phi1_top_cylinder_ptr->RotateY(phi1);
	phi1_top_cylinder_ptr->Translate(Vector3D(xc3, y1 - rb, zc3));
	phi1_top_cylinder_ptr->SetTransformTexture(false);
	m_parts.push_back(phi1_top_cylinder_ptr);


	// top flat surface -----------------------------------------------------------------------------------

	// main part

	Point3D center(0, y1, 0);
	Normal normal(0, 1, 0);
	double r_min = r0 + rb;
	double r_max = r1 - rb;
	phi_min = phi0 + alpha * 180.0 / PI;
	phi_max = phi1 - alpha * 180.0 / PI;

	auto top_main_part_ptr = std::make_shared<PartAnnulus>(center, normal, r_min, r_max, phi_min, phi_max);
	m_parts.push_back(top_main_part_ptr);


	// small phi0 side patch

	r_min = 0.0;
	r_max = s2 - s1;
	phi_min = 0.0;
	phi_max = alpha * 180.0 / PI;

	auto top_phi0_patch_ptr = std::make_shared<Instance>(std::make_shared<PartAnnulus>(center, normal, r_min, r_max, phi_min, phi_max));
	top_phi0_patch_ptr->RotateY(phi0);
	top_phi0_patch_ptr->Translate(Vector3D(xc1, 0.0, zc1));
	top_phi0_patch_ptr->SetTransformTexture(false);
	m_parts.push_back(top_phi0_patch_ptr);


	// small phi1 side patch

	phi_min = 360.0 - alpha * 180.0 / PI;
	phi_max = 360.0;

	auto top_phi1_patch_ptr = std::make_shared<Instance>(std::make_shared<PartAnnulus>(center, normal, r_min, r_max, phi_min, phi_max));
	top_phi1_patch_ptr->RotateY(phi1);
	top_phi1_patch_ptr->Translate(Vector3D(xc3, 0.0, zc3));
	top_phi1_patch_ptr->SetTransformTexture(false);
	m_parts.push_back(top_phi1_patch_ptr);



	// bottom flat surface ---------------------------------------------------------------------------------

	// main part

	center = Point3D(0, y0, 0);
	normal = Normal(0, -1, 0);
	r_min = r0 + rb;
	r_max = r1 - rb;
	phi_min = phi0 + alpha * 180.0 / PI;
	phi_max = phi1 - alpha * 180.0 / PI;

	auto bottom_main_part_ptr = std::make_shared<PartAnnulus>(center, normal, r_min, r_max, phi_min, phi_max);
	m_parts.push_back(bottom_main_part_ptr);


	// small phi0 side patch

	r_min = 0.0;
	r_max = s2 - s1;
	phi_min = 0.0;
	phi_max = alpha * 180.0 / PI;

	auto bottom_phi0_patch_ptr = std::make_shared<Instance>(std::make_shared<PartAnnulus>(center, normal, r_min, r_max, phi_min, phi_max));
	bottom_phi0_patch_ptr->RotateY(phi0);
	bottom_phi0_patch_ptr->Translate(Vector3D(xc1, 0.0, zc1));
	bottom_phi0_patch_ptr->SetTransformTexture(false);
	m_parts.push_back(bottom_phi0_patch_ptr);


	// small phi1 side patch

	phi_min = 360.0 - alpha * 180.0 / PI;
	phi_max = 360.0;

	auto bottom_phi1_patch_ptr = std::make_shared<Instance>(std::make_shared<PartAnnulus>(center, normal, r_min, r_max, phi_min, phi_max));
	bottom_phi1_patch_ptr->RotateY(phi1);
	bottom_phi1_patch_ptr->Translate(Vector3D(xc3, 0.0, zc3));
	bottom_phi1_patch_ptr->SetTransformTexture(false);
	m_parts.push_back(bottom_phi1_patch_ptr);



	// compute the bounding box

	double x[4] = {xc1, xc2, xc3, xc4};
	double z[4] = {zc1, zc2, zc3, zc4};


	// first, assume that the wedge is completely inside a quadrant, which will be true for most wedges

	// work out the maximum and minimum values

	double x0 = HUGE_VALUE;
	double z0 = HUGE_VALUE;

	for (int j = 0; j <= 3; j++)  {
		if (x[j] < x0)
			x0 = x[j];
	}

	for (int j = 0; j <= 3; j++) {
		if (z[j] < z0)
			z0 = z[j];
	}

	double x1 = -HUGE_VALUE;
	double z1 = -HUGE_VALUE;

	for (int j = 0; j <= 3; j++) {
		if (x[j] > x1)
			x1 = x[j];
	}

	for (int j = 0; j <= 3; j++) {
		if (z[j] > z1)
			z1 = z[j];
	}

	// assign values to the bounding box

	bbox.x0 = x0 - rb;
	bbox.y0 = y0;
	bbox.z0 = z0 - rb;
	bbox.x1 = x1 + rb;
	bbox.y1 = y1;
	bbox.z1 = z1 + rb;

	bool spans90 = phi0 < 90 && phi1 > 90;
	bool spans180 = phi0 < 180 && phi1 > 180;
	bool spans270 = phi0 < 270 && phi1 > 270;

	if (spans90 && spans180 && spans270) {
		bbox.x0 = -r1;
		bbox.z0 = -r1;
		bbox.x1 = r1;
		bbox.z1 = std::max(zc2, zc4);
	}
	else if (spans90 && spans180) {
		bbox.x0 = xc4 - rb;
		bbox.z0 = -r1;
		bbox.x1 = r1;
		bbox.z1 = zc2 + rb;
	}
	else if (spans180 && spans270) {
		bbox.x0 = -r1;
		bbox.z0 = -r1;
		bbox.x1 = xc2 + rb;
		bbox.z1 = zc4 + rb;
	}
	else if (spans90) {
		bbox.x0 = std::min(xc1, xc3);
		bbox.z0 = zc4 - rb;
		bbox.x1 = r1;
		bbox.z1 = zc2 + rb;
	}
	else if (spans180) {
		bbox.x0 = xc4 - rb;
		bbox.z0 = -r1;
		bbox.x1 = xc2 + rb;
		bbox.z1 = std::max(zc1, zc3);
	}
	else if (spans270) {
		bbox.x0 = -r1;
		bbox.z0 = zc2 - rb;
		bbox.x1 = std::max(xc1, xc3);
		bbox.z1 = zc4 + rb;
	}
}

bool BeveledWedge::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (bbox.Hit(ray))
		return (Compound::Hit(ray, tmin, sr));
	else
		return (false);
}

}