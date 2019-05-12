#pragma once

#include "raytracing/cameras/Camera.h"
#include "raytracing/maths/Vector3D.h"

namespace rt
{

class Point2D;

class Spherical : public Camera
{
public:
	virtual void RenderScene(const World& w) const override;
    virtual void RenderStereo(const World& w, float x, int pixel_offset) const override;

    void SetHorizontalFov(const float d) { m_lambda_max = d / 2; }
    void SetVerticalFov(const float d) { m_psi_max = d / 2; }

private:
    Vector3D RayDirection(const Point2D& pp, int hres, int vres, float s, float& r_squared) const;

private:
	float m_psi_max = 0;	// in degrees
	float m_lambda_max = 0;

}; // Spherical

}