#pragma once
#include "../Entity/cEntityManager.h"
#include "../Component/Animation.h"
#include "../Globals.h"
#include <thread>
#include <mutex>

namespace Degen
{
	namespace Animation
	{

		class cAnimator
		{
		public:
			cAnimator();
			~cAnimator();
			
			void Update(double dt);

			void AddEntity(Entity::cEntity* entity);


		private:
			void CalculateTransforms(Entity::cEntity* entity);
			void AnimationOLD(Entity::cEntity* entity);
			void AnimationNEW(Entity::cEntity* entity);

			std::vector<Entity::cEntity*> entities;
			
			//usable by all threads
			float dt;
			
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


			class Worker
			{
			public:
				Worker(cAnimator& s) : pool(s) {}
				void operator()();
			private:
				cAnimator& pool;
			};

			void enqueue(Entity::cEntity* entity);
			bool empty_queue();

			std::vector< std::thread > workers;
			std::deque< Entity::cEntity* > tasks;
			std::mutex queue_mutex;
			std::condition_variable worker_condition;
			
			std::mutex loop_done_mutex;
			std::condition_variable loop_done_condition;
			
			bool stop;
			
		};
	}
}
