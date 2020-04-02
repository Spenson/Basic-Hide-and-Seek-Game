#include "cModelLoader.h"

#include <assimp/scene.h>			// To handle the scene, mesh, etc. object
#include <assimp/postprocess.h>		// For generating normals, etc.
#include <iostream>

#include <array>
#include <vector>

#include <fstream>

#include <glm.hpp>

namespace Degen
{
	namespace FileReading
	{

		static const int MAX_BONES_PER_VERTEX = 4;
		struct sVertexBoneData
		{
			//std::array<unsigned int, MAX_BONES_PER_VERTEX> ids;
			std::array<float, MAX_BONES_PER_VERTEX> ids;
			std::array<float, MAX_BONES_PER_VERTEX> weights;

			void AddBoneData(unsigned int BoneID, float Weight);
		};

		void sVertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
		{
			for (unsigned int Index = 0; Index < sizeof(this->ids) / sizeof(this->ids[0]); Index++)
			{
				if (this->weights[Index] == 0.0f)
				{
					this->ids[Index] = (float)BoneID;
					this->weights[Index] = Weight;
					return;
				}
			}
		}





		VAOAndModel::sModelDrawInfo* cModelLoader::LoadBasicModel(std::string file_name, std::string friendly_name, std::string& error, const unsigned int meshIndex)
		{
			const aiScene* pScene;
			if (mImporters.find(file_name) == mImporters.end())
			{
				unsigned int Flags = aiProcess_Triangulate |
					aiProcess_OptimizeMeshes |
					aiProcess_OptimizeGraph |
					aiProcess_JoinIdenticalVertices |
					aiProcess_GenNormals |
					aiProcess_CalcTangentSpace;

				pScene = mImporters[file_name].ReadFile((base_path + file_name).c_str(), Flags);
				if (!pScene)
				{
					error = "File Not Loaded: " + std::string(mImporters[file_name].GetErrorString());
					mImporters.erase(file_name);
					return nullptr;
				}
			}
			else
			{
				pScene = mImporters[file_name].GetScene();
			}

			
			if (!pScene->HasMeshes())
			{
				error = "Scene has no meshes";
				mImporters.erase(file_name);
				return nullptr;
			}
			if (pScene->mNumMeshes <= meshIndex)
			{
				error = "Mesh Index Out Of Range";
				return nullptr;
			}


			aiMesh* mesh = pScene->mMeshes[meshIndex];


			VAOAndModel::sModelDrawInfo* draw_info = new VAOAndModel::sModelDrawInfo();

			draw_info->number_of_vertices = mesh->mNumVertices;


			draw_info->number_of_triangles = mesh->mNumFaces;
			draw_info->number_of_indices = draw_info->number_of_triangles * 3;

			draw_info->vertices = new VAOAndModel::sComplexVertex[draw_info->number_of_vertices];

			for (int vertIndex = 0; vertIndex != draw_info->number_of_vertices; vertIndex++)
			{
				VAOAndModel::sComplexVertex* pCurVert = &(draw_info->vertices[vertIndex]);

				aiVector3D* pAIVert = &(mesh->mVertices[vertIndex]);

				pCurVert->x = pAIVert->x;
				pCurVert->y = pAIVert->y;
				pCurVert->z = pAIVert->z;


				// Normals
				if (mesh->HasNormals())
				{
					pCurVert->nx = mesh->mNormals[vertIndex].x;
					pCurVert->ny = mesh->mNormals[vertIndex].y;
					pCurVert->nz = mesh->mNormals[vertIndex].z;
				}

				// UVs
				if (mesh->GetNumUVChannels() > 0)
				{	// Assume 1st channel is the 2D UV coordinates
					pCurVert->u0 = mesh->mTextureCoords[0][vertIndex].x;
					pCurVert->v0 = mesh->mTextureCoords[0][vertIndex].y;
					if (mesh->GetNumUVChannels() > 1)
					{
						pCurVert->u1 = mesh->mTextureCoords[1][vertIndex].x;
						pCurVert->v1 = mesh->mTextureCoords[1][vertIndex].y;
					}
				}


			}//for ( int vertIndex

			// Side Step: full extents
			draw_info->extent_x = draw_info->max_x - draw_info->min_x;
			draw_info->extent_y = draw_info->max_y - draw_info->min_y;
			draw_info->extent_z = draw_info->max_z - draw_info->min_z;

			draw_info->max_extent = draw_info->extent_x;
			if (draw_info->extent_y > draw_info->max_extent) { draw_info->max_extent = draw_info->extent_y; }
			if (draw_info->extent_z > draw_info->max_extent) { draw_info->max_extent = draw_info->extent_z; }


			// Step 4: Get Face Info
			draw_info->indices = new unsigned int[draw_info->number_of_indices];

			unsigned int index = 0;

			for (unsigned int triIndex = 0; triIndex < mesh->mNumFaces; triIndex++, index += 3)
			{
				aiFace* face = &(mesh->mFaces[triIndex]);

				draw_info->indices[index + 0] = face->mIndices[0];	// Triangle index #0
				draw_info->indices[index + 1] = face->mIndices[1];	// Triangle index #1
				draw_info->indices[index + 2] = face->mIndices[2];	// Triangle index #2

			}//for ( unsigned int triIndex...

			draw_info->mesh_name = friendly_name;
			return draw_info;
		}

		VAOAndModel::sModelDrawInfo* cModelLoader::LoadModel(std::string file_name, std::string friendly_name, std::string& error, const unsigned int meshIndex)
		{
			const aiScene* pScene;
			if (mImporters.find(file_name) == mImporters.end())
			{
				unsigned int Flags = aiProcess_Triangulate |
					aiProcess_OptimizeMeshes |
					aiProcess_OptimizeGraph |
					aiProcess_JoinIdenticalVertices |
					aiProcess_GenNormals |
					aiProcess_CalcTangentSpace;

				pScene = mImporters[file_name].ReadFile((base_path + file_name).c_str(), Flags);
				if (!pScene)
				{
					error = "File Not Loaded: " + std::string(mImporters[file_name].GetErrorString());
					mImporters.erase(file_name);
					return nullptr;
				}
			}
			else
			{
				pScene = mImporters[file_name].GetScene();
			}

			char* temp =(char*) pScene->mRootNode->mChildren[1]->mName.data;

			if (!pScene->HasMeshes())
			{
				error = "Scene has no meshes";
				mImporters.erase(file_name);
				return nullptr;
			}
			if (pScene->mNumMeshes <= meshIndex)
			{
				error = "Mesh Index Out Of Range";
				return nullptr;
			}

			
			aiMesh*	mesh = pScene->mMeshes[meshIndex];

			if(!mesh->HasBones())
			{
				error = "Mesh has no bones (" + std::to_string(pScene->mNumMeshes) + ")";
				return nullptr;
			}

			std::string name = file_name;
			
			std::vector<sVertexBoneData> vertexBoneData;
			vertexBoneData.resize(mesh->mNumVertices);
			for (unsigned int boneIndex = 0; boneIndex != mesh->mNumBones; boneIndex++)
			{
				unsigned int BoneIndex = 0;
				std::string BoneName(mesh->mBones[boneIndex]->mName.data);

				for (unsigned int WeightIndex = 0; WeightIndex != mesh->mBones[boneIndex]->mNumWeights; WeightIndex++)
				{
					unsigned int VertexID = mesh->mBones[boneIndex]->mWeights[WeightIndex].mVertexId;
					float Weight = mesh->mBones[boneIndex]->mWeights[WeightIndex].mWeight;
					vertexBoneData[VertexID].AddBoneData(BoneIndex, Weight);
				}
			}

			VAOAndModel::sModelDrawInfo* draw_info = new VAOAndModel::sModelDrawInfo();

			draw_info->number_of_vertices = mesh->mNumVertices;


			draw_info->number_of_triangles = mesh->mNumFaces;
			draw_info->number_of_indices = draw_info->number_of_triangles * 3;

			draw_info->vertices = new VAOAndModel::sComplexVertex[draw_info->number_of_vertices];

			for (int vertIndex = 0; vertIndex != draw_info->number_of_vertices; vertIndex++)
			{
				VAOAndModel::sComplexVertex* pCurVert = &(draw_info->vertices[vertIndex]);

				aiVector3D* pAIVert = &(mesh->mVertices[vertIndex]);

				pCurVert->x = pAIVert->x;
				pCurVert->y = pAIVert->y;
				pCurVert->z = pAIVert->z;

				// Colours
				if (mesh->GetNumColorChannels() > 0)
				{
					pCurVert->r = mesh->mColors[0]->r;
					pCurVert->g = mesh->mColors[0]->g;
					pCurVert->b = mesh->mColors[0]->b;
					pCurVert->a = mesh->mColors[0]->a;
				}
				else
				{
					pCurVert->r = pCurVert->g = pCurVert->b = pCurVert->a = 1.0f;
				}

				// Normals
				if (mesh->HasNormals())
				{
					pCurVert->nx = mesh->mNormals[vertIndex].x;
					pCurVert->ny = mesh->mNormals[vertIndex].y;
					pCurVert->nz = mesh->mNormals[vertIndex].z;
				}

				// UVs
				if (mesh->GetNumUVChannels() > 0)
				{	// Assume 1st channel is the 2D UV coordinates
					pCurVert->u0 = mesh->mTextureCoords[0][vertIndex].x;
					pCurVert->v0 = mesh->mTextureCoords[0][vertIndex].y;
					if (mesh->GetNumUVChannels() > 1)
					{
						pCurVert->u1 = mesh->mTextureCoords[1][vertIndex].x;
						pCurVert->v1 = mesh->mTextureCoords[1][vertIndex].y;
					}
				}

				// Tangents and Bitangents (bi-normals)
				if (mesh->HasTangentsAndBitangents())
				{
					pCurVert->tx = mesh->mTangents[vertIndex].x;
					pCurVert->ty = mesh->mTangents[vertIndex].y;
					pCurVert->tz = mesh->mTangents[vertIndex].z;

					pCurVert->bx = mesh->mBitangents[vertIndex].x;
					pCurVert->by = mesh->mBitangents[vertIndex].y;
					pCurVert->bz = mesh->mBitangents[vertIndex].z;
				}


				// Bone IDs are being passed OK
				pCurVert->bone_id[0] = vertexBoneData[vertIndex].ids[0];
				pCurVert->bone_id[1] = vertexBoneData[vertIndex].ids[1];
				pCurVert->bone_id[2] = vertexBoneData[vertIndex].ids[2];
				pCurVert->bone_id[3] = vertexBoneData[vertIndex].ids[3];

				// Weights are being passed OK
				pCurVert->bone_weights[0] = vertexBoneData[vertIndex].weights[0];
				pCurVert->bone_weights[1] = vertexBoneData[vertIndex].weights[1];
				pCurVert->bone_weights[2] = vertexBoneData[vertIndex].weights[2];
				pCurVert->bone_weights[3] = vertexBoneData[vertIndex].weights[3];


			}//for ( int vertIndex

			// Side Step: full extents
			draw_info->extent_x = draw_info->max_x - draw_info->min_x;
			draw_info->extent_y = draw_info->max_y - draw_info->min_y;
			draw_info->extent_z = draw_info->max_z - draw_info->min_z;

			draw_info->max_extent = draw_info->extent_x;
			if (draw_info->extent_y > draw_info->max_extent) { draw_info->max_extent = draw_info->extent_y; }
			if (draw_info->extent_z > draw_info->max_extent) { draw_info->max_extent = draw_info->extent_z; }


			// Step 4: Get Face Info
			draw_info->indices = new unsigned int[draw_info->number_of_indices];

			unsigned int index = 0;

			for (unsigned int triIndex = 0; triIndex < mesh->mNumFaces; triIndex++, index += 3)
			{
				aiFace* face = &(mesh->mFaces[triIndex]);

				draw_info->indices[index + 0] = face->mIndices[0];	// Triangle index #0
				draw_info->indices[index + 1] = face->mIndices[1];	// Triangle index #1
				draw_info->indices[index + 2] = face->mIndices[2];	// Triangle index #2

			}//for ( unsigned int triIndex...

			draw_info->mesh_name = friendly_name;
			return draw_info;
		}
	
		
		VAOAndModel::sModelDrawInfo* cModelLoader::LoadPlyModel(std::string file_name, std::string friendly_name, std::string& error)
		{

			std::ifstream theFile((base_path + file_name).c_str());
			if (!theFile.is_open())
			{
				// On no! Where's the file??? 
				return nullptr;
			}

			// Scan the file until I get to "vertex", and stop
			std::string temp;
			//theFile >> temp;
			while (theFile >> temp)
			{
				// Have it hit the word "vertex"?
				if (temp == "vertex")
				{
					break;		// Exit the while loop
				}
			}// while ( theFile >> temp ) 

			unsigned int numberOfVertices;
			theFile >> numberOfVertices;



			while (theFile >> temp)
			{
				// Have it hit the word "face"?
				if (temp == "face")
				{
					break;		// Exit the while loop
				}
			}// while ( theFile >> temp ) 

			unsigned int numberOfTriangles;
			theFile >> numberOfTriangles;

			while (theFile >> temp)
			{
				// Have it hit the word "end_header"?
				if (temp == "end_header")
				{
					break;		// Exit the while loop
				}
			}// while ( theFile >> temp ) 

			
			VAOAndModel::sModelDrawInfo* draw_info = new VAOAndModel::sModelDrawInfo();
			draw_info->number_of_vertices = numberOfVertices;
			draw_info->vertices = new VAOAndModel::sComplexVertex[draw_info->number_of_vertices];

			// Read all the vertices
			for (unsigned int index = 0; index != numberOfVertices; index++)
			{
				// -0.0312216 0.126304 0.00514924
				VAOAndModel::sComplexVertex* tempVertex = &(draw_info->vertices[index]);
				// -0.0312216 0.126304 0.00514924
				theFile >> tempVertex->x >> tempVertex->y >> tempVertex->z;

				// Also load the normals
				glm::vec3 normal;
				theFile >> normal.x >> normal.y >> normal.z;
				normal = glm::normalize(normal);
				tempVertex->nx = normal.x;
				tempVertex->ny = normal.y;
				tempVertex->nz = normal.z;

				// Also load the texture coordinates
				theFile >> tempVertex->u0 >> tempVertex->v0;

			}


			draw_info->number_of_triangles = numberOfTriangles;
			draw_info->number_of_indices = numberOfTriangles*3;

			draw_info->indices = new unsigned int[draw_info->number_of_indices];
			for (unsigned int index = 0; index != draw_info->number_of_indices; index+=3)
			{
				// 3 166 210 265 
				int discardThis;
				//sPlyTriangle tempTriangle;

				theFile >> discardThis
					>> draw_info->indices[index]
					>> draw_info->indices[index+1]
					>> draw_info->indices[index+2];

				// Add this triangle
				//theMesh.vecTriangles.push_back(tempTriangle);
			}

			draw_info->mesh_name = friendly_name;
			return draw_info;
		}
	}
}
