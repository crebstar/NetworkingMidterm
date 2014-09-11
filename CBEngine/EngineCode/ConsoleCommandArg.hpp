#ifndef included_ConsoleCommandArg
#define included_ConsoleCommandArg
#pragma once

#include <string>
#include <vector>

class ConsoleCommandArg {
public:

	~ConsoleCommandArg();
	ConsoleCommandArg();
	explicit ConsoleCommandArg( const std::string & fullCommand, const std::vector< std::string > & commandArgs );

	std::string						m_mainCommand;
	std::string						m_fullCommand;
	std::vector< std::string >		m_commandArgs;

protected:

private:


};



#endif