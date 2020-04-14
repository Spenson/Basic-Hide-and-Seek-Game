#pragma once
#include <string>
#include <map>
#include <glm.hpp>
#include <vector>


namespace Degen
{
	namespace Animation
	{
		struct sModelBoneInfo
		{
			std::map<std::string, unsigned int> bone_name_index;
			unsigned bone_count;

			glm::mat4 mGlobalInverseTransformation;

			std::vector<glm::mat4> Offsets;
		};
	}
}
