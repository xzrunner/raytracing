#include "raytracing/cameras/Orthographic.h"
#include "raytracing/world/World.h"
#include "raytracing/world/ViewPlane.h"
#include "raytracing/tracer/Tracer.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <tbb/queuing_mutex.h>

namespace rt
{

void Orthographic::RenderScene(const World& world) const
{
	const auto& vp    = world.GetViewPlane();
	const int   depth = 0;	// recursion depth
	const int   n     = (int)sqrt((float)vp.GetSamplesNum());

    const float sz = vp.GetPixelSize() / m_zoom;

	const int w = vp.GetWidth(),
		      h = vp.GetHeight();
    tbb::queuing_mutex mutex;
    tbb::parallel_for(tbb::blocked_range2d<int>(0, w, 0, h),
        [&](const tbb::blocked_range2d<int> &r)
    {
        for (int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++) {
            for (int j = r.cols().begin(), j_end = r.cols().end(); j < j_end; j++) {
                tbb::queuing_mutex::scoped_lock lock(mutex);

                auto L = BLACK;

                Ray ray;
                ray.ori = m_eye;
                for (int p = 0; p < n; p++) {			// up pixel
                    for (int q = 0; q < n; q++) {	// across pixel
                        Point2D pp;
                        pp.x = sz * (i - 0.5f * w + (q + 0.5f) / n);
                        pp.y = sz * (j - 0.5f * h + (p + 0.5f) / n);
                        ray.dir = m_lookat - m_eye;
                        ray.dir.Normalize();

                        Vector3D temp = GetDirection(pp);
                        ray.ori = m_eye + temp;
                        //ray.ori = Point3D(pp.x - ray.dir.x, pp.y - ray.dir.y, (eye - lookat).z);	//Just from positive direction along z axis

                        //ray.ori = eye + pp.x * u + pp.y *v;		//Same to get_direction method, this modification is made from 17.13a
                        L += world.GetTracer()->TraceRay(ray, depth);
                    }
                }

                L /= static_cast<float>(vp.GetSamplesNum());
                L *= m_exposure_time;

                world.DisplayPixel(j, i, L);
            }
        }
    });
}

void Orthographic::RenderStereo(const World& wr, float x, int pixel_offset) const
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
                    auto sp = vp.GetSampler()->SampleUnitSquare();

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

Vector3D Orthographic::GetDirection(const Point2D& p) const
{
    Vector3D dir = p.x * m_u + p.y * m_v - m_d * m_w;	//Here we must know that view plane is not normal plane, so normalize() method would make serious mistake during rendering
	//dir.normalize();
	return(dir);
}

}