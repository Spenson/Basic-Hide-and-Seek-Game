#version 420

//uniform mat4 MVP;
//uniform mat4 matModel;		// Model or World 
//uniform mat4 matModelInverseTranspose;		// For normal calculation
struct buffer
{
	mat4 matModel;
	mat4 matModelInverseTranspose;

	vec4 diffuseColour;
	vec4 specularColour;
	vec4 boolModifiers;
};


uniform mat4 matView; 		// View or camera
uniform mat4 matProj;		// Projection transform

in vec4 vColour;				// Was vec3
in vec4 vPosition;				// Was vec3
in vec4 vNormal;				// Vertex normal
in vec4 vUVx2;					// 2 x Texture coords
in vec4 vTangent;				// For bump mapping
in vec4 vBiNormal;				// For bump mapping
in vec4 vBoneID;				// For skinned mesh (FBX)
in vec4 vBoneWeight;			// For skinned mesh (FBX)

//out vec3 color;
//out vec4 vertWorld;			// Location of the vertex in the world
out vec4 fColour;
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;

//uniform sampler2D heightMap;
//uniform bool isWater;
//uniform vec2 textOffset;

uniform bool useOffset;
uniform float offset;


const int MAXNUMBEROFBONES = 100;
uniform mat4 matBonesArray[MAXNUMBEROFBONES];
uniform bool isSkinnedMesh;

void main()
{
	vec4 vertPosition = vPosition;


	if (useOffset)
	{
		// Move the y value by some amount from texture	
		// Since it's black and white, I only sample 1 colour.

//		vec2 texUV1 = vUVx2.st + textOffset.xy;
//		float texValue1 = texture( heightMap, texUV1.st ).r;
//		float texValue2 = texture( heightMap, texUV1.st ).g;
		//float ratio1 = 1.0;


		// This will pick a completely different location
		// (note the reversal of the xy to yx, called a "swizzle")
		//vec2 texUV2 = vUVx2.st + textOffset.yx * vec2(-0.5f, 0.75f);	
		//float texValue2 = texture( heightMap, texUV2.st ).r;
		//float ratio2 = 1.5f;


		//vertPosition.y += (texValue1*ratio1) + (texValue2 * ratio2);
		vertPosition += vNormal * offset;
	}

	if (isSkinnedMesh)
	{
		// *************************************************
		mat4 BoneTransform = matBonesArray[int(vBoneID[0])] * vBoneWeight[0];
		BoneTransform += matBonesArray[int(vBoneID[1])] * vBoneWeight[1];
		BoneTransform += matBonesArray[int(vBoneID[2])] * vBoneWeight[2];
		BoneTransform += matBonesArray[int(vBoneID[3])] * vBoneWeight[3];

		// Apply the bone transform to the vertex:
		vec4 vertOriginal = vec4(vertPosition.xyz, 1.0f);

		vec4 vertAfterBoneTransform = BoneTransform * vertOriginal;

		mat4 matMVP = matProj * matView * matModel;
		// Transform the updated vertex location (from the bone)
		//  and transform with model view projection matrix (as usual)
		gl_Position = matMVP * vertAfterBoneTransform;


		// Then we do the normals, etc.
		fVertWorldLocation = matModel * vertAfterBoneTransform;

		// Updated "world" or "model" transform 
		mat4 matModelAndBone = matModel * BoneTransform;

		vec3 theNormal = normalize(vNormal.xyz);
		fNormal = inverse(transpose(matModelAndBone)) * vec4(theNormal, 1.0f);
		fNormal.xyz = normalize(fNormal.xyz);

		fColour = vColour;
		fUVx2 = vUVx2;


		// *************************************************
	}
	else
	{
		mat4 matMVP = matProj * matView * matModel;

		gl_Position = matMVP * vec4(vertPosition.xyz, 1.0);

		// Vertex location in "world space"
		// Vec4 = mat4x4 * vec4
		fVertWorldLocation = matModel * vec4(vertPosition.xyz, 1.0);

		//mat4 matModelInverseTranspose = inverse(transpose(matModel));

		vec3 theNormal = normalize(vNormal.xyz);
		fNormal = matModelInverseTranspose * vec4(theNormal, 1.0f);
		fNormal.xyz = normalize(fNormal.xyz);

		// Pass the colour and UV unchanged.
		fColour = vColour;
		fUVx2 = vUVx2;
	}
}