#include "raytracing/objects/Rectangle.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Rectangle::Rectangle()
    : m_p0(-1, 0, -1)
    , m_a(0, 0, 2)
    , m_b(2, 0, 0)
    , m_a_len_squared(4.0)
    , m_b_len_squared(4.0)
    , m_normal(0, 1, 0)
    , m_area(4.0)
    , m_inv_area(0.25)
    , m_sampler(nullptr)
{
}

Rectangle::Rectangle(const Point3D& p0, const Vector3D& a, const Vector3D& b)
    : m_p0(m_p0)
    , m_a(m_a)
    , m_b(m_b)
    , m_a_len_squared(m_a.LenSquared())
    , m_b_len_squared(m_b.LenSquared())
    , m_area((float)(m_a.Length() * m_b.Length()))
    , m_inv_area(1 / m_area)
    , m_sampler(nullptr)
{
    m_normal = a ^ b;
    m_normal.Normalize();
}

Rectangle::Rectangle(const Point3D& m_p0, const Vector3D& m_a, const Vector3D& m_b, const Normal& n)
	: m_p0(m_p0)
	, m_a(m_a)
	, m_b(m_b)
	, m_a_len_squared(m_a.LenSquared())
	, m_b_len_squared(m_b.LenSquared())
	, m_normal(n)
	, m_area((float)(m_a.Length() * m_b.Length()))
	, m_inv_area(1 / m_area)
	, m_sampler(nullptr)
{
}

bool Rectangle::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t = (m_p0 - ray.ori) * m_normal / (ray.dir * m_normal);

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;
	Vector3D d = p - m_p0;

	double ddota = d * m_a;

	if (ddota < 0.0 || ddota > m_a_len_squared)
		return (false);

	double ddotb = d * m_b;

	if (ddotb < 0.0 || ddotb > m_b_len_squared)
		return (false);

	tmin 				= t;
	sr.normal 			= m_normal;
	sr.local_hit_point 	= p;

	return (true);
}

bool Rectangle::ShadowHit(const Ray& ray, float& tmin) const
{
	double t = (m_p0 - ray.ori) * m_normal / (ray.dir * m_normal);

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;
	Vector3D d = p - m_p0;

	double ddota = d * m_a;

	if (ddota < 0.0 || ddota > m_a_len_squared)
		return (false);

	double ddotb = d * m_b;

	if (ddotb < 0.0 || ddotb > m_b_len_squared)
		return (false);

	tmin = (float)t;

	return (true);
}

Point3D Rectangle::Sample() const
{
	Point2D sample_point = m_sampler->SampleUnitSquare();
	return (m_p0 + sample_point.x * m_a + sample_point.y * m_b);
}

float Rectangle::Pdf(const ShadeRec& sr) const
{
	return m_inv_area;
}

Normal Rectangle::GetNormal(const Point3D& p) const
{
	return m_normal;
}

}