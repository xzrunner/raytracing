#pragma once

#include "raytracing/camera/Camera.h"
#include "raytracing/maths/Vector3D.h"

namespace rt
{

class Point2D;

class Pinhole : public Camera
{
public:
	Pinhole();

	virtual void RenderScene(const World& w) const override;
    virtual void RenderStereo(const World& w, float x, int pixel_offset) const override;

	void SetViewDistance(const float vpd) { m_d = vpd; }
    void SetZoomFactor(float zoom) { m_zoom = zoom; }

	Vector3D GetDirection(const Point2D& p) const;

private:
	// view plane distance
	float m_d;
	// zoom factor
	float m_zoom;

}; // Pinhole

}