#include "raytracing/cameras/Pinhole.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/world/World.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/tracer/Tracer.h"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <tbb/queuing_mutex.h>

namespace rt
{

Pinhole::Pinhole()
	: m_d(500)
	, m_zoom(1.0)
{
}

void Pinhole::RenderScene(const World& world) const
{
	const auto& vp    = world.GetViewPlane();
	const int   depth = 0;	// recursion depth
	const int   n     = (int)sqrt((float)vp.GetSamplesNum());

	const float sz = vp.GetPixelSize() / m_zoom;

	const int w = vp.GetWidth(),
		      h = vp.GetHeight();
    tbb::queuing_mutex mutex;
    tbb::parallel_for(tbb::blocked_range2d<int>(0, w, 0, h),
        [&]( const tbb::blocked_range2d<int> &r )
    {
        for(int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++) {
            for(int j = r.cols().begin(), j_end = r.cols().end(); j < j_end; j++) {
                tbb::queuing_mutex::scoped_lock lock(mutex);

                auto L = BLACK;

                Ray ray;
                ray.ori = m_eye;
                for(int k = 0; k < vp.GetSamplesNum(); k++)
                {
                    // sample point in [0, 1] x [0, 1]
                    auto sp = vp.GetSampler()->SampleUnitSquare();

                    // sample point on a pixel
                    Point2D pp;
                    pp.x = sz * (i - 0.5f * w + sp.x);
                    pp.y = sz * (j - 0.5f * h + sp.y);

                    ray.dir = GetDirection(pp);
                    L += world.GetTracer()->TraceRay(ray, depth);
                }

                L /= static_cast<float>(vp.GetSamplesNum());
                L *= m_exposure_time;

                world.DisplayPixel(j, i, L);
            }
        }
    });
}

void Pinhole::RenderStereo(const World& wr, float x, int pixel_offset) const
{
	const auto& vp    = wr.GetViewPlane();
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
                tbb::queuing_mutex::scoped_lock lock(mutex);

                RGBColor L = BLACK;

                Ray ray;
                ray.ori = m_eye;
                for (int k = 0; k < vp.GetSamplesNum(); k++)
                {
                    // sample point in [0, 1] x [0, 1]
                    auto sp = vp.GetSampler()->SampleUnitSquare();

                    // sample point on a pixel
                    Point2D pp;
                    pp.x = sz * (i - 0.5f * w + sp.x) + x; 	// asymmetric view frustum
                    pp.y = sz * (j - 0.5f * h + sp.y);

                    ray.dir = GetDirection(pp);
                    L += wr.GetTracer()->TraceRay(ray, depth);
                }

                L /= static_cast<float>(vp.GetSamplesNum());
                L *= m_exposure_time;

                wr.DisplayPixel(j, i + pixel_offset, L);
            }
        }
    });
}

Vector3D Pinhole::GetDirection(const Point2D& p) const
{
	Vector3D dir = p.x * m_u + p.y * m_v - m_d * m_w;
	dir.Normalize();
	return dir;
}

}