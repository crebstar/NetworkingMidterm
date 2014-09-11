#ifndef included_CommandLet
#define included_CommandLet
#pragma once

#include <vector>
#include <string>

class CommandLet {
public:
	~CommandLet();
	CommandLet( const std::string& commandName );

	// Core
	void addCommandArgument( const std::string& commandArg );

	// Mutators
	const std::string& getCommandName() const;
	const std::vector<std::string>& getCommandArgs() const;
	const size_t getNumCommandArgs() const;
	bool isCommandProcessed() const;
	void markAsProcessed() const;

protected:

private:

	std::string						m_commandName;
	std::vector<std::string>		m_commandArgs;
	mutable bool					m_isProcessed;
};


// Inline Functions
inline const std::string& CommandLet::getCommandName() const {

	return m_commandName;
}


inline const std::vector<std::string>& CommandLet::getCommandArgs() const {

	return m_commandArgs;
}


inline const size_t CommandLet::getNumCommandArgs() const {

	return m_commandArgs.size();
}


inline bool CommandLet::isCommandProcessed() const {

	return m_isProcessed;
}


inline void CommandLet::markAsProcessed() const {

	m_isProcessed = true;
}

#endif