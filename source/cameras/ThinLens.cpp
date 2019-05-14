#include "raytracing/cameras/ThinLens.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/world/ViewPlane.h"
#include "raytracing/world/World.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <tbb/queuing_mutex.h>

namespace rt
{

ThinLens::ThinLens()
	: m_lens_radius(1)
	, m_d(500)
	, m_f(75)
	, m_zoom(1)
	, m_sampler(nullptr)
{
}

void ThinLens::RenderScene(const World& world) const
{
	const auto& vp    = world.GetViewPlane();
	const int   depth = 0;
    const float sz    = vp.GetPixelSize() / m_zoom;

	const int w = vp.GetWidth(),
		      h = vp.GetHeight();
    tbb::queuing_mutex mutex;
    tbb::parallel_for(tbb::blocked_range2d<int>(0, w, 0, h),
        [&](const tbb::blocked_range2d<int> &r)
    {
        for (int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++) {
            for (int j = r.cols().begin(), j_end = r.cols().end(); j < j_end; j++) {
                RGBColor L = BLACK;
                for (int k = 0; k < vp.GetSamplesNum(); k++) {
                    // sample point in [0, 1] X [0, 1]
                    auto sp = vp.GetSampler()->SampleUnitSquare();

                    // sample point on a pixel
                    Point2D pp;
                    pp.x = sz * (i - w / 2.0f + sp.x);
                    pp.y = sz * (j - h / 2.0f + sp.y);

                    // sample point on unit disk
                    auto dp = m_sampler->SampleUnitDisk();
                    // sample point on lens
                    auto lp = dp * m_lens_radius;

                    Ray ray;
                    ray.ori = m_eye + lp.x * m_u + lp.y * m_v;
                    ray.dir = RayDirection(pp, lp);
                    L += world.GetTracer()->TraceRay(ray, depth);
                }

                L /= static_cast<float>(vp.GetSamplesNum());
                L *= m_exposure_time;

                {
                    tbb::queuing_mutex::scoped_lock lock(mutex);
                    world.DisplayPixel(j, i, L/*MaxToOneColor(L)*/);
                }
            }
        }
    });
}

void ThinLens::RenderStereo(const World& wr, float x, int pixel_offset) const
{
	const auto& vp    = wr.GetViewPlane();
	const int   depth = 0;
    const float sz    = vp.GetPixelSize() / m_zoom;

	//w.open_window(vp.hres, vp.vres);

    const int w = vp.GetWidth(),
              h = vp.GetHeight();
    tbb::queuing_mutex mutex;
    tbb::parallel_for(tbb::blocked_range2d<int>(0, w, 0, h),
        [&](const tbb::blocked_range2d<int> &r)
    {
        for (int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++) {
            for (int j = r.cols().begin(), j_end = r.cols().end(); j < j_end; j++) {
                RGBColor L = BLACK;
                for (int k = 0; k < vp.GetSamplesNum(); k++)
                {
                    // sample point in [0, 1] X [0, 1]
                    auto sp = vp.GetSampler()->SampleUnitSquare();

                    // sample point on a pixel
                    Point2D pp;
                    pp.x = sz * (i - w / 2.0f + sp.x);
                    pp.y = sz * (j - h / 2.0f + sp.y);

                    // sample point on unit disk
                    auto dp = m_sampler->SampleUnitDisk();
                    // sample point on lens
                    auto lp = dp * m_lens_radius;

                    Ray ray;
                    ray.ori = m_eye + lp.x * m_u + lp.y * m_v;
                    ray.dir = RayDirection(pp, lp);
                    L += wr.GetTracer()->TraceRay(ray, depth);
                }

                L /= static_cast<float>(vp.GetSamplesNum());
                L *= m_exposure_time;

                {
                    tbb::queuing_mutex::scoped_lock lock(mutex);
                    wr.DisplayPixel(j, i + pixel_offset, L/*MaxToOneColor(L)*/);
                }
            }
        }
    });
}

// ----------------------------------------------------------------------------- ray direction
// explained on page 173
Vector3D ThinLens::RayDirection(const Point2D& pixel_point, const Point2D& lens_point) const
{
	// hit point on focal plane
	Point2D p(pixel_point.x * m_f / m_d, pixel_point.y * m_f / m_d);
	Vector3D dir = (p.x - lens_point.x) * m_u + (p.y - lens_point.y) * m_v - m_f * m_w;
	dir.Normalize();
	return dir;
}

void ThinLens::SetSampler(const std::shared_ptr<Sampler>& sampler)
{
    m_sampler = sampler;
	m_sampler->MapSamplesToUnitDisk();
}

}