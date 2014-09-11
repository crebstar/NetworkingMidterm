#include "CommandLet.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

CommandLet::~CommandLet() {

}


CommandLet::CommandLet( const std::string& commandName  ) :
	m_commandName( commandName ),
	m_isProcessed( false )
{

}


void CommandLet::addCommandArgument( const std::string& commandArg ) {

	m_commandArgs.push_back( commandArg );
}