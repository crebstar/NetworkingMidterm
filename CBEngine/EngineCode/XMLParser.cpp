#include "XMLParser.hpp"

#include <vector>
#include <set>
#include <algorithm>

#include "XMLDocument.hpp"
#include "Console.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

XMLParser::~XMLParser() {

}


void XMLParser::loadXMLDocument( const std::string & xmlDocumentFilePath, 
	const std::string & xmlDocumentFileName, 
	bool makeActiveDocument /* = true */ ) 
{
	
	XMLDocument * xmlDocument = new XMLDocument( xmlDocumentFilePath, xmlDocumentFileName );
	
	// At this point the document is valid, otherwise the program would be terminated
	if ( makeActiveDocument ) {
		m_activeDocumentName = xmlDocumentFileName;
		m_activeDocument = xmlDocument;
		
	}

	// No need to check if there is a duplicate insert as there would be no resulting effect
	m_documentCache.insert( std::pair< std::string, XMLDocument * >( m_activeDocumentName, xmlDocument ) );

}


void XMLParser::switchActiveDocument( const std::string & xmlDocumentName ) {
	
	std::map< std::string, XMLDocument * >::iterator it;
	it = m_documentCache.find( xmlDocumentName );
	if ( it != m_documentCache.end() ) {
		// Found
		if ( cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> consoleTextColor( 0.10f, 0.90f, 0.20f, 1.0f );
			std::string consoleText( "Active XML document successfully switched to '" );
			consoleText += xmlDocumentName;
			consoleText += "'";
			sharedConsole->addTextToConsole( consoleText, consoleTextColor );

			m_activeDocumentName = xmlDocumentName;
			m_activeDocument = it->second;
		}
	} else {
		// Not found.. No effect
		if ( cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> consoleTextColor( 0.0f, 1.0f, 1.0f, 1.0f );
			std::string consoleText( "Warning: The XML document named '");
			consoleText += xmlDocumentName;
			consoleText += "' did not exist in the cache. The active document could not be switched.";
			sharedConsole->addTextToConsole( consoleText, consoleTextColor );
		}
	}
}


void XMLParser::releaseDocumentFromCacheWithName( const std::string & xmlDocumentName ) {

	std::map< std::string, XMLDocument * >::iterator it;
	it = m_documentCache.find( xmlDocumentName );
	if ( it != m_documentCache.end() ) {
		// Found
		if ( cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> consoleTextColor( 0.10f, 0.90f, 0.20f, 1.0f );
			std::string consoleText( "XML document successfully released '" );
			consoleText += xmlDocumentName;
			consoleText += "'";
			sharedConsole->addTextToConsole( consoleText, consoleTextColor );

			XMLDocument * xmlDocument = it->second;
			if ( xmlDocument == m_activeDocument ) {
				m_activeDocument = nullptr;
				m_activeDocumentName = "None";
				cbengine::Vector4<float> consoleTextColorWarning( 0.10f, 0.90f, 0.90f, 1.0f );
				std::string activeDocWarning( "Warning: Document released was the currently active document." );
				sharedConsole->addTextToConsole( activeDocWarning, consoleTextColorWarning );
			}
			// Delete and nil the ptr
			delete xmlDocument;
			xmlDocument = nullptr;
			m_documentCache.erase( xmlDocumentName );
		}
	} else {
		// Not found.. No effect
		if ( cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> consoleTextColor( 0.0f, 1.0f, 1.0f, 1.0f );
			std::string consoleText( "Warning: The XML document named '");
			consoleText += xmlDocumentName;
			consoleText += "' did not exist in the cache. The active document could not be released.";
			sharedConsole->addTextToConsole( consoleText, consoleTextColor );

		}
	}
}


void XMLParser::releaseAllDocumentsFromCache() {
	// Ensure there are no memory leaks
	std::map< std::string, XMLDocument * >::iterator it;
	for ( it = m_documentCache.begin(); it != m_documentCache.end(); ++it ) {
		delete it->second;
		it->second = nullptr;
	}
	// Clear all elements from the map
	m_documentCache.clear();

	// Make sure active references point to nothing
	m_activeDocument = nullptr;
	m_activeDocumentName = "None";

	if ( cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
		Console * sharedConsole = Console::getSharedDeveloperConsole();
		cbengine::Vector4<float> consoleTextColor( 0.0f, 1.0f, 0.0f, 1.0f );
		std::string consoleText( "All XML documents have been removed from the document cache.");
		sharedConsole->addTextToConsole( consoleText, consoleTextColor );
	}	
}


// Non inline mutators
XMLNode XMLParser::getActiveDocumentRootNode() const {
	if ( m_activeDocument == nullptr ) {
		XMLNode nodeNotFound( "NotFound" );
		return nodeNotFound;
	} else {
		return m_activeDocument->getRootNodeOfDocument();
	}
}


bool XMLParser::checkForCaseInsensitiveChildNodeMatch( const XMLNode & xmlNode,
	const std::vector< XMLNode > & childNodes,
	const std::string & childNodeName,
	std::string & correctNodeName ) 
{
	UNUSED(xmlNode);
	bool caseInsensitiveMatch = false;
	if ( childNodes.empty() ) {
		return false;
	}

	std::string nodeNameLowerCase( childNodeName );
	std::transform( nodeNameLowerCase.begin(), nodeNameLowerCase.end(), nodeNameLowerCase.begin(), ::tolower );

	for ( size_t i = 0; i < childNodes.size(); ++i ) {
		std::string nodeNameFromListAsLower = childNodes[i].getNodeName();
		std::transform( nodeNameFromListAsLower.begin(), nodeNameFromListAsLower.end(), nodeNameFromListAsLower.begin(), ::tolower );
		if ( nodeNameFromListAsLower == nodeNameLowerCase ) {
			caseInsensitiveMatch = true;
			correctNodeName = childNodes[i].getNodeName();
			break;
		}
	}
	return caseInsensitiveMatch;
}


bool XMLParser::checkForCaseInsensitiveAttributeMatch( const XMLNode & xmlNode,
	const std::string & attributeName,
	std::string & correctAttribName ) 
{
	// User validation function which checks to see if there was a case mismatch. If so, it terminates the program and informs the user
	bool caseInsensitiveMatch = false;
	std::vector<std::string> attributeList;
	xmlNode.getListOfAttributeNames( attributeList );

	if ( attributeList.empty() ) {
		return false;
	}

	std::string attribueNameLowerCase( attributeName );
	std::transform( attribueNameLowerCase.begin(), attribueNameLowerCase.end(), attribueNameLowerCase.begin(), ::tolower );

	// Transform each string in the attribute list to lower case string
	for ( size_t i = 0; i < attributeList.size(); ++i ) {
		std::string attribNameFromListAsLower = attributeList[i];
		std::transform( attribNameFromListAsLower.begin(), attribNameFromListAsLower.end(), attribNameFromListAsLower.begin(), ::tolower );
		if ( attribNameFromListAsLower == attribueNameLowerCase ) {
			if ( attributeName != attributeList[i] ) {
				caseInsensitiveMatch = true;
				correctAttribName = attributeList[i];
				break;
			} // end inner if
		} // end outer if
	} // end for

	return caseInsensitiveMatch;
}


// Validation Functions
void XMLParser::validateXMLAttributes( const XMLNode & xmlNode,
	const std::string & mandatoryAttribsCommaDelimited,
	const std::string & optionalAttribsCommaDelimited,
	bool outputWarningsToConsole /* = true */ )
{
	/* Note: optional parameters missing are considered warnings */
	if ( mandatoryAttribsCommaDelimited.length() == 0 ) {
		if ( outputWarningsToConsole && cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			std::string warningString = "Warning: Mandatory Attribute string provided is an empty string. Cannot validate Mandatory XML attributes.";
			cbengine::Vector4<float> warningColor( 0.0f, 1.0f, 1.0f, 1.0f );
			sharedConsole->addTextToConsole( warningString, warningColor );
		}
	}

	std::vector<std::string> mandatoryStringsVec;
	std::vector<std::string> optionalStringsVec;
	cbengine::convertDelimitedStringToVectorOfStrings( mandatoryAttribsCommaDelimited, XMLSchemaDelimiter, mandatoryStringsVec );
	cbengine::convertDelimitedStringToVectorOfStrings( optionalAttribsCommaDelimited, XMLSchemaDelimiter, optionalStringsVec );

	// First Verify Mandatory
	std::vector<std::string> listOfAttributes;
	xmlNode.getListOfAttributeNames( listOfAttributes );
	std::vector<std::string> mandatoryAttribsFound;
	for ( size_t i = 0; i < mandatoryStringsVec.size(); ++i ) {
		bool matchFound = false;
		for ( size_t j = 0; j < listOfAttributes.size(); ++j ) {
			if ( mandatoryStringsVec[i] == listOfAttributes[j] ) {
				matchFound = true;
				mandatoryAttribsFound.push_back( listOfAttributes[j] );
			}
		} // end inner for
		if ( !matchFound ) {
			// Mandatory Attrib is missing
			std::string caseMismatchString;
			bool caseMismatchFound = checkForCaseInsensitiveAttributeMatch( xmlNode, mandatoryStringsVec[i], caseMismatchString );
			if ( caseMismatchFound ) {
				std::string additionalErrorMsgContents = "This error was triggered from function validateXMLAttributes";
				triggerCaseInsensitiveAttributeErrorAndDie( *(m_activeDocument), xmlNode, mandatoryStringsVec[i], caseMismatchString, additionalErrorMsgContents );
			}
			triggerMandatoryAttribMissingErrorAndDie( *(m_activeDocument), xmlNode, mandatoryStringsVec, listOfAttributes, mandatoryStringsVec[i] );
		}
	} // end outer for

	// Verify Optional
	std::vector<std::string> optionalAttribsNotFound;
	for ( size_t i = 0; i < optionalStringsVec.size(); ++i ) {
		bool matchFound = false;
		for ( size_t j = 0; j < listOfAttributes.size(); ++j ) {
			if ( optionalStringsVec[i] == listOfAttributes[j] ) {
				matchFound = true;
			}
		} // end inner for

		// Second check because I don't want to report warnings for strings covered in the mandatory list
		for ( size_t j = 0; j < mandatoryAttribsFound.size(); ++j ) {
			if ( optionalStringsVec[i] == mandatoryAttribsFound[j] ) {
				matchFound = true;
			}
		}

		if ( !matchFound ) {
			optionalAttribsNotFound.push_back( optionalStringsVec[i] );
		}
	} // end outer for

	// Ask professor for best practices on reporting the optional cases
	if ( !optionalAttribsNotFound.empty() ) {
		if ( outputWarningsToConsole && cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> warningColor( 0.0f, 1.0f, 1.0f, 1.0f );
			std::string warningString = "Warning: Some optional parameters were not found for XML Node '";
			warningString += xmlNode.getNodeName();
			warningString += "'\n";
			sharedConsole->addTextToConsole( warningString, warningColor );
			warningString.clear();
			warningString += "List of optional attributes not found: ";
			for ( size_t i = 0; i < optionalAttribsNotFound.size(); ++i ) {
				warningString += "'";
				warningString += optionalAttribsNotFound[i];
				warningString += "' ";
			}
			sharedConsole->addTextToConsole( warningString, warningColor );
		} // end inner if
	} // end outer if

	// Lastly check if there were attributes on the node not found in either list
	// if one was found then this should trigger an error
	std::vector<std::string> attributesNotFound;
	for ( size_t i = 0; i < listOfAttributes.size(); ++i ) {
		bool matchFound = false;
		for ( size_t j = 0; j < mandatoryStringsVec.size(); ++j ) {
			if ( listOfAttributes[i] == mandatoryStringsVec[j] ) {
				matchFound = true;
			}
		} // end inner for
		
		for ( size_t j = 0; j < optionalStringsVec.size(); ++j ) {
			if ( listOfAttributes[i] == optionalStringsVec[j] ) {
				matchFound = true;
			}
		} // end inner for
		if ( !matchFound ) {
			attributesNotFound.push_back( listOfAttributes[i] );
		}

	} // end outer for

	if ( !attributesNotFound.empty() ) {
		if ( outputWarningsToConsole && cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> warningColor( 0.0f, 1.0f, 1.0f, 1.0f );
			std::string warningString = "Warning: Some parameters found for XML Node: '";
			warningString += xmlNode.getNodeName();
			warningString += "'\n";
			sharedConsole->addTextToConsole( warningString, warningColor );
			warningString.clear();
			warningString += "that were not found in either mandatory or optional string lists";
			sharedConsole->addTextToConsole( warningString, warningColor );
			warningString.clear();
			warningString += "List of non-matching attributes found: ";
			for ( size_t i = 0; i < attributesNotFound.size(); ++i ) {
				warningString += "'";
				warningString += attributesNotFound[i];
				warningString += "' ";
			}
			sharedConsole->addTextToConsole( warningString, warningColor );
			// Trigger an error
			triggerNameNotFoundFromValidationErrorAndDie( *(m_activeDocument), xmlNode, mandatoryStringsVec, optionalStringsVec, listOfAttributes, attributesNotFound, true );

		} // end inner if
	}
}


void XMLParser::validateXMLChildNodes( const XMLNode & xmlNode,
	const std::string & mandatoryChildNodesCommaDelimited,
	const std::string & optionalChildNodesCommaDelimited,
	bool outputWarningsToConsole ) 
{
	/* Note: optional parameters missing are considered warnings */
	if ( mandatoryChildNodesCommaDelimited.length() == 0 ) {
		if ( outputWarningsToConsole && cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			std::string warningString = "Warning: Mandatory Child Node string provided is an empty string. Cannot validate Mandatory XML Nodes.";
			cbengine::Vector4<float> warningColor( 0.0f, 1.0f, 1.0f, 1.0f );
			sharedConsole->addTextToConsole( warningString, warningColor );
		}
	}

	std::vector<std::string> mandatoryStringsVec;
	std::vector<std::string> optionalStringsVec;
	cbengine::convertDelimitedStringToVectorOfStrings( mandatoryChildNodesCommaDelimited, XMLSchemaDelimiter, mandatoryStringsVec );
	cbengine::convertDelimitedStringToVectorOfStrings( optionalChildNodesCommaDelimited, XMLSchemaDelimiter, optionalStringsVec );

	// First Verify Mandatory
	std::vector< XMLNode > listOfChildNodes;
	
	xmlNode.getAllChildrenOfNode( listOfChildNodes );
	
	std::vector<std::string> mandatoryChildNodesFound;
	for ( size_t i = 0; i < mandatoryStringsVec.size(); ++i ) {
		bool matchFound = false;
		for ( size_t j = 0; j < listOfChildNodes.size(); ++j ) {
			if ( mandatoryStringsVec[i] == listOfChildNodes[j].getNodeName() ) {
				matchFound = true;
				mandatoryChildNodesFound.push_back( listOfChildNodes[j].getNodeName() );
			}
		} // end inner for
		if ( !matchFound ) {
			// Mandatory Attrib is missing
			
			std::string caseMismatchString;
			bool caseMismatchFound = checkForCaseInsensitiveChildNodeMatch( xmlNode, listOfChildNodes, mandatoryStringsVec[i], caseMismatchString );
			if ( caseMismatchFound ) {
				std::string additionalErrorMsgContents = "This error was triggered from function ";
				triggerCaseInsensitiveChildNodeNameErrorAndDie( *(m_activeDocument), xmlNode, mandatoryStringsVec[i], caseMismatchString, additionalErrorMsgContents );
			}
			
			// Ugh crappy way of removing duplicates
			// But it is possible for there to be many duplicate children which would clog error message box
			std::vector<std::string> nodeListNoDuplicates;
			std::set<std::string> nodeSetNoDuplicates;
			for ( size_t k = 0; k < listOfChildNodes.size(); ++k ) {
				std::string nodeName = listOfChildNodes[k].getNodeName();
				nodeSetNoDuplicates.insert( nodeName );
			}
			std::set<std::string>::iterator it;
			for ( it = nodeSetNoDuplicates.begin(); it != nodeSetNoDuplicates.end(); ++it ) {
				nodeListNoDuplicates.push_back( *(it) );
			}
			triggerMandatoryChildNodeMissingErrorAndDie( *(m_activeDocument), xmlNode, mandatoryStringsVec, nodeListNoDuplicates, mandatoryStringsVec[i] );
			
		}
	} // end outer for

	// Verify Optional
	
	std::vector<std::string> optionalNodeNamesNotFound;
	for ( size_t i = 0; i < optionalStringsVec.size(); ++i ) {
		bool matchFound = false;
		for ( size_t j = 0; j < listOfChildNodes.size(); ++j ) {
			if ( optionalStringsVec[i] == listOfChildNodes[j].getNodeName() ) {
				matchFound = true;
			}
		} // end inner for

		// Second check because I don't want to report warnings for strings covered in the mandatory list
		for ( size_t j = 0; j < mandatoryChildNodesFound.size(); ++j ) {
			if ( optionalStringsVec[i] == mandatoryChildNodesFound[j] ) {
				matchFound = true;
			}
		}

		if ( !matchFound ) {
			optionalNodeNamesNotFound.push_back( optionalStringsVec[i] );
		}
	} // end outer for

	// Ask professor for best practices on reporting the optional cases
	if ( !optionalNodeNamesNotFound.empty() ) {
		if ( outputWarningsToConsole && cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> warningColor( 0.0f, 1.0f, 1.0f, 1.0f );
			std::string warningString = "Warning: Some optional child node names were not found for Parent Node '";
			warningString += xmlNode.getNodeName();
			warningString += "'\n";
			sharedConsole->addTextToConsole( warningString, warningColor );
			warningString.clear();
			warningString += "List of optional node names not found: ";
			for ( size_t i = 0; i < optionalNodeNamesNotFound.size(); ++i ) {
				warningString += "'";
				warningString += optionalNodeNamesNotFound[i];
				warningString += "' ";
			}
			sharedConsole->addTextToConsole( warningString, warningColor );
		} // end inner if
	} // end outer if

	// Lastly check if there were attributes on the node not found in either list
	std::vector<std::string> nodeNamesNotFound;
	for ( size_t i = 0; i < listOfChildNodes.size(); ++i ) {
		bool matchFound = false;
		for ( size_t j = 0; j < mandatoryStringsVec.size(); ++j ) {
			if ( listOfChildNodes[i].getNodeName() == mandatoryStringsVec[j] ) {
				matchFound = true;
			}
		} // end inner for

		for ( size_t j = 0; j < optionalStringsVec.size(); ++j ) {
			if ( listOfChildNodes[i].getNodeName() == optionalStringsVec[j] ) {
				matchFound = true;
			}
		} // end inner for
		if ( !matchFound ) {
			nodeNamesNotFound.push_back( listOfChildNodes[i].getNodeName() );
		}

	} // end outer for

	if ( !nodeNamesNotFound.empty() ) {
		if ( outputWarningsToConsole && cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			cbengine::Vector4<float> warningColor( 0.0f, 1.0f, 1.0f, 1.0f );
			std::string warningString = "Warning: Some child node names found for Parent Node: '";
			warningString += xmlNode.getNodeName();
			warningString += "'\n";
			sharedConsole->addTextToConsole( warningString, warningColor );
			warningString.clear();
			warningString += "that were not found in either mandatory or optional string lists";
			sharedConsole->addTextToConsole( warningString, warningColor );
			warningString.clear();
			warningString += "List of non-matching child node names found: ";
			for ( size_t i = 0; i < nodeNamesNotFound.size(); ++i ) {
				warningString += "'";
				warningString += nodeNamesNotFound[i];
				warningString += "' ";
			}
			sharedConsole->addTextToConsole( warningString, warningColor );
			std::vector<std::string> nodeListNoDuplicates;
			std::set<std::string> nodeSetNoDuplicates;
			for ( size_t k = 0; k < listOfChildNodes.size(); ++k ) {
				std::string nodeName = listOfChildNodes[k].getNodeName();
				nodeSetNoDuplicates.insert( nodeName );
			}
			std::set<std::string>::iterator it;
			for ( it = nodeSetNoDuplicates.begin(); it != nodeSetNoDuplicates.end(); ++it ) {
				nodeListNoDuplicates.push_back( *(it) );
			}
			triggerNameNotFoundFromValidationErrorAndDie( *(m_activeDocument), xmlNode, mandatoryStringsVec, optionalStringsVec, nodeListNoDuplicates, nodeNamesNotFound, false );
		} // end inner if
	}
}


// Error Reporting Functions
void XMLParser::triggerMandatoryChildNodeMissingErrorAndDie( const XMLDocument & xmlDocument,
	const XMLNode & xmlNode,
	const std::vector<std::string> & mandatoryList,
	const std::vector<std::string> & actualNodeList,
	const std::string & mandatoryChildNodeNameNotFound ) 
{
	UNUSED( mandatoryList );
	std::string errorTitle( "Error - Mandatory Child Node Not Found" );
	std::string errorMessage;

	ptrdiff_t offsetNum = xmlNode.getNodeOffsetFromDocumentStart();
	std::pair<int,int> colRowInfo = xmlDocument.getLocationOfLineNumberAndRow( offsetNum );
	std::string colNumAsString = std::to_string( static_cast<LONGLONG>( colRowInfo.first ) );
	std::string rowNumAsStrong = std::to_string( static_cast<LONGLONG>( colRowInfo.second ) );

	errorMessage += "Error: The mandatory child node name provided:\n'";
	errorMessage += mandatoryChildNodeNameNotFound;
	errorMessage += "'\n";
	errorMessage +=	"Could not be found in:\n'";
	errorMessage += xmlDocument.getDocumentName();
	errorMessage += "'\n";
	errorMessage += "XML Parent Node Name:\n'";
	errorMessage += xmlNode.getNodeName();
	errorMessage += "'\n";

	// Column/Row Information
	errorMessage += "Parent Line Number: ";
	errorMessage += colNumAsString;
	errorMessage += " Parent Row Number: ";
	errorMessage += rowNumAsStrong;
	errorMessage += "\n\n";

	errorMessage += "The following is a list of all the node's children:\n";
	for ( size_t i = 0; i < actualNodeList.size(); ++i ) {
		errorMessage += "'";
		errorMessage += actualNodeList[i];
		errorMessage += "'\n";
	}

	errorMessage += "\n";
	errorMessage += "Please ensure the mandatory child node is provided or there is not a case mismatch.\n\n";

	errorMessage += "The program will now terminate.";

	std::string consoleOutputString;
	//consoleOutputString += "c:\\users\\aw 18\\dropbox\\guildhall\\mod d\\sd iv\\ladybug\\ladybug\\";
	consoleOutputString += xmlDocument.getDocumentFilePath();
	consoleOutputString += "(";
	consoleOutputString += colNumAsString;
	consoleOutputString += "): ";
	consoleOutputString += errorTitle;

	EngineErrorManager & eem = EngineErrorManager::getEngineErrorManager();
	eem.outputErrorMessageToConsole( consoleOutputString );
	eem.reportFatalErrorAndTerminateProgram( errorTitle, errorMessage );

}


void XMLParser::triggerNameNotFoundFromValidationErrorAndDie( const XMLDocument & xmlDocument,
	const XMLNode & xmlNode,
	const std::vector<std::string> & mandatoryList,
	const std::vector<std::string> & optionalList,
	const std::vector<std::string> & actualNodeList,
	const std::vector<std::string> & namesNotFound,
	bool isAttribute,
	const std::string & additionalErrorMessageContent ) 
{
	UNUSED(optionalList);
	UNUSED(mandatoryList);
	std::string errorTitle;
	std::string errorMessage;

	if ( isAttribute ) {
		errorTitle = "Error - Attribute Not In Validation List";
	} else {
		errorTitle = "Error - Child Node Not In Validation List";
	}

	ptrdiff_t offsetNum = xmlNode.getNodeOffsetFromDocumentStart();
	std::pair<int,int> colRowInfo = xmlDocument.getLocationOfLineNumberAndRow( offsetNum );
	std::string colNumAsString = std::to_string( static_cast<LONGLONG>( colRowInfo.first ) );
	std::string rowNumAsStrong = std::to_string( static_cast<LONGLONG>( colRowInfo.second ) );

	errorMessage += "Error: The node names below do not match the validation list:\n";
	for ( size_t i = 0; i < namesNotFound.size(); ++i ) {
		errorMessage += "'";
		errorMessage += namesNotFound[i];
		errorMessage += "'\n";
	}

	errorMessage += "'\n";
	errorMessage +=	"Document File Path:\n'";
	errorMessage += xmlDocument.getDocumentFilePath();
	errorMessage += "'\n";

	if ( isAttribute ) {
		errorMessage += "XML Node Name:\n'";
		errorMessage += xmlNode.getNodeName();
		errorMessage += "'\n";

		// Column/Row Information
		errorMessage += "Line Number: ";
		errorMessage += colNumAsString;
		errorMessage += " Row Number: ";
		errorMessage += rowNumAsStrong;
		errorMessage += "\n\n";

		errorMessage += "The following is a list of all the node's attributes:\n";
		for ( size_t i = 0; i < actualNodeList.size(); ++i ) {
			errorMessage += "'";
			errorMessage += actualNodeList[i];
			errorMessage += "'\n";
		}
	} else {
		errorMessage += "XML Parent Node Name:\n'";
		errorMessage += xmlNode.getNodeName();
		errorMessage += "'\n";

		// Column/Row Information
		errorMessage += "Parent Line Number: ";
		errorMessage += colNumAsString;
		errorMessage += " Parent Row Number: ";
		errorMessage += rowNumAsStrong;
		errorMessage += "\n\n";

		errorMessage += "The following is a list of all the node's children:\n";
		for ( size_t i = 0; i < actualNodeList.size(); ++i ) {
			errorMessage += "'";
			errorMessage += actualNodeList[i];
			errorMessage += "'\n";
		}
	}
	
	errorMessage += "\n";
	errorMessage += "Please ensure the attributes/child nodes match the required schema.\n\n";
	errorMessage += additionalErrorMessageContent;

	errorMessage += "The program will now terminate.";

	std::string consoleOutputString;
	consoleOutputString += xmlDocument.getDocumentFilePath();
	consoleOutputString += "(";
	consoleOutputString += colNumAsString;
	consoleOutputString += "): ";
	consoleOutputString += errorTitle;

	EngineErrorManager & eem = EngineErrorManager::getEngineErrorManager();
	eem.outputErrorMessageToConsole( consoleOutputString );
	eem.reportFatalErrorAndTerminateProgram( errorTitle, errorMessage );

}

void XMLParser::triggerMandatoryAttribMissingErrorAndDie( const XMLDocument & xmlDocument,
	const XMLNode & xmlNode,
	const std::vector<std::string> & mandatoryList,
	const std::vector<std::string> & nodeAttribList,
	const std::string & mandatoryAttribNotFound )
{
	UNUSED( mandatoryList );
	std::string errorTitle( "Error - Mandatory Attribute Not Found" );
	std::string errorMessage;

	ptrdiff_t offsetNum = xmlNode.getNodeOffsetFromDocumentStart();
	std::pair<int,int> colRowInfo = xmlDocument.getLocationOfLineNumberAndRow( offsetNum );
	std::string colNumAsString = std::to_string( static_cast<LONGLONG>( colRowInfo.first ) );
	std::string rowNumAsStrong = std::to_string( static_cast<LONGLONG>( colRowInfo.second ) );

	errorMessage += "Error: The mandatory attribute name provided:\n'";
	errorMessage += mandatoryAttribNotFound;
	errorMessage += "'\n";
	errorMessage +=	"Could not be found in:\n'";
	errorMessage += xmlDocument.getDocumentName();
	errorMessage += "'\n";
	errorMessage += "XML Node Name:\n'";
	errorMessage += xmlNode.getNodeName();
	errorMessage += "'\n";

	// Column/Row Information
	errorMessage += "Line Number: ";
	errorMessage += colNumAsString;
	errorMessage += " Row Number: ";
	errorMessage += rowNumAsStrong;
	errorMessage += "\n\n";

	errorMessage += "The following is a list of all the node's attributes:\n";
	for ( size_t i = 0; i < nodeAttribList.size(); ++i ) {
		errorMessage += "'";
		errorMessage += nodeAttribList[i];
		errorMessage += "'\n";
	}

	errorMessage += "\n";
	errorMessage += "Please ensure the mandatory attribute is provided or there is not a case mismatch.\n\n";

	errorMessage += "The program will now terminate.";

	std::string consoleOutputString;
	//consoleOutputString += "c:\\users\\aw 18\\dropbox\\guildhall\\mod d\\sd iv\\ladybug\\ladybug\\";
	consoleOutputString += xmlDocument.getDocumentFilePath();
	consoleOutputString += "(";
	consoleOutputString += colNumAsString;
	consoleOutputString += "): ";
	consoleOutputString += errorTitle;

	EngineErrorManager & eem = EngineErrorManager::getEngineErrorManager();
	eem.outputErrorMessageToConsole( consoleOutputString );
	eem.reportFatalErrorAndTerminateProgram( errorTitle, errorMessage );
}


void XMLParser::triggerCaseInsensitiveChildNodeNameErrorAndDie( const XMLDocument & xmlDocument,
	const XMLNode & xmlNode,
	const std::string & nodeNameProvided,
	const std::string & correctNodeName,
	const std::string & additionalErrorMessageContent ) 
{
	std::string errorTitle( "Error - Child Node Name Case Mismatch" );
	std::string errorMessage;

	ptrdiff_t offsetNum = xmlNode.getNodeOffsetFromDocumentStart();
	std::pair<int,int> colRowInfo = xmlDocument.getLocationOfLineNumberAndRow( offsetNum );
	std::string colNumAsString = std::to_string( static_cast<LONGLONG>( colRowInfo.first ) );
	std::string rowNumAsStrong = std::to_string( static_cast<LONGLONG>( colRowInfo.second ) );

	errorMessage += "Error: The child node name provided:\n'";
	errorMessage += nodeNameProvided;
	errorMessage += "'\n";
	errorMessage +=	"Could not be found in:\n'";
	errorMessage += xmlDocument.getDocumentName();
	errorMessage += "'\n";
	errorMessage += "XML Parent Node Name:\n'";
	errorMessage += xmlNode.getNodeName();
	errorMessage += "'\n";
	
	// Column/Row Information
	errorMessage += "Parent Node Line Number: ";
	errorMessage += colNumAsString;
	errorMessage += " Parent Node Row Number: ";
	errorMessage += rowNumAsStrong;
	errorMessage += "\n\n";

	errorMessage += "A case insensitive child node match has been detected with name:\n'";
	errorMessage += correctNodeName;
	errorMessage += "'\n";
	errorMessage += "Did you mean to provide this child node name?\n\n";

	errorMessage += additionalErrorMessageContent;

	errorMessage += "\n\nThe program will now terminate.";

	std::string consoleOutputString;
	consoleOutputString += xmlDocument.getDocumentFilePath();
	consoleOutputString += "(";
	consoleOutputString += colNumAsString;
	consoleOutputString += "): ";
	consoleOutputString += errorTitle;

	EngineErrorManager & eem = EngineErrorManager::getEngineErrorManager();
	eem.outputErrorMessageToConsole( consoleOutputString );
	eem.reportFatalErrorAndTerminateProgram( errorTitle, errorMessage );

}


void XMLParser::triggerCaseInsensitiveAttributeErrorAndDie( const XMLDocument & xmlDocument,
	const XMLNode & xmlNode,
	const std::string & attribNameProvided,
	const std::string & correctAttribName,
	const std::string & additionalErrorMessageContent ) 
{

	std::string errorTitle( "Error - Attribute Case Mismatch" );
	std::string errorMessage;

	ptrdiff_t offsetNum = xmlNode.getNodeOffsetFromDocumentStart();
	std::pair<int,int> colRowInfo = xmlDocument.getLocationOfLineNumberAndRow( offsetNum );
	std::string colNumAsString = std::to_string( static_cast<LONGLONG>( colRowInfo.first ) );
	std::string rowNumAsStrong = std::to_string( static_cast<LONGLONG>( colRowInfo.second ) );

	errorMessage += "Error: The attribute name provided:\n'";
	errorMessage += attribNameProvided;
	errorMessage += "'\n";
	errorMessage +=	"Could not be found in:\n'";
	errorMessage += xmlDocument.getDocumentFilePath();
	errorMessage += "'\n";
	errorMessage += "XML Node Name:\n'";
	errorMessage += xmlNode.getNodeName();
	errorMessage += "'\n";

	// Column/Row Information
	errorMessage += "Line Number: ";
	errorMessage += colNumAsString;
	errorMessage += " Row Number: ";
	errorMessage += rowNumAsStrong;
	errorMessage += "\n\n";

	errorMessage += "A case insensitive attribute match has been detected with name:\n'";
	errorMessage += correctAttribName;
	errorMessage += "'\n";
	errorMessage += "Did you mean to provide this attribute name?\n\n";

	errorMessage += additionalErrorMessageContent;

	errorMessage += "\n\nThe program will now terminate.";

	std::string consoleOutputString;
	consoleOutputString += xmlDocument.getDocumentFilePath();
	consoleOutputString += "(";
	consoleOutputString += colNumAsString;
	consoleOutputString += "): ";
	consoleOutputString += errorTitle;

	EngineErrorManager & eem = EngineErrorManager::getEngineErrorManager();
	eem.outputErrorMessageToConsole( consoleOutputString );
	eem.reportFatalErrorAndTerminateProgram( errorTitle, errorMessage );

}

// TODO:: Externalize these error strings to a file
void XMLParser::triggerContainerParseErrorDueToSizeAndDie( const XMLDocument & xmlDocument, 
	const XMLNode & xmlNode, 
	const std::string attribNameProvided, 
	const std::vector< std::string > & attribContents, 
	const std::string & containerType,
	size_t sizeFound, 
	size_t sizeExpected ) 
{
	std::string errorTitle( "Error - Container Size Mismatch" );
	std::string errorMessage;

	ptrdiff_t offsetNum = xmlNode.getNodeOffsetFromDocumentStart();
	std::pair<int,int> colRowInfo = xmlDocument.getLocationOfLineNumberAndRow( offsetNum );
	std::string colNumAsString = std::to_string( static_cast<LONGLONG>( colRowInfo.first ) );
	std::string rowNumAsStrong = std::to_string( static_cast<LONGLONG>( colRowInfo.second ) );

	errorMessage += "Error: The attribute name provided:\n'";
	errorMessage += attribNameProvided;
	errorMessage += "'\n";
	errorMessage +=	"Found in document:\n'";
	errorMessage += xmlDocument.getDocumentName();
	errorMessage += "'\n";
	errorMessage += "XML Node Name:\n'";
	errorMessage += xmlNode.getNodeName();
	errorMessage += "'\n";

	// Column/Row Information
	errorMessage += "Line Number: ";
	errorMessage += colNumAsString;
	errorMessage += " Row Number: ";
	errorMessage += rowNumAsStrong;
	errorMessage += "\n\n";

	errorMessage += "Was parsed and found to contain ";
	errorMessage += std::to_string( static_cast<_ULonglong>(sizeFound) );
	errorMessage += " elements delimited by '";
	errorMessage += XMLSchemaDelimiter;
	errorMessage += "'\n";
	errorMessage += "The expected amount of elements is ";
	errorMessage += std::to_string( static_cast<_ULonglong>(sizeExpected) );
	errorMessage += " for the container type: '";
	errorMessage += containerType;
	errorMessage += "'\n\n";
	errorMessage += "The following is the list of strings which were parsed from the attribute:\n";
	for ( size_t i = 0; i < attribContents.size(); ++i ) {
		errorMessage += "'";
		errorMessage += attribContents[i];
		errorMessage += "'\n";
	}

	errorMessage += "Please ensure the delimiter is a ',' or the correct amount of elements is provided for the container type.\n\n";
	errorMessage += "The program will now terminate.";
	
	std::string consoleOutputString;
	consoleOutputString += xmlDocument.getDocumentFilePath();
	consoleOutputString += "(";
	consoleOutputString += colNumAsString;
	consoleOutputString += "): ";
	consoleOutputString += errorTitle;

	EngineErrorManager & eem = EngineErrorManager::getEngineErrorManager();
	eem.outputErrorMessageToConsole( consoleOutputString );
	eem.reportFatalErrorAndTerminateProgram( errorTitle, errorMessage );


}