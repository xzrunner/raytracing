#include "raytracing/primitive/CutCube.h"
#include "raytracing/primitive/ConcavePartSphere.h"
#include "raytracing/primitive/Sphere.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

CutCube::CutCube()
    : p0(-1, -1, -1)
    , p1(1, 1, 1)
    , sphere(std::make_unique<ConcavePartSphere>(p1, 1, 180, 270, 90, 180))
    , sp(std::make_unique<Sphere>(p1, 1.0f))
{
}

CutCube::CutCube(const Point3D& p0, const Point3D& p1, float radius)
    : p0(p0)
    , p1(p1)
    , sphere(std::make_unique<ConcavePartSphere>(p1, radius, 180, 270, 90, 180))
    , sp(std::make_unique<Sphere>(p1, radius))
{
}

CutCube::~CutCube()
{
}

bool CutCube::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double ox = ray.ori.x; double oy = ray.ori.y; double oz = ray.ori.z;
	double dx = ray.dir.x; double dy = ray.dir.y; double dz = ray.dir.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (p0.x - ox) * a;
		tx_max = (p1.x - ox) * a;
	}
	else {
		tx_min = (p1.x - ox) * a;
		tx_max = (p0.x - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (p0.y - oy) * b;
		ty_max = (p1.y - oy) * b;
	}
	else {
		ty_min = (p1.y - oy) * b;
		ty_max = (p0.y - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (p0.z - oz) * c;
		tz_max = (p1.z - oz) * c;
	}
	else {
		tz_min = (p1.z - oz) * c;
		tz_max = (p0.z - oz) * c;
	}

	double t0, t1;

	// this is the same as Listing 19.1 down to the statement float t0, t1;

	int face_in, face_out;

	// find largest entering t value

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	}
	else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}

	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find smallest exiting t value

	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	}
	else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}

	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}
	double tm = 0;
	ShadeRec srr = sr;
	bool hitsp = false;
	if (t0 < t1 && t1 > EPSILON)
	{
		// condition for a hit
		if (t0 > EPSILON)
		{
			tmin = t0;  			// ray hits outside surface
			if( (hitsp = sphere->Hit(ray, tm, srr)) && tm < tmin)
			{
				tmin = tm;
				sr.local_hit_point = srr.local_hit_point;
				sr.normal = srr.normal;
			}
			else
			{
				if( !sp->Hit(ray, tm, srr) || !hitsp )
				{
					sr.normal = GetNormal(face_in);
				}
				else
					return false;
			}
		}
		else
		{
			tmin = t1;				// ray hits inside surface
			sr.normal = GetNormal(face_out);
		}

		sr.local_hit_point = ray.ori + tmin * ray.dir;
		return (true);
	}
	else
		return (false);
}

bool CutCube::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows) {
        return false;
    }

	double ox = ray.ori.x; double oy = ray.ori.y; double oz = ray.ori.z;
	double dx = ray.dir.x; double dy = ray.dir.y; double dz = ray.dir.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (p0.x - ox) * a;
		tx_max = (p1.x - ox) * a;
	}
	else {
		tx_min = (p1.x - ox) * a;
		tx_max = (p0.x - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (p0.y - oy) * b;
		ty_max = (p1.y - oy) * b;
	}
	else {
		ty_min = (p1.y - oy) * b;
		ty_max = (p0.y - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (p0.z - oz) * c;
		tz_max = (p1.z - oz) * c;
	}
	else {
		tz_min = (p1.z - oz) * c;
		tz_max = (p0.z - oz) * c;
	}

	double t0, t1;

	// this is the same as Listing 19.1 down to the statement float t0, t1;

	int face_in, face_out;

	// find largest entering t value

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	}
	else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}

	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find smallest exiting t value

	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	}
	else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}

	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}
	float tm = 0;
	bool hitsp = false;
	if (t0 < t1 && t1 > EPSILON)
	{
		// condition for a hit
		if (t0 > EPSILON)
		{
			tmin = static_cast<float>(t0);  			// ray hits outside surface
			if( ( hitsp = sphere->ShadowHit(ray, tm) ) && tm < tmin)
			{
				tmin = tm;
			}
			else
			{
				if( sp->ShadowHit(ray, tm)  || !hitsp )
				{
					return false;
				}
			}
		}
		else
		{
			tmin = static_cast<float>(t1);				// ray hits inside surface
		}
		return (true);
	}
	else
		return (false);
}

AABB CutCube::GetBoundingBox() const
{
    return AABB(p0.x, p1.x, p0.y, p1.y, p0.z, p1.z);
}

Normal CutCube::GetNormal(int face_hit) const
{
	switch (face_hit)
    {
		case 0:	return (Normal(-1, 0, 0));	// -x face
		case 1:	return (Normal(0, -1, 0));	// -y face
		case 2:	return (Normal(0, 0, -1));	// -z face
		case 3:	return (Normal(1, 0, 0));	// +x face
		case 4:	return (Normal(0, 1, 0));	// +y face
		case 5:	return (Normal(0, 0, 1));	// +z face
	}
    return Normal(0, 1, 0);
}

}