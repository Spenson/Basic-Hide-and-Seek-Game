#include "cAnimator.h"
#include "../Component/Animation.h"
#include "../Component/Position.h"
#include "../Component/Transform.h"

namespace Degen
{
	namespace Animation
	{
		void cAnimator::Update(double dt)
		{
			for (auto* entity : entities)
			{

			}

		}

		void cAnimator::AddEntity(Entity::cEntity* entity)
		{
			if (entity->HasComponent(Component::ANIMATION_COMPONENT) &&
				(entity->HasComponent(Component::POSITION_COMPONENT) || entity->HasComponent(Component::TRANSFORM_COMPONENT)))
			{
				if (std::find(entities.begin(), entities.end(), entity) == entities.end())
				{
					entities.push_back(entity);
				}
			}
		}


		void cAnimator::CalculateTransforms(Entity::cEntity* entity, double dt)
		{
			Component::Animation* animation_comp = dynamic_cast<Component::Animation*>(entity->GetComponent(Component::ANIMATION_COMPONENT));

			animation_comp->playing_time += dt;


			if (animation_comp->default_animation.empty())
			{
				printf("default animation must be set on %s.\n", entity->name.c_str());
				return;
			}


			// Get current animation
			std::string animation_name = animation_comp->playing_animation;
			if (animation_name.empty())
			{
				animation_name = animation_comp->default_animation;

				animation_comp->play_through = true;
				animation_comp->lock_animation = false;
			}
			else if (std::find(this->animations.begin(), this->animations.end(), animation_name) == this->animations.end())
			{
				printf("animation %s not found.\n", animation_name.c_str());
				animation_name = animation_comp->default_animation;

				animation_comp->play_through = true;
				animation_comp->lock_animation = false;

				if (std::find(this->animations.begin(), this->animations.end(), animation_name) == this->animations.end())
				{
					printf("animation %s not found.\n", animation_name.c_str());
					printf("default animation must be valid on %s.\n", entity->name.c_str());
					return;
				}
			}

			sAnimationInfo* animation_info = this->animations[animation_name];

			std::string next_animation_name = animation_comp->next_animation;

			if (animation_comp->blending_time == 0)
			{
				bool start_blend = true;
				if (animation_comp->lock_animation)
				{
					if ((animation_info->animation_time - animation_comp->playing_animation_blend_time) > animation_comp->playing_time)
					{
						start_blend = false;
					}
				}
				else if (animation_comp->play_through && next_animation_name.empty())
				{
					if ((animation_info->animation_time - animation_comp->playing_animation_blend_time) > animation_comp->playing_time)
					{
						start_blend = false;
					}
				}
				else if (animation_comp->play_through && std::find(this->animations.begin(), this->animations.end(), animation_name) == this->animations.end())
				{
					if ((animation_info->animation_time - animation_comp->playing_animation_blend_time) > animation_comp->playing_time)
					{
						start_blend = false;
					}
				}

			}






			//// Get next animation
			//std::string next_animation = animation_comp->next_animation;
			//if (std::find(this->animations.begin(), this->animations.end(), next_animation) == this->animations.end())
			//{
			//	printf("animation %s not found.\n", next_animation.c_str());
			//	next_animation = "";
			//}

			//if(next_animation.empty())
			//{
			//	
			//}



		}

		bool cAnimator::GetAnimation1(Component::Animation* animation_comp, std::string& animation1)
		{



			return true;
		}



	}
}




