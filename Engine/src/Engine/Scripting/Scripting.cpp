#include <engpch.h>

#include <Python.h>
#include "Scripting.h"

#include "PyBind.h"

namespace Engine
{
	PythonScript::PythonScript()
	{
		
		PyObject* ptrByteCode{ nullptr };
	
		PyImport_AppendInittab("CGL", &PyBind::Py_Import);

		//Initialize Python
		Py_Initialize();
		if (!Py_IsInitialized())
			ENGINE_LOG_FATAL("Python failed to initialize. Make sure Python is installed on your computer!");

		

		HANDLE hPythonSource = CreateFileW(
			L".\\ScriptTesting.py",
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(hPythonSource != INVALID_HANDLE_VALUE)
		{
			DWORD fileSize = GetFileSize(hPythonSource, NULL);
			char* fileBuffer = (char*)malloc(fileSize + 1);
			if(fileBuffer)
			{
				if(ReadFile(hPythonSource, fileBuffer, fileSize, NULL, NULL))
				{
					fileBuffer[fileSize] = '\0';
					ptrByteCode = Py_CompileString(fileBuffer, "ScriptTesting.py", Py_file_input);

				}
				free(fileBuffer);
			}
			CloseHandle(hPythonSource);
		}

		//Loading module
		if(ptrByteCode)
		{
			m_ptrPyModule = PyImport_ExecCodeModule("ScriptTesting", ptrByteCode);
			if(m_ptrPyModule)
			{
				m_ptrStartFunction = PyObject_GetAttrString(m_ptrPyModule, "Start");
				m_ptrUpdateFunction = PyObject_GetAttrString(m_ptrPyModule, "Update");
				m_ptrEventFunction = PyObject_GetAttrString(m_ptrPyModule, "Event");
				m_ptrEnableFunction = PyObject_GetAttrString(m_ptrPyModule, "Enabled");

			}
			
			Py_DECREF(ptrByteCode);
		}
	}

	PythonScript::~PythonScript()
	{
		if (m_ptrStartFunction)		Py_DECREF(m_ptrStartFunction);
		if (m_ptrUpdateFunction)	Py_DECREF(m_ptrUpdateFunction);
		if (m_ptrEventFunction)		Py_DECREF(m_ptrEventFunction);
		if (m_ptrEnableFunction)	Py_DECREF(m_ptrEnableFunction);

		if (m_ptrPyModule)			Py_DECREF(m_ptrPyModule);

		Py_Finalize();
	}

	PythonScript::PythonScript(const std::string& scriptFilePath)
	{

	}

	inline void PythonScript::OnStart()
	{

	}

	inline void PythonScript::OnUpdate()
	{

	}

	inline void PythonScript::OnEvent()
	{

	}

	inline void PythonScript::OnEnable()
	{

	}
}
