#pragma once
#include "Ray.h"
#include "Vec3.h"


class Camera
{
public:
	Camera() : 
		origin(Vec3F(0, 0, 0)), 
		lowerLeft(Vec3F(-2, -1, -1)),
		horizontal(Vec3F(4,0,0)),
		vertical(Vec3F(0, 2, 0))
	{
	}

	Camera(Vec3F LookFrom, Vec3F LookAt, Vec3F LookUp, double VerticalFOV, double AspectRatio, double Apeture, double FocusDistance);

	Ray GetRay(double u, double v);

private:
	Vec3F origin;
	Vec3F lowerLeft;
	Vec3F horizontal;
	Vec3F vertical;
	double lensRadius;
	Vec3F w;
	Vec3F u;
	Vec3F v;
};