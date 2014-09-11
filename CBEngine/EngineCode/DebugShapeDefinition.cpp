
#include "DebugShapeDefinition.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

DebugShapeDefinition::~DebugShapeDefinition() {



} // end destructor


DebugShapeDefinition::DebugShapeDefinition() {

	m_debugShapeType		= DEBUG_SHAPE_UNKNOWN;
	m_depthBuffetSetting	= DEPTH_BUFFER_UNKNOWN;
	m_frameDurationCount	= INFINITE_FRAMES;
	m_lifeSpanDuration		= INFINITE_DURATION;
	m_lineWidth				= NO_LINE_WIDTH;

} // end ctor


void DebugShapeDefinition::applyRenderingSettingsForDebugShape() {

	glActiveTexture( GL_TEXTURE0 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE1 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE2 );
	glDisable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE3 );
	glDisable( GL_TEXTURE_2D );
	//glActiveTexture( GL_TEXTURE4 );
	//glDisable( GL_TEXTURE_2D );

	if ( m_lineWidth != NO_LINE_WIDTH ) {
		glLineWidth( m_lineWidth );
	}

	switch ( m_depthBuffetSetting ) {
	case DEPTH_BUFFER_OFF:

		glDisable( GL_DEPTH_TEST );
		glDepthMask( false );

		break;
	case DEPTH_BUFFER_ON:

		glEnable( GL_DEPTH_TEST );
		glDepthMask( true );

		break;
	case DEPTH_BUFFER_ON_FOR_SKELETAL_IMAGE:
		// TODO :: Add custom code for this feature
		break;
	case DEPTH_BUFFER_UNKNOWN:

		glEnable( GL_DEPTH_TEST );
		glDepthMask( true );

		break;

	} // end switch

}

void DebugShapeDefinition::restoreRenderingSettings() {

	glEnable( GL_DEPTH_TEST );
	glDepthMask( true );
	glEnable( GL_TEXTURE_2D );

}