#include "XMLDocument.hpp"

#include <cassert>
#include <algorithm>
#include <cstdio>

#include "EngineCommon.hpp"
#include "Console.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

XMLDocument::~XMLDocument() {
	if ( m_document ) {
		delete m_document;
	}
} // end dtor


XMLDocument::XMLDocument( const std::string & xmlDocumentFilePath, const std::string & xmlDocumentName ) {

	assert( xmlDocumentName.length() > 0 );
	m_documentLoadedSuccessfully = false;
	m_documentName = xmlDocumentName;
	m_documentFilePath = xmlDocumentFilePath;

	m_document = new pugi::xml_document;
	m_documentParseResult = m_document->load_file( xmlDocumentFilePath.c_str() );

	bool parseSuccessful = false;
	std::string documentParseResultDescription = getParseStatusDescription( parseSuccessful );

	if ( parseSuccessful ) {
		// Check to see if we should log status to console
		if ( cbengine::OUTPUT_IO_STATUS_TO_CONSOLE ) {
			Console * sharedConsole = Console::getSharedDeveloperConsole();
			// TODO:: Have set colors for the levels of warning/success/error messages so it becomes standard
			cbengine::Vector4<float> successColor( 0.20f, 0.90f, 0.10f, 1.0f );
			sharedConsole->addTextToConsole( documentParseResultDescription, successColor );
		}
	} else {
		// Program should terminate with an informative user message
		terminateProgramDueToXMLParseFailureWithErrorMessage( documentParseResultDescription );
	}

	// Used later for line number retrieving
	buildOffsetData( m_documentFilePath.c_str() );

} // end ctor


const std::string XMLDocument::getParseStatusDescription( bool & parseSuccessful ) const {

	if ( m_documentParseResult.status == pugi::status_ok ) {
		std::string documentStatusDescription;
		documentStatusDescription += "XML Document: '";
		documentStatusDescription += m_documentFilePath;
		documentStatusDescription += "' loaded successfully and contains no parsing errors.";
		parseSuccessful = true;
		m_documentLoadedSuccessfully = true;
		return documentStatusDescription;
	}
	
	parseSuccessful = false;
	m_documentLoadedSuccessfully = false;
	std::string errorDescription( m_documentParseResult.description() );
	return errorDescription;
}


void XMLDocument::terminateProgramDueToXMLParseFailureWithErrorMessage( const std::string & errorDescription ) {
	// TODO:: Externalize these hardcoded strings
	std::string parseErrorTitle = "Error Parsing XML Document";
	std::string parseErrorDescription = "XML PARSE ERROR\n\n There was an error parsing the XML document:\n'";
	parseErrorDescription += m_documentName;
	parseErrorDescription += "'\n";
	parseErrorDescription += "Provided file path to XML Document:\n'";
	parseErrorDescription += m_documentFilePath;
	parseErrorDescription += "'\n\n";
	parseErrorDescription += "Error Description:\n";
	parseErrorDescription += errorDescription;
	parseErrorDescription += "\n\n";
	parseErrorDescription += "The Program Will Now Terminate";
	EngineErrorManager errorManager = EngineErrorManager::getEngineErrorManager();
	errorManager.reportFatalErrorAndTerminateProgram( parseErrorTitle, parseErrorDescription );
}


bool XMLDocument::buildOffsetData( const char* file ) {
	// Code taken and modified from http://stackoverflow.com/questions/4814930/get-line-column-of-an-xpath-query-in-pugixml
	
	FILE* f = fopen(file, "rb");
	if (!f) return false;

	ptrdiff_t offset = 0;

	char buffer[1024];
	size_t size;

	while ((size = fread(buffer, 1, sizeof(buffer), f)) > 0)
	{
		for (size_t i = 0; i < size; ++i)
			if (buffer[i] == '\n')
				m_offsetData.push_back(offset + i);

		offset += size;
	}

	fclose(f);

	return true;
}


std::pair<int, int> XMLDocument::getLocationOfLineNumberAndRow( ptrdiff_t offset ) const {
	// Code taken and modified from http://stackoverflow.com/questions/4814930/get-line-column-of-an-xpath-query-in-pugixml
	offset_data_t::const_iterator it = std::lower_bound(m_offsetData.begin(), m_offsetData.end(), offset);
	size_t index = it - m_offsetData.begin();

	return std::make_pair(1 + index, index == 0 ? offset : offset - m_offsetData[index - 1]);
}