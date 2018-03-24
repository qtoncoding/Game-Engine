#define NOMINMAX
#include "Buffer.hpp"
#include <cmath>
#include "Sphere.h"
#include <numeric>
#include "Camera.h"
#include "IMaterial.h"
#include "Metal.h"
#include "Lambertian.h"

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

Vec3F trace(Ray const& r, RayHitableList const& world, int depth)
{
	HitRecord rec{};
	if (world.hit(r, 0.00001f, std::numeric_limits<float>::max(), rec))
	{
		if (depth < 50)
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
		auto unitDirection = UnitVector(r.Direction());
		auto t = 0.5f * (unitDirection.y() + 1.0f);
		return (1.0f - t)*Vec3F(1.0f, 1.0f, 1.0f) + t * Vec3F(0.7f, 0.7f, 1.0f);
	}
}

void GE::Buffer::DrawGradient()
{
	Camera camera;

	IRayHitableVector spheres;
	spheres.push_back(std::make_unique<Sphere>(Vec3F(0, 0, 1), 0.5f, std::make_unique<Lambertian>(Vec3F(0.3f, 0.3f, 0.8f))));
	spheres.push_back(std::make_unique<Sphere>(Vec3F(1, 0, 1), 0.5f, std::make_unique<Metal>(Vec3F(0.8f, 0.6f, 0.8f))));
	spheres.push_back(std::make_unique<Sphere>(Vec3F(-1, 0, 1), 0.5f, std::make_unique<Metal>(Vec3F(0.8f, 0.8f, 0.8f))));
	spheres.push_back(std::make_unique<Sphere>(Vec3F(0, -100.5, 1), 100.0f, std::make_unique<Lambertian>(Vec3F(0.6f, 0.6f, 0.0f))));

	RayHitableList world(std::move(spheres));
	
	int tries = 10;
	for (auto y = Height(); y >= 0; --y)
	{
		for (auto x = 0; x < Width(); ++x)
		{
			Vec3F col(0, 0, 0);
			for (auto i = 0; i < tries; ++i)
			{
				float u = (x + dist(ranDevice)) / static_cast<float>(Width());
				float v = (y + dist(ranDevice)) / static_cast<float>(Height());
				Ray r = camera.GetRay(u, v);
				col += trace(r, world, 0);

			}
			col /= static_cast<float>(tries);

			/*float u = static_cast<float>(x) / static_cast<float>(Width());
			float v = static_cast<float>(y) / static_cast<float>(Height());
			Ray r = camera.GetRay(u, v);
			auto col = trace(r, world, 0);
			*/

			col = Vec3F(std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]));
			DrawPixel(x, y, Color(col.x(), col.y(), col.z()));
		}
	}
}
