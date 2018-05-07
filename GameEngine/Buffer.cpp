#define NOMINMAX
#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>

#include "Buffer.hpp"
#include "Camera.h"
#include "Dielectric.h"
#include "IMaterial.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Sphere.h"

void Impl::drawRectInternal(GE::Buffer & buffer, const Rect & rect, Color fillColor)
{
	int startX = rect.x >= 0 ? rect.x : 0;
	int startY = rect.y >= 0 ? rect.y : 0;
	int endX = startX + rect.width < buffer.Width() ? startX + rect.width : buffer.Width();
	int endY = startY + rect.height < buffer.Height() ? startY + rect.height : buffer.Height();

	for (auto x = startX; x < endX; ++x)
	{
		for (auto y = startY; y < endY; ++y)
		{
			buffer.DrawPixel(x, y, fillColor);
		}
	}
}

GE::Buffer::Buffer(int width, int height) :
	m_width(width),
	m_height(height)
{
	info.bmiHeader.biSize = sizeof(info.bmiHeader);
	info.bmiHeader.biWidth = m_width;
	info.bmiHeader.biHeight = m_height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;

	data = std::vector<Color>(m_width * m_height);
}

BITMAPINFO * GE::Buffer::BMapInfoPtr() { return &info; }
uint32_t * GE::Buffer::Data() { return reinterpret_cast<uint32_t*>(data.data()); }

Color& GE::Buffer::at(int col, int row)
{
	if (col >= 0 && col < m_width &&
		row >= 0 && row < m_height)
	{
		return data[col + (row * m_width)];
	}
	else
	{
		throw std::invalid_argument("invalid pixel coordinate.");
	}
}

void GE::Buffer::DrawPixel(int x, int y, Color newColor)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		auto ratio = static_cast<double>(newColor.color.alpha) / 0xff;
		auto oldColor = this->at(x, y);

		auto oldR = oldColor.color.red;
		auto oldG = oldColor.color.green;
		auto oldB = oldColor.color.blue;

		auto newR = newColor.color.red;
		auto newG = newColor.color.green;
		auto newB = newColor.color.blue;

		Color colorToDraw;
		colorToDraw.color.red = static_cast<unsigned char>(newR * ratio + oldR * (1 - ratio));
		colorToDraw.color.green = static_cast<unsigned char>(newG * ratio + oldG * (1 - ratio));
		colorToDraw.color.blue = static_cast<unsigned char>(newB * ratio + oldB * (1 - ratio));
		colorToDraw.color.alpha = 0xff;

		this->at(x, y) = colorToDraw;
		changed = true;
	}
}

int GE::Buffer::Width() const
{
	return m_width;
}

int GE::Buffer::Height() const
{
	return m_height;
}

void GE::Buffer::DrawFrameTime(std::vector<long long>& frameTime)
{
	for (auto i = 0u; i < frameTime.size(); ++i)
	{
		DrawPixel(i, static_cast<int>(frameTime[i]) + frameOffset, Color{ 0xffffff00 });
	}
}

void GE::Buffer::DrawTargetFrameTime(int targetFrameTime)
{
	for (auto i = 0; i < m_width; ++i)
	{
		DrawPixel(i, targetFrameTime + frameOffset, Color{ 0xff00ff00 });
	}
}

void GE::Buffer::FillFrame()
{
	std::fill(std::begin(data), std::end(data), Color{ 0x00000000 });
}

Vec3F trace(Ray const& r, RayHitableList const& world, int depth = 0)
{
	HitRecord rec{};
	if (world.hit(r, 0.00001, std::numeric_limits<double>::max(), rec))
	{
		if (depth < 20)
		{
			auto [scatter, attenuation, scattered] = rec.mat->scatter(r, rec);
			if (scatter)
			{
				return attenuation * trace(scattered, world, depth + 1);
			}
		}
		return Vec3F(0, 0, 0);
	}
	else
	{
		auto unitDirection = MakeUnitVector(r.Direction());
		auto t = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - t)*Vec3F(1.0, 1.0, 1.0) + t * Vec3F(0.7, 0.7, 1.0);
	}
}

RayHitableList CreateRandomScene()
{
	std::default_random_engine ranDevice;
	std::uniform_real_distribution<double> dist;
	IRayHitableVector spheres;
	// Floor
	spheres.push_back(std::make_unique<Sphere>(Vec3F(0, -1000, 0), 1000.0, std::make_unique<Lambertian>(Vec3F(.5, .5, .5))));

	auto dimension = 11;
	for (auto a = -dimension; a < dimension; ++a)
	{
		for (auto b = -dimension; b < dimension; ++b)
		{
			auto mat = dist(ranDevice);
			Vec3F center(a + 0.9 * dist(ranDevice), 0.2, b + 0.9 * dist(ranDevice));
			if ((center - Vec3F(4.0, .2, 0.0)).Length() > .9)
			{
				// Diffuse ball
				if (mat < 0.8)
				{
					spheres.push_back(std::make_unique<Sphere>(center,
															   .2f,
															   std::make_unique<Lambertian>(Vec3F(dist(ranDevice) * dist(ranDevice),
																								  dist(ranDevice) * dist(ranDevice),
																								  dist(ranDevice) * dist(ranDevice)))));
				}
				// Metal
				else if (mat < 0.95)
				{
					spheres.push_back(std::make_unique<Sphere>(center,
															   .2f, 
															   std::make_unique<Metal>(Vec3F(.5 * (1 + dist(ranDevice)), 
																							 .5 * (1 + dist(ranDevice)), 
																							 .5 * (1 + dist(ranDevice))),
																					   .5 * dist(ranDevice))));
				}
				// Glass
				else
				{
					spheres.push_back(std::make_unique<Sphere>(center,
															   .2,
															   std::make_unique<Dielectric>(1.5)));
				}
			}
		}
	}

	spheres.push_back(std::make_unique<Sphere>(Vec3F(0, 1, 0), 1.0, std::make_unique<Dielectric>(1.5)));
	spheres.push_back(std::make_unique<Sphere>(Vec3F(-4, 1, 0), 1.0, std::make_unique<Lambertian>(Vec3F(.4, .2, .1))));
	spheres.push_back(std::make_unique<Sphere>(Vec3F(4, 1, 0), 1.0, std::make_unique<Metal>(Vec3F(.7, .6, .5), 0.0)));

	return std::move(spheres);
}

void GE::Buffer::DrawRayTrace()
{
	Vec3F lookFrom(13, 2, 3);
	Vec3F lookAt(0, 0, 0);
	double DistanceToFocus = 10.0;
	double Apeture = .1;
	Camera camera(lookFrom, lookAt, Vec3F(0, 1, 0), 20, static_cast<double>(Width()) / static_cast<double>(Height()), Apeture, DistanceToFocus);

	
	RayHitableList world = CreateRandomScene();

	int tries = 40;
	for (auto y = Height(); y >= 0; --y)
	{
		for (auto x = 0; x < Width(); ++x)
		{
			Vec3F col(0, 0, 0);
			for (auto i = 0; i < tries; ++i)
			{
				double u = (x + dist(ranDevice)) / static_cast<double>(Width());
				double v = (y + dist(ranDevice)) / static_cast<double>(Height());
				Ray r = camera.GetRay(u, v);
				col += trace(r, world);
			}
			col /= static_cast<double>(tries);

			col = Vec3F(std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]));
			DrawPixel(x, y, Color(col.y(), col.x(), col.z()));
		}
	}
}

bool GE::Buffer::Changed() const
{
	return changed;
}

void GE::Buffer::ResetChangeStatus()
{
	changed = false;
}
