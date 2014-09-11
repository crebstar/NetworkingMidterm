#version 330
// Fragment Shader, GLSL 3.30

uniform mat4 u_cameraSpaceToWorldSpaceTransform;

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_specularTexture;
uniform sampler2D u_specularColorTexture;
uniform sampler2D u_emissiveTexture;

struct Light {

	vec3	position;
	vec3	direction;
	vec4	colorAndBrightness;
	float	innerRadius;
	float	outerRadius;
	float   innerApertureDot;
	float   outerApertureDot;
	float   fractionAmbient;
	float	ignoresPosition;
	float   isLocalAmbient;

};

const float		specBrightness = 0.65;
const int		MAX_SPEC_POWER = 62;
const int		MAX_NUM_LIGHTS = 16;
uniform Light	u_lightArray[MAX_NUM_LIGHTS];
uniform int		u_numberOfLights;

in vec3			v_screenPosition;
in vec4			v_worldPosition; 
in vec4			v_surfaceColor;
in vec2			v_diffuseTexture;
flat in int		v_useTextures;
in vec3			v_cameraPosition;

// TBN
in  vec3		v_tangent;
in  vec3		v_bitangent;
in  vec3		v_normal;

out vec4 fragColor;


float smoothNumber( float numToSmooth ) {

	float smoothedNum = ( ( 3.0 * numToSmooth * numToSmooth ) - ( 2.0 * ( numToSmooth * numToSmooth * numToSmooth ) ) );
	return smoothedNum;
}


float rangeMapFloat( float inRangeStart, float inRangeEnd, float outRangeStart, float outRangeEnd, float inValue ) {
	// Handle the zero edge case
	if ( inRangeStart == inRangeEnd  ) {
		return 0.50 * ( outRangeStart + outRangeEnd );
	}

	float outValue = inValue;

	outValue = outValue - inRangeStart;
	outValue = outValue / ( inRangeEnd - inRangeStart );
	outValue = outValue * ( outRangeEnd - outRangeStart );
	outValue = outValue + outRangeStart;

	return outValue;
}


void main()
{

	vec4 diffuseTexel = texture2D( u_diffuseTexture, v_diffuseTexture );

	vec4 diffuseColor = vec4( 0.0, 0.0, 0.0, 1.0 );

	vec3 normalizedNormal = normalize( v_normal );
	vec3 normalizedTangent = normalize( v_tangent );
	vec3 normalizedBitgangent = normalize( v_bitangent );

	mat3 tangentToWorldSpace = mat3( normalizedTangent, normalizedBitgangent, normalizedNormal );
	vec4 normalTexel = texture2D( u_normalTexture, v_diffuseTexture );
	normalTexel.x = (normalTexel.x * 2.0) - 1.0;
	normalTexel.y = (normalTexel.y * 2.0) - 1.0;
	normalTexel.z = (normalTexel.z * 2.0) - 1.0;

	vec3 normalInWorldSpace = tangentToWorldSpace * normalTexel.xyz;

	// Specular
	vec3 differenceVectorCameraToPoint = v_worldPosition.xyz - v_cameraPosition.xyz;
	vec3 normalizedVectorCameraToPoint = normalize( differenceVectorCameraToPoint );
	vec4 specularTexel = texture2D( u_specularTexture, v_diffuseTexture );
	vec4 specularColorTexel = texture2D( u_specularColorTexture, v_diffuseTexture );
	vec3 idealReflection = reflect( normalizedVectorCameraToPoint, normalInWorldSpace.xyz );
	vec3 normalizedIdealReflection = normalize( idealReflection );
	vec3 specularColor = vec3( 0.0, 0.0, 0.0 );
	
	for ( int i = 0; i < MAX_NUM_LIGHTS; i++ ) {

		
		// Point Light and Ambient Local
		vec3 differenceVectorLightToPosition = u_lightArray[i].position.xyz - v_worldPosition.xyz;
		float distanceFromPointToLight = length( differenceVectorLightToPosition );
		float fractionBrightnessDueToDistance = ( u_lightArray[i].outerRadius - distanceFromPointToLight ) / ( u_lightArray[i].outerRadius - u_lightArray[i].innerRadius );
		fractionBrightnessDueToDistance = clamp( fractionBrightnessDueToDistance * u_lightArray[i].colorAndBrightness.a , 0.0, 1.0 );
		fractionBrightnessDueToDistance = smoothNumber( fractionBrightnessDueToDistance );

			// Spotlight
		vec3 pointToLight = v_worldPosition.xyz - u_lightArray[i].position.xyz;
		vec3 pointToLightNormalized = normalize( pointToLight );
		vec3 normalizedDifferenceVectorLightToPosition = normalize( differenceVectorLightToPosition );
		float dotProductLToPWithLightForward = dot( pointToLightNormalized, u_lightArray[i].direction );
		float spotlightBrightness = rangeMapFloat( u_lightArray[i].outerApertureDot, u_lightArray[i].innerApertureDot, 0.0, 1.0, dotProductLToPWithLightForward );

		// Alternative way
		//float spotlightBrightness = (  dotProductLToPWithLightForward - u_lightArray[i].innerApertureDot  ) / ( u_lightArray[i].innerApertureDot - u_lightArray[i].outerApertureDot );
		spotlightBrightness = clamp( spotlightBrightness, 0.0, 1.0 );
		spotlightBrightness = smoothNumber( spotlightBrightness );
		
		// Diffuse Light (N) (*)
		float diffuseDotResult = dot( normalInWorldSpace, normalizedDifferenceVectorLightToPosition * ( 1.0 - u_lightArray[i].ignoresPosition ) );
		diffuseColor.x = diffuseColor.x + clamp(( diffuseDotResult * u_lightArray[i].colorAndBrightness.x * fractionBrightnessDueToDistance * spotlightBrightness  ),0.0, 1.0 );
		diffuseColor.y = diffuseColor.y + clamp(( diffuseDotResult * u_lightArray[i].colorAndBrightness.y * fractionBrightnessDueToDistance * spotlightBrightness ), 0.0, 1.0 );
		diffuseColor.z = diffuseColor.z + clamp(( diffuseDotResult * u_lightArray[i].colorAndBrightness.z * fractionBrightnessDueToDistance * spotlightBrightness  ), 0.0, 1.0 );
		
		// Global Directional Light
		float dotResultDirectionWithPixelNormal = dot( u_lightArray[i].direction *  u_lightArray[i].ignoresPosition, normalInWorldSpace );
		diffuseColor.x = diffuseColor.x + clamp( dotResultDirectionWithPixelNormal * u_lightArray[i].colorAndBrightness.x, 0.0, 1.0 );
		diffuseColor.y = diffuseColor.y + clamp( dotResultDirectionWithPixelNormal * u_lightArray[i].colorAndBrightness.y, 0.0, 1.0 );
		diffuseColor.z = diffuseColor.z + clamp( dotResultDirectionWithPixelNormal * u_lightArray[i].colorAndBrightness.z, 0.0, 1.0 );

		// Global Directional Specular Light
		// Ugh negative hacks
		float specGlobalDirectionalDot = dot( normalizedIdealReflection, -u_lightArray[i].direction * u_lightArray[i].ignoresPosition );
		float clampedSpecGlobalDirectionalDot = clamp( specGlobalDirectionalDot, 0.0, 1.0 );
		float globalDirectSpecPower = pow( clampedSpecGlobalDirectionalDot, 1 +  MAX_SPEC_POWER );
		specularColor.x = specularColor.x + ( specularTexel.x * globalDirectSpecPower * specBrightness );
		specularColor.y = specularColor.y + ( specularTexel.y * globalDirectSpecPower * specBrightness );
		specularColor.z = specularColor.z + ( specularTexel.z * globalDirectSpecPower * specBrightness );

		// Specular Light (N) (+)
		float specDotResult = dot( normalizedIdealReflection, normalizedDifferenceVectorLightToPosition  * ( 1.0 - u_lightArray[i].ignoresPosition ) );
		float clampedSpecDotResult = clamp( specDotResult, 0.0, 1.0 );
		float specPower = pow( clampedSpecDotResult, 1 +  MAX_SPEC_POWER );
		specPower = specPower * specBrightness; 
		specularColor.x = specularColor.x + ( specPower * specularTexel.x * specularColorTexel.x * fractionBrightnessDueToDistance * spotlightBrightness );
		specularColor.y = specularColor.y + ( specPower * specularTexel.y * specularColorTexel.y * fractionBrightnessDueToDistance * spotlightBrightness );
		specularColor.z = specularColor.z + ( specPower * specularTexel.z * specularColorTexel.z * fractionBrightnessDueToDistance * spotlightBrightness );

	}

	fragColor.w = 1.0;
	fragColor = ( diffuseTexel * diffuseColor * v_surfaceColor );

	// Apply specular
	specularColor.x = clamp( specularColor.x, 0.0, 1.0 );
	specularColor.y = clamp( specularColor.y, 0.0, 1.0 );
	specularColor.z = clamp( specularColor.z, 0.0, 1.0 );
	fragColor.x = fragColor.x + specularColor.x;
	fragColor.y = fragColor.y + specularColor.y;
	fragColor.z = fragColor.z + specularColor.z;

	// Emissive (1) (+)
	vec4 emissiveTexel = texture2D( u_emissiveTexture, v_diffuseTexture );
	fragColor.x = fragColor.x + emissiveTexel.x;
	fragColor.y = fragColor.y + emissiveTexel.y;
	fragColor.z = fragColor.z + emissiveTexel.z;

	
	//vec3 normalTexelDebug = vec3( normalInWorldSpace.x, normalInWorldSpace.y, normalInWorldSpace.z );
	//normalTexelDebug.x = (normalTexelDebug.x + 1.0) * 0.50;
	//normalTexelDebug.y = (normalTexelDebug.y + 1.0) * 0.50;
	//normalTexelDebug.z = (normalTexelDebug.z + 1.0) * 0.50;

	//fragColor.x = normalTexelDebug.x;
	//fragColor.y = normalTexelDebug.y;
	//fragColor.z = normalTexelDebug.z;
	//fragColor.w = 1.0;

}

