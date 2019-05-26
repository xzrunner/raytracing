#include "raytracing/light/FakeSphericalLight.h"
#include "raytracing/world/World.h"
#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/utilities/ShadeRec.h"

#include <random>

namespace
{

std::random_device rd;

//
// Engines
//
std::mt19937 e2(rd());
//std::knuth_b e2(rd());
//std::default_random_engine e2(rd()) ;

//
// Distribtuions
//
std::uniform_real_distribution<> dist(0, 1);

}

namespace rt
{

RGBColor FakeSphericalLight::L(const ShadeRec& sr) const
{
    if (!m_distance_attenuation)
    {
        return m_ls * m_color;
    }
	else
	{
		//Here we can use many attenuation functions, but for better viewing with arguments in 18.12a, we only use 1/d^2 finally
		float d = sr.hit_point.Distance(m_location);
		return m_ls * m_color / (d * d);		//distance attenuation
	}
}

Vector3D FakeSphericalLight::GetDirection(const ShadeRec& sr) const
{
    float r = this->m_jitter_amount;//3.0;
	Point3D new_location;
	new_location.x = static_cast<float>(m_location.x + r * (2.0 * dist(e2) - 1.0));
	new_location.y = static_cast<float>(m_location.y + r * (2.0 * dist(e2) - 1.0));
	new_location.z = static_cast<float>(m_location.z + r * (2.0 * dist(e2) - 1.0));

	return (new_location - sr.local_hit_point).Normalized();
	//return ((location - sr.local_hit_point).hat());			//here use local_hit_point instead of hit_point
}

bool FakeSphericalLight::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	float t = 10000;		// may be need an initialization
	int num_objects = sr.w.GetObjects().size();
	double d = m_location.Distance(ray.ori);

    for (int j = 0; j < num_objects; j++) {
        if (sr.w.GetObjects()[j]->ShadowHit(ray, t) && t < d) {
            return (true);
        }
    }
	return (false);
}

}