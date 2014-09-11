#version 330
// Fragment Shader, GLSL 3.30

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_depthTexture;

uniform int		u_renderWithDepthBuffer;
uniform int		u_frameBufferEffect;
uniform float   u_time;

in vec3			v_screenPosition;
in vec4			v_worldPosition; 
in vec4			v_surfaceColor;
in vec2			v_diffuseTexture;
flat in int		v_useTextures;
in  vec3		v_cameraPosition;


out vec4 fragColor;
//---------------------------------------------------------------------------------//
// For this example... remember the screen coordinates are NDC coordinates

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

float smoothNumber( float numToSmooth ) {
	float smoothedNum = ( ( 3.0 * numToSmooth * numToSmooth ) - ( 2.0 * ( numToSmooth * numToSmooth * numToSmooth ) ) );
	return smoothedNum;
}


bool doesDiskIntersectDisk( vec2 originOne, float radiusOne, vec2 originTwo, float radiusTwo ) {
	bool doesIntersect = false;

	float xDist = originOne.x - originTwo.x;
	float yDist = originOne.y - originTwo.y;

	float distanceSquaredBetweenDisks = ( xDist * xDist ) + ( yDist * yDist );
	float radiusSumSquared = ( radiusOne + radiusTwo ) * ( radiusOne + radiusTwo );

	doesIntersect = ( radiusSumSquared >= distanceSquaredBetweenDisks );

	return doesIntersect;
}
/*
inline bool doesDiskIntersectDiskOrTouch( const Disk2D & diskOne, const Disk2D & diskTwo ) {

	float xDist = diskOne.origin.x - diskTwo.origin.x;
	float yDist = diskOne.origin.y - diskTwo.origin.y;

	float distanceSquaredBetweenDisks = ( xDist * xDist ) + ( yDist * yDist );
	float radiusSumSquared = ( diskOne.radius + diskTwo.radius ) * ( diskOne.radius + diskTwo.radius );

	return ( radiusSumSquared >= distanceSquaredBetweenDisks );

} // end doesDiskintersectDisk
*/

void main()
{
	
	vec4 diffuseTexel = texture2D( u_diffuseTexture, v_diffuseTexture );
	vec4 depthTexel = texture2D( u_depthTexture, v_diffuseTexture );
	vec4 normalizedDepthTexel = normalize( depthTexel );
	vec2 modifiedTexCoords = v_diffuseTexture;
	modifiedTexCoords.x = v_diffuseTexture.x + 0.02;
	vec4 depthTexelAbove = texture2D( u_depthTexture, modifiedTexCoords );

	if ( u_renderWithDepthBuffer == 1 ) {
		fragColor = ( normalizedDepthTexel );
	} else {

		if ( u_frameBufferEffect == 0 ) {
			// Split Screen Default Effect
			vec4 testColor = vec4( 1.0, 1.0, 1.0, 1.0 );
			if ( v_screenPosition.x > 0 ) {
				testColor.x = 0.0;
			}
			fragColor = ( v_surfaceColor * diffuseTexel * testColor );
		} else if ( u_frameBufferEffect == 1 ) {
			// Night Vision Goggles
			const float innerRadius = 0.45;
			const float outerRadius = 0.48;
			vec2 gogglePosition = vec2( 0.0, 0.0 );
			vec4 finalColor = v_surfaceColor;

			vec2 texelOrigin = vec2( v_screenPosition.x, v_screenPosition.y * ( 9.0/16.0 ) );
			float texelRadius = 0.01;

			bool doesIntersectInner = doesDiskIntersectDisk( gogglePosition, innerRadius, texelOrigin, texelRadius );
			bool doesIntersectOuter = doesDiskIntersectDisk( gogglePosition, outerRadius, texelOrigin, texelRadius );

			if ( doesIntersectInner ) {
				finalColor.x = 0.0;
				finalColor.z = 0.0;
			} else if ( !doesIntersectInner && doesIntersectOuter ) {
				finalColor.x = 0.40;
				finalColor.y = 0.40;
				finalColor.z = 0.40;
			} else {
				finalColor.x = 0.15;
				finalColor.y = 0.15;
				finalColor.z = 0.15;
			}

			fragColor = ( finalColor * diffuseTexel );
		} else if ( u_frameBufferEffect == 2 ) {
			// Underwater Effect
			vec4 underwaterColor = vec4( 0.0, 0.0, 1.0, 1.0 );
			float zThreshhold = 0.0;
			if ( ( v_screenPosition.y * (9.0/16.0) ) < zThreshhold ) {
				
				float freq = 2.0;
				float amp = 0.01;
				float adjustedHorizontalTexel = v_diffuseTexture.x + sin( ( v_diffuseTexture.y + u_time ) * freq ) * amp;
				//float adjustedVerticalTexel = v_diffuseTexture.y + sin( ( v_diffuseTexture.x + u_time ) * freq ) * amp;
				float adjustedVerticalTexel = 1.0 - v_diffuseTexture.y;
				//adjustedVerticalTexel = 1.0 - adjustedVerticalTexel;
				//adjustedHorizontalTexel = 1.0 - adjustedHorizontalTexel;
				vec2 adjustedTextureCoords = v_diffuseTexture;
				adjustedTextureCoords.x = adjustedHorizontalTexel;
				adjustedTextureCoords.y = adjustedVerticalTexel;
				vec4 adjustedTexel = texture2D( u_diffuseTexture, adjustedTextureCoords );
				//u+sin((v+t)*FREQ)*AMP
				fragColor = ( underwaterColor * adjustedTexel );

			} else {
				fragColor = ( diffuseTexel * v_surfaceColor );
			}

		}

		
	}
	
}

