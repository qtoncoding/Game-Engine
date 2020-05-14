#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <array>
#include <type_traits>

#include "Window.hpp"
#include "Frame.hpp"
#include "Buffer.hpp"
#include "Color.h"
#include "Vec3.h"

using namespace GE;

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

static bool Running = true;

void ProcessWindowMessage(HWND& windowHandle)
{
	MSG msg;
	while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{

		default:
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} break;
		}
	}
}

using Point3D = Vec3F;

constexpr double infinity = std::numeric_limits<double>::infinity();

struct Ray {
	Point3D origin;
	Vec3F direction;

	constexpr Point3D at(double t) const {
		return origin + t * direction;
	}
};

struct HitRecord {
	Point3D p;
	Vec3F normal;
	double t;
	Vec3F attenuation;
};

struct Sphere {
	Point3D center;
	double radius;
	Vec3F albedo;
};

constexpr std::pair<bool, HitRecord> hitSphere(Sphere const& sphere, double minT, double maxT, Ray const& r) {
	auto oc = r.origin - sphere.center;
	auto a = dot(r.direction, r.direction);
	auto b = dot(oc, r.direction);
	auto c = dot(oc, oc) - sphere.radius * sphere.radius;
	auto discriminant = b * b - a * c;

	if (discriminant > 0) {
		auto root = const_sqrt(discriminant);
		auto temp = (-b - root) / a;
		if (temp < maxT && temp > minT) {
			auto p = r.at(temp);
			auto normal = (p - sphere.center) / sphere.radius;
			if (dot(r.direction, normal) >= 0) {
				normal = -normal;
			}
			HitRecord newRec{ p, normal, temp, sphere.albedo };
			return std::make_pair(true, newRec);
		}
		temp = (-b + root) / a;
		if (temp < maxT && temp > minT) {
			auto p = r.at(temp);
			auto normal = (p - sphere.center) / sphere.radius;
			if (dot(r.direction, normal) >= 0) {
				normal = -normal;
			}
			HitRecord newRec{ p, normal, temp, sphere.albedo };
			return std::make_pair(true, newRec);
		}
	}

	return std::make_pair(false, HitRecord{});
}

template <std::size_t Count>
constexpr std::pair<bool, HitRecord> hitSpheres(std::array<Sphere, Count> const& spheres, double minT, double maxT, Ray const& r) {
	HitRecord rec{};
	bool hitAny = false;
	auto closest = maxT;

	for (auto const& sphere : spheres) {
		auto [hit, newRec] = hitSphere(sphere, minT, closest, r);
		if (hit) {
			hitAny = true;
			closest = newRec.t;
			rec = newRec;
		}
	}

	return std::make_pair(hitAny, rec);
}

constexpr std::pair<bool, Ray> scatter(Ray const& r, HitRecord const& rec) {
	auto reflected = reflect(MakeUnitVector(r.direction), rec.normal);
	auto scattered = Ray{ rec.p, reflected };
	auto bounce = (dot(scattered.direction, rec.normal) > 0);
	return std::make_pair(bounce, scattered);
}

constexpr Point3D sphere1Origin = { -0.6, 0, -1 };
constexpr Point3D sphere2Origin = { 0.8, -0.35, -1 };
constexpr Point3D sphere3Origin = { 0, -100.6, -1 };
constexpr Vec3F albedo1 = { .9, .9, .8 };
constexpr Vec3F albedo2 = { .9, .6, .6 };
constexpr Vec3F albedo3 = { .6, .6, .8 };
constexpr std::array<Sphere, 3> world{ Sphere { sphere1Origin, 0.5, albedo1 }, Sphere { sphere2Origin, 0.2, albedo2 }, Sphere { sphere3Origin, 100, albedo3 } };

constexpr Vec3F rayColor(Ray const& r, int depth) {
	if (depth <= 0) {
		return Vec3F(0, 0, 0 );
	}

	auto [hit, newRec] = hitSpheres(world, 0.001, infinity, r);
	if (hit) {
		auto [bounce, newRay] = scatter(r, newRec);
		if (bounce) {
			auto newColor = rayColor(newRay, depth - 1);
			return newRec.attenuation * newColor;
		}
		return Vec3F( 0, 0, 0 );
	}

	auto unitDir = MakeUnitVector(r.direction);
	auto t = 0.5 * (unitDir.y + 1.0);
	return (1.0 - t) * Vec3F(1.0, 1.0, 1.0) + t * Vec3F(.5, .7, 1.0);
}

constexpr int PixelCount = GE::DefaultWindowWidth * GE::DefaultWindowHeight;

constexpr Vec3F origin = { 0, 0, 0 };
constexpr Vec3F horizontal = { 4, 0, 0 };
constexpr Vec3F vertical = { 0.0, 2.25, 0.0 };
constexpr Point3D lowerLeftCorner = origin - (horizontal / 2.0) - (vertical / 2.0) - Point3D{ 0, 0, 1 };

constexpr Color getPixel(int index) {
	auto x = index % GE::DefaultWindowWidth;
	auto y = index / GE::DefaultWindowWidth;

	auto u = double(x) / (GE::DefaultWindowWidth - 1);
	auto v = double(y) / (GE::DefaultWindowHeight - 1);
	Ray r{ origin, lowerLeftCorner + u * horizontal + v * vertical };
	auto color = rayColor(r, 10);
	return Color{ color.x, color.y, color.z };
}

template <typename Generator, std::size_t... Indices>
constexpr auto make_array(Generator g, std::index_sequence<Indices...>)
-> std::array<typename std::invoke_result_t<Generator, int>, sizeof...(Indices)>
{
	return { { g(Indices)... } };
}

template <int N, typename Generator>
constexpr auto makeBuffer(Generator g)
-> std::array<typename std::invoke_result_t<Generator, int>, N>
{
	return make_array(g, std::make_index_sequence<N>{});
}

int CALLBACK 
WinMain(HINSTANCE Instance,
		HINSTANCE,
		LPSTR,
		int)
{
	GE::Window window(Instance);

	auto buffer = window.FrameBuffer();
	auto deviceContext = window.DeviceContext();
	auto windowHandle = window.Handle();

	constexpr auto pixelBuffer = makeBuffer<PixelCount>(getPixel);
	buffer.SetData(pixelBuffer.data());

	while (Running)
	{
		ProcessWindowMessage(windowHandle);

		GE::Frame frame(&buffer, &deviceContext, buffer.Width(), buffer.Height());
	}

	return 0;
}