#version 330 

uniform   mat4 u_mvpMatrix;
uniform   int  u_useTextures;
in		  vec4 u_vertex;
in		  vec4 u_color;

in		  vec2 u_diffuseTextureCoords;
in		  vec2 u_normalMapTextureCoords;
in        vec2 u_bumpMapTextureCoords;

out vec3		v_screenPosition;
out vec4		v_worldPosition;
out vec4		v_surfaceColor;
out vec2		v_diffuseTexture;
flat out int	v_useTextures;



void main()
 {
	gl_Position = u_mvpMatrix * u_vertex;
	v_screenPosition = gl_Position.xyz;
	v_worldPosition = u_vertex;
	v_surfaceColor = u_color;
	v_diffuseTexture = u_diffuseTextureCoords.xy;
	v_useTextures = u_useTextures;
	
}


