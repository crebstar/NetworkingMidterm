
#ifndef included_DebugShapeRenderer
#define included_DebugShapeRenderer
#pragma once

#include <set>
#include <map>
#include <string>

#include "EngineMacros.hpp"
#include "DebugShape.hpp"
#include "DebugShapeDefinition.hpp"
#include "Vector4.hpp"

/*
	Singleton and Abstract Factory class for creating and managing all debug shapes
*/

class DebugShapeRenderer {

public:

	static DebugShapeRenderer & sharedDebugRenderer() {

		static DebugShapeRenderer debugShapeRenderer;
		return debugShapeRenderer;

	}

	~DebugShapeRenderer();

	void update( float deltaSeconds );
	void render( float deltaSeconds ) const;

	
	void createDebugPointWithDefinition( DebugShapeDefinition * debugDefinition, 
		const std::string & definitionKey, 
		const cbengine::Vector3<float> & pointPosition,
		const cbengine::Vector4<float> & colorOfPointLines, 
		float pointLineLength );

	void createDebugLineWithDefinition( DebugShapeDefinition * debugDefinition, 
		const std::string & definitionKey, 
		const cbengine::Vector3<float> & startVertex,
		const cbengine::Vector3<float> & endVertex, 
		const cbengine::Vector4<float> & startVertexColor, 
		const cbengine::Vector4<float> & endVertexColor );

	void createDebugArrowWithDefinition( DebugShapeDefinition * debugDefinition, 
		const std::string & definitionKey, 
		const cbengine::Vector3<float> & startVertex,
		const cbengine::Vector3<float> & endVertex, 
		const cbengine::Vector4<float> & arrowColor,
		float arrowLength );

	void createDebugAABBWithDefinition( DebugShapeDefinition * debugDefinition, 
		const std::string & definitionKey, 
		const cbengine::Vector3<float> & minValue, 
		const cbengine::Vector3<float> & maxValue, 
		const cbengine::Vector4<float> & edgeColor,
		const cbengine::Vector4<float> & sideColor );

	void createDebugSphereWithDefinition( DebugShapeDefinition * debugDefinition, 
		const std::string & definitionKey, 
		const cbengine::Vector3<float> & sphereCenterPoint, 
		const cbengine::Vector4<float> & edgeColor, 
		const cbengine::Vector4<float> & sideColor, 
		float radius );

protected:


private:
	PREVENT_COPY_AND_ASSIGN( DebugShapeRenderer );

	DebugShapeRenderer() {

	}

	std::map< std::string, DebugShapeDefinition * >		m_debugShapeDefinitions;
	std::set< DebugShape * >							m_debugShapes;

};

#endif