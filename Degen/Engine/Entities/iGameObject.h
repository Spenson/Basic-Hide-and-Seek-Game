#pragma once
#include <string>

namespace Degen
{
	namespace Object
	{
		class iGameObject
		{
		public:
			// all un-wanted precreated ctors 
			iGameObject() = delete;
			iGameObject(const iGameObject& other) = delete;
			iGameObject(iGameObject&& other) = delete;
			iGameObject& operator=(const iGameObject& other) = delete;
			iGameObject& operator=(iGameObject&& other) = delete;

			// virtual dtor
			virtual ~iGameObject() = default;

			// public name so it can be accessed and changed it is optional but most likely useful
			std::string name;

			// way to get the id and type that are hidden
			inline unsigned GetId() const { return mId; }
			inline unsigned GetType() const { return mType; }
		protected:
			// protected for access only through inheritance forcing it to be used as a interface
			// wanted ctors
			iGameObject(int type) : name(), mType(type), mId(GetNextId()) {}
			iGameObject(int type, std::string& name) : name(name), mType(type), mId(GetNextId()) {}
		private:
			unsigned mType;
			// protected to be uneditable since ids are generated
			unsigned mId;
			static unsigned mNextId;
			static inline unsigned GetNextId() { return mNextId++; }

		};
	}
}
