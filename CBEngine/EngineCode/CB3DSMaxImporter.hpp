#ifndef included_CB3DSMaxImporter
#define included_CB3DSMaxImporter
#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "EngineMacros.hpp"

#include "Vertex.hpp"
#include "Entity.hpp"
#include "Matrix44.hpp"

#include "CBMesh.hpp"
#include "CBNode.hpp"
#include "CBSkinnedMesh.hpp"
#include "EntityMesh.hpp"
#include "CBTriangleBatch.hpp"
#include "CBSkinnedTriangleBatch.hpp"


const std::string TEXTURE_DIRECTORY = "Art\\";
const std::string UNIFORM_BONE_TM_NAME = "u_boneTM"; // TODO:: find better place for this...
const int MAX_NUM_BONES_TO_EXTRACT	= 4;
const int BONE_INDEX_NOT_USED		= -1;

const std::string DEFAULT_NO_DIFFUSE = "Art/DefaultDiffuse.png";
const std::string DEFAULT_NO_NORMAL_MAP = "Art/DefaultNormalMap.png";
const std::string DEFAULT_NO_SPECULAR = "Art/DefaultSpecular.png";
const std::string DEFAULT_NO_SPECULAR_COLOR = "Art/DefaultSpecularColor.png";
const std::string DEFAULT_NO_EMISSIVE = "Art/DefaultEmissive.png";
const std::string NO_TEXTURE_FOUND = "None";

struct MaterialData {
public:
	MaterialData() {
		indexNum = 0;
	}

	int indexNum;
	std::string materialName;
	std::string diffuseTextureFilePath;
	std::string normalMapTextureFilePath;
	std::string specularLevelTextureFilePath;
	std::string specularColorTextureFilePath;
	std::string emissiveTextureFilePath;
};


struct TriBatchData {
public:
	TriBatchData() {
		materialIndex = 0;
		numVerts = 0;
	}

	int materialIndex;
	int numVerts;
	std::vector<cbengine::Vertex> verts;
	std::string	diffuseTextureFilePath;
};


struct BoneData {
public:
	BoneData() {
		boneIndex = -1;
		boneWeight = 0.0f;
	}

	int boneIndex;
	float boneWeight;

};


class CB3DSMaxImporter {
public:
	static CB3DSMaxImporter& getSharedCB3DSMaxImporter() {
		static CB3DSMaxImporter maxImporter;
		return maxImporter;
	}

	~CB3DSMaxImporter();

	bool importMaxCBEFileAndGetEntity( const std::string& cbeFilePath, std::vector<Entity3D*>& maxMeshEntityVec );
	bool importMaxCBEBinaryFileAndGetRootCBNode( const std::string& cbeFilePath, EntityMesh*& meshToCreate );

	//bool importMaxCBETextFileAndGetCBMesh( const std::string& cbeFilePath, CBMesh*& meshToImport );
	
protected:
	CB3DSMaxImporter() {

	}

	void extractMaterialDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream, std::map<int,MaterialData>& materialMap );

	void extractAllNodeDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
		std::map<int,CBNode*>& importCBNodes,
		std::map<int,MaterialData>& materialMap, 
		EntityMesh* entityMesh );

	void extractNodeDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
		CBNode* importCBNode,
		std::map<int,MaterialData>& materialMap );

	void extractMeshDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
		CBMesh* importCBMesh,
		std::map<int,MaterialData>& materialMap );

	void extractTriBatchDataFromMeshNode( std::ifstream* binaryInputStream,
		CBMesh* meshToExtractFrom,
		std::map<int,MaterialData>& materialMap );

	void extractBoneNodeDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
		CBNode* importCBNode,
		std::map<int,MaterialData>& materialMap,
		EntityMesh* entityMesh );

	void extractSkinnedMeshDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
		CBSkinnedMesh* importCBMesh,
		std::map<int,MaterialData>& materialMap );

	void extractSkinnedTriBatchDataFromMeshNode( std::ifstream* binaryInputStream,
		CBSkinnedMesh* meshToExtractFrom,
		std::map<int,MaterialData>& materialMap );
	


	void cleanUpTextureNames( std::string& textureName );
	
private:
	PREVENT_COPY_AND_ASSIGN( CB3DSMaxImporter );

};





#endif