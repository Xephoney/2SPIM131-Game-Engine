#pragma once
#include "Log.h"

#ifdef ENG_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Initialize();
	ENGINE_LOG_INFO("Engine Debug Console Loaded")
	LOG_INFO("Client Console Loaded")
	

	const auto app = Engine::CreateApplication();
	app->run();
	delete app;
}

#endif

