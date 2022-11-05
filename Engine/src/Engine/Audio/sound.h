#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_output.h>
#include <engine/log.h>

namespace Engine {
	class sound
	{
	public:
		FMOD::ChannelGroup* TEST;
		FMOD::Channel Memes;
		FMOD::System* system = nullptr;

		sound() {
			FMOD_RESULT result;
			
			result = FMOD::System_Create(&system);      // Create the main system object.
			if (result != FMOD_OK)
			{
				ENGINE_LOG_ERROR("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
				exit(-1);
			}

			result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
			if (result != FMOD_OK)
			{
				ENGINE_LOG_ERROR("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
				exit(-1);
			}
			

			ENGINE_LOG_INFO("Fmod Initialized");
		};

		void testSound() {
			FMOD::Sound* sound1 = nullptr;
			FMOD::Sound* sound2 = nullptr;
			FMOD::ChannelGroup** test;
			system->createSound("Trekant.mp3", FMOD_DEFAULT, 0, &sound1);
			system->createStream("Trekant.mp3", FMOD_DEFAULT, 0, &sound2);
			sound1->setMode(FMOD_LOOP_OFF);
			system->createChannelGroup("testChannel", &TEST);
			system->playSound(sound2, TEST, false, 0);

			if (system->getChannel();
				ENGINE_LOG_INFO("sound should be playing");
		}
		
	};
}
