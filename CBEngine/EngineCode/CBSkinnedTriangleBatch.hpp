#ifndef included_CBSkinnedTriangleBatch
#define included_CBSkinnedTriangleBatch
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

class CBSkinnedMesh;

class CBSkinnedTriangleBatch {
public:
	friend class CBSkinnedMesh;

	~CBSkinnedTriangleBatch();
	CBSkinnedTriangleBatch( CBSkinnedMesh* parentMesh, const std::vector<cbengine::Vertex>& verts );

	void render( float deltaSeconds, const std::vector<Matrix44<float>>& boneTransformationMatrices ) const;

	void setDiffuseTexture( const std::string& diffuseTextureName );
	void setNormalMapTexture( const std::string& normalMapTexture );
	void setSpecularLevelTexture( const std::string& specularLevelTexture );
	void setSpecularColorTexture( const std::string& specularColorTexture );
	void setEmissiveTexture( const std::string& emissiveTexture );
	cbengine::Texture* getDiffuseTexture() const;
	void setColorOfVertsToBlack();

	std::vector<cbengine::Vertex>& getVertsVectorToModify();

	void createVBOAndMaterial();
protected:
	void createMaterial();

	void updateShaderWithLightData() const;

	void setCBSkinnedTriangleBatchDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( CBSkinnedTriangleBatch );

	CBSkinnedMesh*								m_parentMesh;

	std::vector<cbengine::Vertex>				m_verts;
	cbengine::Texture*							m_diffuseTexture;
	cbengine::Texture*							m_normalMapTexture;
	cbengine::Texture*							m_specularLevelTexture;
	cbengine::Texture*							m_specularColorTexture;
	cbengine::Texture*							m_emissiveTexture;
	int											m_boneTMUniformLocation;
	unsigned int								m_VBOBufferID;
	Material*									m_material;
	mutable bool								m_validForRender;
};


inline std::vector<cbengine::Vertex>& CBSkinnedTriangleBatch::getVertsVectorToModify() {

	return m_verts;
}

#endif