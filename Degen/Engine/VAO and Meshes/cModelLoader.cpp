#include "cModelLoader.h"

#include <assimp/scene.h>			// To handle the scene, mesh, etc. object
#include <assimp/postprocess.h>		// For generating normals, etc.
#include <iostream>



namespace Degen
{
	namespace VAOAndModel
	{
		sModelDrawInfo* cModelLoader::LoadBasicModel(std::string file_name, std::string friendly_name, std::string& errors)
		{
			// Step 1: Read File
			const aiScene* pScene = NULL;
			Assimp::Importer mImporter;

			unsigned int Flags = aiProcess_Triangulate |
				aiProcess_OptimizeMeshes |
				aiProcess_OptimizeGraph |
				aiProcess_JoinIdenticalVertices |
				aiProcess_GenNormals |
				aiProcess_CalcTangentSpace;

			std::cout << std::endl;
			std::cout << "LoadBasicModel:" << file_name << std::endl;

			pScene = mImporter.ReadFile(file_name.c_str(), Flags);

			errors.clear();
			errors.append(mImporter.GetErrorString());
			if (errors != "")
			{
				return false;
			}

			// Step 2: Get Size Info
			//std::cout << "numMeshes = " << pScene->mNumMeshes << std::endl;
			sModelDrawInfo* draw_info = new sModelDrawInfo();

			draw_info->number_of_vertices = 0;
			draw_info->number_of_triangles = 0;
			for (unsigned mesh_idx = 0; mesh_idx < pScene->mNumMeshes; mesh_idx++)
			{
				draw_info->number_of_vertices += pScene->mMeshes[mesh_idx]->mNumVertices;
				draw_info->number_of_triangles += pScene->mMeshes[mesh_idx]->mNumFaces;
			} // for (unsigned mesh_idx...


			draw_info->number_of_indices = draw_info->number_of_triangles * 3;


			// Step 3: Get Vertex Info
			draw_info->vertices = new sComplexVertex[draw_info->number_of_vertices];

			unsigned int draw_info_idx = 0;
			for (unsigned mesh_idx = 0; mesh_idx < pScene->mNumMeshes; mesh_idx++)
			{
				aiMesh* mesh = pScene->mMeshes[mesh_idx];

				for (int vert_idx = 0; vert_idx != mesh->mNumVertices; vert_idx++, draw_info_idx++)
				{
					sComplexVertex* pCurVert = &(draw_info->vertices[draw_info_idx]);

					// Position
					pCurVert->x = pScene->mMeshes[mesh_idx]->mVertices[vert_idx].x;
					pCurVert->y = pScene->mMeshes[mesh_idx]->mVertices[vert_idx].y;
					pCurVert->z = pScene->mMeshes[mesh_idx]->mVertices[vert_idx].z;

					// Side Step: max and min extent
					if (pCurVert->x > draw_info->max_x) { draw_info->max_x = pCurVert->x; }
					if (pCurVert->x < draw_info->min_x) { draw_info->min_x = pCurVert->x; }

					if (pCurVert->y > draw_info->max_y) { draw_info->max_y = pCurVert->y; }
					if (pCurVert->y < draw_info->min_y) { draw_info->min_y = pCurVert->y; }

					if (pCurVert->z > draw_info->max_z) { draw_info->max_z = pCurVert->z; }
					if (pCurVert->z < draw_info->min_z) { draw_info->min_z = pCurVert->z; }

					// Colour
					if (mesh->GetNumColorChannels() > 0)
					{
						pCurVert->r = mesh->mColors[vert_idx]->r;
						pCurVert->g = mesh->mColors[vert_idx]->g;
						pCurVert->b = mesh->mColors[vert_idx]->b;
						pCurVert->a = mesh->mColors[vert_idx]->a;
					}
					else
					{
						pCurVert->r = pCurVert->g = pCurVert->b = pCurVert->a = 1.0f;
					}

					// Normals
					if (mesh->HasNormals())
					{
						pCurVert->nx = mesh->mNormals[vert_idx].x;
						pCurVert->ny = mesh->mNormals[vert_idx].y;
						pCurVert->nx = mesh->mNormals[vert_idx].z;
					}

					// Texture Coords
					if (mesh->GetNumUVChannels() > 0)
					{	// Assume 1st channel is the 2D UV coordinates
						pCurVert->u0 = mesh->mTextureCoords[0][vert_idx].x;
						pCurVert->v0 = mesh->mTextureCoords[0][vert_idx].y;
						if (mesh->GetNumUVChannels() > 1)
						{
							pCurVert->u1 = mesh->mTextureCoords[1][vert_idx].x;
							pCurVert->v1 = mesh->mTextureCoords[1][vert_idx].y;
						}
					}

				} // for (int vert_idx...
			} // for (unsigned mesh_idx...

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
			for (unsigned mesh_idx = 0; mesh_idx < pScene->mNumMeshes; mesh_idx++)
			{
				aiMesh* mesh = pScene->mMeshes[mesh_idx];
				for (unsigned int triIndex = 0; triIndex < mesh->mNumFaces; triIndex++, index += 3)
				{
					aiFace* face = &(mesh->mFaces[triIndex]);

					draw_info->indices[index + 0] = face->mIndices[0];	// Triangle index #0
					draw_info->indices[index + 1] = face->mIndices[1];	// Triangle index #1
					draw_info->indices[index + 2] = face->mIndices[2];	// Triangle index #2

				}//for ( unsigned int triIndex...
			}
			draw_info->mesh_name = friendly_name;
			return draw_info;
		}
	}
}
