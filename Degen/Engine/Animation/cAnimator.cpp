#include "cAnimator.h"
#include "../Component/Animation.h"
#include "../Component/Position.h"
#include "../Component/Transform.h"

namespace Degen
{
	namespace Animation
	{
		glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4& mat)
		{
			return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1,
							 mat.a2, mat.b2, mat.c2, mat.d2,
							 mat.a3, mat.b3, mat.c3, mat.d3,
							 mat.a4, mat.b4, mat.c4, mat.d4);
		}



		cAnimator::cAnimator()
		{
			for (size_t i = 0; i < 5; ++i)
				workers.emplace_back(std::thread(Worker(*this)));

		}

		cAnimator::~cAnimator()
		{
			// stop all threads
			stop = true;
			worker_condition.notify_all();

			// join them
			for (size_t i = 0; i < workers.size(); ++i)
				workers[i].join();
		}

		void cAnimator::Update(double dt)
		{
			//only set here before threads start so do not need to lock
			this->dt = dt;


			for (auto* entity : entities)
			{
				enqueue(entity);
			}
			
			while (!empty_queue())
			{
				std::unique_lock<std::mutex> lock(loop_done_mutex);
				loop_done_condition.wait(lock);
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


		void cAnimator::CalculateTransforms(Entity::cEntity* entity)
		{
			Component::Animation* animation_comp = dynamic_cast<Component::Animation*>(entity->GetComponent(Component::ANIMATION_COMPONENT));

			animation_comp->playing_time += this->dt;

			// TODO: validate default on start
			//if (animation_comp->default_animation.empty())
			//{
			//	printf("default animation_info must be set on %s.\n", entity->name.c_str());
			//	return;
			//}


			// Get current animation_info
			std::string animation_name = animation_comp->playing_animation;
			double animation_blend_time = animation_comp->playing_animation_blend_time;
			if (animation_name == "forest_guard@walk")
			{
				printf("");
			}
			if (animation_name.empty())
			{
				animation_name = animation_comp->default_animation;
				animation_blend_time = animation_comp->default_blend_time;

				animation_comp->playing_animation = animation_comp->default_animation;
				animation_comp->playing_animation_blend_time = animation_comp->default_blend_time;
				animation_comp->play_through = true;
				animation_comp->lock_animation = false;
			}
			else if (!AnimationManager->HasAnimation(animation_name))
				//else if (std::find(this->animations.begin(), this->animations.end(), animation_name) == this->animations.end())
			{
				printf("animation %s not found.\n", animation_name.c_str());
				animation_name = animation_comp->default_animation;
				animation_blend_time = animation_comp->default_blend_time;
				animation_comp->play_through = true;
				animation_comp->lock_animation = false;

				if (!AnimationManager->HasAnimation(animation_name))
				{
					printf("animation %s not found.\n", animation_name.c_str());
					printf("default animation must be valid on %s.\n", entity->name.c_str());
					return;
				}
			}

			sAnimationInfo* animation_info = AnimationManager->GetAnimation(animation_name);

			std::string next_animation_name = animation_comp->next_animation;

			double blending_time = animation_comp->blending_time;
			if (blending_time == 0.0)
			{
				if (animation_name == next_animation_name)
				{
					animation_comp->next_animation.clear();
				}
				else if (animation_comp->lock_animation)
				{
					if ((animation_info->animation_time - animation_blend_time) <= animation_comp->playing_time)
					{
						blending_time = animation_comp->playing_time - (animation_info->animation_time - animation_blend_time);
					}
				}
				else if (animation_comp->play_through && next_animation_name.empty())
				{
					if ((animation_info->animation_time - animation_blend_time) <= animation_comp->playing_time)
					{
						blending_time = animation_comp->playing_time - (animation_info->animation_time - animation_blend_time);
					}
				}
				else if (animation_comp->play_through && !AnimationManager->HasAnimation(next_animation_name))
				{
					printf("animation %s not found.\n", next_animation_name.c_str());
					if ((animation_info->animation_time - animation_blend_time) <= animation_comp->playing_time)
					{
						blending_time = animation_comp->playing_time - (animation_info->animation_time - animation_blend_time);

					}
				}
				else
				{
					blending_time += this->dt;
				}

				if (next_animation_name.empty())
				{
					next_animation_name = animation_comp->default_animation;
				}

			}
			else
			{
				blending_time += this->dt;
			}

			if (blending_time > animation_blend_time)
			{
				animation_comp->playing_animation = animation_comp->next_animation;
				animation_comp->playing_animation_blend_time = animation_comp->next_animation_blend_time;
				animation_comp->play_through = animation_comp->next_play_through;
				animation_comp->lock_animation = animation_comp->next_lock_animation;

				animation_comp->playing_time = blending_time;
				animation_comp->next_animation = "";

				blending_time = 0.0;
			}

			animation_comp->blending_time = blending_time;


			std::vector< glm::mat4x4 > offsets;
			std::vector< glm::mat4x4 > globals;

			BoneTransform(animation_comp->playing_time, animation_info, animation_comp->bone_transforms, globals, offsets);

			if (blending_time > 0.0)
			{
				sAnimationInfo* next_animation_info = AnimationManager->GetAnimation(next_animation_name);
				if (!next_animation_info)
				{
					return;
				}
				std::vector<glm::mat4x4> bone_transforms;
				std::vector<glm::mat4x4> globals_next;
				std::vector<glm::mat4x4> offsets_next;

				BoneTransform(animation_comp->blending_time, next_animation_info, bone_transforms, globals_next, offsets_next);

				float blend_amount = glm::smoothstep(0.0, animation_comp->playing_animation_blend_time, animation_comp->blending_time);

				for (unsigned int BoneIndex = 0; BoneIndex < animation_comp->bone_transforms.size(); BoneIndex++)
				{
					//animation_comp->bone_transforms[BoneIndex] = glm::mix(animation_comp->bone_transforms[BoneIndex],bone_transforms[BoneIndex], blend_amount);

					animation_comp->bone_transforms[BoneIndex] = animation_comp->bone_transforms[BoneIndex] * (1.f - blend_amount)
						+ bone_transforms[BoneIndex] * blend_amount;
				}

			}


		}

		const aiNodeAnim* cAnimator::FindNodeAnimationChannel(const aiAnimation* pAnimation, aiString boneName)
		{
			for (unsigned int ChannelIndex = 0; ChannelIndex != pAnimation->mNumChannels; ChannelIndex++)
			{
				if (pAnimation->mChannels[ChannelIndex]->mNodeName == boneName)
				{
					return pAnimation->mChannels[ChannelIndex];
				}
			}
			return nullptr;
		}

		void cAnimator::ReadNodeHeirarchy(float AnimationTime, sAnimationInfo* animation_info, sModelBoneInfo* bone_info, const aiNode* pNode, const glm::mat4& parentTransformMatrix, std::vector<glm::mat4>& FinalTransformation, std::vector<glm::mat4>& Globals)
		{

			aiString NodeName(pNode->mName.data);


			// Original version picked the "main scene" animation_info...
			const aiAnimation* pAnimation = animation_info->animation;



			glm::mat4 NodeTransformation = AIMatrixToGLMMatrix(pNode->mTransformation);

			const aiNodeAnim* pNodeAnim = this->FindNodeAnimationChannel(pAnimation, NodeName);

			if (pNodeAnim)
			{
				// Get interpolated scaling
				glm::vec3 scale;
				this->CalcGLMInterpolatedScaling(AnimationTime, pNodeAnim, scale);
				glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), scale);

				// Get interpolated rotation (quaternion)
				glm::quat ori;
				this->CalcGLMInterpolatedRotation(AnimationTime, pNodeAnim, ori);
				glm::mat4 RotationM = glm::mat4_cast(ori);

				// Get interpolated position 
				glm::vec3 pos;
				this->CalcGLMInterpolatedPosition(AnimationTime, pNodeAnim, pos);
				glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), pos);


				// Combine the above transformations
				NodeTransformation = TranslationM * RotationM * ScalingM;
			}

			glm::mat4 ObjectBoneTransformation = parentTransformMatrix * NodeTransformation;

			std::map<std::string, unsigned int>::iterator it = bone_info->bone_name_index.find(std::string(NodeName.data));
			if (it != bone_info->bone_name_index.end())
			{

				unsigned int BoneIndex = it->second;
				//, FinalTransformation, Globals
				Globals[BoneIndex] = ObjectBoneTransformation;
				FinalTransformation[BoneIndex] = bone_info->mGlobalInverseTransformation
					* ObjectBoneTransformation
					* bone_info->Offsets[BoneIndex];

			}

			for (unsigned int ChildIndex = 0; ChildIndex != pNode->mNumChildren; ChildIndex++)
			{
				this->ReadNodeHeirarchy(AnimationTime, animation_info, bone_info,
										pNode->mChildren[ChildIndex], ObjectBoneTransformation, FinalTransformation, Globals);
			}

		}


		void cAnimator::BoneTransform(float TimeInSeconds, sAnimationInfo* animation_info, std::vector<glm::mat4>& FinalTransformation, std::vector<glm::mat4>& Globals, std::vector<glm::mat4>& Offsets)
		{
			glm::mat4 Identity(1.0f);

			float TicksPerSecond = static_cast<float>(animation_info->animation->mTicksPerSecond != 0 ?
													  animation_info->animation->mTicksPerSecond : 30.0);

			float TimeInTicks = TimeInSeconds * TicksPerSecond;

			float AnimationTime = fmod(TimeInTicks, (float)animation_info->animation->mDuration);

			sModelBoneInfo* bone_info = AnimationManager->model_bones[animation_info->model];

			FinalTransformation.resize(bone_info->bone_count);
			Globals.resize(bone_info->bone_count);
			Offsets.resize(bone_info->bone_count);

			this->ReadNodeHeirarchy(AnimationTime, animation_info, bone_info, animation_info->scene->mRootNode, Identity, FinalTransformation, Globals);

			for (unsigned int BoneIndex = 0; BoneIndex < bone_info->bone_count; BoneIndex++)
			{
				Offsets[BoneIndex] = bone_info->Offsets[BoneIndex];
			}

		}


		void cAnimator::CalcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out)
		{
			if (pNodeAnim->mNumRotationKeys == 1)
			{
				out.w = pNodeAnim->mRotationKeys[0].mValue.w;
				out.x = pNodeAnim->mRotationKeys[0].mValue.x;
				out.y = pNodeAnim->mRotationKeys[0].mValue.y;
				out.z = pNodeAnim->mRotationKeys[0].mValue.z;
				return;
			}

			unsigned int RotationIndex = this->FindRotation(AnimationTime, pNodeAnim);
			unsigned int NextRotationIndex = (RotationIndex + 1);
			assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
			float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
			float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
			if (Factor < 0.0f) Factor = 0.0f;
			if (Factor > 1.0f) Factor = 1.0f;
			const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
			const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

			glm::quat StartGLM = glm::quat(StartRotationQ.w, StartRotationQ.x, StartRotationQ.y, StartRotationQ.z);
			glm::quat EndGLM = glm::quat(EndRotationQ.w, EndRotationQ.x, EndRotationQ.y, EndRotationQ.z);

			out = glm::slerp(StartGLM, EndGLM, Factor);

			out = glm::normalize(out);

			return;
		}

		void cAnimator::CalcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out)
		{
			if (pNodeAnim->mNumPositionKeys == 1)
			{
				out.x = pNodeAnim->mPositionKeys[0].mValue.x;
				out.y = pNodeAnim->mPositionKeys[0].mValue.y;
				out.z = pNodeAnim->mPositionKeys[0].mValue.z;
				return;
			}

			unsigned int PositionIndex = this->FindPosition(AnimationTime, pNodeAnim);
			unsigned int NextPositionIndex = (PositionIndex + 1);
			assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
			float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
			float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
			if (Factor < 0.0f) Factor = 0.0f;
			if (Factor > 1.0f) Factor = 1.0f;
			const aiVector3D& StartPosition = pNodeAnim->mPositionKeys[PositionIndex].mValue;
			const aiVector3D& EndPosition = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
			glm::vec3 start = glm::vec3(StartPosition.x, StartPosition.y, StartPosition.z);
			glm::vec3 end = glm::vec3(EndPosition.x, EndPosition.y, EndPosition.z);

			out = (end - start) * Factor + start;

			return;
		}

		void cAnimator::CalcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out)
		{
			if (pNodeAnim->mNumScalingKeys == 1)
			{
				out.x = pNodeAnim->mScalingKeys[0].mValue.x;
				out.y = pNodeAnim->mScalingKeys[0].mValue.y;
				out.z = pNodeAnim->mScalingKeys[0].mValue.z;
				return;
			}

			unsigned int ScalingIndex = this->FindScaling(AnimationTime, pNodeAnim);
			unsigned int NextScalingIndex = (ScalingIndex + 1);
			assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
			float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
			float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
			if (Factor < 0.0f) Factor = 0.0f;
			if (Factor > 1.0f) Factor = 1.0f;
			const aiVector3D& StartScale = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
			const aiVector3D& EndScale = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
			glm::vec3 start = glm::vec3(StartScale.x, StartScale.y, StartScale.z);
			glm::vec3 end = glm::vec3(EndScale.x, EndScale.y, EndScale.z);
			out = (end - start) * Factor + start;

			return;
		}

		unsigned int cAnimator::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			for (unsigned int RotationKeyIndex = 0; RotationKeyIndex != pNodeAnim->mNumRotationKeys - 1; RotationKeyIndex++)
			{
				if (AnimationTime < (float)pNodeAnim->mRotationKeys[RotationKeyIndex + 1].mTime)
				{
					return RotationKeyIndex;
				}
			}

			return 0;
		}

		unsigned int cAnimator::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			for (unsigned int PositionKeyIndex = 0; PositionKeyIndex != pNodeAnim->mNumPositionKeys - 1; PositionKeyIndex++)
			{
				if (AnimationTime < (float)pNodeAnim->mPositionKeys[PositionKeyIndex + 1].mTime)
				{
					return PositionKeyIndex;
				}
			}

			return 0;
		}

		unsigned int cAnimator::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			for (unsigned int ScalingKeyIndex = 0; ScalingKeyIndex != pNodeAnim->mNumScalingKeys - 1; ScalingKeyIndex++)
			{
				if (AnimationTime < (float)pNodeAnim->mScalingKeys[ScalingKeyIndex + 1].mTime)
				{
					return ScalingKeyIndex;
				}
			}

			return 0;
		}

		void cAnimator::enqueue(Entity::cEntity* entity)
		{
			{ // acquire lock
				std::unique_lock<std::mutex> lock(queue_mutex);

				// add the task
				tasks.push_back(entity);
			} // release lock

			// wake up one thread
			worker_condition.notify_one();
		}

		bool cAnimator::empty_queue()
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			return tasks.empty();
		}

		void cAnimator::Worker::operator()()
		{
			//std::function<ThreadPool::FuncSig> task;
			Entity::cEntity* entity;
			while (true)
			{
				{   // acquire lock
					std::unique_lock<std::mutex> lock(pool.queue_mutex);

					// look for a work item
					while (!pool.stop && pool.tasks.empty())
					{ // No Work
						pool.worker_condition.wait(lock);
					}

					if (pool.stop) return; //Done

					entity = pool.tasks.front();

					pool.tasks.pop_front();

				}   // release lock

				// execute the task
				pool.CalculateTransforms(entity);

				// so I don't have to do a is done loop can just wait to be notified
				if (pool.empty_queue())
				{
					pool.loop_done_condition.notify_one();
				}
			}

		}
	}
}


