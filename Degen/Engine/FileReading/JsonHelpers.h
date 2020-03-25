#pragma once
#include <json/json.h>

namespace Degen
{
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
