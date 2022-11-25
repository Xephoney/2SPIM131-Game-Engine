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
	private:
		std::unordered_map<std::string, FMOD::Sound*> mSounds;
		std::vector<class sound*> soundList;
		FMOD::ChannelGroup* gameSound = nullptr;
		FMOD::System* system = nullptr;
		FMOD::Reverb3D* reverb;
		//FMOD::ChannelGroup mChannels[4]; COME BACK TO THIS LATER
	public:
		SoundManager();

		static SoundManager& getSoundManager();
		
		void addSound(const char* file, std::string name, bool in3DSpace); // creates an FMOD sound object from the given soundfile, adds it to list of working sounds
		
		bool successCheck(FMOD_RESULT result);

		void update();

		bool bSoundExists(std::string name);

		void playSound(const std::string& name);

		void addToSoundlist(class sound* inSound);

		FMOD_VECTOR ToFMODVec(glm::vec3 in);

		FMOD::Reverb3D* getReverb();
		FMOD::System* getSystem();
		
	};

	class sound
	{
	private:
		static sound& s_sound;
		bool is3D = false;
		std::string mName = "EMPTY";
		FMOD_VECTOR mPos = { 0.f,0.f,0.f };
		float minDist = 10.f;
		float maxDist = 20.f;
	public:
		// constructors
		sound() 
		{
#ifdef DEBUG
			ENGINE_LOG_INFO("SOUND.H : Created sound object, but no sound was passed in")
#endif // DEBUG
		}
		sound(std::string name);
		sound(const char* file, std::string name, bool is3D);

		// sound functions
		void playSound(std::string name);
		static sound& getSoundManager();
		//static FMOD_CHANNEL& createChannel(std::string name);
		bool bSoundExists();
		bool bSoundExists(std::string name);
		bool swapSound(std::string name);
		void update();

		// get values
		std::string getName();
		FMOD_VECTOR getPos();


		// set values
		void setPos(glm::vec3 pos);
		void setPos(FMOD_VECTOR pos);
		void setMinDist(float dist);
		void setMaxDist(float dist);
		
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
