#include "raytracing/world/World.h"
#include "raytracing/world/RenderOutput.h"
#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/light/Light.h"
#include "raytracing/light/Ambient.h"
#include "raytracing/sampler/Sampler.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/camera/Camera.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/maths/Point3D.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"

#include <algorithm>

#include <float.h>
#include <assert.h>

namespace rt
{

World::World()
	: m_background_color(BLACK)
{
}

World::~World()
{
}

//void World::RenderScene()
//{
//	RGBColor pixel_color;
//	Ray	ray;
//	Point2D pos;
//
//	int width = m_vp.GetWidth(),
//		height = m_vp.GetHeight();
//	float size = m_vp.GetPixelSize();
//
//	// hardwired in
//	float zw = 100.0;
//
//	for (int y = 0, h = m_vp.GetHeight(); y < h; ++y) {
//		for (int x = 0, w = m_vp.GetWidth(); x < w; ++x) {
//			for (int i = 0, n = m_vp.GetSamplesNum(); i < n; ++i) {
//				const Point2D& sp = m_vp.GetSampler()->SampleUnitSquare();
//				pos.x = size * (x - 0.5f * width + sp.x);
//				pos.y = size * (y - 0.5f * height + sp.y);
//				ray.ori = Point3D(size * (x - width / 2.0f + 0.5f), size * (y - height / 2.0f + 0.5f), zw);
//				pixel_color += m_tracer->TraceRay(ray);
//			}
//		}
//	}
//}

ShadeRec World::HitObjects(const Ray& ray) const
{
	ShadeRec	sr(*this);
	double		t;
	Normal		normal;
	Point3D		local_hit_point;
	double		tmin = DBL_MAX;
	for (int i = 0, n = m_objects.size(); i < n; i++)
	{
		if (m_objects[i]->Hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object	= true;
			tmin 				= t;
			sr.material			= m_objects[i]->GetMaterial();
			sr.hit_point 		= ray.ori + t * ray.dir;
			normal 				= sr.normal;
			local_hit_point	 	= sr.local_hit_point;
		}
	}

	if(sr.hit_an_object) {
		sr.t = tmin;
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}

	return(sr);
}

void World::AddObject(std::unique_ptr<GeoPrimitive> obj)
{
	m_objects.push_back(std::move(obj));
}

void World::AddLight(std::unique_ptr<Light> light)
{
	m_lights.push_back(std::move(light));
}

void World::SetAmbient(std::unique_ptr<Light> ambient)
{
    m_ambient = std::move(ambient);
}

void World::SetCamera(std::unique_ptr<Camera> camera)
{
    m_camera = std::move(camera);
}

void World::SetTracer(std::unique_ptr<Tracer> tracer)
{
    m_tracer = std::move(tracer);
}

void World::DisplayPixel(const int row, const int column, const RGBColor& raw_color) const
{
	RGBColor mapped_color;

	if (m_vp.ShowOutOfGamut()) {
		mapped_color = ClampToColor(raw_color);
	} else {
		mapped_color = MaxToOneColor(raw_color);
	}

	if (m_vp.GetGamma() != 1.0)
		mapped_color = mapped_color.Powc(m_vp.GetInvGamma());

	//have to start from max y coordinate to convert to screen coordinates
	int x = column;
	int y = m_vp.GetHeight() - row - 1;

    assert(m_output);
	m_output->SetPixel(x, y, (int)(mapped_color.r * 255),
		(int)(mapped_color.g * 255),
		(int)(mapped_color.b * 255));
}

}