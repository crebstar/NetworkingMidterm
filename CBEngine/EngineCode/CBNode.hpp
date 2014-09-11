#ifndef included_CBNode
#define included_CBNode
#pragma once

#include <vector>
#include <string>
#include <cassert>

#include "EngineCommon.hpp"

#include "Matrix44.hpp"

const int BONE_INDEX_NONE = -1;
const int PARENT_INDEX_NONE = -1;

class EntityMesh;

class CBNode {
public:
	virtual ~CBNode();
	explicit CBNode();
	explicit CBNode( EntityMesh* entityMesh );

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	virtual void addChildToNode( CBNode* childToAdd );
	virtual void addAnimationTransform( const Matrix44<float>& animTransform );
	virtual const Matrix44<float>& getAnimationAtTime( float animationDuration ) const;

	// Inline Mutators
	virtual void setInitialLocalToWorldTransform( const Matrix44<float>& localToWorldTransform	);
	virtual void setInitialWorldToLocalTransform( const Matrix44<float>& worldToLocalTransform );
	virtual const Matrix44<float>& getInitialLocalToWorldTransform() const;
	virtual const Matrix44<float>& getInitialWorldToLocalTransform() const;

	virtual void setEntityMesh( EntityMesh* entityMesh );
	virtual const EntityMesh* getEntityMesh() const;

	virtual void setParentNode( CBNode* parentNode );
	virtual void setNodeIndex( int index );
	virtual int getNodeIndex() const;
	virtual void setBoneIndex( int boneIndex );
	virtual int getBoneIndex() const;
	virtual void setParentIndex( int parentIndex );
	virtual int getParentIndex() const;

	const std::string& getNodeName();
	void setNodeName( const std::string& nodeName );

protected:

	void setNodeDefaults();

	std::string						m_nodeName;
	EntityMesh*						m_entityMesh;
	CBNode*							m_parentNode;
	std::vector<CBNode*>			m_childrenNodes;
	int								m_parentIndex;
	int								m_nodeIndex;
	int								m_boneIndex;

	Matrix44<float>					m_initialLocalToWorldTransform;
	Matrix44<float>					m_initialWorldToLocalTransform;
	std::vector<Matrix44<float>>	m_animationTransforms;
	float							m_animationDuration;

	bool							m_isVisible;
private:
	PREVENT_COPY_AND_ASSIGN( CBNode );

};

// Inline Mutators
inline void CBNode::addChildToNode( CBNode* childToAdd ) {

	assert( childToAdd != nullptr );
	m_childrenNodes.push_back( childToAdd );
}


inline void CBNode::addAnimationTransform( const Matrix44<float>& animTransform ) {

	m_animationTransforms.push_back( animTransform );
}


inline void CBNode::setInitialLocalToWorldTransform( const Matrix44<float>& localToWorldTransform ) {

	m_initialLocalToWorldTransform = localToWorldTransform;
}


inline void CBNode::setInitialWorldToLocalTransform( const Matrix44<float>& worldToLocalTransform ) {

	m_initialWorldToLocalTransform = worldToLocalTransform;
}

inline const Matrix44<float>& CBNode::getInitialLocalToWorldTransform() const {

	return m_initialLocalToWorldTransform;
}


inline const Matrix44<float>& CBNode::getInitialWorldToLocalTransform() const {

	return m_initialWorldToLocalTransform;
}


inline void CBNode::setParentNode( CBNode* parentNode ) {

	m_parentNode = parentNode;
}


inline void CBNode::setNodeIndex( int index ) {
	m_nodeIndex = index;
}


inline int CBNode::getNodeIndex() const {
	return m_nodeIndex;
}


inline void CBNode::setBoneIndex( int boneIndex ) {

	m_boneIndex = boneIndex;
}



inline int CBNode::getBoneIndex() const {

	return m_boneIndex;
}


inline void CBNode::setParentIndex( int parentIndex ) {

	m_parentIndex = parentIndex;
}


inline int CBNode::getParentIndex() const {

	return m_parentIndex;
}


inline const std::string& CBNode::getNodeName() {

	return m_nodeName;
}


inline void CBNode::setNodeName( const std::string& nodeName ) {

	if ( nodeName == "" ) {
		m_nodeName = "Unknown";
	} else {
		m_nodeName = nodeName;
	}
}


#endif