#include "cAnimationManager.h"

namespace Degen
{
	namespace Animation
	{
		void cAnimationManager::AddAnimation(sAnimationInfo* info)
		{
			animations[info->name] = info;

			animations[info->name]->animation_time = animations[info->name]->animation->mDuration / animations[info->name]->animation->mTicksPerSecond;
			
		}
		sAnimationInfo* cAnimationManager::GetAnimation(std::string name)
		{
			std::map<std::string, sAnimationInfo*>::iterator it = animations.find(name);
			if (it == animations.end())
			{
				return nullptr;
			}
			return it->second;
		}
		bool cAnimationManager::HasAnimation(std::string name)
		{
			std::map<std::string, sAnimationInfo*>::iterator it = animations.find(name);
			if (it == animations.end())
			{
				return false;
			}
			return true;
		}
	}
}