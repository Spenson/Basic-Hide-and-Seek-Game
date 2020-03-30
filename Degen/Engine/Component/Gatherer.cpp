#include "Gatherer.h"
#include "../FileReading/JsonHelpers.h"

namespace Degen
{
	namespace Component
	{
		Gatherer::Gatherer() :iComponent(GATHERER_COMPONENT),
			current_state(search),
			time(0.f)
		{
		}

		bool Gatherer::Deserialize(Json::Value& json)
		{
			int istate = -1;
			JsonHelp::Set(json["current_state"], istate);
			if(istate != -1)
			{
				current_state = (state)istate;
			}
			JsonHelp::Set(json["time"], time);

			return false;
		}

	}
}
