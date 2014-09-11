#version 330 

uniform   mat4 u_mvpMatrix;
uniform   int  u_useTextures;
uniform   vec3 u_cameraPosition;

in		  vec4 u_vertex;
in		  vec4 u_color;

in		  vec2 u_diffuseTextureCoords;

out vec3		v_screenPosition;
out vec4		v_worldPosition;
out vec4		v_surfaceColor;
out vec2		v_diffuseTexture;
out vec3		v_cameraPosition;
flat out int	v_useTextures; // = - ) 


void main()
 {
	gl_Position = u_mvpMatrix * u_vertex;
	v_screenPosition = gl_Position.xyz;
	v_worldPosition = u_vertex;
	v_surfaceColor = u_color;
	v_diffuseTexture = u_diffuseTextureCoords.xy;
	
}
