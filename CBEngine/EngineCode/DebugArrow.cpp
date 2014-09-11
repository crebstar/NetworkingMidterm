
#include "DebugArrow.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

DebugArrow::~DebugArrow() {

}

DebugArrow::DebugArrow() {

}

void DebugArrow::update( float deltaSeconds ) {

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

void DebugArrow::render( float deltaSeconds ) const {
	// TODO: Remove the vertical pointing dependency
	UNUSED(deltaSeconds);
	glPushMatrix();

	m_debugDefinition->applyRenderingSettingsForDebugShape();

	glTranslatef( startPosition.x, startPosition.y, startPosition.z );
	glColor4f( 0.70f, 0.90f, 0.20f, 1.0f );

	glBegin( GL_LINES ); {

		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, ( endPosition.z - startPosition.z ) );

	} glEnd();

	const float halfLengthOfArrow = lengthOfArrow * 0.50f;
	glBegin( GL_TRIANGLES ); {

		// South
		glVertex3f( -halfLengthOfArrow, halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );
		glVertex3f( 0.0f, 0.0f, ( endPosition.z - startPosition.z ) );
		glVertex3f( -halfLengthOfArrow, -halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );
		// East
		glVertex3f( -halfLengthOfArrow, -halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );
		glVertex3f( 0.0f, 0.0f, ( endPosition.z - startPosition.z ));
		glVertex3f( halfLengthOfArrow, -halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );
		// North
		glVertex3f( halfLengthOfArrow, -halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );
		glVertex3f( 0.0f, 0.0f, ( endPosition.z - startPosition.z ) );
		glVertex3f( halfLengthOfArrow, halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );
		// West
		glVertex3f( halfLengthOfArrow, halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );
		glVertex3f( 0.0f, 0.0f, ( endPosition.z - startPosition.z ) );
		glVertex3f( -halfLengthOfArrow, halfLengthOfArrow, ( endPosition.z - startPosition.z ) + lengthOfArrow );

	} glEnd();

	m_debugDefinition->restoreRenderingSettings();

	glPopMatrix();

}
