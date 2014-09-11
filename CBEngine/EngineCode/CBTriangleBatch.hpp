#ifndef included_CBTriangleBatch
#define included_CBTriangleBatch
#pragma once

#include <cassert>
#include <vector>
#include <string>

#include "EngineCommon.hpp"

#include "Texture.hpp"
#include "Vertex.hpp"
#include "Vector3D.hpp"
#include "Matrix44.hpp"
#include "Material.hpp"

class CBMesh;

class CBTriangleBatch {
public:
	friend class CBMesh;

	~CBTriangleBatch();
	CBTriangleBatch( CBMesh* parentMesh, const std::vector<cbengine::Vertex>& verts );

	void render( float deltaSeconds ) const;

	void setDiffuseTexture( const std::string& diffuseTextureName );
	void setNormalMapTexture( const std::string& normalMapTexture );
	void setSpecularLevelTexture( const std::string& specularLevelTexture );
	void setSpecularColorTexture( const std::string& specularColorTexture );
	void setEmissiveTexture( const std::string& emissiveTexture );
	cbengine::Texture* getDiffuseTexture() const;
	void setColorOfVertsToBlack();

	void createVBOAndMaterial();
protected:
	void createMaterial();

	void setCBTriangleBatchDefaults();

	void updateShaderWithLightData() const;
private:
	PREVENT_COPY_AND_ASSIGN( CBTriangleBatch );

	CBMesh*										m_parentMesh;

	std::vector<cbengine::Vertex>				m_verts;
	cbengine::Texture*							m_diffuseTexture;
	cbengine::Texture*							m_normalMapTexture;
	cbengine::Texture*							m_specularLevelTexture;
	cbengine::Texture*							m_specularColorTexture;
	cbengine::Texture*							m_emissiveTexture;
	unsigned int								m_VBOBufferID;
	Material*									m_material;
	mutable bool								m_validForRender;
};


inline cbengine::Texture* CBTriangleBatch::getDiffuseTexture() const {
	return m_diffuseTexture;
}


inline void CBTriangleBatch::setColorOfVertsToBlack() {

	for ( size_t i = 0; i < m_verts.size(); ++i ) {

		cbengine::Vertex& vertToUpdate = m_verts[i];
		vertToUpdate.vertexColor.x = 0.0f;
		vertToUpdate.vertexColor.y = 0.0f;
		vertToUpdate.vertexColor.z = 0.0f;
	}
}


#endif