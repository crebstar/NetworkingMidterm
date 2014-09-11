#include "CBNode.hpp"

#include "MatrixStack.hpp"

#include "EntityMesh.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CBNode::~CBNode() {

}


CBNode::CBNode() {

	setNodeDefaults();
}


CBNode::CBNode( EntityMesh* entityMesh ) {

	setNodeDefaults();
	setEntityMesh( entityMesh );
}


void CBNode::update( float deltaSeconds ) {
	
	m_animationDuration = m_animationDuration + deltaSeconds;

	for ( size_t i = 0; i < m_childrenNodes.size(); ++i ) {

		CBNode* childNode = m_childrenNodes[i];
		childNode->update( deltaSeconds );
	}
}


void CBNode::render( float deltaSeconds ) const {

	UNUSED(deltaSeconds);

	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();

	if ( !m_animationTransforms.empty() ) {

		int numberOfAnimationsTransforms = m_animationTransforms.size();
		int currentAnimation = static_cast<int>( m_animationDuration * 30 ) % numberOfAnimationsTransforms;
		const Matrix44<float>& animMatrix = m_animationTransforms[currentAnimation];
		matrixStack->applyTransformAndPushToStack( animMatrix );
		matrixStack->setCurrentModelTransformMatrix( animMatrix ); // TODO:: double check that I need this
	} else {

		matrixStack->applyTransformAndPushToStack( m_initialLocalToWorldTransform );
		matrixStack->setCurrentModelTransformMatrix( m_initialWorldToLocalTransform );
	}

	matrixStack->popFromTopOfStack();

	for ( size_t i = 0; i < m_childrenNodes.size(); ++i ) {

		CBNode* childNode = m_childrenNodes[i];
		childNode->render( deltaSeconds );
	}
}

// TODO:: Incorporate frames per second variable
const Matrix44<float>& CBNode::getAnimationAtTime( float animationDuration ) const {

	UNUSED( animationDuration );
	int numberOfAnimationsTransforms = m_animationTransforms.size();

	if ( numberOfAnimationsTransforms > 0 ) {

		int currentAnimation = static_cast<int>( m_animationDuration * 30 ) % numberOfAnimationsTransforms;
		const Matrix44<float>& animMatrix = m_animationTransforms[currentAnimation];
		return animMatrix;

	} else {

		return m_initialWorldToLocalTransform;
	}
}


void CBNode::setEntityMesh( EntityMesh* entityMesh ) {

	assert( entityMesh != nullptr );
	m_entityMesh = entityMesh;
}


const EntityMesh* CBNode::getEntityMesh() const {

	return m_entityMesh;
}


void CBNode::setNodeDefaults() {

	m_initialLocalToWorldTransform = Matrix44<float>::createIdentityMatrixFloat();
	m_initialWorldToLocalTransform = Matrix44<float>::createIdentityMatrixFloat();
	m_nodeName = "Unknown";
	m_parentNode = nullptr;
	m_isVisible = true;
	m_nodeIndex = 0;
	m_boneIndex = BONE_INDEX_NONE;
	m_parentIndex = PARENT_INDEX_NONE;
	m_animationDuration = 0.0f;
	m_entityMesh = nullptr;
}