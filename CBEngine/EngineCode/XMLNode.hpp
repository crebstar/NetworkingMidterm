#ifndef included_XMLNode
#define included_XMLNode
#pragma once

#include <string>
#include <vector>

#include "EngineCommon.hpp"
#include "Vector4.hpp"
#include "pugixml.hpp"
#include "Console.hpp"

/*
	XMLNode which wraps around the pugi xml node. The node is not loaded at construction
*/

class XMLNode {
public:
	~XMLNode();
	explicit XMLNode();
	explicit XMLNode( const std::string & xmlNodeName );
	explicit XMLNode( pugi::xml_node & node );

	// Finding and loading nodes
	void findNodeAsChildOfNode( XMLNode & parentNode );

	// Finding attributes
	void findNodeAttributeWithName( const std::string & attributeName, pugi::xml_attribute & attribute ) const;

	// Helper Functions
	bool getNextSibling( XMLNode& siblingNode ) const;
	void getChildOfNode( const std::string & childNodeName, XMLNode & childNode ) const;
	void getAllChildrenOfNode( std::vector< XMLNode > & children ) const;
	ptrdiff_t getNodeOffsetFromDocumentStart() const;

	// Does not modify the vector if the node is invalid	
	void getListOfAttributeNames( std::vector< std::string > & nodeAttributeNames ) const;

	// Validation Functions
	bool isNodeValid() const;
	bool didNodeLoadSuccessfully() const;

	//Inline Mutators
	const std::string & getNodeName() const;

protected:

private:
	
	pugi::xml_node			m_node;
	std::string				m_nodeName;
	bool					m_nodeLoadedSuccessfully;
};


inline bool XMLNode::isNodeValid() const {
	// Note: Pugi returns the empty string for nodes which were not found during a query
	bool nodeIsValid = ( m_nodeLoadedSuccessfully || ( m_nodeName == "" ) );

	if ( cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
		Console * sharedConsole = Console::getSharedDeveloperConsole();
		cbengine::Vector4<float> textColor;
		std::string consoleText;
		if ( nodeIsValid ) {
			textColor.x = 0.0f;
			textColor.y = 1.0f;
			textColor.z = 0.0f;
			textColor.w = 1.0f;
			consoleText = "The XML Node '";
			consoleText += m_nodeName;
			consoleText += "' is valid.";
			sharedConsole->addTextToConsole( consoleText, textColor );
		} else {
			textColor.x = 1.0f;
			textColor.y = 0.0f;
			textColor.z = 0.0f;
			textColor.w = 1.0f;
			consoleText = "The XML Node '";
			consoleText += m_nodeName;
			consoleText += "' is NOT valid.";
			sharedConsole->addTextToConsole( consoleText, textColor );
		}
	}

	return nodeIsValid;
}


inline bool XMLNode::didNodeLoadSuccessfully() const {
	return m_nodeLoadedSuccessfully;
}


inline void XMLNode::findNodeAttributeWithName( const std::string & attributeName, pugi::xml_attribute & attribute ) const {
	attribute = m_node.attribute( attributeName.c_str() );
}


inline const std::string & XMLNode::getNodeName() const {
	return m_nodeName;
}


inline ptrdiff_t XMLNode::getNodeOffsetFromDocumentStart() const {
	if( !m_nodeLoadedSuccessfully ) {
		return 0;
	} 
	return m_node.offset_debug();
}

#endif