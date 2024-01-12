#pragma once

#include <random>

namespace eg
{

	class RandomFloatGenerator
	{
	public:
        float generate(const float min, const float max)
		{
			return std::uniform_real_distribution<float>{min, max}(eng);
		}

	private:
		std::mt19937 eng{ std::random_device{}() };
	};

	class RandomIntGenerator
	{
    public:
        int generate(const int min, const int max)
        {
            return std::uniform_int_distribution<int>{min, max}(eng);
        }

	private:
        std::mt19937 eng{std::random_device{}()};
	};

} // namespace eg