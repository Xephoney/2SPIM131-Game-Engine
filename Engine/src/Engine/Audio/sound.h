#pragma once
#include <utility>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_output.h>
#include <engine/log.h>

#define DEBUG


namespace Engine {
	class sound
	{
	public:
		FMOD::ChannelGroup* gameSound = nullptr;
		FMOD::System* system = nullptr;
		
		std::vector<std::pair<FMOD::Sound*, std::string>> mSounds;
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
			
#ifdef DEBUG
			ENGINE_LOG_INFO("Fmod Initialized");
#endif // DEBUG

			//set up channel group
			result = system->createChannelGroup("gameSounds", &gameSound);
			if (!successCheck(result))
				exit(-1);

			//adding some basic sounds
			addSound("../bin/Debug-windows-x86_64/Sandbox/Trekant.mp3", "Trekant", false);
			addSound("../bin/Debug-windows-x86_64/Sandbox/pop.mp3", "Pop", false);

#ifdef DEBUG
			ENGINE_LOG_INFO("Sound should work");
#endif // DEBUG

		};

		void addSound(const char* file, std::string name, bool in3DSpace) { // creates an FMOD sound object from the given soundfile, adds it to vector of working sounds

			FMOD::Sound* tempSound = nullptr;
			
			if (!in3DSpace) //3D spacing to be tested out when sources and listeners are set up
				system->createSound(file, FMOD_DEFAULT, nullptr, &tempSound);
			else
				system->createSound(file, FMOD_3D, nullptr, &tempSound);

			mSounds.push_back(std::pair(tempSound,name));

#ifdef DEBUG
			ENGINE_LOG_INFO("Created new sound {0} and added to list of sounds", name);
#endif //DEBUG
		}
		bool successCheck(FMOD_RESULT result) {
			if (result != FMOD_OK) {
				ENGINE_LOG_ERROR("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
				return false;
			}
			return true;
		}

		void update() {
			system->update(); 
		}
		
		void playSound(std::string name) {
			FMOD_RESULT  result;
			FMOD::Channel* channel = nullptr;

			if (mSounds.size() < 1) {
				ENGINE_LOG_WARNING("NO SOUNDS LOADED TO PLAY!");
				return;
			}

			for (int i = 0; i < mSounds.size(); i++) {
				if (mSounds[i].second == name) {
					FMOD::Sound* temp = mSounds[i].first;
					result = system->playSound(temp, nullptr, false, &channel);
					if (!successCheck(result))
						exit(-1);
#ifdef DEBUG
					ENGINE_LOG_INFO("played sound");
#endif //DEBUG
					return;
				}
			}
			ENGINE_LOG_ERROR("{0} WAS NOT FOUND IN LIST OF LOADED SOUNDS!", name);
		}
	};
}
