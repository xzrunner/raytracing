#include "raytracing/primitive/ConcavePartTorus.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/maths/maths.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

ConcavePartTorus::ConcavePartTorus()
    : bbox(-a - b, a + b, -b, b, -a - b, a + b)
{
}

ConcavePartTorus::ConcavePartTorus(double a, double b, double min_azimuth, double max_azimuth, double min_polar, double max_polar)
	: a(a)
    , b(b)
    , min_azimuth(min_azimuth * PI_ON_180)
    , max_azimuth(max_azimuth * PI_ON_180)
    , min_polar(min_polar * PI_ON_180)
    , max_polar(max_polar * PI_ON_180)
    , bbox(-a - b, a + b, -b, b, -a - b, a + b)
{
}

bool ConcavePartTorus::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (!bbox.Hit(ray))
		return (false);

	double x1 = ray.ori.x; double y1 = ray.ori.y; double z1 = ray.ori.z;
	double d1 = ray.dir.x; double d2 = ray.dir.y; double d3 = ray.dir.z;

	double coeffs[5];	// coefficient array for the quartic equation
	double roots[4];	// solution array for the quartic equation

	// define the coefficients of the quartic equation

	double sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	double e			= x1 * x1 + y1 * y1 + z1 * z1 - a * a - b * b;
	double f			= x1 * d1 + y1 * d2 + z1 * d3;
	double four_a_sqrd	= 4.0 * a * a;

	coeffs[0] = e * e - four_a_sqrd * (b * b - y1 * y1); 	// constant term
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
	// the roots array must be sorted in part torus

	for (int j = 0; j < num_real_roots; j++)
	{
		t = roots[j];
		if (roots[j] > EPSILON) {
			intersected = true;
		}
		for( int k = j; k < num_real_roots; ++k)
		{
			if( roots[k] < t)
			{
				roots[j] = roots[k];
				roots[k] = t;
				t = roots[k];
			}
		}
		/*if (roots[j] < t)
				t = roots[j];*/
	}

	//if (roots[j] > EPSILON) {
	if(!intersected)
		return (false);

	for(int j = 0; j < num_real_roots; ++j)
	{
		if (roots[j] > EPSILON )
		{
			t = roots[j];
			Point3D Hit = ray.ori + t * ray.dir;
			double phi = atan2(Hit.x, Hit.z);
					if (phi < 0.0)
						phi += TWO_PI;

			if(phi >= min_azimuth && phi <= max_azimuth)
			{
				Point3D point(a * cos(-phi+PI/2),0,a * sin(-phi+PI/2));

				Vector3D dir = Hit - point;
				dir.Normalize();

				if( Hit.y <= b * cos(min_polar) && Hit.y >= b * cos(max_polar) )
				{
					tmin 			 	= t;
					sr.local_hit_point 	= ray.ori + t * ray.dir;
					if(Hit.Distance(Point3D(0,0,0)) > a)
					{
						sr.normal 		 	= ComputeNormal(sr.local_hit_point);
					}
					else
					{
						sr.normal 		 	= -ComputeNormal(sr.local_hit_point);
					}
				}
				return (true);
			}
		}
	}
	/*
	Point3D Hit = ray.ori + t * ray.dir;
	double phi = atan2(Hit.x, Hit.z);
			if (phi < 0.0)
				phi += TWO_PI;

	if(phi >= min_azimuth && phi <= max_azimuth)
	{
		Point3D point(a * cos(-phi+PI/2),0,a * sin(-phi+PI/2));

		Vector3D dir = Hit - point;
		dir.Normalize();

		Hit.y = 0;

		double angle = acos(dir * Vector3D(0,1,0));

		if(Hit.distance(Point3D(0,0,0)) > a && angle < min_polar )
		{
			if( angle >= min_polar && angle <= max_polar )
			{

			}
		}
		else
		{

		}

		tmin 			 	= t;
		sr.local_hit_point 	= ray.ori + t * ray.dir;
		sr.normal 		 	= ComputeNormal(sr.local_hit_point);

		return (true);
	}*/

	return false;
}

bool ConcavePartTorus::ShadowHit(const Ray& ray, float& tmin) const
{
	if (!m_shadows || !bbox.Hit(ray))
		return (false);

	double x1 = ray.ori.x; double y1 = ray.ori.y; double z1 = ray.ori.z;
	double d1 = ray.dir.x; double d2 = ray.dir.y; double d3 = ray.dir.z;

	double coeffs[5];	// coefficient array for the quartic equation
	double roots[4];	// solution array for the quartic equation

	// define the coefficients of the quartic equation

	double sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	double e			= x1 * x1 + y1 * y1 + z1 * z1 - a * a - b * b;
	double f			= x1 * d1 + y1 * d2 + z1 * d3;
	double four_a_sqrd	= 4.0 * a * a;

	coeffs[0] = e * e - four_a_sqrd * (b * b - y1 * y1); 	// constant term
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

Normal ConcavePartTorus::ComputeNormal(const Point3D& p) const
{
    Normal normal;
	double param_squared = a * a + b * b;

	double x = p.x;
	double y = p.y;
	double z = p.z;
	double sum_squared = x * x + y * y + z * z;

	normal.x = 4.0 * x * (sum_squared - param_squared);
	normal.y = 4.0 * y * (sum_squared - param_squared + 2.0 * a * a);
	normal.z = 4.0 * z * (sum_squared - param_squared);
	normal.Normalize();

	return (normal);
}

}