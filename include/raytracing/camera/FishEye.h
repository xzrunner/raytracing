#pragma once

#include "raytracing/camera/Camera.h"
#include "raytracing/maths/Point2D.h"

namespace rt
{

class FishEye : public Camera
{
public:
	FishEye();

	virtual void RenderScene(const World& w) const override;
    virtual void RenderStereo(const World& w, float x, int pixel_offset) const override;

	void SetFov(float fov);

private:
	Vector3D RayDirection(const Point2D& pixel_point, const int hres,
		const int vres, const float s, float& r_squared) const;

private:
	// in degrees
	float m_psi_max;

}; // FishEye

}
