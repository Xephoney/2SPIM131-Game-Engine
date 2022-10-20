#pragma once

#ifdef ENG_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	const auto app = Engine::CreateApplication();
	app->run();
	delete app;
}

#endif
