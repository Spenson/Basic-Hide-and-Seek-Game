#include "Render.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Render::Render() : iComponent(RENDER_COMPONENT),
			mesh(),
			texture1("grid"),
			texture1_amount(0.f),
			texture2(),
			texture2_amount(0.f),
			is_cubemap_textures(false),
			alpha_mode(0),
			diffuse_colour(1.f),
			diffuse_amount(1.f),
			specular_colour(1.f, 1.f, 1.f, 1000.f),
			scale(1.f),
			ignore_lighting(true),
			is_wireframe(false),
			cull_face_back(true),
			bump_map(),
			use_bump_map(false),
			specular_map(),
			use_specular_map(false),
		tesselate(true)
		{
		}

		bool Render::Deserialize(Json::Value& json)
		{
			JsonHelp::Set(json["mesh"], mesh);
			JsonHelp::Set(json["texture1"], texture1);
			JsonHelp::Set(json["texture1_amount"], texture1_amount);
			JsonHelp::Set(json["alpha_mode"], alpha_mode);
			JsonHelp::Set(json["texture2"], texture2);
			JsonHelp::Set(json["texture2_amount"], texture2_amount);
			JsonHelp::Set(json["is_cubemap_texture"], is_cubemap_textures);
			JsonHelp::Set(json["diffuse"], diffuse_colour);
			JsonHelp::Set(json["diffuse_amount"], diffuse_amount);
			JsonHelp::Set(json["specular"], specular_colour);
			JsonHelp::Set(json["scale"], scale);
			JsonHelp::Set(json["ignore_lighting"], ignore_lighting);
			JsonHelp::Set(json["is_wireframe"], is_wireframe);
			JsonHelp::Set(json["cull_face_back"], cull_face_back);


			JsonHelp::Set(json["bump_map"], bump_map);
			JsonHelp::Set(json["use_bump_map"], use_bump_map);
			
			JsonHelp::Set(json["specular_map"], specular_map);
			JsonHelp::Set(json["use_specular_map"], use_specular_map);

			
			JsonHelp::Set(json["tesselate"], tesselate);



			return true;
		}
	}
}
