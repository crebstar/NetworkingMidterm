#ifndef included_XMLParser
#define included_XMLParser
#pragma once

#include <map>
#include <vector>
#include <string>

#include "EngineCommon.hpp"
#include "CBStringHelper.hpp"

#include "XMLNode.hpp"
#include "Vector2.hpp"
#include "IntVector2.hpp"
#include "Vector3D.hpp"
#include "Vector4.hpp"

/*
	XML Parsing utility class which wraps around pugi xml
*/

// Parameter Defaults For Convenience Functions
const char XMLSchemaDelimiter = ',';
const cbengine::Vector2 defaultFloatVec2Value( 0.0f, 0.0f );
const cbengine::IntVector2 defaultIntVec2Value( 0, 0 );
const cbengine::Vector3<float> defaultFloatVec3Value( 0.0f, 0.0f, 0.0f );
const cbengine::Vector3<int> defaultIntVec3Value( 0, 0, 0 );
const cbengine::Vector4<float> defaultFloatVec4Value( 0.0f, 0.0f, 0.0f, 0.0f );
const bool defaultBoolValue = false;

class XMLDocument;

class XMLParser {
public:

	static XMLParser & getSharedXMLParser() {
		static XMLParser xmlParser;
		return xmlParser;
	}

	~XMLParser();

	void loadXMLDocument( const std::string & xmlDocumentFilePath,
		const std::string & xmlDocumentFileName, 
		bool makeActiveDocument = true );

	// Switches currently active document to another document in the cache
	// If it does not exist, an warning message is posted to the console
	void switchActiveDocument( const std::string & xmlDocumentName );
	void releaseDocumentFromCacheWithName( const std::string & xmlDocumentName );
	void releaseAllDocumentsFromCache();

	// Convenience Functions
	int							getIntXMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, int defaultValueIfNotFound = 0 );
	float						getFloatXMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, float defaultValueIfNotFound = 0.0f );
	std::string					getStringXMLAttribute( const XMLNode & xmlNode, const std::string & atrributeName, const std::string & defaultValueIfNotFound = "" );
	cbengine::Vector2			getFloatVector2XMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, const cbengine::Vector2 & defaultValueIfNotFound = defaultFloatVec2Value );
	cbengine::IntVector2		getIntVector2XMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, const cbengine::IntVector2 & defaultValueIfNotFound = defaultIntVec2Value );
	cbengine::Vector3<float>	getFloatVector3XMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, const cbengine::Vector3<float> & defaultValueIfNotFound = defaultFloatVec3Value );
	// TODO :: Make an IntVector3 class
	cbengine::Vector3<int>		getIntVector3XMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, const cbengine::Vector3<int> & defaultValueIfNotFound = defaultIntVec3Value );
	// TODO :: Make a RGBA Color class when time permits
	cbengine::Vector4<float>    getFloatVector4XMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, const cbengine::Vector4<float> & defaultValueIfNotFound = defaultFloatVec4Value );
	bool						getBoolXMLAttribute( const XMLNode& xmlNode, const std::string& attributeName, const bool& defaultValueIfNotFound = defaultBoolValue );

	// Validation Functions
	void validateXMLAttributes( const XMLNode & xmlNode, const std::string & mandatoryAttribsCommaDelimited, const std::string & optionalAttribsCommaDelimited, bool outputWarningsToConsole = true );
	void validateXMLChildNodes( const XMLNode & xmlNode, const std::string & mandatoryChildNodesCommaDelimited, const std::string & optionalChildNodesCommaDelimited, bool outputWarningsToConsole = true );

	// Error Functions
	bool checkForCaseInsensitiveAttributeMatch( const XMLNode & xmlNode, const std::string & attributeName, std::string & correctAttribName );
	bool checkForCaseInsensitiveChildNodeMatch( const XMLNode & xmlNode, const std::vector< XMLNode > & childNodes, const std::string & childNodeName, std::string & correctNodeName );

	void triggerCaseInsensitiveAttributeErrorAndDie( const XMLDocument & xmlDocument, const XMLNode & xmlNode, const std::string & attribNameProvided, const std::string & correctAttribName, const std::string & additionalErrorMessageContent = "" );
	void triggerCaseInsensitiveChildNodeNameErrorAndDie( const XMLDocument & xmlDocument, const XMLNode & xmlNode, const std::string & nodeNameProvided, const std::string & correctNodeName, const std::string & additionalErrorMessageContent = "" );
	void triggerContainerParseErrorDueToSizeAndDie( const XMLDocument & xmlDocument, const XMLNode & xmlNode, const std::string attribNameProvided, const std::vector< std::string > & attribContents, const std::string & containerType, size_t sizeFound, size_t sizeExpected );
	void triggerMandatoryAttribMissingErrorAndDie( const XMLDocument & xmlDocument, const XMLNode & xmlNode, const std::vector<std::string> & mandatoryList, const std::vector<std::string> & nodeAttribList, const std::string & mandatoryAttribNotFound );
	void triggerMandatoryChildNodeMissingErrorAndDie( const XMLDocument & xmlDocument, const XMLNode & xmlNode, const std::vector<std::string> & mandatoryList, const std::vector<std::string> & actualNodeList, const std::string & mandatoryChildNodeNameNotFound );
	void triggerNameNotFoundFromValidationErrorAndDie( const XMLDocument & xmlDocument, const XMLNode & xmlNode, const std::vector<std::string> & mandatoryList, const std::vector<std::string> & optionalList, const std::vector<std::string> & actualNodeList, const std::vector<std::string> & namesNotFound, bool isAttribute , const std::string & additionalErrorMessageContent = "" );

	// Inline Mutators
	const std::string & getActiveDocumentName() const;
	const XMLDocument * getActiveDocument() const;

	// Non inline mutators
	XMLNode getActiveDocumentRootNode() const;

protected:
	// Singleton Constructor
	explicit XMLParser() {
		m_activeDocumentName = "None";
		m_activeDocument = nullptr;
	}

private:
	PREVENT_COPY_AND_ASSIGN( XMLParser );

	// Private Instance Variables
	std::map< std::string, XMLDocument * >			m_documentCache;
	std::string										m_activeDocumentName;
	XMLDocument *									m_activeDocument;

};

// Inline Mutator Definitions
inline const std::string & XMLParser::getActiveDocumentName() const {
	return m_activeDocumentName;
}


inline const XMLDocument * XMLParser::getActiveDocument() const {
	return m_activeDocument;
}


// Inline Convenience Functions
inline int XMLParser::getIntXMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, int defaultValueIfNotFound /* = 0 */ ) {
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	} 

	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			// Terminate program with an informative message to the user
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}
		
		return defaultValueIfNotFound;
	}
	int attributeValueAsInt = nodeAttribute.as_int( defaultValueIfNotFound );		
	return attributeValueAsInt;
}


inline float XMLParser::getFloatXMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, float defaultValueIfNotFound /* = 0.0f */ ) {
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}
	
	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	
	float attributeValueAsFloat = nodeAttribute.as_float( defaultValueIfNotFound );
	return attributeValueAsFloat;
}


inline std::string XMLParser::getStringXMLAttribute( const XMLNode & xmlNode, const std::string & attributeName, const std::string & defaultValueIfNotFound /* = "" */ ) {
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}
	
	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	
	std::string attributeValueAsFloat( nodeAttribute.as_string( defaultValueIfNotFound.c_str() ) );
	
	return attributeValueAsFloat;
}


inline cbengine::Vector2 XMLParser::getFloatVector2XMLAttribute( const XMLNode & xmlNode, 
	const std::string & attributeName, 
	const cbengine::Vector2 & defaultValueIfNotFound /* = defaultFloatVec2Value */ ) 
{
	const size_t expectedSize = 2;
	const std::string containerType = "FloatVector2";
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}

	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	
	std::string attributeValueAsString( nodeAttribute.as_string() );
	std::vector< std::string > vectorOfStrings;
	cbengine::convertDelimitedStringToVectorOfStrings( attributeValueAsString, XMLSchemaDelimiter, vectorOfStrings );

	if ( vectorOfStrings.size() != expectedSize ) {
		triggerContainerParseErrorDueToSizeAndDie( *(m_activeDocument), xmlNode, attributeName, vectorOfStrings, containerType, vectorOfStrings.size(), expectedSize );
	}
	
	// If control is at this point, the result is likely valid
	// TODO:: think of more cases where this might fail and come up with error messages
	float firstComponent = cbengine::convertStringToFloat( vectorOfStrings[0] );
	float secondComponent = cbengine::convertStringToFloat( vectorOfStrings[1] );
	cbengine::Vector2 floatVec2ToReturn( firstComponent, secondComponent );
	return floatVec2ToReturn;

}


inline cbengine::IntVector2 XMLParser::getIntVector2XMLAttribute( const XMLNode & xmlNode, 
	const std::string & attributeName, 
	const cbengine::IntVector2 & defaultValueIfNotFound /* = defaultIntVec2Value */ ) 
{
	const size_t expectedSize = 2;
	const std::string containerType = "IntVector2";
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}

	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	
	std::string attributeValueAsString( nodeAttribute.as_string() );
	std::vector< std::string > vectorOfStrings;
	cbengine::convertDelimitedStringToVectorOfStrings( attributeValueAsString, XMLSchemaDelimiter, vectorOfStrings );

	if ( vectorOfStrings.size() != expectedSize ) {
		triggerContainerParseErrorDueToSizeAndDie( *(m_activeDocument), xmlNode, attributeName, vectorOfStrings, containerType, vectorOfStrings.size(), expectedSize );
	}

	int firstComponent = cbengine::convertStringToInt( vectorOfStrings[0] );
	int secondComponent = cbengine::convertStringToInt( vectorOfStrings[1] );
	cbengine::IntVector2 intVec2ToReturn( firstComponent, secondComponent );
	return intVec2ToReturn;
}


inline cbengine::Vector3<float> XMLParser::getFloatVector3XMLAttribute( const XMLNode & xmlNode, 
	const std::string & attributeName, 
	const cbengine::Vector3<float> & defaultValueIfNotFound /* = defaultFloatVec3Value */ ) 
{
	const size_t expectedSize = 3;
	const std::string containerType = "FloatVector3";
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}

	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	
	std::string attributeValueAsString( nodeAttribute.as_string() );
	std::vector< std::string > vectorOfStrings;
	cbengine::convertDelimitedStringToVectorOfStrings( attributeValueAsString, XMLSchemaDelimiter, vectorOfStrings );

	if ( vectorOfStrings.size() != expectedSize ) {
		triggerContainerParseErrorDueToSizeAndDie( *(m_activeDocument), xmlNode, attributeName, vectorOfStrings, containerType, vectorOfStrings.size(), expectedSize );
	}
	
	float firstComponent = cbengine::convertStringToFloat( vectorOfStrings[0] );
	float secondComponent = cbengine::convertStringToFloat( vectorOfStrings[1] );
	float thirdComponent = cbengine::convertStringToFloat( vectorOfStrings[2] );
	cbengine::Vector3<float> floatVec3ToReturn( firstComponent, secondComponent, thirdComponent );
	return floatVec3ToReturn;
}


inline cbengine::Vector3<int> XMLParser::getIntVector3XMLAttribute( const XMLNode & xmlNode, 
	const std::string & attributeName, 
	const cbengine::Vector3<int> & defaultValueIfNotFound /* = defaultIntVec3Value */ ) 
{
	const size_t expectedSize = 3;
	const std::string containerType = "IntVector3";
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}

	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	
	std::string attributeValueAsString( nodeAttribute.as_string() );
	std::vector< std::string > vectorOfStrings;
	cbengine::convertDelimitedStringToVectorOfStrings( attributeValueAsString, XMLSchemaDelimiter, vectorOfStrings );

	if ( vectorOfStrings.size() != expectedSize ) {
		triggerContainerParseErrorDueToSizeAndDie( *(m_activeDocument), xmlNode, attributeName, vectorOfStrings, containerType, vectorOfStrings.size(), expectedSize );
	}

	int firstComponent = cbengine::convertStringToInt( vectorOfStrings[0] );
	int secondComponent = cbengine::convertStringToInt( vectorOfStrings[1] );
	int thirdComponent = cbengine::convertStringToInt( vectorOfStrings[2] );
	cbengine::Vector3<int> intVec3ToReturn( firstComponent, secondComponent, thirdComponent );
	return intVec3ToReturn;
}

// TODO: Make a RGBA Color class
inline cbengine::Vector4<float> XMLParser::getFloatVector4XMLAttribute( const XMLNode & xmlNode, 
	const std::string & attributeName,
	const cbengine::Vector4<float> & defaultValueIfNotFound /* = defaultFloatVec4Value */ ) 
{
	const size_t expectedSize = 4;
	const std::string containerType = "FloatVector4";
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}

	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	
	std::string attributeValueAsString( nodeAttribute.as_string() );
	std::vector< std::string > vectorOfStrings;
	cbengine::convertDelimitedStringToVectorOfStrings( attributeValueAsString, XMLSchemaDelimiter, vectorOfStrings );

	if ( vectorOfStrings.size() != expectedSize ) {
		triggerContainerParseErrorDueToSizeAndDie( *(m_activeDocument), xmlNode, attributeName, vectorOfStrings, containerType, vectorOfStrings.size(), expectedSize );
	}
	
	float firstComponent = cbengine::convertStringToFloat( vectorOfStrings[0] );
	float secondComponent = cbengine::convertStringToFloat( vectorOfStrings[1] );
	float thirdComponent = cbengine::convertStringToFloat( vectorOfStrings[2] );
	float fourthComponent = cbengine::convertStringToFloat( vectorOfStrings[3] );
	cbengine::Vector4<float> floatVec4ToReturn( firstComponent, secondComponent, thirdComponent, fourthComponent );
	return floatVec4ToReturn;

}


inline bool XMLParser::getBoolXMLAttribute( const XMLNode& xmlNode,
	const std::string& attributeName,
	const bool& defaultValueIfNotFound /* = defaultBoolValue */ ) 
{
	bool nodeLoadedSuccessfully = xmlNode.didNodeLoadSuccessfully();
	if ( !nodeLoadedSuccessfully ) {
		return defaultValueIfNotFound;
	}

	pugi::xml_attribute nodeAttribute;
	xmlNode.findNodeAttributeWithName( attributeName, nodeAttribute );
	std::string nodeName = nodeAttribute.name();
	if ( nodeName == "" ) {
		bool caseInsensitiveMatch = false;
		std::string correctAttributeName;
		caseInsensitiveMatch = checkForCaseInsensitiveAttributeMatch( xmlNode, attributeName, correctAttributeName );
		if ( caseInsensitiveMatch ) {
			triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, attributeName, correctAttributeName );
		}

		return defaultValueIfNotFound;
	}	

	std::string attributeValueAsString( nodeAttribute.as_string() );
	bool attribValueAsBool;
	bool conversionSuccessful = cbengine::convertStringToBool( attributeValueAsString, attribValueAsBool );
	if ( conversionSuccessful ) {
		return attribValueAsBool;
	}
	
	return defaultValueIfNotFound;

}

#endif