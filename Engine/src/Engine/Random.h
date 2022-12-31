#pragma once

#include <random>

class Random
{
public:
	static void init()
	{
		if (Initiated)
			return;

		s_RandomEngine.seed(std::random_device()());

		std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6); // distribution in range [1, 6]

		Initiated = true;
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
	static float Float(float min, float max)
	{
		/*std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);*/
		/*return static_cast<float>(distribution(s_RandomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());*/
		return 10;
	}

	static double Double()
	{
		return (double)s_Distribution(s_RandomEngine) / (double)std::numeric_limits<uint32_t>::max();
	}

	inline static bool Initiated = false ;
private:
	
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};