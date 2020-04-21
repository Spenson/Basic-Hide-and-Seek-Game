#version 400 core


layout(triangles, equal_spacing, ccw) in;


struct TessContOut
{
	vec4 colour;
	vec4 position;
	vec4 normal;
	vec4 uv_x2;
	vec4 tangent;
	vec4 bi_normal;
};
in TessContOut tcOut[];


struct TessEvalOut
{
	vec4 colour;
	vec4 position;
	vec4 normal;
	vec4 uv_x2;
	vec4 tangent;
	vec4 bi_normal;
};
out TessEvalOut teOut;

vec2 interpolate(vec2 v0, vec2 v1, vec2 v2)
{
	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate(vec3 v0, vec3 v1, vec3 v2)
{
	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}
vec4 interpolate(vec4 v0, vec4 v1, vec4 v2)
{
	return vec4(gl_TessCoord.x) * v0 + vec4(gl_TessCoord.y) * v1 + vec4(gl_TessCoord.z) * v2;
}


void main()
{

	teOut.colour.xyz = interpolate(tcOut[0].colour.xyz, tcOut[1].colour.xyz, tcOut[2].colour.xyz);
	teOut.colour.w = 1.f;

	teOut.position.xyz = interpolate(tcOut[0].position.xyz, tcOut[1].position.xyz, tcOut[2].position.xyz);
	teOut.position.w = 0.f;

	teOut.normal.xyz = interpolate(tcOut[0].normal.xyz, tcOut[1].normal.xyz, tcOut[2].normal.xyz);
	teOut.normal.xyz = normalize(teOut.normal.xyz);
	teOut.normal.w = 1.f;

	teOut.uv_x2.xy = interpolate(tcOut[0].uv_x2.xy, tcOut[1].uv_x2.xy, tcOut[2].uv_x2.xy);
	teOut.uv_x2.zw = interpolate(tcOut[0].uv_x2.zw, tcOut[1].uv_x2.zw, tcOut[2].uv_x2.zw);


	teOut.tangent.xyz = interpolate(tcOut[0].tangent.xyz, tcOut[1].tangent.xyz, tcOut[2].tangent.xyz);
	teOut.tangent.xyz = normalize(teOut.tangent.xyz);
	teOut.tangent.w = 1.f;

	teOut.bi_normal.xyz = interpolate(tcOut[0].bi_normal.xyz, tcOut[1].bi_normal.xyz, tcOut[2].bi_normal.xyz);
	teOut.bi_normal.xyz = normalize(teOut.bi_normal.xyz);
	teOut.bi_normal.w = 1.f;


	gl_Position.xyzw = interpolate(gl_in[0].gl_Position.xyzw, gl_in[1].gl_Position.xyzw, gl_in[2].gl_Position.xyzw);


	// Displace the vertex along the normal
	//float Displacement = texture(gDisplacementMap, TexCoord_FS_in.xy).x;
	//WorldPos_FS_in += Normal_FS_in * Displacement * gDispFactor;
	//gl_Position = gVP * vec4(WorldPos_FS_in, 1.0);



	// gl_TessCoord is the coordinate of the vertex within the current patch

	// Push out the location based on the normal
	//gl_Position.xyzw =	gl_in[0].gl_Position.xyzw * gl_TessCoord.x +
	//					gl_in[1].gl_Position.xyzw * gl_TessCoord.y +
	//					gl_in[2].gl_Position.xyzw * gl_TessCoord.z;

	//teOut.position =	tcOut[0].position.xyzw * gl_TessCoord.x +
	//					tcOut[0].position.xyzw * gl_TessCoord.y +
	//					tcOut[0].position.xyzw * gl_TessCoord.z;

	//// You can't use a loop for this as the indices must be constant
	//teOut.normal.xyzw = tcOut[0].normal.xyzw * gl_TessCoord.x +
	//					tcOut[1].normal.xyzw * gl_TessCoord.y +
	//					tcOut[2].normal.xyzw * gl_TessCoord.z;

	//teOut.normal.w = 1.0f;


	//teOut.uv_x2 =	tcOut[0].uv_x2 * gl_TessCoord.x +
	//				tcOut[1].uv_x2 * gl_TessCoord.y +
	//				tcOut[2].uv_x2 * gl_TessCoord.z;

	////teOut.uv_x2.zw = tcOut[0].uv_x2.zw * gl_TessCoord.x +
	////					tcOut[1].uv_x2.zw * gl_TessCoord.y +
	////					tcOut[2].uv_x2.zw * gl_TessCoord.z;

	//teOut.colour =	tcOut[0].colour * gl_TessCoord.x +
	//				tcOut[1].colour * gl_TessCoord.y +
	//				tcOut[2].colour * gl_TessCoord.z;
	//
	//teOut.tangent = tcOut[0].tangent * gl_TessCoord.x +
	//				tcOut[1].tangent * gl_TessCoord.y +
	//				tcOut[2].tangent * gl_TessCoord.z;

	//teOut.bi_normal =	tcOut[0].bi_normal * gl_TessCoord.x +
	//					tcOut[1].bi_normal * gl_TessCoord.y +
	//					tcOut[2].bi_normal * gl_TessCoord.z;




	return;
}
