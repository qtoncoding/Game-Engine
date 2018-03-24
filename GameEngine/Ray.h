#pragma once
#include "Vec3.h"

class Ray 
{
public:
	Ray() = delete;
	Ray(Vec3F A, Vec3F B) : origin(A), direction(B) {}
	Vec3F Direction() const { return direction; }
	Vec3F Origin() const { return origin; }
	Vec3F PointAtParameter(float t) const { return origin + t * direction; }
private:	
	Vec3F origin;
	Vec3F direction;
};