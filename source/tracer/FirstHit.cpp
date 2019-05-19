#include "raytracing/tracer/FirstHit.h"
#include "raytracing/world/World.h"
#include "raytracing/materials/Material.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

FirstHit::FirstHit(const World& world)
    : Tracer(world)
{
}

RGBColor FirstHit::TraceRay(const Ray& ray) const
{
    ShadeRec sr(m_world.HitObjects(ray));
    if (sr.hit_an_object) {
        sr.ray = ray;
        return sr.material->Shade(sr);
    } else {
        return m_world.GetBackgroundColor();
    }
}

RGBColor FirstHit::TraceRay(const Ray& ray, int depth) const
{
    return TraceRay(ray);
}

}