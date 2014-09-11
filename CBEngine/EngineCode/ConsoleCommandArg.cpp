#include "ConsoleCommandArg.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

ConsoleCommandArg::~ConsoleCommandArg() {

}

ConsoleCommandArg::ConsoleCommandArg() {
	m_fullCommand = "";
	m_mainCommand = "";
}

ConsoleCommandArg::ConsoleCommandArg( const std::string & fullCommand, const std::vector< std::string > & commandArgs ) {
	m_fullCommand = fullCommand;
	m_commandArgs = commandArgs;
}