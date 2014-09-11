#include "XMLNode.hpp"

#include <cassert>

#include "XMLParser.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

XMLNode::~XMLNode() {

}


XMLNode::XMLNode() {
	m_nodeLoadedSuccessfully = false;
}


XMLNode::XMLNode( const std::string & xmlNodeName ) {
	assert( xmlNodeName.length() > 0 );
	m_nodeName = xmlNodeName;
	m_nodeLoadedSuccessfully = false;
}


XMLNode::XMLNode( pugi::xml_node & node ) {
	m_nodeName = node.name();
	if ( m_nodeName == "" ) {
		// Note:: Pugi returns a node object with an empty string for nodes which were not found
		m_nodeLoadedSuccessfully = false;
	} else {
		m_nodeLoadedSuccessfully = true;
	}
	m_node = node;
}

// Finding and loading nodes
void XMLNode::findNodeAsChildOfNode( XMLNode & parentNode ) {

	parentNode.getChildOfNode( m_nodeName, *(this) );
	
	// Pugi returns an empty object with an empty string to signify when a node isn't found
	m_nodeName = m_node.name();
	if ( m_nodeName == "" ) {
		m_nodeLoadedSuccessfully = false;
	} else {
		m_nodeLoadedSuccessfully = true;
	}

}


// Helper Functions
bool XMLNode::getNextSibling( XMLNode& siblingNode ) const {
	bool siblingFound = false;
	pugi::xml_node sibling = m_node.next_sibling();
	std::string nodeName( sibling.name() );
	if (nodeName == "" ) {
		return siblingFound;
	}
	siblingFound = true;
	siblingNode.m_node = sibling;
	siblingNode.m_nodeName = nodeName;
	siblingNode.m_nodeLoadedSuccessfully = true;
	return siblingFound;
}



void XMLNode::getChildOfNode( const std::string & childNodeName, XMLNode & childNode ) const {
	// Note this will fail silently
	childNode.m_node = m_node.child( childNodeName.c_str() );
	std::string nodeName = childNode.m_node.name();
	if ( !( nodeName == "" ) ) {
		childNode.m_nodeLoadedSuccessfully = true;
		childNode.m_nodeName = childNode.m_node.name();
	}
}


void XMLNode::getListOfAttributeNames( std::vector< std::string > & nodeAttributeNames ) const {
	// Does not modify the vector if the node is invalid
	if ( !m_nodeLoadedSuccessfully ) {
		return;
	}

	// Iterate through all attributes capturing their names as strings
	std::string attribName;
	pugi::xml_node::attribute_iterator itAttrib;
	for ( itAttrib = m_node.attributes_begin(); itAttrib != m_node.attributes_end(); ++itAttrib ) {
		attribName = itAttrib->name();
		nodeAttributeNames.push_back( attribName );
	}

}


void XMLNode::getAllChildrenOfNode( std::vector< XMLNode > & children ) const {
	if ( !m_nodeLoadedSuccessfully ) {
		return;
	}
	// Note: In future this may need to check for duplicate node names...
	pugi::xml_node::iterator itNode;
	for ( itNode = m_node.begin(); itNode != m_node.end(); ++itNode ) {
		XMLNode nodeToAdd( *(itNode) );
		children.push_back( nodeToAdd );
	}

}


