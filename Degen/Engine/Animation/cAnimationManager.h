#pragma once

#include <assimp/anim.h>
#include <assimp/scene.h>

#include "sModelBoneInfo.h"

namespace Degen
{
	namespace Animation
	{
		struct sAnimationInfo
		{
			// loader created
			aiAnimation* animation;
			const aiScene* scene; // for node heirarchy

			double frames;
			unsigned frames_per_sec;

			double animation_time;


			//read in from assets
			std::string name;
			std::string file;
			std::string model;

			int animation_index; //index of animation for file with multiple

			bool update_position_x;
			bool update_position_y;
			bool update_position_z;

			bool update_rotation_x;
			bool update_rotation_y;
			bool update_rotation_z;
		};

		class cAnimationManager
		{
		public:
			void AddAnimation(sAnimationInfo* info);
			sAnimationInfo* GetAnimation(std::string name);
			bool HasAnimation(std::string name);

			std::map<std::string, sAnimationInfo*> animations;
			std::map<std::string, sModelBoneInfo*> model_bones;
		};
	}
}