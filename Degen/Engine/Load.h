#pragma once

#include <json/json.h>

namespace Degen
{
	namespace Load
	{
		
		bool LoadModels(const Json::Value& jsonModels, const std::string& shader_name);
		bool LoadEntities(const Json::Value& jsonEntities);
	}
}
