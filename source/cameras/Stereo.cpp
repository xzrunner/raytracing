#include "raytracing/cameras/Stereo.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/world/ViewPlane.h"
#include "raytracing/world/World.h"
#include "raytracing/maths/Vector3D.h"

namespace rt
{

Stereo::Stereo(void)
{
}

Stereo::Stereo(const std::shared_ptr<Camera>& left,
               const std::shared_ptr<Camera>& right)
    : m_left_cam(left)
    , m_right_cam(right)
{
}

void
Stereo::RenderScene(const World& w) const
{
	auto& vp   = w.GetViewPlane();
	int   hres = vp.GetWidth();
	int   vres = vp.GetHeight();

	//w.open_window(2 * hres + pixel_gap, vres);

    float r = m_eye.Distance(m_lookat);
	float x = static_cast<float>(r * tan(0.5f * m_beta * PI_ON_180));

	if (m_viewing_type == ViewingType::Parallel) {
		m_left_cam->RenderStereo(w, x, 0);						// left view on left
		m_right_cam->RenderStereo(w, -x, hres + m_pixel_gap);   // right view on right
	}

	if (m_viewing_type == ViewingType::Transverse) {
		m_right_cam->RenderStereo(w, -x, 0);   					// right view on left
		m_left_cam->RenderStereo(w, x, hres + m_pixel_gap);    	// left view on right
	}
}

void Stereo::RenderStereo(const World& w, float x, int pixel_offset) const
{
    auto& vp   = w.GetViewPlane();
	int   hres = vp.GetWidth();
	int   vres = vp.GetHeight();

	//w.open_window(2 * hres + pixel_gap, vres);

	double r = m_eye.Distance(m_lookat);
	//double x = r * tan(0.5 * m_beta * PI_ON_180);

	if (m_viewing_type == ViewingType::Parallel) {
		m_left_cam->RenderStereo(w, x, 0);						// left view on left
		m_right_cam->RenderStereo(w, -x, hres + m_pixel_gap);   // right view on right
	}

	if (m_viewing_type == ViewingType::Transverse) {
		m_right_cam->RenderStereo(w, -x, 0);   					// right view on left
		m_left_cam->RenderStereo(w, x, hres + m_pixel_gap);    	// left view on right
	}
}

void Stereo::SetupCameras()
{
	double r = m_eye.Distance(m_lookat);
	double x = r * tan(0.5 * m_beta * PI_ON_180);  //  half the camera separation distance

	m_left_cam->SetEye(Vector3D(m_eye) - x * m_u);
	m_left_cam->SetLookat(Vector3D(m_lookat) - x * m_u);
	m_left_cam->ComputeUVW();

	m_right_cam->SetEye(m_eye + x * m_u);
	m_right_cam->SetLookat(m_lookat + x * m_u);
	m_right_cam->ComputeUVW();
}

}