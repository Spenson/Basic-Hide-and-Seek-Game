#version 420


in vec4 vColour;
in vec4 vPosition;
in vec4 vNormal;				// Vertex normal
in vec4 vUVx2;					// 2 x Texture coords
in vec4 vTangent;				// For bump mapping
in vec4 vBiNormal;				// For bump mapping
in vec4 vBoneID;				// For skinned mesh (FBX)
in vec4 vBoneWeight;			// For skinned mesh (FBX)

// set per loop
uniform mat4 matView; 		// View or camera
uniform mat4 matProj;		// Projection transform


// set per object
uniform mat4 matModel;
uniform mat4 matInvTrans;

const int MAXNUMBEROFBONES = 100;
uniform bool isSkinnedMesh;

uniform mat4 animation[MAXNUMBEROFBONES];

struct VertOut
{
	vec4 colour;
	vec4 position;
	vec4 normal;
	vec4 uv_x2;
	vec4 tangent;
	vec4 bi_normal;
};
out VertOut vOut;




void main()
{
	//in vec4 vColour;
	//in vec4 vUVx2;					// 2 x Texture coords
	// vec4 colour;
	// vec4 uv_x2;
	vOut.colour = vColour;
	vOut.uv_x2 = vUVx2;

	vOut.tangent = vTangent;
	vOut.bi_normal = vBiNormal;


	mat4 matMVP = matProj * matView * matModel;
	if (isSkinnedMesh)
	{


		// *************************************************
		//in vec4 vBoneID;				// For skinned mesh (FBX)
		//in vec4 vBoneWeight;			// For skinned mesh (FBX)
		mat4 BoneTransform = animation[int(vBoneID[0])] * vBoneWeight[0];
		BoneTransform1 += animation[int(vBoneID[1])] * vBoneWeight[1];
		BoneTransform1 += animation[int(vBoneID[2])] * vBoneWeight[2];
		BoneTransform1 += animation[int(vBoneID[3])] * vBoneWeight[3];


		// position after animation
		//in vec4 vPosition;
		vec4 position_bone_transformed = BoneTransform * vec4(vPosition.xyz, 1.0f);

	
		// Projected/screen relitive position
		gl_Position = matMVP * position_bone_transformed;


		// Position in 3d space
		// vec4 position;
		vOut.position = matModel * position_bone_transformed;

		//invert transpose bone transform
		mat4 matBonematInvTrans = inverse(transpose(BoneTransform));

		//in vec4 vNormal;				// Vertex normal
		vec4 normal_bonetrans = matBonematInvTrans * vec4(normalize(vNormal.xyz), 1.0f);
		// vec4 normal;
		normal_bonetrans = matInvTrans * normal_bonetrans;
		vOut.normal = normal_bonetrans;
		vOut.normal.w = 1.0f;

		//in vec4 vTangent;				// For bump mapping
		vec4 tangent_bonetrans = matBonematInvTrans * vTangent;
		// vec4 tangent;
		vOut.tangent = matInvTrans * tangent_bonetrans;


		//in vec4 vBiNormal;				// For bump mapping
		vec4 binorm_bonetrans = matBonematInvTrans * vBiNormal;
		// vec4 bi_normal;
		vOut.bi_normal = matInvTrans * binorm_bonetrans;


		// *************************************************
	}
	else
	{	

		//in vec4 vPosition;
		gl_Position = matMVP * vec4(vPosition.xyz, 1.0f);

		// Vertex location in "world space"
		// Vec4 = mat4x4 * vec4
		//in vec4 vPosition;
		// vec4 position;
		vOut.position = matModel * vec4(vPosition.xyz, 1.0f);


		//in vec4 vNormal;				// Vertex normal
		// vec4 normal;
		vOut.normal = matInvTrans * vec4(normalize(vNormal.xyz), 1.0f);
		vOut.normal.w = 1.0f;

	}

	return;
}
