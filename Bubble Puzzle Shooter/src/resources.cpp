#include "resources.h"

Path::Path(const std::string& path) :
	_path{ path }
{}
Path::~Path() {}

Path::operator std::string() const { return _path; }

Path operator+ (const Path& left, const char* right) { return left._path + PATH_SEPARATOR + right; }
Path operator+ (const Path& left, const std::string& right) { return left._path + PATH_SEPARATOR + right; }
Path operator+ (const Path& left, const Path& right) { return left._path + PATH_SEPARATOR + right._path; }

Path& operator+= (Path& left, const char* right) { return left += std::string{ right }; }
Path& operator+= (Path& left, const std::string& right) { left._path += PATH_SEPARATOR + right; return left; }
Path& operator+= (Path& left, const Path& right) { left._path += PATH_SEPARATOR + right._path; return left; }

Path& operator<< (Path& left, const char* right) { return left << std::string{ right }; }
Path& operator<< (Path&& left, const char* right) { return left << std::string{ right }; }
Path& operator<< (Path& left, const std::string& right) { left._path += PATH_SEPARATOR + right; return left; }
Path& operator<< (Path&& left, const std::string& right) { left._path += PATH_SEPARATOR + right; return left; }
Path& operator<< (Path& left, const Path& right) { left._path += PATH_SEPARATOR + right._path; return left; }
Path& operator<< (Path&& left, const Path& right) { left._path += PATH_SEPARATOR + right._path; return left; }

Path& operator>> (Path& left, std::string& right) { right = left._path; return left; }
const Path& operator>> (const Path& left, std::string& right) { right = left._path; return left; }
Path& operator>> (Path& left, Path& right) { right._path = left._path; return left; }
const Path& operator>> (const Path& left, Path& right) { right._path = left._path; return left; }

std::ostream& operator<< (std::ostream& os, const Path& path) { return os << path._path; }
std::istream& operator>> (std::istream& is, Path& path) { return is >> path._path; }

Path operator"" _p(const char* str, size_t size) { return std::string{ str, size }; }


namespace ResourcePoint
{
	const Path Textures{ "data"_p << "textures" };
	const Path Audio{ "data"_p << "audio" };
	const Path Sounds{ "data"_p << "audio" << "sounds" };
	const Path Musics{ "data"_p << "audio" << "musics" };
	const Path BubbleModels{ "data"_p << "bubble_models" };
}
