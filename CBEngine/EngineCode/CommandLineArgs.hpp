#ifndef included_CommandLineArgs
#define included_CommandLineArgs
#pragma once

#include <vector>
#include <string>

#include "EngineMacros.hpp"
#include "EngineErrorManager.hpp"


const size_t SIZE_OF_DELIMITER = 1;
const char COMMANDLET_DELIMITER = '-';
const char ARGUMENT_DELIMITER = ' ';
const char TEXT_LITERAL = '\"';

class CommandLet;

class CommandLineArgs {
public:
	~CommandLineArgs();
	CommandLineArgs( const char* rawCommandLineArgs );

	void parseCommandLineArgsAndCreateCommandLets();
	void verifyCommandLetsWereProcessed();

	const std::string& getRawCommandLineArgs() const;
	const std::vector<std::string>& getCommandLineTokens() const;
	const std::vector<CommandLet*>& getCommandLets() const;

protected:

	void tokenizeRawCommandLineArgs();
	void formCommandLets();
	void createCommandLetFromTokensWithStartIndex( size_t currentIndex, size_t& endingIndex );

private:
	PREVENT_COPY_AND_ASSIGN( CommandLineArgs );

	void freeCommandLetMemory();

	std::string						m_rawCommandLineArgs;
	std::vector<std::string>		m_commandLineTokens;
	std::vector<CommandLet*>		m_commandLets;
	mutable bool					m_areCommandLetsFormed;
};

// Inline Functions
inline const std::string& CommandLineArgs::getRawCommandLineArgs() const {

	return m_rawCommandLineArgs;
}


inline const std::vector<std::string>& CommandLineArgs::getCommandLineTokens() const {

	return m_commandLineTokens;
}


// TODO:: Show warning if they are formed
inline const std::vector<CommandLet*>& CommandLineArgs::getCommandLets() const {

	RECOVERABLE_GUARANTEE( m_areCommandLetsFormed, "Warning: CommandLets have not been formed and will be empty. No commands will able to be processed. Please ensure CommandLets are formed before attempting to process them." );

	return m_commandLets;
}

#endif