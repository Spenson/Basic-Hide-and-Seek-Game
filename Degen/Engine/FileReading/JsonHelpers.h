#pragma once
#include <json/json.h>
#include <glm.hpp>

namespace Degen
{
	namespace JsonHelp
	{
		// set a string
		inline bool Set(const Json::Value& jsonVal, std::string& val)
		{
			if (!jsonVal.isString()) return false;
			val = jsonVal.asString();
			return true;
		}
		// set a bool
		inline bool Set(const Json::Value& jsonVal, bool& val)
		{
			if (!jsonVal.isBool()) return false;
			val = jsonVal.asBool();
			return true;
		}
		// set a float
		inline bool Set(const Json::Value& jsonVal, float& val)
		{
			if (!jsonVal.isNumeric()) return false;
			val = jsonVal.asFloat();
			return true;
		}
		// set a size_t
		inline bool Set(const Json::Value& jsonVal, size_t& val)
		{
			if (!jsonVal.isNumeric()) return false;
			val = (size_t)jsonVal.asUInt();
			return true;
		}
		// set a vec3
		inline bool Set(const Json::Value& jsonVal, glm::vec3& val)
		{
			if (!jsonVal[0].isNumeric() 
				|| !jsonVal[1].isNumeric() 
				|| !jsonVal[2].isNumeric()) return false;
			
			val.x = jsonVal[0].asFloat();
			val.y = jsonVal[1].asFloat();
			val.z = jsonVal[2].asFloat();
			return true;
		}
		// set a vec4
		inline bool Set(const Json::Value& jsonVal, glm::vec4& val)
		{
			if (!jsonVal[0].isNumeric()
				|| !jsonVal[1].isNumeric()
				|| !jsonVal[2].isNumeric()
				|| !jsonVal[3].isNumeric()) return false;

			val.x = jsonVal[0].asFloat();
			val.y = jsonVal[1].asFloat();
			val.z = jsonVal[2].asFloat();
			val.w = jsonVal[3].asFloat();
			return true;
		}
		
	}


	
	namespace FileReading
	{




		inline bool FileExists(const char* fileName)
		{
			struct stat buffer;
			return (stat(fileName, &buffer) == 0);
		}

		bool ReadJsonFile(const std::string& fileName, Json::Value& jsonOut, std::string& errors);

	}
}
