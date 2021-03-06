#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/Matrix.h"

namespace rt
{

class GeoInstance : public GeoPrimitive
{
public:
	GeoInstance();
	GeoInstance(const std::shared_ptr<GeoPrimitive>& obj);

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool ShadowHit(const Ray& ray, float& t) const;

	virtual AABB GetBoundingBox() const;

	void ComputeBoundingBox();

	void Translate(const Vector3D& trans);
	void Scale(const Vector3D& scale);
	void RotateX(double r);
	void RotateY(double r);
	void RotateZ(double r);
	void Shear(const Matrix& m);

    void SetTransformTexture(bool transform) { m_transform_the_texture = transform; }

private:
	std::shared_ptr<GeoPrimitive> m_object = nullptr;  // object to be transformed
	Matrix				m_inv_matrix;		    	// inverse transformation matrix
	static 	Matrix		m_forward_matrix; 			// transformation matrix
	AABB				m_aabb;						// transformed object's bounding box
	bool				m_transform_the_texture;	// do we transform the texture?

}; // GeoInstance

}
