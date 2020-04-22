#version 420


layout(vertices = 3) out;



struct VertOut
{
	vec4 colour;
	vec4 position;
	vec4 normal;
	vec4 uv_x2;
	vec4 tangent;
	vec4 bi_normal;
};
in VertOut vOut[];


struct TessContOut
{
	vec4 colour;
	vec4 position;
	vec4 normal;
	vec4 uv_x2;
	vec4 tangent;
	vec4 bi_normal;
};
out TessContOut tcOut[];


uniform vec4 eyeLocation;


float GetTessLevel(float Distance0, float Distance1)
{
	float AvgDistance = (Distance0 + Distance1) / 2.0;

	if (AvgDistance <= 2.0)
	{
		return 10.0;
	}
	else if (AvgDistance <= 5.0)
	{
		return 7.0;
	}
	else
	{
		return 3.0;
	}
}


void main(void)
{

	tcOut[gl_InvocationID].colour =		vOut[gl_InvocationID].colour;
	tcOut[gl_InvocationID].position =	vOut[gl_InvocationID].position;
	tcOut[gl_InvocationID].normal =		vOut[gl_InvocationID].normal;
	tcOut[gl_InvocationID].uv_x2 =		vOut[gl_InvocationID].uv_x2;
	tcOut[gl_InvocationID].tangent =	vOut[gl_InvocationID].tangent;
	tcOut[gl_InvocationID].bi_normal =	vOut[gl_InvocationID].bi_normal;


	vec3 face_norm = (vOut[0].normal.xyz +
					  vOut[1].normal.xyz +
					  vOut[2].normal.xyz) / 3.0f;

	vec3 tri_center = (vOut[0].position.xyz +
					   vOut[1].position.xyz +
					   vOut[2].position.xyz) / 3.0f;




	float tess_level = (1.f - smoothstep(distance(tri_center, eyeLocation.xyz), 0, 10)) * 5.f;
	tess_level = ceil(tess_level);
	tess_level = clamp(tess_level, 1.f, 5.f);


	vec3 vert_eye_vec = eyeLocation.xyz - tri_center;
	vert_eye_vec = normalize(vert_eye_vec);
	float eye_normal_dot = dot(vert_eye_vec, face_norm);

	if (eye_normal_dot < -0.3)
	{
		// don't tessellate triangles facing away
		gl_TessLevelOuter[0] = 1;
		gl_TessLevelOuter[1] = 1;
		gl_TessLevelOuter[2] = 1;
		gl_TessLevelInner[0] = 1;
	}
	else if (eye_normal_dot < 0.4)
	{
		float mod = (1.f - eye_normal_dot) * 6.f;
		// The triangle is "on an edge", so tesselate extra
		gl_TessLevelOuter[0] = tess_level * mod;
		gl_TessLevelOuter[1] = tess_level * mod;
		gl_TessLevelOuter[2] = tess_level * mod;
		gl_TessLevelInner[0] = tess_level * mod;		// 3.0;
	}
	else
	{
		gl_TessLevelOuter[0] = tess_level * 3.f;
		gl_TessLevelOuter[1] = tess_level * 3.f;
		gl_TessLevelOuter[2] = tess_level * 3.f;
		gl_TessLevelInner[0] = tess_level * 3.f;
	}


	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;




	return;
}