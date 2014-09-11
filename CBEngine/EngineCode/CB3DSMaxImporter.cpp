#include "CB3DSMaxImporter.hpp"

#include <stdio.h>
#include <algorithm>

#include "CBStringHelper.hpp"

#include "Vector3D.hpp"
#include "Vector2.hpp"
#include "Texture.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CB3DSMaxImporter::~CB3DSMaxImporter() {

}


bool CB3DSMaxImporter::importMaxCBEBinaryFileAndGetRootCBNode( const std::string& cbeFilePath, EntityMesh*& meshToCreate ) {

	bool didImport = false;

	std::ifstream* importBinaryStream = new std::ifstream( cbeFilePath.c_str(), std::ios::binary | std::ios::in );

	if ( !importBinaryStream || !( importBinaryStream->is_open() ) ) {
		return didImport;
	}

	const int rootNodeIndex = -1;
	const int defaultParentIndexForRoot = -2;
	std::map<int,CBNode*> importCBNodes;
	CBNode* nodeToCreate = new CBNode;
	nodeToCreate->setNodeIndex( rootNodeIndex );
	nodeToCreate->setParentIndex( defaultParentIndexForRoot );

	meshToCreate = new EntityMesh( nodeToCreate );
	nodeToCreate->setEntityMesh( meshToCreate );

	importCBNodes.insert( std::pair<int,CBNode*>( rootNodeIndex, nodeToCreate ) );

	std::map<int,MaterialData> materialMap;

	extractMaterialDataFromCBEImportBinaryFile( importBinaryStream, materialMap );

	extractAllNodeDataFromCBEImportBinaryFile( importBinaryStream, importCBNodes, materialMap, meshToCreate );

	std::map<int,CBNode*>::iterator itOut;
	for ( itOut = importCBNodes.begin(); itOut != importCBNodes.end(); ++itOut ) {

		int outerIndex = itOut->first;
		CBNode* nodeOuter = itOut->second;
		std::map<int,CBNode*>::iterator itInner;

		for ( itInner = importCBNodes.begin(); itInner != importCBNodes.end(); ++itInner ) {

			CBNode* nodeInner = itInner->second;
			int innerParentIndex = nodeInner->getParentIndex();

			if ( innerParentIndex == outerIndex ) {
				nodeOuter->addChildToNode( nodeInner );
				nodeInner->setParentNode( nodeOuter );
			}
		}

	}

	importBinaryStream->close();
	delete importBinaryStream;

	return didImport;
}


void CB3DSMaxImporter::extractMaterialDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream, std::map<int,MaterialData>& materialMap ) {

	int totalNumMaterials = 0;
	binaryInputStream->read( (char*) &totalNumMaterials, sizeof( int ) );

	int sizeOfMaterialNameString = 0;
	std::string materialNameString;

	int sizeOfDiffuseTextureString = 0;
	std::string diffuseNameString;

	int sizeOfNormalMapTextureString = 0;
	std::string normalMapNameString;

	int sizeOfSpecularLevelTextureString = 0;
	std::string specularLevelTextureNameString;

	int sizeOfSpecularColorTextureString = 0;
	std::string specularColorTextureNameString;

	int sizeOfEmissiveTextureString = 0;
	std::string emissiveTextureNameString;

	for ( size_t i = 0; i < static_cast<size_t>( totalNumMaterials ); ++i ) {

		MaterialData materialData;

		// Material Index
		binaryInputStream->read( (char*) &materialData.indexNum, sizeof( materialData.indexNum ) );
		
		// Material Name
		binaryInputStream->read( (char*) &sizeOfMaterialNameString, sizeof( sizeOfMaterialNameString ) );
		materialNameString.resize( sizeOfMaterialNameString );
		binaryInputStream->read( &materialNameString[0], sizeOfMaterialNameString );
		materialData.materialName = materialNameString;

		// Diffuse Name
		binaryInputStream->read( (char*) &sizeOfDiffuseTextureString, sizeof( sizeOfDiffuseTextureString ) );
		diffuseNameString.resize( sizeOfDiffuseTextureString );
		binaryInputStream->read( &diffuseNameString[0], sizeOfDiffuseTextureString );

		cleanUpTextureNames( diffuseNameString );

		if ( diffuseNameString == NO_TEXTURE_FOUND ) {
			diffuseNameString = DEFAULT_NO_DIFFUSE;
		}

		// Normal Name
		binaryInputStream->read( (char*) &sizeOfNormalMapTextureString, sizeof( sizeOfNormalMapTextureString ) );
		normalMapNameString.resize( sizeOfNormalMapTextureString );
		binaryInputStream->read( (char*) &normalMapNameString[0], sizeOfNormalMapTextureString );

		cleanUpTextureNames( normalMapNameString );

		if ( normalMapNameString == NO_TEXTURE_FOUND ) {
			normalMapNameString = DEFAULT_NO_NORMAL_MAP;
		} 

		// Specular Level Name
		binaryInputStream->read( (char*) &sizeOfSpecularLevelTextureString, sizeof( sizeOfSpecularLevelTextureString ) );
		specularLevelTextureNameString.resize( sizeOfSpecularLevelTextureString );
		binaryInputStream->read( (char*) &specularLevelTextureNameString[0], sizeOfSpecularLevelTextureString );

		cleanUpTextureNames( specularLevelTextureNameString );

		if ( specularLevelTextureNameString == NO_TEXTURE_FOUND ) {
			specularLevelTextureNameString = DEFAULT_NO_SPECULAR;
		} 
		
		// Specular Color Name
		binaryInputStream->read( (char*) &sizeOfSpecularColorTextureString, sizeof( sizeOfSpecularColorTextureString ) );
		specularColorTextureNameString.resize( sizeOfSpecularColorTextureString );
		binaryInputStream->read( (char*) &specularColorTextureNameString[0], sizeOfSpecularColorTextureString );

		cleanUpTextureNames( specularColorTextureNameString );

		if ( specularColorTextureNameString == NO_TEXTURE_FOUND ) {
			specularColorTextureNameString = DEFAULT_NO_SPECULAR_COLOR;
		}

		// Emissive Name
		binaryInputStream->read( (char*) &sizeOfEmissiveTextureString, sizeof( sizeOfEmissiveTextureString ) );
		emissiveTextureNameString.resize( sizeOfEmissiveTextureString );
		binaryInputStream->read( (char*) &emissiveTextureNameString[0], sizeOfEmissiveTextureString ); 

		cleanUpTextureNames( emissiveTextureNameString );

		if ( emissiveTextureNameString == NO_TEXTURE_FOUND ) {
			emissiveTextureNameString = DEFAULT_NO_EMISSIVE;
		}

		materialData.diffuseTextureFilePath = diffuseNameString;
		materialData.normalMapTextureFilePath = normalMapNameString;
		materialData.specularLevelTextureFilePath = specularLevelTextureNameString;
		materialData.specularColorTextureFilePath = specularColorTextureNameString;
		materialData.emissiveTextureFilePath = emissiveTextureNameString;

		materialMap.insert( std::pair<int,MaterialData>( materialData.indexNum, materialData ) );

	}
}


void CB3DSMaxImporter::extractAllNodeDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
	std::map<int,CBNode*>&  importCBNodes,
	std::map<int,MaterialData>& materialMap,
	EntityMesh* entityMesh ) 
{
	
	int totalNumberOfNodes = 0;
	binaryInputStream->read( (char*) &totalNumberOfNodes, sizeof( totalNumberOfNodes ) );

	for ( int i = 0; i < totalNumberOfNodes; ++i ) {

		// Read in the type of Node
		int sizeOfNodeType = 0;
		binaryInputStream->read( (char*) &sizeOfNodeType, sizeof( sizeOfNodeType ) );

		std::string nodeType;
		nodeType.resize( sizeOfNodeType );
		binaryInputStream->read( &nodeType[0], sizeOfNodeType );

		if ( nodeType == "Node" ) {

			CBNode* newNode = new CBNode( entityMesh );
			extractNodeDataFromCBEImportBinaryFile( binaryInputStream, newNode, materialMap );
			importCBNodes.insert( std::pair<int,CBNode*>( newNode->getNodeIndex(), newNode ) );

		} else if ( nodeType == "Mesh" ) {

			CBMesh* newMesh = new CBMesh( entityMesh );
			extractMeshDataFromCBEImportBinaryFile( binaryInputStream, newMesh, materialMap );
			importCBNodes.insert( std::pair<int,CBNode*>( newMesh->getNodeIndex(), newMesh ) );

		} else if ( nodeType == "SkinnedMesh" ) {
			
			CBSkinnedMesh* newSkinnedMesh = new CBSkinnedMesh( entityMesh );
			extractSkinnedMeshDataFromCBEImportBinaryFile( binaryInputStream, newSkinnedMesh, materialMap );
			newSkinnedMesh->generateBoneTransformationMatrixUniformLocationValues();
			importCBNodes.insert( std::pair<int,CBSkinnedMesh*>( newSkinnedMesh->getNodeIndex(), newSkinnedMesh ) );

		} else if ( nodeType == "Bone" ) {
			
			CBNode* newBoneNode = new CBNode( entityMesh );
			extractBoneNodeDataFromCBEImportBinaryFile( binaryInputStream, newBoneNode, materialMap, entityMesh );
			importCBNodes.insert( std::pair<int,CBNode*>( newBoneNode->getNodeIndex(), newBoneNode ) );

		} else {
			// Shit
			assert( true == true );
		}
	}
}


void CB3DSMaxImporter::extractNodeDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
	CBNode* importCBNode,
	std::map<int,MaterialData>& materialMap ) 
{

	UNUSED( materialMap );
	int sizeOfNodeNameString = 0;
	std::string nodeNameString;
	binaryInputStream->read( (char*) &sizeOfNodeNameString, sizeof( sizeOfNodeNameString ) );
	nodeNameString.resize( sizeOfNodeNameString );
	binaryInputStream->read( (char*) &nodeNameString[0], sizeOfNodeNameString );
	importCBNode->setNodeName( nodeNameString );

	// Node Index
	int nodeIndex = 0;
	binaryInputStream->read( (char*) &nodeIndex, sizeof( nodeIndex ) );
	importCBNode->setNodeIndex( nodeIndex );

	// Parent Node Index
	int parentNodeIndex = -1;
	binaryInputStream->read( (char*) &parentNodeIndex, sizeof( parentNodeIndex ) );
	importCBNode->setParentIndex( parentNodeIndex );

	Matrix44<float> localToWorldTransform;
	binaryInputStream->read( (char*) &localToWorldTransform.matrixData[0], sizeof( Matrix44<float> ) );
	importCBNode->setInitialLocalToWorldTransform( localToWorldTransform );

	Matrix44<float> worldToLocalTransform;
	binaryInputStream->read( (char*) &worldToLocalTransform.matrixData[0], sizeof( Matrix44<float> ) ); 
	importCBNode->setInitialWorldToLocalTransform( worldToLocalTransform );

	int numberOfAnimTransforms = 0;
	binaryInputStream->read( (char*) &numberOfAnimTransforms, sizeof( numberOfAnimTransforms ) );

	for ( int i = 0; i < numberOfAnimTransforms; ++i ) {

		Matrix44<float> animTransform;
		binaryInputStream->read( (char*) &animTransform.matrixData[0], sizeof( Matrix44<float> ) );
		importCBNode->addAnimationTransform( animTransform );
	}
	
}


void CB3DSMaxImporter::extractMeshDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
	CBMesh* importCBMesh,
	std::map<int,MaterialData>& materialMap )
{

	int sizeOfNodeNameString = 0;
	std::string nodeNameString;
	binaryInputStream->read( (char*) &sizeOfNodeNameString, sizeof( sizeOfNodeNameString ) );
	nodeNameString.resize( sizeOfNodeNameString );
	binaryInputStream->read( (char*) &nodeNameString[0], sizeOfNodeNameString );
	importCBMesh->setNodeName( nodeNameString );

	// Node Index
	int nodeIndex = 0;
	binaryInputStream->read( (char*) &nodeIndex, sizeof( nodeIndex ) );
	importCBMesh->setNodeIndex( nodeIndex );

	// Parent Node Index
	int parentNodeIndex = -1;
	binaryInputStream->read( (char*) &parentNodeIndex, sizeof( parentNodeIndex ) );
	importCBMesh->setParentIndex( parentNodeIndex );

	Matrix44<float> localToWorldTransform;
	binaryInputStream->read( (char*) &localToWorldTransform.matrixData, sizeof( Matrix44<float> ) );
	importCBMesh->setInitialLocalToWorldTransform( localToWorldTransform );

	Matrix44<float> worldToLocalTransform;
	binaryInputStream->read( (char*) &worldToLocalTransform.matrixData[0], sizeof( Matrix44<float> ) ); 
	importCBMesh->setInitialWorldToLocalTransform( worldToLocalTransform );

	int numberOfAnimTransforms = 0;
	binaryInputStream->read( (char*) &numberOfAnimTransforms, sizeof( numberOfAnimTransforms ) );


	for ( int i = 0; i < numberOfAnimTransforms; ++i ) {

		Matrix44<float> animTransform;
		binaryInputStream->read( (char*) &animTransform.matrixData[0], sizeof( Matrix44<float> ) );
		importCBMesh->addAnimationTransform( animTransform );
	}

	extractTriBatchDataFromMeshNode( binaryInputStream, importCBMesh, materialMap );
}


void CB3DSMaxImporter::extractSkinnedMeshDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
	CBSkinnedMesh* importCBMesh,
	std::map<int,MaterialData>& materialMap )
{
	int sizeOfNodeNameString = 0;
	std::string nodeNameString;
	binaryInputStream->read( (char*) &sizeOfNodeNameString, sizeof( sizeOfNodeNameString ) );
	nodeNameString.resize( sizeOfNodeNameString );
	binaryInputStream->read( (char*) &nodeNameString[0], sizeOfNodeNameString );
	importCBMesh->setNodeName( nodeNameString );

	// Node Index
	int nodeIndex = 0;
	binaryInputStream->read( (char*) &nodeIndex, sizeof( nodeIndex ) );
	importCBMesh->setNodeIndex( nodeIndex );

	// Parent Node Index
	int parentNodeIndex = -1;
	binaryInputStream->read( (char*) &parentNodeIndex, sizeof( parentNodeIndex ) );
	importCBMesh->setParentIndex( parentNodeIndex );

	Matrix44<float> localToWorldTransform;
	binaryInputStream->read( (char*) &localToWorldTransform.matrixData, sizeof( Matrix44<float> ) );
	importCBMesh->setInitialLocalToWorldTransform( localToWorldTransform );

	Matrix44<float> worldToLocalTransform;
	binaryInputStream->read( (char*) &worldToLocalTransform.matrixData[0], sizeof( Matrix44<float> ) ); 
	importCBMesh->setInitialWorldToLocalTransform( worldToLocalTransform );

	int numberOfAnimTransforms = 0;
	binaryInputStream->read( (char*) &numberOfAnimTransforms, sizeof( numberOfAnimTransforms ) );


	for ( int i = 0; i < numberOfAnimTransforms; ++i ) {

		Matrix44<float> animTransform;
		binaryInputStream->read( (char*) &animTransform.matrixData[0], sizeof( Matrix44<float> ) );
		importCBMesh->addAnimationTransform( animTransform );
	}

	extractSkinnedTriBatchDataFromMeshNode( binaryInputStream, importCBMesh, materialMap );
}


void CB3DSMaxImporter::extractBoneNodeDataFromCBEImportBinaryFile( std::ifstream* binaryInputStream,
	CBNode* importCBNode,
	std::map<int,MaterialData>& materialMap,
	EntityMesh* entityMesh ) 
{
	UNUSED( materialMap );
	int sizeOfNodeNameString = 0;
	std::string nodeNameString;
	binaryInputStream->read( (char*) &sizeOfNodeNameString, sizeof( sizeOfNodeNameString ) );
	nodeNameString.resize( sizeOfNodeNameString );
	binaryInputStream->read( (char*) &nodeNameString[0], sizeOfNodeNameString );
	importCBNode->setNodeName( nodeNameString );

	// Node Index
	int nodeIndex = 0;
	binaryInputStream->read( (char*) &nodeIndex, sizeof( nodeIndex ) );
	importCBNode->setNodeIndex( nodeIndex );

	// Bone Index
	int boneIndex = 0;
	binaryInputStream->read( (char*) &boneIndex, sizeof( boneIndex ) );
	importCBNode->setBoneIndex( boneIndex );

	// Parent Node Index
	int parentNodeIndex = -1;
	binaryInputStream->read( (char*) &parentNodeIndex, sizeof( parentNodeIndex ) );
	importCBNode->setParentIndex( parentNodeIndex );

	Matrix44<float> localToWorldTransform;
	binaryInputStream->read( (char*) &localToWorldTransform.matrixData[0], sizeof( Matrix44<float> ) );
	importCBNode->setInitialLocalToWorldTransform( localToWorldTransform );

	Matrix44<float> worldToLocalTransform;
	binaryInputStream->read( (char*) &worldToLocalTransform.matrixData[0], sizeof( Matrix44<float> ) ); 
	importCBNode->setInitialWorldToLocalTransform( worldToLocalTransform );

	int numberOfAnimTransforms = 0;
	binaryInputStream->read( (char*) &numberOfAnimTransforms, sizeof( numberOfAnimTransforms ) );

	for ( int i = 0; i < numberOfAnimTransforms; ++i ) {

		Matrix44<float> animTransform;
		binaryInputStream->read( (char*) &animTransform.matrixData[0], sizeof( Matrix44<float> ) );
		importCBNode->addAnimationTransform( animTransform );
	}

	entityMesh->addBoneNodeToBoneHashMap( boneIndex, importCBNode );
}


void CB3DSMaxImporter::extractTriBatchDataFromMeshNode( std::ifstream* binaryInputStream,
	CBMesh* meshToExtractFrom,
	std::map<int,MaterialData>& materialMap )
{

	int numTriBatchesInMesh = 0;
	binaryInputStream->read( (char*) &numTriBatchesInMesh, sizeof( numTriBatchesInMesh ) );

	for ( int i = 0; i < numTriBatchesInMesh; ++i ) {

		TriBatchData triBatchData;

		int materialIndex = 0;
		binaryInputStream->read( (char*) &materialIndex, sizeof( materialIndex ) );
		triBatchData.materialIndex = materialIndex;

		int numVertsInTriBatch = 0;
		binaryInputStream->read( (char*) &numVertsInTriBatch, sizeof( numVertsInTriBatch ) );
		triBatchData.numVerts = numVertsInTriBatch;

		for ( int vertIndex = 0; vertIndex < numVertsInTriBatch; ++vertIndex ) {

			cbengine::Vertex triBatchVert;
			binaryInputStream->read( (char*) &triBatchVert, sizeof( cbengine::Vertex ) );
			// Adjust for lack of importing colors in exporter
			triBatchVert.vertexColor.x = 1.0f;
			triBatchVert.vertexColor.y = 1.0f;
			triBatchVert.vertexColor.z = 1.0f;
			triBatchVert.vertexColor.w = 1.0f;

			// Adjust for our inverted tex coord importer
			triBatchVert.vertexTextureCoords.y = 1.0f - triBatchVert.vertexTextureCoords.y;

			triBatchData.verts.push_back( triBatchVert );
		}

		std::map<int,MaterialData>::iterator it;
		it = materialMap.find( triBatchData.materialIndex );
		if ( it != materialMap.end() ) {

			const MaterialData& matdata = it->second;
			CBTriangleBatch* triBatchToAdd = new CBTriangleBatch( meshToExtractFrom, triBatchData.verts );
			triBatchToAdd->setDiffuseTexture( matdata.diffuseTextureFilePath );
			triBatchToAdd->setNormalMapTexture( matdata.normalMapTextureFilePath );
			triBatchToAdd->setSpecularLevelTexture( matdata.specularLevelTextureFilePath );
			triBatchToAdd->setSpecularColorTexture( matdata.specularColorTextureFilePath );
			triBatchToAdd->setEmissiveTexture( matdata.emissiveTextureFilePath );

			triBatchToAdd->createVBOAndMaterial();
			// Add trasnform matrix
			assert( triBatchToAdd != nullptr );
			meshToExtractFrom->addTriangleBatchToMesh( triBatchToAdd );

		} else {
			assert( true == true );
		}
	}
}


void CB3DSMaxImporter::extractSkinnedTriBatchDataFromMeshNode( std::ifstream* binaryInputStream,
	CBSkinnedMesh* meshToExtractFrom,
	std::map<int,MaterialData>& materialMap ) 
{
	int numTriBatchesInMesh = 0;
	binaryInputStream->read( (char*) &numTriBatchesInMesh, sizeof( numTriBatchesInMesh ) );

	for ( int i = 0; i < numTriBatchesInMesh; ++i ) {

		TriBatchData triBatchData;

		int materialIndex = 0;
		binaryInputStream->read( (char*) &materialIndex, sizeof( materialIndex ) );
		triBatchData.materialIndex = materialIndex;

		int numVertsInTriBatch = 0;
		binaryInputStream->read( (char*) &numVertsInTriBatch, sizeof( numVertsInTriBatch ) );
		triBatchData.numVerts = numVertsInTriBatch;

		for ( int vertIndex = 0; vertIndex < numVertsInTriBatch; ++vertIndex ) {

			cbengine::Vertex triBatchVert;
			binaryInputStream->read( (char*) &triBatchVert, sizeof( cbengine::Vertex ) );
			// Adjust for lack of importing colors in exporter
			triBatchVert.vertexColor.x = 1.0f;
			triBatchVert.vertexColor.y = 1.0f;
			triBatchVert.vertexColor.z = 1.0f;
			triBatchVert.vertexColor.w = 1.0f;

			triBatchVert.vertexBoneIndexes.x = BONE_INDEX_NOT_USED;
			triBatchVert.vertexBoneIndexes.y = BONE_INDEX_NOT_USED;
			triBatchVert.vertexBoneIndexes.z = BONE_INDEX_NOT_USED;
			triBatchVert.vertexBoneIndexes.w = BONE_INDEX_NOT_USED;

			// Adjust for our inverted tex coord importer
			triBatchVert.vertexTextureCoords.y = 1.0f - triBatchVert.vertexTextureCoords.y;

			std::vector<BoneData> boneDataVector;
			int numBonesForVert = 0;
			binaryInputStream->read( (char*) &numBonesForVert, sizeof( numBonesForVert ) );

			for ( int boneIndex = 0; boneIndex < numBonesForVert; ++boneIndex ) {

				BoneData boneData;
				binaryInputStream->read( (char*) &boneData.boneIndex, sizeof( boneData.boneIndex ) );
				binaryInputStream->read( (char*) &boneData.boneWeight, sizeof( boneData.boneWeight ) );
				boneDataVector.push_back( boneData );
			}

			float totalExtractedWeight = 0.0f;
			for ( int boneDataIndex = 0; boneDataIndex < static_cast<int>( boneDataVector.size() ); ++boneDataIndex ) {

				BoneData& boneData = boneDataVector[ boneDataIndex ];
				
				if ( boneDataIndex == ( MAX_NUM_BONES_TO_EXTRACT - 1 ) ) {

					boneData.boneWeight = 1.0f - totalExtractedWeight;
				}

				totalExtractedWeight += boneData.boneWeight;

				switch( boneDataIndex ) {
				case 0:
					triBatchVert.vertexBoneIndexes.x = boneData.boneIndex;
					triBatchVert.vertexBoneWeights.x = boneData.boneWeight;
					break;
				case 1:
					triBatchVert.vertexBoneIndexes.y = boneData.boneIndex;
					triBatchVert.vertexBoneWeights.y = boneData.boneWeight;
					break;
				case 2:
					triBatchVert.vertexBoneIndexes.z = boneData.boneIndex;
					triBatchVert.vertexBoneWeights.z = boneData.boneWeight;
					break;
				case 3:
					triBatchVert.vertexBoneIndexes.w = boneData.boneIndex;
					triBatchVert.vertexBoneWeights.w = boneData.boneWeight;
					break;
				}
			}

			triBatchData.verts.push_back( triBatchVert );
		}

		std::map<int,MaterialData>::iterator it;
		it = materialMap.find( triBatchData.materialIndex );
		if ( it != materialMap.end() ) {

			const MaterialData& matdata = it->second;
			CBSkinnedTriangleBatch* triBatchToAdd = new CBSkinnedTriangleBatch( meshToExtractFrom, triBatchData.verts );
			
			triBatchToAdd->setDiffuseTexture(  matdata.diffuseTextureFilePath );
			triBatchToAdd->setNormalMapTexture( matdata.normalMapTextureFilePath );
			triBatchToAdd->setSpecularLevelTexture( matdata.specularLevelTextureFilePath );
			triBatchToAdd->setSpecularColorTexture( matdata.specularColorTextureFilePath );
			triBatchToAdd->setEmissiveTexture( matdata.emissiveTextureFilePath );
			
			// Can't do this here with SkinnedTriBatches
			triBatchToAdd->createVBOAndMaterial();

			// Add trasnform matrix
			assert( triBatchToAdd != nullptr );
			meshToExtractFrom->addSkinnedTriangleBatchToMesh( triBatchToAdd );

		} else {
			assert( true == true );
		}
	}

}

void CB3DSMaxImporter::cleanUpTextureNames( std::string& textureName ) {

	const std::string NoTextureString( "None" );
	if ( textureName == NoTextureString || textureName.empty() ) {
		return;
	}

	int index = textureName.size() - 1;
	std::string textureNameCleaned;

	while ( ( index >= 0 ) && textureName[index] != '\\' ) {

		textureNameCleaned += textureName[index];
		--index;
	}

	std::reverse( textureNameCleaned.begin(), textureNameCleaned.end() );

	if ( textureNameCleaned.size() > 3 ) {

		std::string replaceString( "png" );
		std::string checkForDDS( "sdd" );
		std::string currentString;
		for ( int i = ( textureNameCleaned.size() - 1 ); i >= 0; --i ) {

			currentString += textureNameCleaned[i];
			if ( currentString == checkForDDS ) {
				textureNameCleaned[ textureNameCleaned.size() - 1 ] = replaceString[2];
				textureNameCleaned[ textureNameCleaned.size() - 2 ] = replaceString[1];
				textureNameCleaned[ textureNameCleaned.size() - 3 ] = replaceString[0];
			}
		}
	}

	if ( index == -1 ) {
		textureName = "None";
	}
	
	textureName.clear();
	textureName += TEXTURE_DIRECTORY;
	textureName += textureNameCleaned;
}


