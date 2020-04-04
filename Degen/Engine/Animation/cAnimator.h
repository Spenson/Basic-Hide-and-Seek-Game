#pragma once
#include "../Entity/cEntityManager.h"
#include "../Component/Animation.h"
#include <assimp/anim.h>


namespace Degen
{
	namespace Animation
	{
		struct sAnimationInfo
		{
			// loader created
			aiAnimation* animation;

			double frames;
			unsigned frames_per_sec;

			double animation_time;
			

			//read in from assets
			std::string name;
			std::string file;

			int animation_index; //index in file

			bool update_position_x;
			bool update_position_y;
			bool update_position_z;

			bool update_rotation_x;
			bool update_rotation_y;
			bool update_rotation_z;
			

		};

		class cAnimator
		{
		public:
			void Update(double dt);

			void AddEntity(Entity::cEntity* entity);


			void CalculateTransforms(Entity::cEntity* entity, double dt);

			std::map<std::string, sAnimationInfo*> animations;
			std::vector<Entity::cEntity*> entities;
		private:
			bool GetAnimation1(Component::Animation* animation_comp, std::string& animation1);
		};
	}
}
