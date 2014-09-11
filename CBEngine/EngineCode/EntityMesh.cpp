#include "EntityMesh.hpp"

#include "CBNode.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

EntityMesh::~EntityMesh() {

	if ( m_rootEntityMeshNode != nullptr ) {

		// TODO::
	}
}


EntityMesh::EntityMesh( CBNode* rootNode ) {

	setEntityMeshDefaults();

	assert( rootNode != nullptr );
	m_rootEntityMeshNode = rootNode;
}


void EntityMesh::update( float deltaSeconds ) {

	m_rootEntityMeshNode->update( deltaSeconds );
	
}


void EntityMesh::render( float deltaSeconds ) const {

	m_rootEntityMeshNode->render( deltaSeconds );
}


void EntityMesh::setEntityMeshDefaults() {

	m_rootEntityMeshNode = nullptr;
	m_isSkeletalMesh = false;
}