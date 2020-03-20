#pragma once
#include <string>
#include "sComplexVertex.h"


namespace Degen
{
	namespace VAOAndModel
	{
		struct sModelDrawInfo
		{
			sModelDrawInfo();

			std::string mesh_name;

			unsigned int vao_id;

			unsigned int vertex_buffer_id;
			unsigned int vertex_buffer_start_index;
			unsigned int number_of_vertices;

			unsigned int index_buffer_id;
			unsigned int index_buffer_start_index;
			unsigned int number_of_indices;
			
			unsigned int number_of_triangles;

			// The "local" (i.e. "CPU side" temporary array)
			// To be deleted when sent to GPU
			sComplexVertex* vertices;
			// The index buffer (CPU side)
			unsigned int* indices;

			// You could store the max and min values of the 
			//  vertices here (determined when you load them):
			float max_x, max_y, max_z;
			float min_x, min_y, min_z;

			float extent_x, extent_y, extent_z;
			float max_extent;
		};
	}
}
