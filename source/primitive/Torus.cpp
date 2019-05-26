#include "raytracing/primitive/Torus.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/maths.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Torus::Torus()
    : m_a(2.0)
    , m_b(0.5)
    , m_bbox(-m_a - m_b, m_a + m_b, -m_b, m_b, -m_a - m_b, m_a + m_b)
{
}

Torus::Torus(double a, double b)
    : m_a(a)
    , m_b(b)
    , m_bbox(-a - b, a + b, -b, b, -a - b, a + b)
{
}

bool Torus::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (!m_bbox.Hit(ray))
		return (false);

	double x1 = ray.ori.x; double y1 = ray.ori.y; double z1 = ray.ori.z;
	double d1 = ray.dir.x; double d2 = ray.dir.y; double d3 = ray.dir.z;

	double coeffs[5];	// coefficient array for the quartic equation
	double roots[4];	// solution array for the quartic equation

	// define the coefficients of the quartic equation

	double sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	double e			= x1 * x1 + y1 * y1 + z1 * z1 - m_a * m_a - m_b * m_b;
	double f			= x1 * d1 + y1 * d2 + z1 * d3;
	double four_a_sqrd	= 4.0 * m_a * m_a;

	coeffs[0] = e * e - four_a_sqrd * (m_b * m_b - y1 * y1); 	// constant term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4

	// find roots of the quartic equation

	int num_real_roots = SolveQuartic(coeffs, roots);

	bool	intersected = false;
	double 	t 		 	= HUGE_VALUE;

	if (num_real_roots == 0)  // ray misses the torus
		return(false);

	// find the smallest root greater than EPSILON, if any
	// the roots array is not sorted

	for (int j = 0; j < num_real_roots; j++)
		if (roots[j] > EPSILON) {
			intersected = true;
			if (roots[j] < t)
				t = roots[j];
		}

	if(!intersected)
		return (false);

	tmin 			 	= t;
	sr.local_hit_point 	= ray.ori + t * ray.dir;
	sr.normal 		 	= ComputeNormal(sr.local_hit_point);

	return (true);
}

bool Torus::ShadowHit(const Ray& ray, float& tmin) const
{
	if (!m_shadows || !m_bbox.Hit(ray))
		return (false);

	double x1 = ray.ori.x; double y1 = ray.ori.y; double z1 = ray.ori.z;
	double d1 = ray.dir.x; double d2 = ray.dir.y; double d3 = ray.dir.z;

	double coeffs[5];	// coefficient array for the quartic equation
	double roots[4];	// solution array for the quartic equation

	// define the coefficients of the quartic equation

	double sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	double e			= x1 * x1 + y1 * y1 + z1 * z1 - m_a * m_a - m_b * m_b;
	double f			= x1 * d1 + y1 * d2 + z1 * d3;
	double four_a_sqrd	= 4.0 * m_a * m_a;

	coeffs[0] = e * e - four_a_sqrd * (m_b * m_b - y1 * y1); 	// constant term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4

	// find roots of the quartic equation

	int num_real_roots = SolveQuartic(coeffs, roots);

	bool	intersected = false;
	double 	t 		 	= HUGE_VALUE;

	if (num_real_roots == 0)  // ray misses the torus
		return(false);

	// find the smallest root greater than EPSILON, if any
	// the roots array is not sorted

	for (int j = 0; j < num_real_roots; j++)
		if (roots[j] > EPSILON) {
			intersected = true;
			if (roots[j] < t)
				t = roots[j];
		}

	if(!intersected)
		return (false);

	tmin = static_cast<float>(t);
	return (true);
}

Normal Torus::ComputeNormal(const Point3D& p) const
{
    Normal normal;
    double param_squared = m_a * m_a + m_b * m_b;

    double x = p.x;
    double y = p.y;
    double z = p.z;
    double sum_squared = x * x + y * y + z * z;

    normal.x = 4.0 * x * (sum_squared - param_squared);
    normal.y = 4.0 * y * (sum_squared - param_squared + 2.0 * m_a * m_a);
    normal.z = 4.0 * z * (sum_squared - param_squared);
    normal.Normalize();

    return (normal);
}

}