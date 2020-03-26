#include "Render.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Render::Render() : iComponent(RENDER_TYPE),
			mesh(),
			texture(),
			diffuse_colour(1.f),
			specular_colour(1.f, 1.f, 1.f, 1000.f),
			scale(1.f),
			ignore_lighting(true),
			is_wireframe(false)
		{
		}
		bool Render::Serialize(Json::Value& json)
		{
			JsonHelp::Set(json["mesh"], mesh);
			JsonHelp::Set(json["texture"], texture);
			JsonHelp::Set(json["diffuse"], diffuse_colour);
			JsonHelp::Set(json["specular"], specular_colour);
			JsonHelp::Set(json["scale"], scale);
			JsonHelp::Set(json["ignore_lighting"], ignore_lighting);
			JsonHelp::Set(json["is_wireframe"], is_wireframe);

			return true;
		}
	}
}
