#pragma once

#include "raytracing/cameras/Camera.h"
#include "raytracing/maths/Point2D.h"

#include <memory>

namespace rt
{

class Sampler;

class ThinLens : public Camera
{
public:
	ThinLens();

	virtual void RenderScene(const World& w) const override;
    virtual void RenderStereo(const World& w, float x, int pixel_offset) const override;

	void SetLensRadius(float radius) { m_lens_radius = radius; }
	void SetViewDistance(const float vpd) { m_d = vpd; }
	void SetFocalDistance(const float f) { m_f = f; }
	void SetZoom(const float zoom_factor) { m_zoom = zoom_factor; }
	void SetSampler(const std::shared_ptr<Sampler>& sampler);

private:
	Vector3D RayDirection(const Point2D& pixel_point, const Point2D& lens_point) const;

private:
	float m_lens_radius;	// lens radius
	float m_d;			    // view plane distance
	float m_f;			    // focal distance
	float m_zoom;			// zoom factor
	std::shared_ptr<Sampler> m_sampler = nullptr;		// sampler object

}; // ThinLens

}
