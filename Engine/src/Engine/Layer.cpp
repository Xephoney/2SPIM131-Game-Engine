
#include "Engine/engpch.h"
#include "Layer.h"

namespace Engine
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	= default;
}
