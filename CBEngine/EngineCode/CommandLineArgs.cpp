#include "CommandLineArgs.hpp"

#include <algorithm> // for std::remove

#include "CommandLet.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CommandLineArgs::~CommandLineArgs() {

	freeCommandLetMemory();
}


void CommandLineArgs::freeCommandLetMemory() {

	for ( size_t i = 0; i < m_commandLets.size(); ++i ) {

		CommandLet* commandLetToDelete = m_commandLets[i];
		delete commandLetToDelete;
	}
}


CommandLineArgs::CommandLineArgs( const char* rawCommandLineArgs ) :
	m_rawCommandLineArgs( rawCommandLineArgs ),
	m_areCommandLetsFormed( false )
{

}


void CommandLineArgs::parseCommandLineArgsAndCreateCommandLets() {

	tokenizeRawCommandLineArgs();
	formCommandLets();

	m_areCommandLetsFormed = true;
}


void CommandLineArgs::verifyCommandLetsWereProcessed() {

	for ( size_t i = 0; i < m_commandLets.size(); ++i ) {

		CommandLet* commandLet = m_commandLets[i];
		FATAL_ASSERTION( commandLet != nullptr, "Error: CommandLet value is nullptr." );

		bool wasProcessed = commandLet->isCommandProcessed();

		if ( !wasProcessed ) {

			std::string commandLetNonProcessedErrorString = "Error: A command was not recognized! Command Name: ";
			commandLetNonProcessedErrorString += commandLet->getCommandName();

			FATAL_ERROR( commandLetNonProcessedErrorString.c_str() );
		}
	}
}


void CommandLineArgs::tokenizeRawCommandLineArgs() {

	size_t startIndex = 0;
	size_t endIndex = 0;

	while ( ( endIndex != std::string::npos ) && ( startIndex < m_rawCommandLineArgs.size() ) ) {

		if ( m_rawCommandLineArgs[startIndex] == TEXT_LITERAL ) {

			endIndex = m_rawCommandLineArgs.find( TEXT_LITERAL, ( startIndex + 1 ) );

			// +1 To include the quotation at the end
			endIndex = ( endIndex == std::string::npos ) ? std::string::npos : ( endIndex - startIndex ) + 1; 
			std::string tokenToInsert = m_rawCommandLineArgs.substr( startIndex, endIndex );

			m_commandLineTokens.push_back( tokenToInsert );
		
		} else {

			endIndex = m_rawCommandLineArgs.find( ARGUMENT_DELIMITER, startIndex );

			endIndex = ( endIndex == std::string::npos ) ? std::string::npos : ( endIndex - startIndex );
			std::string tokenToInsert = m_rawCommandLineArgs.substr( startIndex, endIndex );

			if ( tokenToInsert != "" ) {
				// Don't want to push empty strings (This also accomplishes task of removing white space
				m_commandLineTokens.push_back( tokenToInsert );
			}
		}

		startIndex = startIndex + endIndex + SIZE_OF_DELIMITER;
	}
}


void CommandLineArgs::formCommandLets() {

	size_t currentIndex = 0;

	while ( currentIndex < m_commandLineTokens.size() ) {

		const std::string& token = m_commandLineTokens[currentIndex];
		FATAL_ASSERTION( !token.empty(), "Error: An empty token has been detected from the command line arguments. Please enter valid arguments." );

		const char& tokenFirstChar = token[0];
		FATAL_ASSERTION( tokenFirstChar == COMMANDLET_DELIMITER, "Error: No CommandLet commands detected but command line arguments are provided. All CommandLets must begin with the '-' character." );

		if ( tokenFirstChar == COMMANDLET_DELIMITER ) {

			createCommandLetFromTokensWithStartIndex( currentIndex, currentIndex );
		}
	}
}


void CommandLineArgs::createCommandLetFromTokensWithStartIndex( size_t currentIndex, size_t& endingIndex ) {

	const std::string& commandLetCommandWithDash = m_commandLineTokens[currentIndex];
	FATAL_ASSERTION( commandLetCommandWithDash.size() > 1, "Error: CommandLet commands must have a command following the '-' character." );

	std::string commandLetCommandWithoutDash = commandLetCommandWithDash.substr( 1, std::string::npos );
	CommandLet* commandLetToCreate = new CommandLet( commandLetCommandWithoutDash );

	// Gather the associated arguments
	char firstTokenChar = ' ';
	++currentIndex;

	while ( currentIndex < m_commandLineTokens.size() ) {

		const std::string& token = m_commandLineTokens[currentIndex];
		FATAL_ASSERTION( !token.empty(), "Error: An empty token has been detected from the command line arguments. Please enter valid arguments." );

		firstTokenChar = token[0];

		if ( firstTokenChar != COMMANDLET_DELIMITER ) {

			if ( firstTokenChar == TEXT_LITERAL ) {
				// Strip the quotes

				std::string commandLetArgStringNoQuotes = token;
				commandLetArgStringNoQuotes.erase( std::remove( commandLetArgStringNoQuotes.begin(),
					commandLetArgStringNoQuotes.end(),
					TEXT_LITERAL ),
					commandLetArgStringNoQuotes.end() );
			
				commandLetToCreate->addCommandArgument( commandLetArgStringNoQuotes );
			} else {

				commandLetToCreate->addCommandArgument( token );
			}

		} else {
			// Have reached a new command. Pass control back to calling function to parse more commandlets
			break;
		}

		++currentIndex;
	}

	m_commandLets.push_back( commandLetToCreate );
	endingIndex = currentIndex;
}