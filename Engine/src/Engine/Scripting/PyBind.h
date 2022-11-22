#pragma once
#include <pytypedefs.h>

namespace Engine
{
	class PyBind
	{
	public:
		PyBind() = default;
		static PyObject* Py_Import();
	};
}
