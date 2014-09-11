
#ifndef included_CommandFunctions
#define included_CommandFunctions
#pragma once

#include <string>

#include "Vector4.hpp"
#include "EngineMacros.hpp"
#include "Console.hpp"

class ConsoleCommandArg;

inline void quitGame( const ConsoleCommandArg & args ) {
	UNUSED( args );
	std::exit( 0 );
}

inline void clearConsole( const ConsoleCommandArg & args ) {
	UNUSED( args );
	Console * sharedConsole = Console::getSharedDeveloperConsole();
	sharedConsole->clearCommandHistory();
}


inline void consoleLog( const ConsoleCommandArg & args ) {
	Console * sharedConsole = Console::getSharedDeveloperConsole();
	std::string logMessage;
	cbengine::Vector4<float> color( 0.10f, 0.80f, 0.30f, 1.0f );
	
	for ( size_t i = 0; i < args.m_commandArgs.size(); ++i ) {
		logMessage += args.m_commandArgs[i];
		logMessage += " ";
	}
	sharedConsole->addStringToConsole( logMessage, color );
}

#endif