
#include "DebugPoint.hpp"

#include <cassert>


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

DebugPoint::~DebugPoint() {



}

DebugPoint::DebugPoint() {

	m_pointLineLength = 3.0f;

}


void DebugPoint::update( float deltaSeconds ) {

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

void DebugPoint::render( float deltaSeconds ) const {
	UNUSED(deltaSeconds);
	glUseProgram(0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();
	matrixStack->applyTranslationAndPushToStack( m_position );
	const Matrix44<float> & topOfStack = matrixStack->getMatrixFromTopOfStack();

	glLoadMatrixf( topOfStack.matrixData );

	m_debugDefinition->applyRenderingSettingsForDebugShape();
	glColor4f( m_color.x, m_color.y, m_color.z, m_color.w );

	glBegin( GL_LINES ); {

		glVertex3f( m_pointLineLength, m_pointLineLength, m_pointLineLength );
		glVertex3f( -m_pointLineLength, -m_pointLineLength, -m_pointLineLength );

		glVertex3f( m_pointLineLength, -m_pointLineLength, m_pointLineLength );
		glVertex3f( -m_pointLineLength, m_pointLineLength, -m_pointLineLength );

		glVertex3f( -m_pointLineLength, -m_pointLineLength, m_pointLineLength );
		glVertex3f( m_pointLineLength, m_pointLineLength, -m_pointLineLength );

		glVertex3f( -m_pointLineLength, m_pointLineLength, m_pointLineLength );
		glVertex3f( m_pointLineLength, -m_pointLineLength, -m_pointLineLength );

		glVertex3f( 0.0f, 0.0f, m_pointLineLength );
		glVertex3f( 0.0f, 0.0f, -m_pointLineLength );

		glVertex3f( m_pointLineLength, 0.0f, 0.0f );
		glVertex3f( -m_pointLineLength, 0.0f, 0.0f );

		glVertex3f( 0.0f, m_pointLineLength, 0.0f );
		glVertex3f( 0.0f, -m_pointLineLength, 0.0f );

	} glEnd();

	m_debugDefinition->restoreRenderingSettings();

	matrixStack->popFromTopOfStack();

} // end render

