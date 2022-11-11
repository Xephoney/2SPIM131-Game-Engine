#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_output.h>
#include <engine/log.h>

namespace Engine {
	class sound
	{
	public:
		FMOD::ChannelGroup* TEST = nullptr;
		FMOD::Sound* soundTest = nullptr;
		//FMOD::Channel* testChannel = nullptr;
		FMOD::System* system = nullptr;

		sound() {
			FMOD_RESULT result;
			
			// INITIALIZATION CODE FROM FMOD STARTUP GUIDE //
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

			//set up channel group
			result = system->createChannelGroup("gameSounds", &TEST);
			if (!successCheck(result))
				exit(-1);

			//set up sound
			system->createSound("../Engine/Assets/Sound/Trekant.mp3", FMOD_DEFAULT, nullptr, &soundTest);

			ENGINE_LOG_INFO("Sound should work");

		};

		bool successCheck(FMOD_RESULT result) {
			if (result != FMOD_OK) {
				ENGINE_LOG_ERROR("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
				return false;
			}
			return true;
		}

		void testSound() {
			FMOD_RESULT  result;
			FMOD::Channel* channel = nullptr;
			
			result = system->playSound(soundTest, nullptr, false, &channel);
			if (!successCheck(result))
				//exit(-1);
			ENGINE_LOG_INFO("played sound");
		}

		void update() {
			system->update(); 
		}
		
	};
}
