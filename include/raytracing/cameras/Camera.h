#pragma once

#include "raytracing/maths/Point3D.h"

namespace rt
{

class World;

class Camera
{
public:
	Camera();

	virtual void RenderScene(const World& w) const = 0;
    virtual void RenderStereo(const World& w, float x, int pixel_offset) const = 0;

	void SetEye(const Point3D& p);

	void SetLookat(const Point3D& p);

	void SetUpVector(const Vector3D& up);

	void SetRollAngle(float ra);

	void ComputeUVW();

protected:
	// eye point
	Point3D m_eye;
	// look at point
	Point3D m_lookat;

	// up vector
	Vector3D m_up;
	// orthonormal basis vectors
	Vector3D m_u, m_v, m_w;

	float m_exposure_time;

}; // Camera

}
