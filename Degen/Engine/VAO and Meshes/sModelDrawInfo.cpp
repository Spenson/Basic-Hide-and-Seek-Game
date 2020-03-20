#include "sModelDrawInfo.h"

namespace Degen
{
	namespace VAOAndModel
	{
		sModelDrawInfo::sModelDrawInfo() :
			mesh_name(),
			vertex_buffer_id(0),
			vertex_buffer_start_index(0),
			number_of_vertices(0),
			index_buffer_id(0),
			index_buffer_start_index(0),
			number_of_indices(0),
			number_of_triangles(0),
			vertices(NULL),
			indices(NULL),
			max_x(FLT_MIN), max_y(FLT_MIN), max_z(FLT_MIN),
			min_x(FLT_MAX), min_y(FLT_MAX), min_z(FLT_MAX),
			extent_x(0.f), extent_y(0.f), extent_z(0.f),
			max_extent(0.f)
		{
		}
	}
}