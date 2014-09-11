#version 330 

uniform   vec3 u_cameraPosition;
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

 	vec4 tangentVec4 = vec4( a_tangent.x, a_tangent.y, a_tangent.z, 0.0 );
 	vec4 bitangentVec4 = vec4( a_bitangent.x, a_bitangent.y, a_bitangent.z, 0.0 );
 	vec4 normalVec4 = vec4( a_normal.x, a_normal.y, a_normal.z, 0.0 );

 	vec4 transformedTangent = u_modelSpaceTransform * tangentVec4;
 	vec4 transformedBitangent = u_modelSpaceTransform * bitangentVec4;
 	vec4 transformedNormal = u_modelSpaceTransform * normalVec4;

 	v_tangent = transformedTangent.xyz;
 	v_bitangent = transformedBitangent.xyz;
 	v_normal = transformedNormal.xyz;

	gl_Position = u_mvpMatrix * u_vertex;
	v_screenPosition = gl_Position.xyz;
	v_worldPosition = u_modelSpaceTransform * u_vertex;
	v_surfaceColor = u_color;
	v_diffuseTexture = u_diffuseTextureCoords.xy;
	v_useTextures = u_useTextures;

	v_cameraPosition = u_cameraPosition;
	
}
