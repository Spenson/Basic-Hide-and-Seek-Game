#include "Load.h"
#include "Globals.h"
#include "FileReading/JsonHelpers.h"
#include "Component/Transform.h"
#include "Component/Render.h"
#include "Component/Position.h"
#include "Component/Rotation.h"

namespace Degen
{
	namespace Load
	{
		bool LoadModels(const Json::Value& jsonModels, const std::string& shader_name)
		{
			if (!jsonModels.isArray()) { return false; }

			std::string model_name;
			std::string model_file;
			bool is_basic = true;
			std::string error;
			for (unsigned int idx = 0; idx < jsonModels.size(); idx++)
			{
				Json::Value jsonCurModel = jsonModels[idx];
				if (jsonCurModel["name"].isString()) model_name = jsonCurModel["name"].asString();
				if (jsonCurModel["file"].isString()) model_file = jsonCurModel["file"].asString();
				if (jsonCurModel["is_basic"].isBool()) is_basic = jsonCurModel["is_basic"].asBool();
				VAOAndModel::sModelDrawInfo* mdi = nullptr;

				if (is_basic)
				{
					mdi = ModelLoader->LoadBasicModel(model_file, model_name, error);
				}
				else
				{
					printf("Advanced models not implemented.\n");
				}

				if (!mdi)
				{
					printf("Model '%s' not loaded: %s\n", model_name.c_str(), error.c_str());
					continue;
				}

				if (!VAOManager->LoadModelDrawInfoIntoVAO(*mdi, ShaderManager->GetProgramIDFromFriendlyName(shader_name)))
				{
					printf("Model '%s' not pushed to VAO.\n", model_name.c_str());
				}
			}
			return true;
		}
		bool LoadEntities(const Json::Value& jsonEntities)
		{
			if (!jsonEntities.isArray()) { return false; }

			for (unsigned int idx = 0; idx < jsonEntities.size(); idx++)
			{
				Json::Value jsonCurEntity = jsonEntities[idx];
				Entity::cEntity* ent = Entity::cEntityManager::CreateEntity();
				std::vector<std::string> component_names = jsonCurEntity.getMemberNames();
				for (unsigned int i = 0; i < component_names.size(); i++)
				{
					if (component_names[i] == "name")
					{
						JsonHelp::Set(jsonCurEntity[component_names[i]], ent->name);
					}
					if (component_names[i] == "transform")
					{
						Component::iComponent* comp = ent->AddComponent<Component::Transform>();
						comp->Deserialize(jsonCurEntity[component_names[i]]);
					}
					if (component_names[i] == "render")
					{
						Component::iComponent* comp = ent->AddComponent<Component::Render>();
						comp->Deserialize(jsonCurEntity[component_names[i]]);
					}
					if (component_names[i] == "position")
					{
						Component::iComponent* comp = ent->AddComponent<Component::Position>();
						comp->Deserialize(jsonCurEntity[component_names[i]]);
					}
					if (component_names[i] == "rotation")
					{
						Component::iComponent* comp = ent->AddComponent<Component::Rotation>();
						comp->Deserialize(jsonCurEntity[component_names[i]]);
					}
				}
			}

			return true;
		}
	}
}
