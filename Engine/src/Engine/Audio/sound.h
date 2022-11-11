#pragma once
#include <utility>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_output.h>
#include <engine/log.h>
#include <vector>
#include <string>

#define DEBUG


namespace Engine {

	class SoundManager 
	{
	public:
		std::unordered_map<std::string, FMOD::Sound*> mSounds;
		FMOD::ChannelGroup* gameSound = nullptr;
		FMOD::System* system = nullptr;
	
		SoundManager() 
		{
			FMOD_RESULT result;

			result = FMOD::System_Create(&system);
			if (!successCheck(result))
				exit(-1);
			result = system->init(512, FMOD_INIT_NORMAL, 0);
			if (!successCheck(result))
				exit(-1);
#ifdef DEBUG
				ENGINE_LOG_INFO("Fmod Initialized");
#endif // DEBUG

			//set up channel group
			result = system->createChannelGroup("gameSounds", &gameSound);
			if (!successCheck(result))
				exit(-1);

			//adding some basic sounds
			addSound("../Engine/Assets/Sound/Trekant.mp3", "Trekant", false);
			addSound("../Engine/Assets/Sound/pop.mp3", "Pop", false);
			addSound("../Engine/Assets/Sound/delete_sound.mp3", "Delete", false);

#ifdef DEBUG
			ENGINE_LOG_INFO("Default sounds added, no issues reported.");
#endif // DEBUG

		}
		void addSound(const char* file, std::string name, bool in3DSpace) // creates an FMOD sound object from the given soundfile, adds it to list of working sounds
		{ 
			ENGINE_LOG_INFO("Adding sound");
			FMOD::Sound* tempSound = nullptr;

			auto got = mSounds.find(name);
			if (got != mSounds.end()) 
			{
#ifdef DEBUG
				ENGINE_LOG_INFO("tried adding sound with existing name: '{0}'", name);
#endif // DEBUG
				return;
			}
				
			if (!in3DSpace) //3D spacing to be tested out when sources and listeners are set up
				system->createSound(file, FMOD_DEFAULT, nullptr, &tempSound);
			else
				system->createSound(file, FMOD_3D, nullptr, &tempSound);
			mSounds.insert(std::pair(name,tempSound));

#ifdef DEBUG
			ENGINE_LOG_INFO("Created new sound '{0}' and added to list of sounds", name);
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

		void playSound(const std::string name) {
			FMOD_RESULT  result;
			FMOD::Channel* channel = nullptr;

			if (mSounds.size() < 1) {
				ENGINE_LOG_WARNING("NO SOUNDS LOADED TO PLAY!");
				return;
			}

			//Looking through mSounds to see if called sound exists
			//TODO : maybe have a toUpper to avoid having to care about capital letters?
			auto CUM = mSounds.find(name);
			if (CUM == mSounds.end())
			{
				ENGINE_LOG_ERROR("Sound '{0}' not found in list of sounds!", name);
				return;
			}
			result = system->playSound(mSounds[name], nullptr, false, &channel);
			if (!successCheck(result))
				exit(-1);
#ifdef DEBUG
			ENGINE_LOG_INFO("played sound");
#endif //DEBUG
			return;
		}

	};

	class sound
	{
	private:
		static sound& s_sound;
		SoundManager mSoundmanager;
	public:
		
		sound() 
		{
#ifdef DEBUG
			ENGINE_LOG_INFO("SOUND.H : Created sound object, but no sound was passed in")
#endif // DEBUG
		}

		sound(const char* file, std::string name, SoundManager manager)
		{
			mSoundmanager = manager;
			mSoundmanager.addSound(file, name, false);

		}
		

		


		bool successCheck(FMOD_RESULT result) {
			if (result != FMOD_OK) {
				ENGINE_LOG_ERROR("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
				return false;
			}
			return true;
		}

		
		
		
		
		static sound& getSoundManager();

		static FMOD_CHANNEL& createChannel(std::string name);
		
	};

	inline sound& sound::getSoundManager() {
		static sound instance;
		return instance;
	}

	inline  sound& sound_createChannel(std::string name) {
		
	}
}
