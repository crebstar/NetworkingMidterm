
#ifndef included_DebugShapeDefinition
#define included_DebugShapeDefinition
#pragma once

#include "EngineMacros.hpp"
#include "OpenGLRenderer.hpp"

typedef enum {

	DEBUG_SHAPE_UNKNOWN,
	DEBUG_SHAPE_POINT,
	DEBUG_SHAPE_LINE,
	DEBUG_SHAPE_ARROW,
	DEBUG_SHAPE_WIRE_AABB,
	DEBUG_SHAPE_SOLID_AABB,
	DEBUG_SHAPE_WIRE_SPHERE,
	DEBUG_SHAPE_SOLID_SPHERE,

} DebugShapeType;

typedef enum {

	DEPTH_BUFFER_UNKNOWN,
	DEPTH_BUFFER_OFF,
	DEPTH_BUFFER_ON,
	DEPTH_BUFFER_ON_FOR_SKELETAL_IMAGE,

} DebugShapeDepthBufferSetting;

// Note these are both the defaults if one is not specified for the definition
const float INFINITE_DURATION						= -1.0f;
const int	INFINITE_FRAMES							= -1;
const float NO_LINE_WIDTH							= -1.0f;
const float COLOR_DIM_COEFICIENT_FOR_DUAL_MODE		= 0.85f;

class DebugShapeDefinition {

public:

	~DebugShapeDefinition();
	DebugShapeDefinition();

	void applyRenderingSettingsForDebugShape();
	void restoreRenderingSettings();

	DebugShapeType						m_debugShapeType;
	DebugShapeDepthBufferSetting		m_depthBuffetSetting;
	int									m_frameDurationCount;
	float								m_lifeSpanDuration;
	float								m_lineWidth;

protected:


private:
	


};

#endif