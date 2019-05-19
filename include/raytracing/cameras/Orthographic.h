#pragma once

#include "raytracing/cameras/Camera.h"

namespace rt
{

class Point2D;

class Orthographic : public Camera
{
public:
    Orthographic() {}

    virtual void RenderScene(const World& w) const override;
    virtual void RenderStereo(const World& w, float x, int pixel_offset) const override;

    void SetViewDistance(const float vpd) { m_d = vpd; }
    void SetZoomFactor(float zoom) { m_zoom = zoom; }

    Vector3D GetDirection(const Point2D& p) const;

private:
    float m_d    = 0.0f;	// view plane distance
    float m_zoom = 1.0f;	// zoom factor

}; // Orthographic

}