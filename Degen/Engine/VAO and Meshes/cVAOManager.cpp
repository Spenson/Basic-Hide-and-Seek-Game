#include "cVAOManager.h"
#include <glad/glad.h>

namespace Degen
{
	namespace VAOAndModel
	{
		// TODO: convert to single VAO with all models
		bool cVAOManager::LoadModelDrawInfoIntoVAO(sModelDrawInfo& drawInfo, unsigned int shaderProgramID)
		{
			// Ask OpenGL for a new buffer ID...
			glGenVertexArrays(1, &(drawInfo.vao_id));
			// "Bind" this buffer:
			// - aka "make this the 'current' VAO buffer
			glBindVertexArray(drawInfo.vao_id);


			// NOTE: OpenGL error checks have been omitted for brevity
			glGenBuffers(1, &(drawInfo.vertex_buffer_id));

			//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertex_buffer_id);
			// sVert vertices[3]
			glBufferData(GL_ARRAY_BUFFER,
						 sizeof(sComplexVertex) * drawInfo.number_of_vertices,
						 (GLvoid*)drawInfo.vertices,
						 GL_DYNAMIC_DRAW);
			// GL_STATIC_DRAW );


		// Copy the index buffer into the video card, too
		// Create an index buffer.
			glGenBuffers(1, &(drawInfo.index_buffer_id));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.index_buffer_id);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
						 sizeof(unsigned int) * drawInfo.number_of_indices,
						 (GLvoid*)drawInfo.indices,
						 GL_STATIC_DRAW);

			GLint vpos_location =		 glGetAttribLocation(shaderProgramID, "vPosition");	// program
			GLint vcol_location =		 glGetAttribLocation(shaderProgramID, "vColour");	// program;
			GLint vnorm_location =		 glGetAttribLocation(shaderProgramID, "vNormal");	// program;
			GLint vUV_location =		 glGetAttribLocation(shaderProgramID, "vUVx2");	// program;
			// Added					 
			GLint vTangent_location =	 glGetAttribLocation(shaderProgramID, "vTangent");
			GLint vBiNormal_location =	 glGetAttribLocation(shaderProgramID, "vBiNormal");
			GLint vBoneID_location =	 glGetAttribLocation(shaderProgramID, "vBoneID");
			GLint vBoneWeight_location = glGetAttribLocation(shaderProgramID, "vBoneWeight");


			// Set the vertex attributes for this shader
			glEnableVertexAttribArray(vpos_location);	// vPos
			glVertexAttribPointer(vpos_location, 4,		// now a vec4
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),						// sizeof(float) * 6,
								  (void*)(offsetof(sComplexVertex, x)));

			glEnableVertexAttribArray(vcol_location);	// vCol
			glVertexAttribPointer(vcol_location, 4,		// vCol
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),
								  (void*)(offsetof(sComplexVertex, r)));


			//	float nx, ny, nz, nw;
			glEnableVertexAttribArray(vnorm_location);	// vNormal
			glVertexAttribPointer(vnorm_location, 4,		// vNormal
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),
								  (void*)(offsetof(sComplexVertex, nx)));

			//	float u0, v0, u1, v1;
			glEnableVertexAttribArray(vUV_location);		// vUVx2
			glVertexAttribPointer(vUV_location, 4,		// vUVx2
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),
								  (void*)(offsetof(sComplexVertex, u0)));

			// New stuff added in animation (for bump mapping and skinned mesh)
			glEnableVertexAttribArray(vTangent_location);
			glVertexAttribPointer(vTangent_location, 4,
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),
								  (void*)(offsetof(sComplexVertex, tx)));

			glEnableVertexAttribArray(vBiNormal_location);
			glVertexAttribPointer(vBiNormal_location, 4,
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),
								  (void*)(offsetof(sComplexVertex, bx)));

			glEnableVertexAttribArray(vBoneID_location);
			glVertexAttribPointer(vBoneID_location, 4,
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),
								  (void*)(offsetof(sComplexVertex, bone_id[0])));

			glEnableVertexAttribArray(vBoneWeight_location);
			glVertexAttribPointer(vBoneWeight_location, 4,
								  GL_FLOAT, GL_FALSE,
								  sizeof(sComplexVertex),
								  (void*)(offsetof(sComplexVertex, bone_weights[0])));

			// Now that all the parts are set up, set the VAO to zero
			// At this point, whatever state the:
			// - vertex buffer (VBO)
			// - index buffer
			// - vertex layout 
			// ...is "remembered"
			glBindVertexArray(0);
			// Now, that VAO is "not active" (bound), so OpenGL is 
			//  not "pay attention" to any changes to any of those 3 things


			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableVertexAttribArray(vpos_location);
			glDisableVertexAttribArray(vcol_location);
			glDisableVertexAttribArray(vnorm_location);
			glDisableVertexAttribArray(vUV_location);
			// And the other ones
			glDisableVertexAttribArray(vTangent_location);
			glDisableVertexAttribArray(vBiNormal_location);
			glDisableVertexAttribArray(vBoneID_location);
			glDisableVertexAttribArray(vBoneWeight_location);


			// Store the draw information into the map
			this->m_name_draw_info[drawInfo.mesh_name] = drawInfo;


			return true;
		}

		bool cVAOManager::FindDrawInfoByModelName(std::string filename, sModelDrawInfo& drawInfo)
		{
			auto itDrawInfo = this->m_name_draw_info.find(filename);

			// Find it? 
			if (itDrawInfo == this->m_name_draw_info.end())
			{
				// Nope
				return false;
			}

			// Else we found the thing to draw
			// ...so 'return' that information
			drawInfo = itDrawInfo->second;
			return true;
		}

	}
}
