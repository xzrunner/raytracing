#include "raytracing/cameras/FishEye.h"
#include "raytracing/world/ViewPlane.h"
#include "raytracing/world/World.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <tbb/queuing_mutex.h>

namespace rt
{

FishEye::FishEye()
	: m_psi_max(180.0)
{
}

void FishEye::RenderScene(const World& wr) const
{
	const auto& vp    = wr.GetViewPlane();
	const float s     = vp.GetPixelSize();
	const int   depth = 0;

	const int w = vp.GetWidth(),
		      h = vp.GetHeight();
    tbb::queuing_mutex mutex;
    tbb::parallel_for(tbb::blocked_range2d<int>(0, w, 0, h),
        [&](const tbb::blocked_range2d<int> &r)
    {
        for (int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++) {
            for (int j = r.cols().begin(), j_end = r.cols().end(); j < j_end; j++) {
                auto L = BLACK;

                Ray ray;
                ray.ori = m_eye;
                for (int k = 0; k < vp.GetSamplesNum(); k++) {
                    // sample point in [0, 1] x [0, 1]
                    auto sp = vp.GetSampler()->SampleUnitSquare();

                    // sample point on a pixel
                    Point2D pp;
                    pp.x = s * (i - 0.5f * w + sp.x);
                    pp.y = s * (j - 0.5f * h + sp.y);

                    // sum of squares of normalised device coordinates
                    float r_squared;
                    ray.dir = RayDirection(pp, w, h, s, r_squared);

                    if (r_squared <= 1.0f) {
                        L += wr.GetTracer()->TraceRay(ray, depth);
                    }
                }

                L /= static_cast<float>(vp.GetSamplesNum());
                L *= m_exposure_time;

                {
                    tbb::queuing_mutex::scoped_lock lock(mutex);
                    wr.DisplayPixel(j, i, L);
                }
            }
        }
    });
}

void FishEye::RenderStereo(const World& wr, float x, int pixel_offset) const
{
	const auto& vp    = wr.GetViewPlane();
	const float	s     = vp.GetPixelSize();
	const int   depth = 0;

	//wr.open_window(vp.hres, vp.vres);

    const int w = vp.GetWidth(),
              h = vp.GetHeight();
    tbb::queuing_mutex mutex;
    tbb::parallel_for(tbb::blocked_range2d<int>(0, w, 0, h),
        [&](const tbb::blocked_range2d<int> &r)
    {
        for (int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++) {
            for (int j = r.cols().begin(), j_end = r.cols().end(); j < j_end; j++) {
                RGBColor L = BLACK;

                Ray ray;
                ray.ori = m_eye;
                for (int k = 0; k < vp.GetSamplesNum(); k++)
                {
                    // sample point in [0, 1] x [0, 1]
                    auto sp = vp.GetSampler()->SampleUnitSquare();

                    // sample point on a pixel
                    Point2D pp;
                    pp.x = s * (i - 0.5f * w + sp.x);
                    pp.y = s * (j - 0.5f * h + sp.y);

                    // sum of squares of normalised device coordinates
                    float r_squared;
                    ray.dir = RayDirection(pp, w, h, s, r_squared);

                    if (r_squared <= 1.0f) {
                        L += wr.GetTracer()->TraceRay(ray, depth);
                    }
                }

                L /= static_cast<float>(vp.GetSamplesNum());
                L *= m_exposure_time;

                {
                    tbb::queuing_mutex::scoped_lock lock(mutex);
                    wr.DisplayPixel(j, i + pixel_offset, L);
                }
            }
        }
    });
}

void FishEye::SetFov(float fov)
{
	m_psi_max = fov;
}

Vector3D FishEye::RayDirection(const Point2D& pp, const int hres, const int vres,
							   const float s, float& r_squared) const
{
	// compute the normalized device coordinates
	Point2D pn(2.0f / (s * hres) * pp.x, 2.0f / (s * vres) * pp.y);
	r_squared = pn.x * pn.x + pn.y * pn.y;

	if (r_squared <= 1.0f) {
		float r 		= sqrt(r_squared);
		float psi 		= static_cast<float>(r * m_psi_max * PI_ON_180);
		float sin_psi 	= sin(psi);
		float cos_psi 	= cos(psi);
		float sin_alpha = pn.y / r;
		float cos_alpha = pn.x / r;
		Vector3D dir 	= sin_psi * cos_alpha * m_u +  sin_psi * sin_alpha * m_v - cos_psi * m_w;

		return dir;
	} else {
		return Vector3D(0, 0, 0);
	}
}

}