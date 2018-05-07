#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

#include "Camera.h"

Vec3F randomInUnitDisk()
{
	std::default_random_engine ranDevice;
	std::uniform_real_distribution<double> dist;

	Vec3F p;
	do
	{
		p = 2.0 * Vec3F(dist(ranDevice), dist(ranDevice), 0) - Vec3F(1, 1, 0);
	} while (dot(p, p) >= 1.0);

	return p;
}

Camera::Camera(Vec3F LookFrom, Vec3F LookAt, Vec3F LookUp, double VerticalFOV, double AspectRatio, double Apeture, double FocusDistance) : 
	origin(LookFrom)
{
	lensRadius = Apeture / 2;
	auto theta = VerticalFOV * M_PI / 180;
	auto halfHeight = std::tan(theta / 2);
	auto halfWidth = AspectRatio * halfHeight;
	w = MakeUnitVector(LookFrom - LookAt);
	u = MakeUnitVector(cross(LookUp, w));
	v = cross(w, u);

	lowerLeft = origin - halfWidth * FocusDistance * u - halfHeight * FocusDistance * v - w * FocusDistance;
	horizontal = 2 * halfWidth * FocusDistance * u;
	vertical = 2 * halfHeight * FocusDistance * v;
}

Ray Camera::GetRay(double s, double t)
{
	Vec3F rd = lensRadius * randomInUnitDisk();
	auto offset = u * rd.x() + v * rd.y();
	return Ray(origin + offset, lowerLeft + s * horizontal + t * vertical - origin - offset);
}
