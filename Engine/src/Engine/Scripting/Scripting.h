#pragma once
#include <string>
#include <Python.h>
#include "Engine/Core.h"

namespace Engine
{
	class PythonScript
	{
		PyObject* m_ptrPyModule{nullptr};

		PyObject* m_ptrStartFunction{ nullptr };
		PyObject* m_ptrUpdateFunction{ nullptr };
		PyObject* m_ptrEnableFunction{ nullptr };
		PyObject* m_ptrEventFunction{ nullptr };
		
	public :
		PythonScript();
		PythonScript(const std::string& scriptFilePath);

		virtual void OnStart();
		virtual void OnUpdate();
		virtual void OnEnable();
		virtual void OnEvent();
		virtual ~PythonScript() ;
	};
	
}
      