
#ifndef included_DebugShape
#define included_DebugShape
#pragma once

#include <vector>

#include "EngineCommon.hpp"
#include "DebugShapeDefinition.hpp"
#include "Vector3D.hpp"
#include "Vector4.hpp"
#include "MatrixStack.hpp"

/*
	Base abstract class for all debug shapes in cbengine 
*/

class DebugShape {

public:

	DebugShape() { 
		m_flaggedForDestruction = false;
		m_debugDefinition = nullptr;
		m_durationExisted = 0;
		m_framesExisted = 0;
	}
	virtual ~DebugShape() {}

	virtual void update( float deltaSeconds )		= 0;
	virtual void render( float deltaSeconds ) const = 0;
	

	// Inline mutators
	virtual void setDebugShapeColor( const cbengine::Vector4<float> & colorToSet );
	virtual void addVertexToDebugShape( const cbengine::Vector3<float> & vertexToAdd );
	virtual void destroy();

	DebugShapeDefinition *							m_debugDefinition;
	cbengine::Vector3<float>						m_position;
	bool											m_flaggedForDestruction;

protected:

	
	
	std::vector<cbengine::Vector3<float>>			m_verts;
	cbengine::Vector4<float>						m_color;
	float											m_durationExisted;
	int												m_framesExisted;


private:
	

};

// Inline mutators
inline void DebugShape::setDebugShapeColor( const cbengine::Vector4<float> & colorToSet ) {

	m_color.x = colorToSet.x;
	m_color.y = colorToSet.y;
	m_color.z = colorToSet.z;
	m_color.w = colorToSet.w;

} // end setDebugShapeColor

inline void DebugShape::addVertexToDebugShape( const cbengine::Vector3<float> & vertexToAdd ) {
	m_verts.push_back( vertexToAdd );
}

inline void DebugShape::destroy() {
	m_flaggedForDestruction = true;
}

#endif