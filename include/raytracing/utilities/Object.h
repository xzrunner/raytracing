#pragma once

namespace rt
{

class Object
{
public:
	Object();
	virtual ~Object() {}

	virtual void Retain() const;
	virtual void Release() const;

private:
	mutable int m_count;

}; // Object

inline void obj_assign(const Object*& left, const Object* right);

}

#include "raytracing/utilities/Object.inl"
