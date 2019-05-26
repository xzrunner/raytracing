#pragma once

#include "raytracing/camera/Camera.h"

#include <memory>

namespace rt
{

class Stereo : public Camera
{
public:
	Stereo();
	Stereo(const std::shared_ptr<Camera>& left,
        const std::shared_ptr<Camera>& right);

    virtual void RenderScene(const World& w) const override;
    virtual void RenderStereo(const World& w, float x, int pixel_offset) const override;

    void SetLeftCam(const std::shared_ptr<Camera>& cam) { m_left_cam = cam; }
    void SetRightCam(const std::shared_ptr<Camera>& cam) { m_right_cam = cam; }

    enum class ViewingType
    {
        Parallel,
        Transverse
    };
    void SetViewingType(ViewingType type) { m_viewing_type = type; }

	void SetPixelGap(int g) { m_pixel_gap = g; }
	void SetStereoAngle(double b) { m_beta = static_cast<float>(b); }

	void SetupCameras();

private:
	ViewingType	m_viewing_type = ViewingType::Parallel; // parallel or transverse viewing
	int			m_pixel_gap    = 0;		                // gap in pixels between the left and right images
	float		m_beta         = 0;			            // stereo separation angle
	std::shared_ptr<Camera> m_left_cam  = nullptr;	    // left eye camera, note: here we use Pinhole directly to avoid too many stereo function
    std::shared_ptr<Camera> m_right_cam = nullptr;	    // right eye camera

}; // Stereo

}