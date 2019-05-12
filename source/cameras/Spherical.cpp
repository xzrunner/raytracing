#include "raytracing/cameras/Spherical.h"
#include "raytracing/utilities/RGBColor.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/world/ViewPlane.h"
#include "raytracing/world/World.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/tracer/Tracer.h"

namespace rt
{

void Spherical::RenderScene(const World& wr) const
{
	RGBColor	L;
	auto&       vp(wr.GetViewPlane());
	int 		hres		= vp.GetWidth();
	int 		vres 		= vp.GetHeight();
	float		s 			= vp.GetPixelSize();
	Ray			ray;
	int 		depth 		= 0;
	Point2D 	sp; 					// sample point in [0, 1] X [0, 1]
	Point2D 	pp;						// sample point on the pixel
	float		r_squared;				// sum of squares of normalised device coordinates

	//wr.open_window(vp.hres, vp.vres);
	ray.ori = m_eye;

    for (int r = 0; r < vres; r++)		// up
    {
		for (int c = 0; c < hres; c++) // // across
        {
			L = BLACK;

			for (int j = 0; j < vp.GetSamplesNum(); j++)
            {
                sp = vp.GetSampler()->SampleUnitSquare();
				pp.x = s * (c - 0.5f * hres + sp.x);
				pp.y = s * (r - 0.5f * vres + sp.y);
				ray.dir = RayDirection(pp, hres, vres, s, r_squared);

                if (r_squared <= 1.0f) {
                    L += wr.GetTracer()->TraceRay(ray, depth);
                }
			}

			L /= static_cast<float>(vp.GetSamplesNum());
			L *= m_exposure_time;
			wr.DisplayPixel(r, c, L);
		}
    }
}

void Spherical::RenderStereo(const World& wr, float x, int pixel_offset) const
{
	RGBColor	L;
	auto&       vp(wr.GetViewPlane());
	int 		hres		= vp.GetWidth();
	int 		vres 		= vp.GetHeight();
	float		s 			= vp.GetPixelSize();
	Ray			ray;
	int 		depth 		= 0;
	Point2D 	sp; 					// sample point in [0, 1] X [0, 1]
	Point2D 	pp;						// sample point on the pixel
	float		r_squared;				// sum of squares of normalised device coordinates

	//wr.open_window(vp.hres, vp.vres);
	ray.ori = m_eye;

    for (int r = 0; r < vres; r++)		// up
    {
        for (int c = 0; c < hres; c++) 	// across
        {
            L = BLACK;

            for (int j = 0; j < vp.GetSamplesNum(); j++)
            {
                sp = vp.GetSampler()->SampleUnitSquare();
                pp.x = s * (c - 0.5f * hres + sp.x);
                pp.y = s * (r - 0.5f * vres + sp.y);
                ray.dir = RayDirection(pp, hres, vres, s, r_squared);

                if (r_squared <= 1.0f) {
                    L += wr.GetTracer()->TraceRay(ray, depth);
                }
            }

            L /= static_cast<float>(vp.GetSamplesNum());
            L *= m_exposure_time;
            wr.DisplayPixel(r, c + pixel_offset, L);
        }
    }
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