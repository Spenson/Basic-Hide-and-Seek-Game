#version 420

layout(triangles)               in;
layout(triangle_strip)          out;
layout(max_vertices = 3)        out;

uniform bool use_bump_map;

struct GeoOut
{
	vec4 colour;
	vec4 position;	// "World space"
	vec4 normal;	// "Model space"
	vec4 uv_x2;		// To the next shader stage
	//vec4 tangent;	// Tangent to the surface
	//vec4 bi_normal;	// bi-normal (or bi-tangent) to the surface
	mat3 tbn;
};
out GeoOut gOut;

//*
struct TessEvalOut
{
	vec4 colour;
	vec4 position;
	vec4 normal;
	vec4 uv_x2;
	vec4 tangent;
	vec4 bi_normal;
};
in TessEvalOut teOut[];



void main()
{
	gOut.colour =	teOut[0].colour;
	gOut.position = teOut[0].position;
	gOut.normal =	teOut[0].normal;
	gOut.uv_x2 =	teOut[0].uv_x2;
	//gOut.tangent =	teOut[0].tangent;
	//gOut.bi_normal =	teOut[0].bi_normal;

		gOut.tbn = (mat3(normalize(teOut[0].tangent.xyz), normalize(teOut[0].bi_normal.xyz), normalize(teOut[0].normal.xyz)));
	
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();


	gOut.colour = teOut[1].colour;
	gOut.position = teOut[1].position;
	gOut.normal = teOut[1].normal;
	gOut.uv_x2 = teOut[1].uv_x2;
	///gOut.tangent =	teOut[1].tangent;
	//gOut.bi_normal =	teOut[1].bi_normal;

		gOut.tbn = (mat3(normalize(teOut[1].tangent.xyz), normalize(teOut[1].bi_normal.xyz), normalize(teOut[1].normal.xyz)));
	
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();


	gOut.colour = teOut[2].colour;
	gOut.position = teOut[2].position;
	gOut.normal = teOut[2].normal;
	gOut.uv_x2 = teOut[2].uv_x2;
	//gOut.tangent =	teOut[2].tangent;
	//gOut.bi_normal =	teOut[2].bi_normal;

		gOut.tbn = (mat3(normalize(teOut[2].tangent.xyz), normalize(teOut[2].bi_normal.xyz), normalize(teOut[2].normal.xyz)));
	
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();


	EndPrimitive();
}
//*/

/*
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


void main()
{
	gOut.colour =		vOut[0].colour;
	gOut.position =		vOut[0].position;
	gOut.normal =		vOut[0].normal;
	gOut.uv_x2 =		vOut[0].uv_x2;
	gOut.tangent =		vOut[0].tangent;
	gOut.bi_normal =	vOut[0].bi_normal;

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();


	gOut.colour =		vOut[1].colour;
	gOut.position =		vOut[1].position;
	gOut.normal =		vOut[1].normal;
	gOut.uv_x2 =		vOut[1].uv_x2;
	gOut.tangent =		vOut[1].tangent;
	gOut.bi_normal =	vOut[1].bi_normal;

	gl_Position = gl_in[1].gl_Position;
	EmitVertex();


	gOut.colour =		vOut[2].colour;
	gOut.position =		vOut[2].position;
	gOut.normal =		vOut[2].normal;
	gOut.uv_x2 =		vOut[2].uv_x2;
	gOut.tangent =		vOut[2].tangent;
	gOut.bi_normal =	vOut[2].bi_normal;

	gl_Position = gl_in[2].gl_Position;
	EmitVertex();


	EndPrimitive();
}
//*/
