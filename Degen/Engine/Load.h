#pragma once

#include <json/json.h>

namespace Degen
{
	namespace Load
	{
		bool LoadTextures(const Json::Value& jsonTextures);
		bool LoadCubemaps(const Json::Value& jsonCubemaps);
		bool LoadModels(const Json::Value& jsonModels, const std::string& shader_name);
		bool LoadAnimations(const Json::Value& jsonAnimations);
		bool LoadEntities(const Json::Value& jsonEntities);
	}
}
