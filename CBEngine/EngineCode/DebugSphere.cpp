
#include "DebugSphere.hpp"
#include "geometry.h"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

DebugSphere::~DebugSphere() {

}

DebugSphere::DebugSphere() {

}

void DebugSphere::update( float deltaSeconds ) {

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

void DebugSphere::render( float deltaSeconds ) const {
	UNUSED(deltaSeconds);
	// TODO: Remove the vertical pointing dependency
	glPushMatrix();

	m_debugDefinition->applyRenderingSettingsForDebugShape();

	glTranslatef( m_position.x, m_position.y, m_position.z );
	
	glLineWidth( SPHERE_LINE_WIDTH );
	glColor4f( edgeColor.x, edgeColor.y, edgeColor.z, edgeColor.w );
	renderWireSphere( radius, NUMBER_SLICES, NUMBER_STACKS );


	glColor4f( sideColor.x, sideColor.y, sideColor.z, sideColor.w );
	renderSolidSphere( radius, NUMBER_SLICES, NUMBER_STACKS );

	m_debugDefinition->restoreRenderingSettings();

	glPopMatrix();

}