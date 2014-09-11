#include "CBMesh.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLShaderProgram.hpp"
#include "OpenGLShaderError.hpp"

#include "CBTriangleBatch.hpp"
#include "EntityMesh.hpp"

#include "Vector2.hpp"

#include "MatrixStack.hpp"
#include "TextureManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CBMesh::~CBMesh() {

}


CBMesh::CBMesh( EntityMesh* entityMesh ) {

	setMeshDefaults();
	setEntityMesh( entityMesh );
}


void CBMesh::update( float deltaSeconds ) {

	m_animationDuration = m_animationDuration + deltaSeconds;

	for ( size_t i = 0; i < m_childrenNodes.size(); ++i ) {

		CBNode* childNode = m_childrenNodes[i];
		childNode->update( deltaSeconds );
	}
}


void CBMesh::render( float deltaSeconds ) const {

	UNUSED(deltaSeconds);

	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();

	if ( !m_animationTransforms.empty() ) {

		int numberOfAnimationsTransforms = m_animationTransforms.size();
		int currentAnimation = static_cast<int>( m_animationDuration * 30 ) % numberOfAnimationsTransforms;
		const Matrix44<float>& animMatrix = m_animationTransforms[currentAnimation];
		Matrix44<float> idM = Matrix44<float>::createIdentityMatrixFloat();
		matrixStack->applyTransformAndPushToStack( animMatrix );
		matrixStack->setCurrentModelTransformMatrix( animMatrix );

	} else {

		matrixStack->applyTransformAndPushToStack( m_initialLocalToWorldTransform );
		matrixStack->setCurrentModelTransformMatrix( m_initialLocalToWorldTransform );
	}

	
	for ( size_t i = 0; i < m_triangleBatches.size(); ++i ) {
		
		CBTriangleBatch* triBatch = m_triangleBatches[i];
		triBatch->render( deltaSeconds );
	}
	
	matrixStack->popFromTopOfStack();

	for ( size_t i = 0; i < m_childrenNodes.size(); ++i ) {

		CBNode* childNode = m_childrenNodes[i];
		childNode->render( deltaSeconds );
	}

}



void CBMesh::addTriangleBatchToMesh( CBTriangleBatch* triBatchToAdd ) {

	assert( triBatchToAdd != nullptr );
	m_triangleBatches.push_back( triBatchToAdd );
}


void CBMesh::setMeshDefaults() {

}