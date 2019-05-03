#pragma once

#include "raytracing/cameras/Camera.h"
#include "raytracing/maths/Vector3D.h"

namespace rt
{

class Point2D;

class Pinhole : public Camera
{
public:
	Pinhole();
	
	virtual void RenderScene(const World& w) const;

	void SetViewDistance(const float vpd);

	Vector3D GetDirection(const Point2D& p) const;

private:
	// view plane distance
	float m_d;
	// zoom factor
	float m_zoom;

}; // Pinhole

}