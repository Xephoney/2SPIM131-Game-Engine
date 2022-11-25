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

		//set up reverb
		
		result = system->createReverb3D(&reverb);
		FMOD_REVERB_PROPERTIES reverbProp = FMOD_PRESET_CONCERTHALL;
		reverb->setProperties(&reverbProp);

		//set up 3D sound
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
			system->createSound(file, FMOD_3D_LINEARROLLOFF, 0, &tempSound);
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
		
		FMOD_VECTOR earPos = ToFMODVec(CamVel);
		FMOD_VECTOR earUp = ToFMODVec(CamUp);
		FMOD_VECTOR earfwd = ToFMODVec(CamFwd);

		for (int i = 0; i < soundList.size(); i++)
			soundList[i]->update();

		system->set3DListenerAttributes(0, &earPos, 0, &earfwd, &earUp);
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

	void SoundManager::addToSoundlist(sound* inSound)
	{
		soundList.push_back(inSound);
		ENGINE_LOG_INFO("Added sound to soundlist");
	}

	FMOD_VECTOR SoundManager::ToFMODVec(glm::vec3 in)
	{
		FMOD_VECTOR result;
		result.x = in.x;
		result.y = in.y;
		result.z = in.z;
//		ENGINE_LOG_INFO("CameraPos:{0},{1},{2}", result->x, result->y, result->z);
		return result;
	}

	FMOD::Reverb3D* SoundManager::getReverb()
	{
		return reverb;
	}

	FMOD::System* SoundManager::getSystem()
	{
		return system;
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

	FMOD_VECTOR sound::getPos()
	{
		return mPos;
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

	bool sound::bSoundExists(std::string name)
	{
		return SoundManager::getSoundManager().bSoundExists(name);
	}

	bool sound::swapSound(std::string name)
	{
		if (bSoundExists(name)) {
			mName = name;
			return true;
		}
		return false;
	}

	void sound::update()
	{
		SoundManager::getSoundManager().getReverb()->set3DAttributes(&mPos, minDist, maxDist);
		ENGINE_LOG_WARNING("MIN DIST: {0}   MAX DIST: {1}", minDist, maxDist);
	}

	void sound::setPos(glm::vec3 pos)
	{
		mPos.x = pos.x;
		mPos.y = pos.y;
		mPos.z = pos.z;
	}

	void sound::setPos(FMOD_VECTOR pos)
	{
		mPos = pos;
	}

	void sound::setMinDist(float dist)
	{
		minDist = dist;
	}

	void sound::setMaxDist(float dist)
	{
		maxDist = dist;
	}

}