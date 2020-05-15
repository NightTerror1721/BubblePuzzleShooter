#pragma once

#include <pybind11/embed.h>

#include "common.h"

namespace pybind = pybind11;

typedef pybind::scoped_interpreter PyInterpreter;



#define __BINDED_PTR(type) namespace { type* _##type{ nullptr }; } \
namespace pylib { \
inline void bind##type (type* const& ptr) { _##type = ptr; } \
inline void unbind##type () { _##type = nullptr; } \
}


#undef __BINDED_PTR


namespace pylib
{
	pybind::object executePythonScript(const std::string& filepath);
	void loadResourceCaches();
}
