#include "JsonHelpers.h"
#include <fstream>

namespace Degen
{
	namespace FileReading
	{
		bool ReadJsonFile(const std::string& fileName, Json::Value& jsonOut, std::string& errors)
		{
			if (!FileExists(fileName.c_str()))
			{
				errors = "file doesn't exists, or is inaccessible";
				return false;
			}
			std::ifstream jsonFile(fileName, std::ifstream::binary);
			Json::CharReaderBuilder charReaderBuilder;
			std::string errs;
			return Json::parseFromStream(charReaderBuilder, jsonFile, &jsonOut, &errors);
		}
	}
}