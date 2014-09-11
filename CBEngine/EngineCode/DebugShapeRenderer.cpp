
#include "DebugShapeRenderer.hpp"
#include "DebugPoint.hpp"
#include "DebugLine.hpp"
#include "DebugArrow.hpp"
#include "DebugAABB.hpp"
#include "DebugSphere.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

DebugShapeRenderer::~DebugShapeRenderer() {

	std::set< DebugShape * >::iterator it;
	for ( it = m_debugShapes.begin(); it != m_debugShapes.end(); ++it ) {
		delete *(it);
	}

} // end destructor 


void DebugShapeRenderer::update( float deltaSeconds ) {

	std::set< DebugShape * >::iterator it;
	std::vector< DebugShape* > shapesToRemove;
	for ( it = m_debugShapes.begin(); it != m_debugShapes.end(); ++it ) {

		DebugShape * debugShape = *(it);
		debugShape->update( deltaSeconds );

		if ( debugShape->m_flaggedForDestruction ) {
			shapesToRemove.push_back( debugShape );
		}

	}

	for ( size_t i = 0; i < shapesToRemove.size(); ++i ) {
		m_debugShapes.erase( shapesToRemove[i] );
		delete shapesToRemove[i];
	}
	
} // end update

void DebugShapeRenderer::render( float deltaSeconds ) const {

	std::set< DebugShape * >::iterator it;
	for ( it = m_debugShapes.begin(); it != m_debugShapes.end(); ++it ) {
		DebugShape * debugShape = *(it);
		debugShape->render( deltaSeconds );
	}

}


void DebugShapeRenderer::createDebugPointWithDefinition( DebugShapeDefinition * debugDefinition, 
	const std::string & definitionKey, 
	const cbengine::Vector3<float> & pointPosition,
	const cbengine::Vector4<float> & colorOfPointLines, 
	float pointLineLength ) {

	assert( debugDefinition != nullptr );

	m_debugShapeDefinitions.insert( std::pair< std::string, DebugShapeDefinition * >( definitionKey, debugDefinition ));
	DebugPoint * debugPoint = new DebugPoint;

	debugPoint->setDebugShapeColor( colorOfPointLines );
	debugPoint->m_pointLineLength = pointLineLength;
	debugPoint->m_debugDefinition = debugDefinition;
	debugPoint->m_position.x = pointPosition.x;
	debugPoint->m_position.y = pointPosition.y;
	debugPoint->m_position.z = pointPosition.z;
	m_debugShapes.insert( debugPoint );

}

void DebugShapeRenderer::createDebugLineWithDefinition( DebugShapeDefinition * debugDefinition, 
	const std::string & definitionKey, 
	const cbengine::Vector3<float> & startVertex,
	const cbengine::Vector3<float> & endVertex, 
	const cbengine::Vector4<float> & startVertexColor, 
	const cbengine::Vector4<float> & endVertexColor ) {

	assert( debugDefinition != nullptr );

	m_debugShapeDefinitions.insert( std::pair< std::string, DebugShapeDefinition * >( definitionKey, debugDefinition ));
	DebugLine * debugLine = new DebugLine;
	debugLine->m_debugDefinition = debugDefinition;

	debugLine->startVertex.x = startVertex.x;
	debugLine->startVertex.y = startVertex.y;
	debugLine->startVertex.z = startVertex.z;

	debugLine->endVertex.x = endVertex.x;
	debugLine->endVertex.y = endVertex.y;
	debugLine->endVertex.z = endVertex.z;

	debugLine->startingVertexColor.x = startVertexColor.x;
	debugLine->startingVertexColor.y = startVertexColor.y;
	debugLine->startingVertexColor.z = startVertexColor.z;
	debugLine->startingVertexColor.w = startVertexColor.w;

	debugLine->endingVertexColor.x = endVertexColor.x;
	debugLine->endingVertexColor.y = endVertexColor.y;
	debugLine->endingVertexColor.z = endVertexColor.z;
	debugLine->endingVertexColor.w = endVertexColor.w;

	m_debugShapes.insert( debugLine );

}

void DebugShapeRenderer::createDebugArrowWithDefinition( DebugShapeDefinition * debugDefinition, 
	const std::string & definitionKey, 
	const cbengine::Vector3<float> & startVertex,
	const cbengine::Vector3<float> & endVertex, 
	const cbengine::Vector4<float> & arrowColor,
	float arrowLength ) {

	assert( debugDefinition != nullptr );
	m_debugShapeDefinitions.insert( std::pair< std::string, DebugShapeDefinition * >( definitionKey, debugDefinition ));

	DebugArrow * debugArrow = new DebugArrow;
	debugArrow->m_debugDefinition = debugDefinition;
	debugArrow->lengthOfArrow = arrowLength;

	debugArrow->startPosition.x = startVertex.x;
	debugArrow->startPosition.y = startVertex.y;
	debugArrow->startPosition.z = startVertex.z;
	debugArrow->endPosition.x = endVertex.x;
	debugArrow->endPosition.y = endVertex.y;
	debugArrow->endPosition.z = endVertex.z;

	debugArrow->setDebugShapeColor( arrowColor );

	m_debugShapes.insert( debugArrow );

}

void DebugShapeRenderer::createDebugAABBWithDefinition( DebugShapeDefinition * debugDefinition, 
	const std::string & definitionKey, 
	const cbengine::Vector3<float> & minValue, 
	const cbengine::Vector3<float> & maxValue, 
	const cbengine::Vector4<float> & edgeColor,
	const cbengine::Vector4<float> & sideColor ) {

	assert( debugDefinition != nullptr );
	m_debugShapeDefinitions.insert( std::pair< std::string, DebugShapeDefinition * >( definitionKey, debugDefinition ));

	DebugAABB * debugAABB = new DebugAABB;
	debugAABB->m_debugDefinition = debugDefinition;

	debugAABB->minPosition.x = minValue.x;
	debugAABB->minPosition.y = minValue.y;
	debugAABB->minPosition.z = minValue.z;

	debugAABB->maxPosition.x = maxValue.x;
	debugAABB->maxPosition.y = maxValue.y;
	debugAABB->maxPosition.z = maxValue.z;

	debugAABB->edgeColor = edgeColor;
	debugAABB->sideColor = sideColor;

	m_debugShapes.insert( debugAABB );

}

void DebugShapeRenderer::createDebugSphereWithDefinition( DebugShapeDefinition * debugDefinition, 
	const std::string & definitionKey, 
	const cbengine::Vector3<float> & sphereCenterPoint, 
	const cbengine::Vector4<float> & edgeColor, 
	const cbengine::Vector4<float> & sideColor, 
	float radius ) {

	assert( debugDefinition != nullptr );
	m_debugShapeDefinitions.insert( std::pair< std::string, DebugShapeDefinition * >( definitionKey, debugDefinition ));

	DebugSphere * debugSphere = new DebugSphere;
	debugSphere->m_debugDefinition = debugDefinition;

	debugSphere->m_position = sphereCenterPoint;
	debugSphere->edgeColor = edgeColor;
	debugSphere->sideColor = sideColor;
	debugSphere->radius = radius;

	m_debugShapes.insert( debugSphere );

}