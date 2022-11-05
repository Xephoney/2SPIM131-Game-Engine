
#include "engpch.h"
#include "Layer.h"

#include "Log.h"

namespace Engine
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{

	}

	Layer::~Layer()
	= default;
}
