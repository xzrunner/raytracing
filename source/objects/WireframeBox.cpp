#include "raytracing/objects/WireframeBox.h"
#include "raytracing/objects/OpenCylinder.h"
#include "raytracing/objects/Instance.h"

namespace rt
{

WireframeBox::WireframeBox(const Point3D& p0, const Point3D& p1, float br)
	: m_p0(p0)
	, m_p1(p1)
	, m_br(br)
{
	double width = p1.z - p0.z;
	double length = p1.x - p0.x;
	double height = p1.y - p0.y;

	auto ocd = std::make_shared<OpenCylinder>(p0.y, p1.y, br);

	std::vector<std::shared_ptr<Instance>> parts;
	parts.resize(12, 0);
    for (int i = 0; i < 12; ++i) {
        parts[i] = std::make_shared<Instance>(std::make_shared<OpenCylinder>(*ocd));
    }

	parts[0]->Translate(Vector3D(-width/2+br/2,0,-length/2));
	parts[1]->Translate(Vector3D(-width/2+br/2,0,length/2));
	parts[2]->Translate(Vector3D(width/2-br/2,0,length/2));
	parts[3]->Translate(Vector3D(width/2-br/2,0,-length/2));

	parts[4]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[4]->RotateZ(90);
	parts[4]->Translate(Vector3D(0,height/2-br/2,width/2));
	parts[5]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[5]->RotateZ(90);
	parts[5]->Translate(Vector3D(0,-height/2+br/2,width/2));
	parts[6]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[6]->RotateZ(90);
	parts[6]->Translate(Vector3D(0,-height/2+br/2,-width/2));
	parts[7]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[7]->RotateZ(90);
	parts[7]->Translate(Vector3D(0,height/2-br/2,-width/2));

	parts[8]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[8]->RotateX(90);
	parts[8]->Translate(Vector3D(-length/2+br/2,height/2-br/2,0));
	parts[9]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[9]->RotateX(90);
	parts[9]->Translate(Vector3D(-length/2+br/2,-height/2+br/2,0));
	parts[10]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[10]->RotateX(90);
	parts[10]->Translate(Vector3D(length/2-br/2,-height/2+br/2,0));
	parts[11]->Translate(Vector3D(0,-(p0.y + p1.y)/2,0));
	parts[11]->RotateX(90);
	parts[11]->Translate(Vector3D(length/2-br/2,height/2-br/2,0));

	copy(parts.begin(), parts.end(), back_inserter(m_parts));

	m_aabb.x0 = p0.x - br;
	m_aabb.y0 = p0.y - br;
	m_aabb.z0 = p0.z - br;
	m_aabb.x1 = p1.x + br;
	m_aabb.y1 = p1.y + br;
	m_aabb.z1 = p1.z + br;
}

bool WireframeBox::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (m_aabb.Hit(ray)) {
		return Compound::Hit(ray, tmin, sr);
	} else {
		return false;
	}
}

bool WireframeBox::ShadowHit(const Ray& ray, float& tmin) const
{
	if (m_aabb.Hit(ray)) {
		return Compound::ShadowHit(ray, tmin);
	} else {
		return false;
	}
}

}