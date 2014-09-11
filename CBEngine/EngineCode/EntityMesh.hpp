#ifndef included_EntityMesh
#define included_EntityMesh
#pragma once

#include <cassert>
#include <map>
#include <unordered_map>

#include "EngineCommon.hpp"

#include "Matrix44.hpp"
#include "Light.hpp"

class CBNode;

class EntityMesh {
public:
	~EntityMesh();
	EntityMesh( CBNode* rootNode );

	void update( float deltaSeconds );
	void render( float deltaSeconds ) const;

	// Inline Convenience Functions
	void addBoneNodeToBoneHashMap( int boneNodeIndex, CBNode* boneNode );
	void buildBoneTransformMatrixVector();
	const CBNode* getBoneNodeWithIndex( int boneIndex ) const;

	// Inline Mutators
	const CBNode* getRootNode() const;
	bool isBoneTransformMatrixVectorDirty() const;

	// For now
	std::vector<Light>					m_lightsEffectingMesh;

protected:

	void setEntityMeshDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( EntityMesh );

	CBNode*								m_rootEntityMeshNode;
	std::unordered_map<int,CBNode*>		m_boneNodes;

	bool								m_isSkeletalMesh;
};


inline void EntityMesh::addBoneNodeToBoneHashMap( int boneNodeIndex, CBNode* boneNode ) {

	assert( boneNode != nullptr );
	m_boneNodes.insert( std::pair<int,CBNode*>( boneNodeIndex, boneNode ) );

}


inline const CBNode* EntityMesh::getRootNode() const {

	return m_rootEntityMeshNode;
}


inline const CBNode* EntityMesh::getBoneNodeWithIndex( int boneIndex ) const {

	CBNode* boneNodeToReturn = nullptr;
	std::unordered_map<int,CBNode*>::const_iterator itBone = m_boneNodes.find( boneIndex );

	if ( itBone != m_boneNodes.end() ) {

		boneNodeToReturn = itBone->second;
	}

	return boneNodeToReturn;
}



#endif