#include "raytracing/cameras/Pinhole.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/world/World.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/tracer/Tracer.h"

namespace rt
{

Pinhole::Pinhole()
	: m_d(500)
	, m_zoom(1.0)
{
}

void Pinhole::RenderScene(const World& world) const
{
	RGBColor	L;
	auto&       vp = world.GetViewPlane();
	Ray			ray;
	int 		depth = 0;	// recursion depth
	Point2D 	sp;			// sample point in [0, 1] x [0, 1]
	Point2D 	pp;			// sample point on a pixel
	int n = (int)sqrt((float)vp.GetSamplesNum());

	float sz = vp.GetPixelSize() / m_zoom;
	ray.ori = m_eye;

	int w = vp.GetWidth(),
		h = vp.GetHeight();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			L = BLACK;
			for (int j = 0; j < vp.GetSamplesNum(); j++) {
				sp = vp.GetSampler()->SampleUnitSquare();
				pp.x = sz * (x - 0.5f * h + sp.x);
				pp.y = sz * (y - 0.5f * w + sp.y);
				ray.dir = GetDirection(pp);
				L += world.GetTracer()->TraceRay(ray, depth);
			}

			L /= static_cast<float>(vp.GetSamplesNum());
			L *= m_exposure_time;

			world.DisplayPixel(y, x, MaxToOneColor(L));
		}
	}
}

void Pinhole::RenderStereo(const World& wr, float x, int pixel_offset) const
{
	RGBColor	L;
	Ray			ray;
	auto&       vp 			= wr.GetViewPlane();
	int 		depth 		= 0;
	Point2D 	sp; 				// sample point in [0, 1] X [0, 1]
	Point2D 	pp;					// sample point on the pixel

    float sz = vp.GetPixelSize() / m_zoom;
	ray.ori = m_eye;

    int w = vp.GetWidth(),
        h = vp.GetHeight();
    for (int r = 0; r < h; r++)			// up
    {
        for (int c = 0; c < w; c++)		// across
        {
            L = BLACK;

            for (int j = 0; j < vp.GetSamplesNum(); j++)
            {
                sp = vp.GetSampler()->SampleUnitSquare();
                pp.x = sz * (c - 0.5f * h + sp.x) + x; 	// asymmetric view frustum
                pp.y = sz * (r - 0.5f * w + sp.y);
                ray.dir = GetDirection(pp);
                L += wr.GetTracer()->TraceRay(ray, depth);
            }

            L /= static_cast<float>(vp.GetSamplesNum());
            L *= m_exposure_time;
            wr.DisplayPixel(r, c + pixel_offset, L);
        }
    }
}

void Pinhole::SetViewDistance(const float vpd)
{
	m_d = vpd;
}

Vector3D Pinhole::GetDirection(const Point2D& p) const
{
	Vector3D dir = p.x * m_u + p.y * m_v - m_d * m_w;
	dir.Normalize();
	return dir;
}

}