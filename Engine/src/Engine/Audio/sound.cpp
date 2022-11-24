#include "engpch.h"
#include "sound.h"

namespace Engine {

	SoundManager::SoundManager()
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
		system->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
		system->setOutput(FMOD_OUTPUTTYPE_WINSONIC);

		//adding some basic sounds
		addSound("../Engine/Assets/Sound/Trekant.mp3", "Trekant", true);
		addSound("../Engine/Assets/Sound/pop.mp3", "Pop", true);
		addSound("../Engine/Assets/Sound/delete_sound.mp3", "Delete", true);
		addSound("../Engine/Assets/Sound/delete_all.mp3", "DeleteAll", true);

#ifdef DEBUG
		ENGINE_LOG_INFO("Default sounds added, no issues reported.");
#endif // DEBUG



	}

	SoundManager& SoundManager::getSoundManager()
	{
		static SoundManager instance;
		return instance;
	}

	void SoundManager::addSound(const char* file, std::string name, bool in3DSpace) // creates an FMOD sound object from the given soundfile, adds it to list of working sounds
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
		{
			system->createSound(file, FMOD_DEFAULT, nullptr, &tempSound);
		}
		else
			system->createSound(file, FMOD_3D, 0, &tempSound);
		mSounds.insert(std::pair(name, tempSound));

#ifdef DEBUG
		ENGINE_LOG_INFO("Created new sound '{0}' and added to list of sounds", name);
		if (in3DSpace)
			ENGINE_LOG_INFO("Sound '{0}' is created in 3D space", name);
#endif //DEBUG
	}

	bool SoundManager::successCheck(FMOD_RESULT result) 
	{
		if (result != FMOD_OK) 
		{
			ENGINE_LOG_ERROR("FMOD error! ({0}) {1}", result, FMOD_ErrorString(result));
			return false;
		}
		return true;
	}

	void SoundManager::update() 
	{
		//using scene camera for testing, will change to using listener components after
		glm::vec3 CamPos = Renderer::GetRenderCamera()->GetPosition();
		glm::vec3 CamFwd = Renderer::GetRenderCamera()->Forward();
		glm::vec3 CamUp = Renderer::GetRenderCamera()->Up();
		glm::vec3 CamVel = glm::vec3(0.f, 0.f, 0.f);

		FMOD_VECTOR* EarPos=ToFMODVec(CamPos);
		FMOD_VECTOR* EarVel=ToFMODVec(CamVel);
		FMOD_VECTOR* EarUp=ToFMODVec(CamUp);
		FMOD_VECTOR* EarFwd = ToFMODVec(CamFwd);
		
		system->set3DListenerAttributes(0, EarPos, 0, EarFwd, EarUp);
		system->update();

	}

	bool SoundManager::bSoundExists(std::string name)
	{
		if (mSounds.find(name) != mSounds.end())
			return true;
		else
			ENGINE_LOG_ERROR("GIVEN SOUND '{0}' DOES NOT EXIST IN MEMORY", name);
			return false;
	}

	void SoundManager::playSound(const std::string& name) {
		FMOD_RESULT  result;
		FMOD::Channel* channel = nullptr;

		if (mSounds.size() < 1) {
			ENGINE_LOG_WARNING("NO SOUNDS LOADED TO PLAY!");
			return;
		}

		//Looking through mSounds to see if called sound exists
		//TODO : maybe have a toUpper to avoid having to care about capital letters?
		auto current = mSounds.find(name);
		if (current == mSounds.end())
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

	FMOD_VECTOR* SoundManager::ToFMODVec(glm::vec3 in)
	{
		FMOD_VECTOR* result=new FMOD_VECTOR();
		result->x = in.x;
		result->y = in.y;
		result->z = in.z;
		ENGINE_LOG_INFO("CameraPos:{0},{1},{2}", in.x, in.y, in.z);
		return result;
	}

	sound::sound(std::string name)
	{
		mName = name;
		if (!SoundManager::getSoundManager().bSoundExists(name))
			ENGINE_LOG_WARNING("NEW SOUND {0} CREATED, BUT FILE NAME NOT FOUND IN MEMORY!", name);
	}

	sound::sound(const char* file, std::string name, bool is3D)
	{
	
		if (!SoundManager::getSoundManager().bSoundExists(name))
			SoundManager::getSoundManager().addSound(file, name, false);

		else
			ENGINE_LOG_WARNING("Tried adding a sound that was already saved in memory: {0}", name);
		mName = name;
	}

	std::string sound::getName() {
		return mName;
	}

	void sound::playSound(std::string name)
	{
		if(bSoundExists())
			SoundManager::getSoundManager().playSound(name);
	}

	bool sound::bSoundExists()
	{
		return SoundManager::getSoundManager().bSoundExists(mName);
	}

}