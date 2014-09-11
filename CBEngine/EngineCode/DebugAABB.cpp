#include "DebugAABB.hpp"

#include <math.h>


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

DebugAABB::~DebugAABB() {

}

DebugAABB::DebugAABB() {

}

void DebugAABB::update( float deltaSeconds ) {

	assert( m_debugDefinition != nullptr );

	// TODO: Update Position or follow another entity
	if ( m_debugDefinition->m_lifeSpanDuration != INFINITE_DURATION ) {
		if ( m_durationExisted > m_debugDefinition->m_lifeSpanDuration ) {
			m_flaggedForDestruction = true;
		}
	}

	if ( m_debugDefinition->m_frameDurationCount != INFINITE_FRAMES ) {
		if ( m_framesExisted > m_debugDefinition->m_frameDurationCount ) {
			m_flaggedForDestruction = true;
		}
	}

	m_durationExisted += deltaSeconds;
	m_framesExisted += 1;

}

void DebugAABB::render( float deltaSeconds ) const {
	UNUSED(deltaSeconds);
	const float length = fabs( maxPosition.x - minPosition.x );
	const float width = fabs( maxPosition.y - minPosition.y );
	const float height = fabs( maxPosition.z - minPosition.z );


	if ( m_debugDefinition->m_depthBuffetSetting == DEPTH_BUFFER_ON_FOR_SKELETAL_IMAGE ) {
		renderDualModeImage();
	}

	// TODO: Remove the vertical pointing dependency
	glPushMatrix();

	m_debugDefinition->applyRenderingSettingsForDebugShape();

	glTranslatef( minPosition.x, minPosition.y, minPosition.z );

	glLineWidth( AABB_EDGE_LINE_WIDTH );
	glColor4f( edgeColor.x, edgeColor.y, edgeColor.z, edgeColor.w );

	glBegin( GL_LINE_LOOP ); {

		// South Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( 0.0f, -width, height );
		glVertex3f( 0.0f, 0.0f, height );


	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// East Side
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, -width, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// North Side
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );
		glVertex3f( length, 0.0f, height );
		glVertex3f( length, -width, height );


	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// Bottom Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );
		

	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// Top Side
		glVertex3f( length, -width, height );
		glVertex3f( length, 0.0f, height );
		glVertex3f( 0.0f, 0.0f, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();
	
	glColor4f( sideColor.x, sideColor.y, sideColor.z, sideColor.w );
	glBegin( GL_QUADS ); {

		// South Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( 0.0f, -width, height );
		glVertex3f( 0.0f, 0.0f, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// East Side
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, -width, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// North Side
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );
		glVertex3f( length, 0.0f, height );
		glVertex3f( length, -width, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// West Side
		glVertex3f( length, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, height );
		glVertex3f( length, 0.0f, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// Bottom Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );


	} glEnd();

	glBegin( GL_QUADS ); {

		// Top Side
		glVertex3f( length, -width, height );
		glVertex3f( length, 0.0f, height );
		glVertex3f( 0.0f, 0.0f, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();

	m_debugDefinition->restoreRenderingSettings();

	glPopMatrix();

}

void DebugAABB::renderDualModeImage() const {

	const float length = fabs( maxPosition.x - minPosition.x );
	const float width = fabs( maxPosition.y - minPosition.y );
	const float height = fabs( maxPosition.z - minPosition.z );

	// TODO: Remove the vertical pointing dependency
	glPushMatrix();

	glDisable( GL_DEPTH_TEST );
	glDepthMask( false );

	glTranslatef( minPosition.x, minPosition.y, minPosition.z );

	glLineWidth( AABB_EDGE_LINE_WIDTH );
	glColor4f( edgeColor.x * COLOR_DIM_COEFICIENT_FOR_DUAL_MODE, 
		edgeColor.y * COLOR_DIM_COEFICIENT_FOR_DUAL_MODE, 
		edgeColor.z * COLOR_DIM_COEFICIENT_FOR_DUAL_MODE, 
		edgeColor.w );

	glBegin( GL_LINE_LOOP ); {

		// South Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( 0.0f, -width, height );
		glVertex3f( 0.0f, 0.0f, height );


	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// East Side
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, -width, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// North Side
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );
		glVertex3f( length, 0.0f, height );
		glVertex3f( length, -width, height );


	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// Bottom Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );


	} glEnd();

	glBegin( GL_LINE_LOOP ); {

		// Top Side
		glVertex3f( length, -width, height );
		glVertex3f( length, 0.0f, height );
		glVertex3f( 0.0f, 0.0f, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();

	glColor4f( sideColor.x * COLOR_DIM_COEFICIENT_FOR_DUAL_MODE,
		sideColor.y * COLOR_DIM_COEFICIENT_FOR_DUAL_MODE,
		sideColor.z * COLOR_DIM_COEFICIENT_FOR_DUAL_MODE, 
		sideColor.w );
	glBegin( GL_QUADS ); {

		// South Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( 0.0f, -width, height );
		glVertex3f( 0.0f, 0.0f, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// East Side
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, -width, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// North Side
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );
		glVertex3f( length, 0.0f, height );
		glVertex3f( length, -width, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// West Side
		glVertex3f( length, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, height );
		glVertex3f( length, 0.0f, height );


	} glEnd();

	glBegin( GL_QUADS ); {

		// Bottom Side
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, -width, 0.0f );
		glVertex3f( length, -width, 0.0f );
		glVertex3f( length, 0.0f, 0.0f );


	} glEnd();

	glBegin( GL_QUADS ); {

		// Top Side
		glVertex3f( length, -width, height );
		glVertex3f( length, 0.0f, height );
		glVertex3f( 0.0f, 0.0f, height );
		glVertex3f( 0.0f, -width, height );


	} glEnd();

	m_debugDefinition->restoreRenderingSettings();

	glPopMatrix();


}