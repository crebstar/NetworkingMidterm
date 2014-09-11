#include "CBSkinnedMesh.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLShaderProgram.hpp"
#include "OpenGLShaderError.hpp"

#include "CBSkinnedTriangleBatch.hpp"
#include "CB3DSMaxImporter.hpp"
#include "EntityMesh.hpp"
#include "CB3DSMaxImporter.hpp"
#include "Uniform.hpp"

#include "Vector2.hpp"

#include "MatrixStack.hpp"
#include "TextureManager.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CBSkinnedMesh::~CBSkinnedMesh() {
	
}


CBSkinnedMesh::CBSkinnedMesh( EntityMesh* entityMesh ) {

	setSkinnedMeshDefaults();
	setEntityMesh( entityMesh );
	initializeBoneTMMatricesVector();
}


void CBSkinnedMesh::update( float deltaSeconds ) {

	m_animationDuration = m_animationDuration + deltaSeconds;

	for ( size_t i = 0; i < m_childrenNodes.size(); ++i ) {

		CBNode* childNode = m_childrenNodes[i];
		childNode->update( deltaSeconds );

	}
}


void CBSkinnedMesh::render( float deltaSeconds ) const {

	UNUSED(deltaSeconds);

	rebuildBoneTransformationMatrixVector();

	MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();
	matrixStack->setCurrentModelTransformMatrix( m_initialLocalToWorldTransform );

	for ( size_t i = 0; i < m_skinnedTriangleBatches.size(); ++i ) {

		CBSkinnedTriangleBatch* triBatch = m_skinnedTriangleBatches[i];
		triBatch->render( deltaSeconds, m_boneTransformationMatrices );
	}

	for ( size_t i = 0; i < m_childrenNodes.size(); ++i ) {

		CBNode* childNode = m_childrenNodes[i];
		childNode->render( deltaSeconds );
	}

}


void CBSkinnedMesh::rebuildBoneTransformationMatrixVector() const {

	assert( m_entityMesh != nullptr );

	std::set<int>::iterator itBoneIndex;
	for ( itBoneIndex = m_boneIndexesUsedByMesh.begin(); itBoneIndex != m_boneIndexesUsedByMesh.end(); ++itBoneIndex ) {

		int boneIndex = *(itBoneIndex);
		const CBNode* boneNode = m_entityMesh->getBoneNodeWithIndex( boneIndex );

		assert( boneNode != nullptr ); // TODO:: for now until supporting detaching bones or other bone reorg features

		const Matrix44<float>& meshWorldTM = this->getInitialLocalToWorldTransform();
		const Matrix44<float>& boneWorldToLocalTM = boneNode->getInitialWorldToLocalTransform();
		const Matrix44<float>& boneAnimTMForTime = boneNode->getAnimationAtTime( m_animationDuration );
		
		Matrix44<float> meshWorldTMMultboneWorldToLocal;
		Matrix44<float>::matrixMultiply( meshWorldTM, boneWorldToLocalTM, meshWorldTMMultboneWorldToLocal );

		Matrix44<float>& matrixAtIndex = m_boneTransformationMatrices[boneIndex];
		Matrix44<float>::matrixMultiply( meshWorldTMMultboneWorldToLocal, boneAnimTMForTime, matrixAtIndex );
		
	}
}


void CBSkinnedMesh::generateBoneTransformationMatrixUniformLocationValues() {

	for ( size_t i = 0; i < m_skinnedTriangleBatches.size(); ++i ) {

		CBSkinnedTriangleBatch* triBatch = m_skinnedTriangleBatches[i];
		Material* triBatchMaterial = triBatch->m_material;

		int boneTMUniformLocation = triBatchMaterial->addUniformToMaterial( UNIFORM_BONE_TM_NAME, m_boneTransformationMatrices );
		triBatch->m_boneTMUniformLocation = boneTMUniformLocation;
	}
}


void CBSkinnedMesh::addSkinnedTriangleBatchToMesh( CBSkinnedTriangleBatch* triBatchToAdd ) {

	assert( triBatchToAdd != nullptr );
	m_skinnedTriangleBatches.push_back( triBatchToAdd );

	updateBoneIndexSetWithNewTriangleBatchBones( triBatchToAdd );
}


void CBSkinnedMesh::updateBoneIndexSetWithNewTriangleBatchBones( CBSkinnedTriangleBatch* triBatchToAdd ) {

	assert( triBatchToAdd != nullptr );

	std::vector<cbengine::Vertex>& triBatchVerts = triBatchToAdd->getVertsVectorToModify();

	for ( size_t i = 0; i < triBatchVerts.size(); ++i ) {

		cbengine::Vertex& vert = triBatchVerts[i];
		if ( vert.vertexBoneIndexes.x != BONE_INDEX_NOT_USED ) {
			m_boneIndexesUsedByMesh.insert( vert.vertexBoneIndexes.x );
		} 

		if ( vert.vertexBoneIndexes.y != BONE_INDEX_NOT_USED ) {
			m_boneIndexesUsedByMesh.insert( vert.vertexBoneIndexes.y );
		}

		if ( vert.vertexBoneIndexes.z != BONE_INDEX_NOT_USED ) {
			m_boneIndexesUsedByMesh.insert( vert.vertexBoneIndexes.z );
		}

		if ( vert.vertexBoneIndexes.w != BONE_INDEX_NOT_USED ) {
			m_boneIndexesUsedByMesh.insert( vert.vertexBoneIndexes.w );
		}
	}
}


void CBSkinnedMesh::setSkinnedMeshDefaults() {

}


void CBSkinnedMesh::initializeBoneTMMatricesVector() {

	m_boneTransformationMatrices.resize( NUM_BONE_MATRIX_PASSED_TO_SHADER );

	for ( size_t i = 0; i < m_boneTransformationMatrices.size(); ++i ) {

		Matrix44<float>& boneTM = m_boneTransformationMatrices[i];
		boneTM = Matrix44<float>::createIdentityMatrixFloat();
	}

}