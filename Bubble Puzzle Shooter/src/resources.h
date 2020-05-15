#pragma once

#include "common.h"

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

class Path
{
private:
	std::string _path;

public:
	Path(const std::string& path);
	Path(const Path&) = default;
	Path(Path&&) = default;
	~Path();

	Path& operator= (const Path&) = default;
	Path& operator= (Path&&) = default;

	operator std::string() const;

	friend Path operator+ (const Path& left, const char* right);
	friend Path operator+ (const Path& left, const std::string& right);
	friend Path operator+ (const Path& left, const Path& right);

	friend Path& operator+= (Path& left, const char* right);
	friend Path& operator+= (Path& left, const std::string& right);
	friend Path& operator+= (Path& left, const Path& right);

	friend Path& operator<< (Path& left, const char* right);
	friend Path& operator<< (Path&& left, const char* right);
	friend Path& operator<< (Path& left, const std::string& right);
	friend Path& operator<< (Path&& left, const std::string& right);
	friend Path& operator<< (Path& left, const Path& right);
	friend Path& operator<< (Path&& left, const Path& right);

	friend Path& operator>> (Path& left, std::string& right);
	friend const Path& operator>> (const Path& left, std::string& right);
	friend Path& operator>> (Path& left, Path& right);
	friend const Path& operator>> (const Path& left, Path& right);

	friend std::ostream& operator<< (std::ostream& os, const Path& path);
	friend std::istream& operator>> (std::istream& is, Path& path);
};

Path operator"" _p(const char* str, size_t size);

namespace ResourcePoint
{
	extern const Path Textures;
	extern const Path Audio;
	extern const Path Sounds;
	extern const Path Musics;
	extern const Path BubbleModels;
}

