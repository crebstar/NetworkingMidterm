#version 330
// Fragment Shader, GLSL 3.30

uniform sampler2D u_diffuseTexture;

in vec3			v_screenPosition;
in vec4			v_worldPosition; 
in vec4			v_surfaceColor;
in vec2			v_diffuseTexture;
flat in int		v_useTextures;

out vec4 fragColor;
//---------------------------------------------------------------------------------//
// For this example... remember the screen coordinates are NDC coordinates
void main()
{
	if ( v_useTextures == 1 ) {
		vec4 diffuseTexel = texture2D( u_diffuseTexture, v_diffuseTexture );
		fragColor = ( diffuseTexel * v_surfaceColor );
	} else {
		fragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
	}
	

}

