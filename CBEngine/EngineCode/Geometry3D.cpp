
#include "Geometry3D.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

void cbengine::drawAxis( float lengthOfAxis, float lineWidth ) {

	glLineWidth( lineWidth );
	glDisable( GL_TEXTURE_2D );

	glBegin( GL_LINES ); {

		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( lengthOfAxis, 0.0f, 0.0f );

		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, lengthOfAxis, 0.0f );

		glColor3f( 0.0f, 0.0f, 1.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, lengthOfAxis );

	} glEnd();

} // end drawAxis

void cbengine::drawVoxelCube( const cbengine::Vector3<float> & cubeSideSizes ) {

	// 6 Sides Total
	// 8 Corners
	// 24 Verts

	// Starts drawing at the bottom left corner of the cube

	glBegin( GL_QUADS ); {

		glColor3f( 1.0f, 1.0f, 1.0f );
		// Front Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( cubeSideSizes.x, 0.0f, 0.0f );
		glVertex3f( cubeSideSizes.x, cubeSideSizes.y, 0.0f );
		glVertex3f( 0.0f, cubeSideSizes.y, 0.0f );
		
		glColor3f( 1.0f, 0.0f, 0.0f );
		// Right Side 
		glVertex3f( cubeSideSizes.x, 0.0f, 0.0f );
		glVertex3f( cubeSideSizes.x, 0.0f, cubeSideSizes.z );
		glVertex3f( cubeSideSizes.x, cubeSideSizes.y, cubeSideSizes.z );
		glVertex3f( cubeSideSizes.x, cubeSideSizes.y, 0.0f );
		
		glColor3f( 1.0f, 0.0f, 1.0f );
		// Back Side
		glVertex3f(  cubeSideSizes.x, 0.0f, cubeSideSizes.z );
		glVertex3f( 0.0f, 0.0f, cubeSideSizes.z );
		glVertex3f( 0.0f, cubeSideSizes.y, cubeSideSizes.z );
		glVertex3f( cubeSideSizes.x, cubeSideSizes.y, cubeSideSizes.z );

		// Left Side
		glColor3f( 0.0f, 0.0f, 1.0f );
		glVertex3f( 0.0f, 0.0f, cubeSideSizes.z );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, cubeSideSizes.y, 0.0f );
		glVertex3f( 0.0f, cubeSideSizes.y, cubeSideSizes.z );

		// Bottom Side
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, cubeSideSizes.z );
		glVertex3f( cubeSideSizes.x, 0.0f, cubeSideSizes.z );
		glVertex3f( cubeSideSizes.x, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );

		// Top Side
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( 0.0f, cubeSideSizes.y, 0.0f );
		glVertex3f( cubeSideSizes.x, cubeSideSizes.y, 0.0f );
		glVertex3f( cubeSideSizes.x, cubeSideSizes.y, cubeSideSizes.z );
		glVertex3f( 0.0f, cubeSideSizes.y, cubeSideSizes.z );


	} glEnd();

}

