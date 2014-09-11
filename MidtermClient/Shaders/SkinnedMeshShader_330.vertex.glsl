#version 330 

uniform   vec3 u_cameraPosition;
uniform   mat4 u_boneTM[ 200 ];
uniform   mat4 u_mvpMatrix;
uniform	  mat4 u_cameraSpaceToWorldSpaceTransform;
uniform   mat4 u_worldSpaceToCameraSpaceTransform;
uniform   mat4 u_modelSpaceTransform;
uniform   mat4 u_projectionMatrix;
uniform   int  u_useTextures;
in		  vec4 u_vertex;
in		  vec4 u_color;

in		  vec3 a_tangent;
in        vec3 a_bitangent;
in		  vec3 a_normal;

in		  vec2 u_diffuseTextureCoords;
in		  vec2 u_normalMapTextureCoords;
in        vec2 u_bumpMapTextureCoords;

in		  ivec4 a_boneIndexes;
in		   vec4 a_boneWeights;  



out vec3		v_screenPosition;
out vec4		v_worldPosition;
out vec4		v_surfaceColor;
out vec2		v_diffuseTexture;
flat out int	v_useTextures;
out vec3		v_cameraPosition;

out vec3		v_tangent;
out vec3		v_bitangent;
out vec3		v_normal;

void main()
 {


	vec4 vertPosition = vec4( 0.0, 0.0, 0.0, 0.0 );
	vec4 skinnedNormal = vec4( 0.0, 0.0, 0.0, 0.0 );
	vec4 skinnedTangent = vec4( 0.0, 0.0, 0.0, 0.0 );
	vec4 skinnedBitangent = vec4( 0.0, 0.0, 0.0, 0.0 );
	vec4 normalVec4 = vec4( a_normal.x, a_normal.y, a_normal.z, 0.0 );
	vec4 tangentVec4 = vec4( a_tangent.x, a_tangent.y, a_tangent.z, 0.0 );
	vec4 bitangentVec4 = vec4( a_bitangent.x, a_bitangent.y, a_bitangent.z, 0.0 );

	int boneIndex = 0;
	for ( boneIndex; boneIndex < 4; ++boneIndex ) {

		float boneWeight = a_boneWeights[boneIndex];
		int matrixIndex = a_boneIndexes[boneIndex];
		mat4 currentBoneTM = u_boneTM[ matrixIndex ];

		vertPosition += ( ( currentBoneTM * u_vertex ) * boneWeight );
		skinnedNormal += ( ( currentBoneTM * normalVec4 ) * boneWeight );
		skinnedTangent += ( ( currentBoneTM * tangentVec4 ) * boneWeight );
		skinnedBitangent += ( ( currentBoneTM * bitangentVec4 ) * boneWeight );
	}

	vertPosition.w = 1.0;

	gl_Position = u_mvpMatrix * vertPosition;
	v_screenPosition = gl_Position.xyz;
	v_worldPosition = u_modelSpaceTransform * u_vertex;
	v_surfaceColor = u_color;
	v_diffuseTexture = u_diffuseTextureCoords.xy;
	v_useTextures = u_useTextures;
	
	v_normal = skinnedNormal.xyz;
	v_tangent = skinnedTangent.xyz;
	v_bitangent = skinnedBitangent.xyz;

	v_cameraPosition = u_cameraPosition;
}

