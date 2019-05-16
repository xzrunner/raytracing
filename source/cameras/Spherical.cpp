#include "raytracing/cameras/Spherical.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/world/ViewPlane.h"
#include "raytracing/world/World.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/tracer/Tracer.h"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <tbb/queuing_mutex.h>

namespace rt
{

void Spherical::RenderScene(const World& wr) const
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
                    wr.DisplayPixel(j, i, L);
                }
            }
        }
    });
}

void Spherical::RenderStereo(const World& wr, float x, int pixel_offset) const
{
	const auto& vp    = wr.GetViewPlane();
	const float s     = vp.GetPixelSize();
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

Vector3D Spherical::RayDirection(const Point2D& pp, int hres, int vres, float s, float& r_squared) const
{
	// compute the normalized device coordinates


	// compute the normalised device coordinates

	Point2D pn( 2.0f / (s * hres) * pp.x, 2.0f / (s * vres) * pp.y);

	// compute the angles lambda and phi in radians

	float lambda 	= static_cast<float>(pn.x * m_lambda_max * PI_ON_180);
	float psi 		= static_cast<float>(pn.y * m_psi_max * PI_ON_180);

	// compute the regular azimuth and polar angles

	float phi 		= static_cast<float>(PI - lambda);
	float theta 	= static_cast<float>(0.5f * PI - psi);

	float sin_phi 	= sin(phi);
	float cos_phi 	= cos(phi);
	float sin_theta = sin(theta);
	float cos_theta = cos(theta);

	Vector3D dir 	= sin_theta * sin_phi * m_u + cos_theta * m_v + sin_theta * cos_phi * m_w;

	return (dir);
}

}