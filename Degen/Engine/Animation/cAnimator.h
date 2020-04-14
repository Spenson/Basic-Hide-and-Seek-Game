#pragma once
#include "../Entity/cEntityManager.h"
#include "../Component/Animation.h"
#include "../Globals.h"

namespace Degen
{
	namespace Animation
	{
		class cAnimator
		{
		public:
			void Update(double dt);

			void AddEntity(Entity::cEntity* entity);


			void CalculateTransforms(Entity::cEntity* entity, double dt);

			std::vector<Entity::cEntity*> entities;
		private:
			void BoneTransform(float TimeInSeconds, sAnimationInfo* animation_info,
							   std::vector<glm::mat4>& FinalTransformation,
							   std::vector<glm::mat4>& Globals,
							   std::vector<glm::mat4>& Offsets);
			const aiNodeAnim* FindNodeAnimationChannel(const aiAnimation* pAnimation, aiString boneName);
			void ReadNodeHeirarchy(float AnimationTime, sAnimationInfo* animation_info, sModelBoneInfo* bone_info, const aiNode* pNode, const glm::mat4& parentTransformMatrix,
								   std::vector<glm::mat4>& FinalTransformation, std::vector<glm::mat4>& Globals);

			void CalcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out);
			void CalcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);
			void CalcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);

			unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
			unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
			unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

		};
	}
}
