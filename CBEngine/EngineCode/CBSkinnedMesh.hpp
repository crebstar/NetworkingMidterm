#ifndef included_CBSkinnedMesh
#define included_CBSkinnedMesh
#pragma once

#include "CBNode.hpp"

#include <cassert>
#include <vector>
#include <set>

#include "EngineCommon.hpp"

#include "Texture.hpp"
#include "Vertex.hpp"
#include "Vector3D.hpp"
#include "Matrix44.hpp"
#include "Material.hpp"

class CBSkinnedTriangleBatch;
class EntityMesh;

class CBSkinnedMesh : public CBNode {
public:
	friend class CBSkinnedTriangleBatch;

	virtual ~CBSkinnedMesh();
	explicit CBSkinnedMesh( EntityMesh* entityMesh );

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	void addSkinnedTriangleBatchToMesh( CBSkinnedTriangleBatch* triBatchToAdd );	
	void generateBoneTransformationMatrixUniformLocationValues();

protected:
	void createMaterial();

	
	void updateBoneIndexSetWithNewTriangleBatchBones( CBSkinnedTriangleBatch* triBatchToAdd );
	void rebuildBoneTransformationMatrixVector() const;

	void setSkinnedMeshDefaults();
	void initializeBoneTMMatricesVector();
private:
	PREVENT_COPY_AND_ASSIGN( CBSkinnedMesh );

	std::vector<CBSkinnedTriangleBatch*>				m_skinnedTriangleBatches;
	std::set<int>										m_boneIndexesUsedByMesh;
	mutable std::vector<Matrix44<float>>				m_boneTransformationMatrices;

};



#endif