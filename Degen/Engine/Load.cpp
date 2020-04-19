#include "Load.h"
#include "Globals.h"
#include "FileReading/JsonHelpers.h"
#include "Component/Transform.h"
#include "Component/Render.h"
#include "Component/Position.h"
#include "Component/Rotation.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/Velocity.h"
#include "Component/Gatherer.h"
#include "Component/Animation.h"
#include "Component/Physics.h"

namespace Degen
{
	namespace Load
	{
		bool LoadTextures(const Json::Value& jsonTextures)
		{
			if (!jsonTextures.isArray()) { return false; }
			TextureManager->SetBasePath("assets/textures/");
			std::string texture_file;
			std::string texture_name;
			std::string texture_format;
			std::string error_info;

			for (unsigned int idx = 0; idx < jsonTextures.size(); idx++)
			{
				JsonHelp::Set(jsonTextures[idx]["name"], texture_name);
				JsonHelp::Set(jsonTextures[idx]["file"], texture_file);
				JsonHelp::Set(jsonTextures[idx]["format"], texture_format);
				if (texture_format == "png")
				{
					if (!TextureManager->Create2DTextureFromPNGFile(texture_file, texture_name, error_info))
					{
						printf("Texture %s not loaded.\n\t%s\n", texture_name.c_str(), error_info.c_str());
					}
				}
			}

			return true;
		}
		bool LoadCubemaps(const Json::Value& jsonCubemaps)
		{
			if (!jsonCubemaps.isArray()) { return false; }
			TextureManager->SetBasePath("assets/textures/cubemaps/");
			std::string texture_file_posX;
			std::string texture_file_negX;
			std::string texture_file_posY;
			std::string texture_file_negY;
			std::string texture_file_posZ;
			std::string texture_file_negZ;
			std::string cubemap_name;
			std::string cubemap_format;
			std::string error_info;

			for (unsigned int idx = 0; idx < jsonCubemaps.size(); idx++)
			{
				JsonHelp::Set(jsonCubemaps[idx]["name"], cubemap_name);
				JsonHelp::Set(jsonCubemaps[idx]["+x"], texture_file_posX);
				JsonHelp::Set(jsonCubemaps[idx]["-x"], texture_file_negX);
				JsonHelp::Set(jsonCubemaps[idx]["+y"], texture_file_posY);
				JsonHelp::Set(jsonCubemaps[idx]["-y"], texture_file_negY);
				JsonHelp::Set(jsonCubemaps[idx]["+z"], texture_file_posZ);
				JsonHelp::Set(jsonCubemaps[idx]["-z"], texture_file_negZ);
				JsonHelp::Set(jsonCubemaps[idx]["format"], cubemap_format);
				if (cubemap_format == "png")
				{
					if (!TextureManager->CreateCubeTextureFromPNGFiles(cubemap_name,
																	   texture_file_posX, texture_file_negX,
																	   texture_file_posY, texture_file_negY,
																	   texture_file_posZ, texture_file_negZ,
																	   true, error_info))
					{
						printf("Texture %s not loaded.\n\t%s\n", cubemap_name.c_str(), error_info.c_str());
					}
				}
			}

			return true;
		}
		bool LoadModels(const Json::Value& jsonModels, const std::string& shader_name)
		{
			if (!jsonModels.isArray()) { return false; }

			std::string model_name;
			std::string model_file;
			bool is_basic = true;
			unsigned meshidx = 0;
			std::string error;
			for (unsigned int idx = 0; idx < jsonModels.size(); idx++)
			{
				Json::Value jsonCurModel = jsonModels[idx];
				if (jsonCurModel["name"].isString()) model_name = jsonCurModel["name"].asString();
				if (jsonCurModel["file"].isString()) model_file = jsonCurModel["file"].asString();
				if (jsonCurModel["is_basic"].isBool()) is_basic = jsonCurModel["is_basic"].asBool();
				if (jsonCurModel["mesh_number"].isUInt())
					meshidx = jsonCurModel["mesh_number"].asUInt();

				VAOAndModel::sModelDrawInfo* mdi = nullptr;

				if (is_basic)
				{
					mdi = ModelLoader->LoadBasicModel(model_file, model_name, error);
				}
				else
				{
					mdi = ModelLoader->LoadModel(model_file, model_name, error, meshidx);
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
				//std::vector<std::string> component_names = jsonCurEntity.getMemberNames();

				JsonHelp::Set(jsonCurEntity["name"], ent->name);
				JsonHelp::Set(jsonCurEntity["id"], ent->user_id);

				if (jsonCurEntity["components"].isArray())
				{
					Json::Value components = jsonCurEntity["components"];
					for (unsigned int i = 0; i < components.size(); i++)
					{
						if (components[i]["component"] == "transform")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Transform>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "render")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Render>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "position")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Position>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "rotation")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Rotation>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "camera")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Camera>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "light")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Light>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "velocity")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Velocity>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "gatherer")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Gatherer>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "animation")
						{
							Component::iComponent* comp = ent->AddComponent<Component::Animation>();
							comp->Deserialize(components[i]);
						}
						else if (components[i]["component"] == "physics")
						{
							Component::Physics* comp = ent->AddComponent<Component::Physics>();
							comp->Deserialize(components[i]);
							comp->comp->SetEntityId(ent->unique_id);
							comp->comp->SetSecondaryId(ent->user_id);
						}
					}
				}
			}

			return true;
		}
		bool LoadAnimations(const Json::Value& jsonAnimations)
		{
			if (!jsonAnimations.isArray()) { return false; }

			std::string model_name;
			std::string animation_file;
			std::string animation_name;
			unsigned animation_idx = 0;
			std::string error;

			for (unsigned int idx = 0; idx < jsonAnimations.size(); idx++)
			{
				Json::Value jsonCurModel = jsonAnimations[idx];
				if (jsonCurModel["name"].isString()) animation_name = jsonCurModel["name"].asString();
				if (jsonCurModel["file"].isString()) animation_file = jsonCurModel["file"].asString();
				if (jsonCurModel["model"].isString()) model_name = jsonCurModel["model"].asString();
				if (jsonCurModel["index"].isUInt())
					animation_idx = jsonCurModel["index"].asUInt();


				Animation::sAnimationInfo* ani = ModelLoader->LoadAnimation(animation_file, animation_name, model_name, error, animation_idx);
				if (ani)
				{
					AnimationManager->AddAnimation(ani);
				}

			}
			return true;
		}
	}
}
