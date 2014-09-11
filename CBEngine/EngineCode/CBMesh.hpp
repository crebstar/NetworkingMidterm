#ifndef included_CBMesh
#define included_CBMesh
#pragma once

#include "CBNode.hpp"

#include <cassert>

#include "EngineCommon.hpp"

#include "Texture.hpp"
#include "Vertex.hpp"
#include "Vector3D.hpp"
#include "Matrix44.hpp"
#include "Material.hpp"

class CBTriangleBatch;
class EntityMesh;

class CBMesh : public CBNode {
public:

	virtual ~CBMesh();
	explicit CBMesh( EntityMesh* entityMesh );

	virtual void update( float deltaSeconds );
	virtual void render( float deltaSeconds ) const;

	void addTriangleBatchToMesh( CBTriangleBatch* triBatchToAdd );	

protected:
	void createMaterial();

	void setMeshDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( CBMesh );

	std::vector<CBTriangleBatch*>				m_triangleBatches;

};


#endif