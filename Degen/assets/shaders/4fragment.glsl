#version 420

//in vec4 fColour;
//in vec4 gOut.position;
//in vec4 fNormal;
//in vec4 fUVx2;

struct GeoOut
{
	vec4 colour;		
	vec4 position;	// "World space"
	vec4 normal;	// "Model space"
	vec4 uv_x2;		// To the next shader stage
	vec4 tangent;	// Tangent to the surface
	vec4 bi_normal;	// bi-normal (or bi-tangent) to the surface
};
in GeoOut gOut;



// set per loop
uniform vec4 eyeLocation;
uniform float Width;
uniform float Height;

// set per pass
uniform int passNumber;

//set per object
uniform vec4 diffuseColour;				// use a for transparency		
uniform vec4 specularColour;
uniform vec4 colour_ratios;
uniform vec4 modifiers;


// Texture samplers
uniform sampler2D texture00;
uniform sampler2D texture01;
uniform samplerCube skybox00;
uniform samplerCube skybox01;




// deffered
uniform sampler2D textureColour;
uniform sampler2D textureNormal;
uniform sampler2D texturePosition;
uniform sampler2D textureSpecular;

out vec4 colourOut;			// RGB A   (0 to 1) 
out vec4 normalOut;
out vec4 worldPosOut;
out vec4 specularOut;


// Lighting
struct sLight
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
					// 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

uniform float Ambient;
const int NUMBEROFLIGHTS = 100;
layout(std140) uniform LightBlock
{
	sLight theLights[NUMBEROFLIGHTS];
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;




// methods
void Pass00(void);
void Pass01(void);
void Pass02(void);
vec4 calcualteLightContrib(vec3 vertexMaterialColour, vec3 vertexNormal, vec3 vertexWorldPos, vec4 vertexSpecular);


void main()
{
	// init so something happens even if render doesn't work
	colourOut = gOut.colour;
	normalOut = gOut.normal;
	worldPosOut = gOut.position;
	worldPosOut.w = 1;
	specularOut = specularColour;

	if (passNumber == 0)
	{
		Pass00();
		return;
	}
	if (passNumber == 1)
	{
		Pass01();
		return;
	}


	return;
}


vec4 calcualteLightContrib(vec3 vertexMaterialColour, vec3 vertexNormal, vec3 vertexWorldPos, vec4 vertexSpecular)
{
	vec3 norm = normalize(vertexNormal);

	vec4 finalObjectColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	for (int index = 0; index < NUMBEROFLIGHTS; index++)
	{
		// ********************************************************
		// is light "on"
		if (theLights[index].param2.x == 0.0f)
		{	// it's off
			continue;
		}

		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);

		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if (intLightType == DIRECTIONAL_LIGHT_TYPE)		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;

			// Get the dot product of the light and normalize
			float dotProduct = dot(-theLights[index].direction.xyz,
								   normalize(norm.xyz));	// -1 to 1

			dotProduct = max(0.0f, dotProduct);		// 0 to 1

			lightContrib *= dotProduct;

			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib);
			//+ (materialSpecular.rgb * lightSpecularContrib.rgb);
		// NOTE: There isn't any attenuation, like with sunlight.
		// (This is part of the reason directional lights are fast to calculate)

		// All done with this light.
			continue;
		}

		// Assume it's a point light 
		// intLightType = 0

		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);

		// Add a distance check here. 
		// Are we "too far" from this light (i.e. the light isn't contributing to the scene)?
		// (atten.w is the DistanceCutOff for this light)
		if (distanceToLight > theLights[index].atten.w)
		{
			// Make light contribution black
			finalObjectColour.rgb += vec3(1.0f, 0.0f, 0.0f);
			// All done with this light
			continue;
		}

		// If here, then the light IS contributing to the scene.

		vec3 lightVector = normalize(vLightToVertex);
		// -1 to 1
		float dotProduct = dot(lightVector, vertexNormal.xyz);

		// If it's negative, will clamp to 0 --- range from 0 to 1
		dotProduct = max(0.0f, dotProduct);

		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;


		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);

		vec3 reflectVector = reflect(-lightVector, normalize(norm.xyz));

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w;

		//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
		//			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
		lightSpecularContrib = pow(max(0.0f, dot(eyeVector, reflectVector)), objectSpecularPower)
			* theLights[index].specular.rgb;

		// Attenuation
		float attenuation = 1.0f /
			(theLights[index].atten.x +
			 theLights[index].atten.y * distanceToLight +
			 theLights[index].atten.z * distanceToLight * distanceToLight);

		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;


		// But is it a spot light
		if (intLightType == SPOT_LIGHT_TYPE)		// = 1
		{


			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
				= dot(vertexToLight.xyz, theLights[index].direction.xyz);

			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));

			// Is it completely outside of the spot?
			if (currentLightRayAngle < outerConeAngleCos)
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if (currentLightRayAngle < innerConeAngleCos)
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) /
					(innerConeAngleCos - outerConeAngleCos);

				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}

		}// if ( intLightType == 1 )



		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
			+ (vertexSpecular.rgb * lightSpecularContrib.rgb);

	}//for(intindex=0...

	finalObjectColour.a = 1.0f;

	return finalObjectColour;
}



void Pass00(void)
{
	bool is_cube_texture = bool(modifiers.x);
	bool ignore_lighting = bool(modifiers.y);
	bool TBD1 = bool(modifiers.z);
	bool TBD2 = bool(modifiers.w);

	// Shader Type #2 Imposters
	//if (int(modifiers.x) == IMPOSTER_RENDER_MODE)
	//{
	//	// If true, then:
	//	// - don't light
	//	// - texture map
	//	// - Use colour to compare to black and change alpha 
	//	// - Use colour to compare the black for discard
	//	//vec3 texRGB = texture( texture00, fUVx2.st ).rgb;

	//	vec3 tex0_RGB = texture(texture00, fUVx2.st).rgb;
	//	vec3 tex1 = texture(texture01, fUVx2.st).rgb;
	//	vec3 tex2 = texture(texture02, fUVx2.st).rgb;
	//	vec3 tex3_RGB = texture(texture03, fUVx2.st).rgb;

	//	vec3 texRGB = (colour_ratios.x * tex0_RGB)
	//		+ (colour_ratios.y * tex1)
	//		+ (colour_ratios.z * tex2)
	//		+ (colour_ratios.w * tex3_RGB);

	//	// Note that your eye doesn't see this, 
	//	// Use this equation instead: 0.21 R + 0.72 G + 0.07 B
	//	float grey = (texRGB.r + texRGB.g + texRGB.b) / 3.0f;

	//	// If it's REALLY black, then discard
	//	if (grey < 0.05) { discard; }


	//	colourOut.rgb = texRGB.rgb;

	//	// Otherwise control alpha with "black and white" amount
	//	colourOut.a = grey;
	//	if (colourOut.a < diffuseColour.a)
	//	{
	//		colourOut.a = diffuseColour.a;
	//	}


	//	normalOut = fNormal;
	//	normalOut.a = 0.f;

	//	worldPosOut = gOut.position;
	//	worldPosOut.a = 1.f;

	//	specularOut = specularColour;
	//	specularOut.a = 1.f;


	//	// colourOut.a = diffuseColour.a;
	//	return;
	//}

	vec4 tex1 = vec4(0.f,0.f,0.f,0.f);
	vec4 tex2 = vec4(0.f,0.f,0.f,0.f);

	// cube map textures
	if (is_cube_texture)
	{
		tex1 = texture(skybox00, gOut.normal.xyz);
		tex2 = texture(skybox01, gOut.normal.xyz);
	}
	// 2d textures
	else
	{
		tex1 = texture(texture00, gOut.uv_x2.st);
		tex2 = texture(texture01, gOut.uv_x2.st);
	}

	vec4 materialColour = diffuseColour;

	materialColour =
		(diffuseColour * colour_ratios.x) +
		(tex1 * colour_ratios.y) +
		(tex2 * colour_ratios.z);



	if (diffuseColour.a <= 0.01f)		// Basically "invisable"
	{
		discard;
	}


	colourOut = materialColour;
	colourOut.a = 1.f;

	normalOut.xyz = gOut.normal.xyz;
	if (ignore_lighting)
	{
		normalOut.w = 0.f;
	}
	else
	{
		normalOut.w = 1.f;
		worldPosOut.xyz = gOut.position.xyz;
		worldPosOut.w = 1.f;


		specularOut.rgb = specularColour.rgb;

		// scale specular power to fix blend issues
		specularOut.w = specularColour.w * (1.0f / 10000.0f);
		if (specularOut.w > 1.f) specularOut.w = 1.f;
		if (specularOut.w < 0.f) specularOut.w = 0.f;
	}


	return;
}

void Pass01(void)
{
	vec2 uvs = gOut.uv_x2.st;

	uvs.s = gl_FragCoord.x / float(Width);		// "u" or "x"
	uvs.t = gl_FragCoord.y / float(Height);		// "v" or "y"


	vec4 vertexcolour = texture(textureColour, uvs.st).rgba;
	vec4 normal = texture(textureNormal, uvs.st).rgba;
	vec4 position = texture(texturePosition, uvs.st).rgba;
	vec4 specular = texture(textureSpecular, uvs.st).rgba;

	if (normal.a == 1.f)
	{
		specular.w *= 10000.f;
		vec4 colour = calcualteLightContrib(vertexcolour.rgb, normalize(normal.xyz), position.xyz, specular);
		colourOut.rgb = colour.rgb;
		colourOut.a = 1.0f;

		//colourOut.rgb = normal.rgb;
	}
	else
	{
		colourOut.rgb = vertexcolour.rgb;
		colourOut.a = 1.0f;
	}



	return;
}