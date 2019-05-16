#pragma once

#include "raytracing/objects/Object.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Sampler;

class Disk : public Object
{
public:
    Disk() {}
    Disk(const Point3D& point, const Normal& normal, double radius);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

    virtual Point3D Sample() const override;
    virtual float Pdf(const ShadeRec& sr) const override { return m_inv_area; }

    // The following two functions allow us to simplify the code for smooth shaded triangle meshes
    virtual Normal GetNormal(void) const override { return m_normal; }
    virtual Normal GetNormal(const Point3D& p) const override { return m_normal; }

    void SetSampler(const std::shared_ptr<Sampler>& sampler) {
        m_sampler = sampler;
    }
private:
    Point3D m_center;   				    // point through which Disk passes
    Normal 	m_normal = Normal(0, 1, 0);     // m_normal to the Disk
    double  m_r_squared = 1;
    double  m_r = 1;

    std::shared_ptr<Sampler> m_sampler = nullptr;
    bool m_shadows = false;

    float m_area = 1.0f;			// for disk lights
    float m_inv_area = 1.0f;		// for disk lights
    Vector3D m_u, m_v, m_w;

}; // Disk

}