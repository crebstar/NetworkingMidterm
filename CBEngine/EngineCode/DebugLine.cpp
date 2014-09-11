
#include "DebugLine.hpp"

#include <cassert>


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

DebugLine::~DebugLine() {

}

DebugLine::DebugLine() {

	startingVertexColor.x = 1.0f;
	startingVertexColor.y = 1.0f;
	startingVertexColor.z = 1.0f;
	startingVertexColor.w = 1.0f;

	endingVertexColor.x = 1.0f;
	endingVertexColor.y = 1.0f;
	endingVertexColor.z = 1.0f;
	endingVertexColor.w = 1.0f;

}

void DebugLine::update( float deltaSeconds ) {

	assert( m_debugDefinition != nullptr );

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

void DebugLine::render( float deltaSeconds ) const {
	UNUSED(deltaSeconds);
	glPushMatrix();

	m_debugDefinition->applyRenderingSettingsForDebugShape();

	glTranslatef( startVertex.x, startVertex.y, startVertex.z );

	glBegin( GL_LINES ); {

		glColor4f( startingVertexColor.x, startingVertexColor.y, startingVertexColor.z, startingVertexColor.w );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glColor4f( endingVertexColor.x, endingVertexColor.y, endingVertexColor.z, endingVertexColor.w );
		glVertex3f( endVertex.x, endVertex.y, endVertex.z );

	} glEnd();

	m_debugDefinition->restoreRenderingSettings();

	glPopMatrix();

}