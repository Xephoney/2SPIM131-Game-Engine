#pragma once
#include <utility>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_output.h>
#include <engine/log.h>
#include <vector>
#include <string>
#include "Engine/Renderer/Renderer.h"

#define DEBUG


namespace Engine {

	class SoundManager 
	{
	public:
		std::unordered_map<std::string, FMOD::Sound*> mSounds;
		FMOD::ChannelGroup* gameSound = nullptr;
		FMOD::System* system = nullptr;
		//FMOD::ChannelGroup mChannels[4]; COME BACK TO THIS LATER
		SoundManager();

		static SoundManager& getSoundManager();
		
		void addSound(const char* file, std::string name, bool in3DSpace); // creates an FMOD sound object from the given soundfile, adds it to list of working sounds
		
		bool successCheck(FMOD_RESULT result);

		void update();

		bool bSoundExists(std::string name);

		void playSound(const std::string& name);

		FMOD_VECTOR* ToFMODVec(glm::vec3 in);

		
	};

	class sound
	{
	private:
		static sound& s_sound;
		bool is3D = false;
		std::string mName = "EMPTY";
	public:
		
		sound() 
		{
#ifdef DEBUG
			ENGINE_LOG_INFO("SOUND.H : Created sound object, but no sound was passed in")
#endif // DEBUG
		}
		sound(std::string name);
		sound(const char* file, std::string name, bool is3D);

		std::string getName();
		
		void playSound(std::string name);
		
		static sound& getSoundManager();

		bool bSoundExists();

		static FMOD_CHANNEL& createChannel(std::string name);
		
	};

	inline sound& sound::getSoundManager() {
		static sound instance;
		return instance;
	}


	inline  sound& sound_createChannel(std::string name) {
		
	}

	enum SoundChannel
	{
		MAIN=0, 
		GAMEPLAY=1,
		MUSIC=2,
		MISC=3
	};
}
