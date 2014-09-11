#version 330 

uniform   mat4 u_mvpMatrix;
uniform   int  u_useTextures;
uniform	  vec3 u_lightPosition;
uniform   vec3 u_cameraPosition;

// Fog
uniform   vec4  u_fogColor;
uniform   float u_fogStartDist;
uniform   float u_fogEndDist;

in		  vec4 u_vertex;
in		  vec4 u_color;

in		  vec3 u_tangent;
in        vec3 u_bitangent;
in		  vec3 u_normal;

in		  vec2 u_diffuseTextureCoords;
// These may not be needed since tex coords should be the same as diffuse
in		  vec2 u_normalMapTextureCoords;
in        vec2 u_bumpMapTextureCoords;

out vec3		v_screenPosition;
out vec4		v_worldPosition;
out vec4		v_surfaceColor;
out vec2		v_diffuseTexture;
out vec3		v_lightPosition;
out vec3		v_cameraPosition;
flat out int	v_useTextures; // = - ) 

out vec3		v_tangent;
out vec3		v_bitangent;
out vec3		v_normal;

// Fog
out vec4  v_fogColor;
out float v_fogStartDist;
out float v_fogEndDist;



void main()
 {
	gl_Position = u_mvpMatrix * u_vertex;
	v_screenPosition = gl_Position.xyz;
	v_worldPosition = u_vertex;
	v_surfaceColor = u_color;
	v_diffuseTexture = u_diffuseTextureCoords.xy;
	v_useTextures = u_useTextures;
	v_lightPosition = u_lightPosition;
	v_cameraPosition = u_cameraPosition;

	v_tangent = u_tangent;
	v_bitangent = u_bitangent;
	v_normal = u_normal;

	v_fogColor = u_fogColor;
	v_fogStartDist = u_fogStartDist;
	v_fogEndDist = u_fogEndDist;
	
}

