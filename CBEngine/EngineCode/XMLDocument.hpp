#ifndef included_XMLDocument
#define included_XMLDocument
#pragma once

#include <string>

#include "pugixml.hpp"
#include "pugiconfig.hpp"

#include "XMLNode.hpp"

/*
	Class which encapsulates the pugi xml document 
*/

typedef std::vector<ptrdiff_t> offset_data_t;

class XMLDocument {
public:

	~XMLDocument();
	explicit XMLDocument( const std::string & xmlDocumentFilePath, const std::string & xmlDocumentName );

	// Core Functions
	const std::string getParseStatusDescription( bool & parseSuccessful ) const;
	std::pair<int, int> getLocationOfLineNumberAndRow( ptrdiff_t offset ) const;

	// Inline Mutators
	const std::string & getDocumentName() const;
	const std::string & getDocumentFilePath() const;
	XMLNode getRootNodeOfDocument() const;

protected:

	// For debugging purposes.. Builds an index of column/row numbers
	bool buildOffsetData( const char* file );
	
private:

	void terminateProgramDueToXMLParseFailureWithErrorMessage( const std::string & errorDescription );

	// Ivars
	pugi::xml_document *				m_document;
	std::string							m_documentName;
	std::string							m_documentFilePath;
	pugi::xml_parse_result				m_documentParseResult;
	mutable bool						m_documentLoadedSuccessfully;

	offset_data_t						m_offsetData;
};

// Inline Mutator Definitions
inline const std::string & XMLDocument::getDocumentName() const {
	return m_documentName;
}


inline XMLNode XMLDocument::getRootNodeOfDocument() const {
	// Construct an XMLNode with the root node of the document
	pugi::xml_node rootNode = m_document->first_child();
	XMLNode rootXMLNode( rootNode );
	return rootXMLNode;
}


inline const std::string & XMLDocument::getDocumentFilePath() const {
	return m_documentFilePath;
}


#endif