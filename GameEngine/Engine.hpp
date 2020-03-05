#ifndef ENGINE_LIB_HEADER
#define ENGINE_LIB_HEADER
#pragma warning(push, 0)        
//Some includes with unfixable warnings
#define WIN32_LEAN_AND_MEAN
#include <Unknwn.h>
#include <windows.h>
#include <gdiplus.h>
// GDI+ needs min and max, so we let them live until here
#undef min
#undef max
#include <Shlwapi.h>
#include <GL/GL.h>
typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
static wglSwapInterval_t* wglSwapInterval;
#pragma warning(pop)

#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <string_view>
#include <vector>
#include <filesystem>
#include <memory>

namespace GE
{
	struct Pixel
	{
		Pixel() : val(0xFF000000) {}

		Pixel(uint32_t c) : val(c) {}

		Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) :
			red(r), green(g), blue(b), alpha(a)
		{
		}

		Pixel& operator=(uint32_t c)
		{
			val = c;
			return *this;
		}

		bool operator==(Pixel const& other) const
		{
			return val == other.val;
		}

		bool operator!=(Pixel const& other) const
		{
			return val != other.val;
		}

		enum class Mode { NORMAL, MASK, ALPHA, CUSTOM };

#pragma warning(push)
#pragma warning(disable:4201)
		union
		{
			uint32_t val = 0xFF000000;
			struct
			{
				uint8_t blue;
				uint8_t red;
				uint8_t green;
				uint8_t alpha;
			};
		};
	};
#pragma warning(pop)

	static const Pixel 
		WHITE(255, 255, 255),
		GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
		RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
		YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
		GREEN(0, 255, 0), DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
		CYAN(0, 255, 255), DARK_CYAN(0, 128, 128), VERY_DARK_CYAN(0, 64, 64),
		BLUE(0, 0, 255), DARK_BLUE(0, 0, 128), VERY_DARK_BLUE(0, 0, 64),
		MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64),
		BLACK(0, 0, 0),
		BLANK(0, 0, 0, 0);

	template <typename T>
	struct Vec2D
	{
		inline Vec2D() : x(0), y(0) {}
		inline Vec2D(T _x, T _y) : x(_x), y(_x) {}
		inline Vec2D(Vec2D const& v) : x(v.x), y(v.y) {}
		inline T Magnitude() const { return std::sqrt(x * x + y * y); }
		inline T Magnitude2() const { return x * x + y * y; }
		inline Vec2D Norm() const { T r = 1 / Magnitude(); return Vec2D(x * r, y * r); }
		inline Vec2D Perp() const { return Vec2D(-y, x); }
		inline T Dot(Vec2D const& rhs) { return x * rhs.x + y * rhs.y; }
		inline T Cross(Vec2D const& rhs) { return x * rhs.y - y * rhs.x; }
		inline Vec2D operator+ (Vec2D const& rhs) const { return Vec2D(x + rhs.x, y + rhs.y); }
		inline Vec2D operator- (Vec2D const& rhs) const { return Vec2D(x - rhs.x, y - rhs.y); }
		inline Vec2D operator* (T const& rhs) const { return Vec2D(x * rhs, y * rhs); }
		inline Vec2D operator/ (T const& rhs) const { return Vec2D(x / rhs, y / rhs); }
		inline Vec2D& operator+= (Vec2D const& rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
		inline Vec2D& operator-= (Vec2D const& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
		inline Vec2D& operator*= (T const& rhs) { this->x *= rhs; this->y *= rhs; return *this; }
		inline Vec2D& operator/= (T const& rhs) { this->x /= rhs; this->y /= rhs; return *this; }
		inline operator Vec2D<int32_t>() const { return { static_cast<int32_t>(x), static_cast<int32_t>(y) }; }
		inline operator Vec2D<float>() const { return { static_cast<float>(x), static_cast<float>(y) }; }
		inline T& operator[] (std::size_t i) { return data[i]; }

#pragma warning(push)
#pragma warning(disable:4201)
		union
		{
			std::array<T, 2> data{ 0, 0 };
			struct
			{
				T x;
				T y;
			};
		};
#pragma warning(pop)
	};

	template <typename T> inline Vec2D<T> operator* (float const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
	template <typename T> inline Vec2D<T> operator* (double const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
	template <typename T> inline Vec2D<T> operator* (int const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
	template <typename T> inline Vec2D<T> operator/ (float const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
	template <typename T> inline Vec2D<T> operator/ (double const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
	template <typename T> inline Vec2D<T> operator/ (int const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }

	using vi2d = Vec2D<int32_t>;
	using vf2d = Vec2D<float>;
	using vd2d = Vec2D<double>;

	struct ResourceBuffer : public std::streambuf
	{
		ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size)
		{
			Memory.resize(size);
			ifs.seekg(offset);
			ifs.read(Memory.data(), Memory.size());
			setg(Memory.data(), Memory.data(), Memory.data() + size);
		}

		std::vector<char> Memory;
	};

	class ResourcePack : public std::streambuf 
	{
	public:
		ResourcePack() {}
		~ResourcePack() { baseFile.close(); }

		bool AddFile(std::string file)
		{
			auto filePath = makePosix(file);

			if (std::filesystem::exists(filePath))
			{
				ResourceFile e;
				e.size = (uint32_t)std::filesystem::file_size(filePath);
				e.offset = 0; // Unknown at this stage			
				mapFiles[file] = e;
				return true;
			}
			return false;
		}

		bool LoadPack(std::string_view file, std::string_view key)
		{
			baseFile.open(file.data(), std::ifstream::binary);
			if (!baseFile.is_open()) return false;

			// Read scrambled index
			uint32_t indexSize = 0;
			baseFile.read((char*)&indexSize, sizeof(uint32_t));

			std::string buffer;
			buffer.reserve(indexSize);
			std::generate_n(std::back_inserter(buffer), indexSize, [this]() { return (char)baseFile.get(); });

			auto decoded = scramble(buffer, key);
			std::stringstream iss(decoded);

			// Read map
			uint32_t mapEntries;
			iss.read((char*)&mapEntries, sizeof(uint32_t));

			auto readFileName = [](std::stringstream& stream)
			{
				uint32_t pathSize;
				stream.read((char*)&pathSize, sizeof(uint32_t));

				std::string buffer;
				buffer.reserve(pathSize);
				std::generate_n(std::back_inserter(buffer), pathSize, [&stream]() { return (char)stream.get(); });

				return buffer;
			};

			auto getResourceFile = [](std::stringstream& stream)
			{
				ResourceFile e;
				stream.read((char*)&e.size, sizeof(uint32_t));
				stream.read((char*)&e.offset, sizeof(uint32_t));

				return e;
			};

			std::generate_n(std::inserter(mapFiles, std::end(mapFiles)), mapEntries, [&iss, readFileName, getResourceFile]()
				{
					return std::make_pair(readFileName(iss), getResourceFile(iss));
				});

			return true;
		}

		bool SavePack(std::string_view file, std::string_view key)
		{
			std::ofstream ofs(file.data(), std::ofstream::binary);
			if (!ofs.is_open()) return false;

			// Write index size
			uint32_t indexSize = 0;
			ofs.write((char*)&indexSize, sizeof(uint32_t));

			// Write map size
			uint32_t mapSize = static_cast<uint32_t>(mapFiles.size());
			ofs.write((char*)&mapSize, sizeof(uint32_t));

			// Write map records
			for (auto& e : mapFiles)
			{
				// Write path
				auto pathSize = e.first.size();
				ofs.write((char*)&pathSize, sizeof(uint32_t));
				ofs.write(e.first.data(), pathSize);

				// Write entry
				ofs.write((char*)&e.second.size, sizeof(uint32_t));
				ofs.write((char*)&e.second.offset, sizeof(uint32_t));
			}

			// Write data
			std::streampos offset = ofs.tellp();
			indexSize = (uint32_t)offset;
			for (auto& e : mapFiles)
			{
				// Store beginning of file offset within resource pack file
				e.second.offset = (uint32_t)offset;

				// Load the file to be added
				std::vector<char> vBuffer(e.second.size);
				std::ifstream i(e.first, std::ifstream::binary);
				i.read(vBuffer.data(), e.second.size);
				i.close();

				// Write the loaded file into resource pack file
				ofs.write(vBuffer.data(), e.second.size);
				offset += e.second.size;
			}

			// Scramble index
			// 3) Scramble Index
			std::stringstream oss;
			oss.write((char*)&mapSize, sizeof(uint32_t));
			for (auto& e : mapFiles)
			{
				// Write the path of the file
				size_t nPathSize = e.first.size();
				oss.write((char*)&nPathSize, sizeof(uint32_t));
				oss.write(e.first.c_str(), nPathSize);

				// Write the file entry properties
				oss.write((char*)&e.second.size, sizeof(uint32_t));
				oss.write((char*)&e.second.offset, sizeof(uint32_t));
			}
			auto indexString = scramble(oss.str(), key);

			// 4) Rewrite Map (it has been updated with offsets now)
			// at start of file
			ofs.seekp(std::ios::beg);
			ofs.write((char*)&indexSize, sizeof(uint32_t));
			ofs.write(indexString.c_str(), indexSize);
			ofs.close();
			return true;
		}

		ResourceBuffer GetFileBuffer(std::string const& file)
		{
			return ResourceBuffer(baseFile, mapFiles.at(file).offset, mapFiles.at(file).size);
		}

		bool Loaded() const
		{
			return baseFile.is_open();
		}

	private:
		struct ResourceFile { uint32_t size; uint32_t offset; };
		std::ifstream baseFile;
		std::unordered_map<std::string, ResourceFile> mapFiles;
		
		std::string const scramble(std::string_view data, std::string_view key) const
		{
			std::size_t c = 0;
			std::string out;
			for (auto s : data)
			{
				out += std::string(1, s ^ key[(c++) % key.size()]);
			}
			return out;
		}

		std::string makePosix(std::string_view path) const
		{
			std::string out;
			out.reserve(path.length());
			std::transform(std::begin(path), std::end(path), std::back_inserter(out), [](auto c) { return c == '\\' ? '/' : c; });
			return out;
		}
	};

	enum class RCode
	{
		FAIL = 0,
		OK = 1,
		NO_FILE = -1
	};

	inline std::wstring S2W(std::string const& s)
	{
		auto count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
		std::wstring w(count, ' ');
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, w.data(), count);

		return w;
	}

	class Sprite
	{
	public:
		int32_t width = 0;
		int32_t height = 0;
		enum class Mode { NORMAL, PERIODIC };

		Sprite(int32_t w, int32_t h) : width(w), height(h)
		{
			if (colData)
				colData.reset();

			colData = std::make_unique<Pixel[]>(width * height);
		}

		RCode WriteToSpriteFile(std::string const& file) const
		{
			if (colData == nullptr) return RCode::FAIL;

			std::ofstream ofs;
			ofs.open(file, std::ofstream::binary);
			if (ofs.is_open())
			{
				ofs.write((char*)&width, sizeof(int32_t));
				ofs.write((char*)&height, sizeof(int32_t));
				ofs.write((char*)colData.get(), width*height*sizeof(uint32_t));
				ofs.close();
				return RCode::OK;
			}
			return RCode::FAIL;
		}

		RCode LoadFromSpriteFile(std::string const& file, ResourcePack* pack)
		{
			if (colData != nullptr)
				colData.reset();

			auto ReadData = [this](std::istream& is)
			{
				is.read((char*)&width, sizeof(int32_t));
				is.read((char*)&height, sizeof(int32_t));
				colData = std::make_unique<Pixel[]>(width * height);
				is.read((char*)colData.get(), width * height * sizeof(uint32_t));
			};

			if (pack == nullptr)
			{
				std::ifstream ifs;
				ifs.open(file, std::ifstream::binary);
				if (ifs.is_open())
				{
					ReadData(ifs);
					return RCode::OK;
				}
				
				return RCode::FAIL;
			}
			else
			{
				auto rb = pack->GetFileBuffer(file);
				std::istream is(&rb);
				ReadData(is);
				return RCode::OK;
			}

			return RCode::FAIL;
		}

		RCode LoadFromFile(std::string imageFile, ResourcePack* pack)
		{
			std::unique_ptr<Gdiplus::Bitmap> bitmap(nullptr);
			if (pack != nullptr)
			{
				ResourceBuffer buffer = pack->GetFileBuffer(imageFile);
				bitmap.reset(Gdiplus::Bitmap::FromStream(SHCreateMemStream((BYTE*)buffer.Memory.data(), (UINT)buffer.Memory.size())));
			} 
			else
			{
				bitmap.reset(Gdiplus::Bitmap::FromFile(S2W(imageFile).c_str()));
			}

			if (!bitmap) return RCode::NO_FILE;

			width = bitmap->GetWidth();
			height = bitmap->GetHeight();
			colData = std::make_unique<Pixel[]>(width * height);

			// Set pixel
			for (int x = 0; x < width; ++x)
			{
				for (int y = 0; y < height; ++y)
				{
					Gdiplus::Color c;
					bitmap->GetPixel(x, y, &c);
					SetPixel(x, y, Pixel(c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha()));
				}
			}
			return RCode::OK;
		}

		bool SetPixel(int32_t x, int32_t y, Pixel p)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				colData[y * width + x] = p;
				return true;
			}
			
			return false;
		}

		Pixel GetPixel(int32_t x, int32_t y) const
		{
			if (modeSample == Mode::NORMAL)
			{
				if (x >= 0 && x < width && y >= 0 && y < height)
				{
					return colData[y * width + x];
				}
				else
				{
					return Pixel();
				}
			}
			else
			{
				return colData[std::abs(y % height) * width + std::abs(x % width)];
			}
		}

		void SetSampleMode(Mode mode)
		{
			modeSample = mode;
		}

		Pixel Sample(float x, float y) const
		{
			auto sx = std::min((int32_t)((x * (float)width)), width - 1);
			auto sy = std::min((int32_t)((y * (float)width)), width - 1);
			return GetPixel(sx, sy);
		}
	private:
		std::unique_ptr<Pixel[]> colData = nullptr;
		Mode modeSample = Mode::NORMAL;
	};

	class Engine
	{

	private:
		std::string appName = "Undefined";
		uint32_t ScreenWidth;
		uint32_t ScreenHeight;
		uint32_t PixelWidth;
		uint32_t PixelHeight;
	};
}
#endif