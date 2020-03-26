#pragma once

#include <json/json.h>

namespace Degen
{
	namespace Component
	{
		class iComponent
		{
		public:
			virtual ~iComponent() = default;

			virtual bool Serialize(Json::Value& json) = 0;
			
			unsigned int Type() { return mType; }
		protected:
			iComponent(unsigned int type) : mType(type) {}
		private:
			unsigned int mType;
		};
	}
}
