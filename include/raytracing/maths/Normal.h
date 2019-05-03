#pragma once

namespace rt
{

class Normal
{
public:
	double x, y, z;

public:
	Normal();
	Normal(double x, double y, double z);
	Normal(const Vector3D& v);

	Normal operator - () const;

	Normal& operator = (const Vector3D& v);

	// dot product with a vector on the right
	double operator * (const Vector3D& v) const;

	void Normalize();

}; // Normal

inline Normal::
Normal()
	: x(0), y(0), z(0)
{
}

inline Normal::
Normal(double x, double y, double z)
	: x(x), y(y), z(z)
{
}

inline Normal::
Normal(const Vector3D& v)
	: x(v.x), y(v.y), z(v.z)
{}

inline
Normal Normal::operator - () const {
	return Normal(-x, -y, -z);
}

inline Normal& Normal::
operator = (const Vector3D& v) {
	x = v.x; y = v.y; z = v.z;
	return *this;
}

inline double Normal::
operator * (const Vector3D& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

inline void Normal::
Normalize()
{
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}

}
