#ifndef included_EngineErrorManager
#define included_EngineErrorManager
#pragma once

#include <string>

typedef enum {

	SEVERITY_INFORMATION,
	SEVERITY_QUESTION,
	SEVERITY_WARNING,
	SEVERITY_FATAL,
} SeverityLevel;

/*
	Singleton for managing engine error reporting, assertions, recoverable errors, and warnings
*/

class EngineErrorManager {
public:

	static EngineErrorManager & getEngineErrorManager() {
		static EngineErrorManager engineErrorManager;
		return engineErrorManager;
	}

	~EngineErrorManager();

	// Note: String formatting should be done by the error reporter
	// This method will choose the appropriate OS dialogue box
	void reportFatalErrorAndTerminateProgram( const std::string& errorTitle, const std::string & errorMessage );
	void outputErrorMessageToConsole( const std::string& consoleMessage );
	void outputErrorMessageAsCStringToConsole( const char* consoleMessage );
	

	// Credit to Squirrel Eiserloh for inspiration for many of the below functions ( From Gypsy Engine )
	bool isDebuggerAvailable() const;

	void recoverableError( const std::string& cppFileName,
		int cppLineNumber,
		const std::string& errorMessageContent,
		const std::string& conditionText = "" );

	__declspec( noreturn ) void fatalError( const std::string& cppFileName,
		int cppLineNumber,
		const std::string& errorMessageContent,
		const std::string& conditionText = "" );

	void systemDialogueOkay( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity );
	bool systemDialogueOkayCancel( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity );
	bool systemDialogueYesNo( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity );
	int systemDialogueYesNoCancel( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity );
	
protected:

	EngineErrorManager() {}

private:

};


// Credit to Squirrel Eiserloh From Gypsy Engine for following Macro
/////////////////////////////////////////////////////////////////////////////////////////////////
// FATAL_ERROR
//
// Present in all builds.
// No condition; always triggers if reached.
// Depending on the platform, this typically:
//	- Logs a warning to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Shuts down the app
//
// Use this when reaching a certain line of code should never happen under any circumstances,
// and continued execution is dangerous or impossible.
//
#define FATAL_ERROR( errorMessageText )															\
{																								\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();		\
	engineErrorManager.fatalError( __FILE__, __LINE__, errorMessageText );						\
}

#define FATAL_ERROR_GENERIC																		\
{																								\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();		\
	engineErrorManager.fatalError( __FILE__, __LINE__, "Fatal unspecified error" );				\
}


// Credit to Squirrel Eiserloh From Gypsy Engine for following Macro
/////////////////////////////////////////////////////////////////////////////////////////////////
// FATAL_GUARANTEE
//
// Present in all builds.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs a warning to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Shuts down the app
//
#define FATAL_GUARANTEE( condition, errorMessageText )													\
{																										\
	if( !(condition) )																					\
{																										\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();				\
	const char* conditionText = #condition;																\
	std::string conditionTextAsString( conditionText );													\
	engineErrorManager.fatalError( __FILE__, __LINE__, errorMessageText, conditionTextAsString );		\
}																										\
}																										


#define FATAL_GUARANTEE_GENERIC( condition )																\
{																											\
	if( !(condition) )																						\
{																											\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	const char* conditionText = #condition;																	\
	std::string conditionTextAsString( conditionText );														\
	engineErrorManager.fatalError( __FILE__, __LINE__, "Fatal unmet guarantee", conditionTextAsString );	\
}																											\
}


// Credit to Squirrel Eiserloh From Gypsy Engine for following Macro
/////////////////////////////////////////////////////////////////////////////////////////////////
// FATAL_ASSERTION
//
// Removed if CBENGINE_DISABLE_ASSERTS is enabled, typically in a Final build configuration.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs a warning to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Shuts down the app
//
#if defined( CBENGINE_DISABLE_ASSERTS )
#define FATAL_ASSERTION( condition, errorMessageText ) { (void)( condition ); }
#else
#define FATAL_ASSERTION( condition, errorMessageText )															\
{																												\
	if( !(condition) )																							\
	{																											\
		EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
		const char* conditionText = #condition;																	\
		std::string conditionTextAsString( conditionText );														\
		engineErrorManager.fatalError( __FILE__, __LINE__, errorMessageText, conditionTextAsString );			\
	}																											\
}
#endif

#if defined( CBENGINE_DISABLE_ASSERTS )
#define FATAL_ASSERTION_GENERIC( condition ) { (void)( condition ); }
#else
#define FATAL_ASSERTION_GENERIC( condition )																\
{																											\
	if( !(condition) )																						\
	{																										\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	const char* conditionText = #condition;																	\
	std::string conditionTextAsString( conditionText );														\
	engineErrorManager.fatalError( __FILE__, __LINE__, "Fatal unmet assertion", conditionTextAsString );	\
}																											\
}
#endif


// Credit to Squirrel Eiserloh From Gypsy Engine for following Macro
/////////////////////////////////////////////////////////////////////////////////////////////////
// RECOVERABLE_ERROR
//
// Present in all builds.
// No condition; always triggers if reached.
// Depending on the platform, this typically:
//	- Logs a warning to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Continues execution
//
#define RECOVERABLE_ERROR( errorMessageText )																\
{																											\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	engineErrorManager.recoverableError( __FILE__, __LINE__, errorMessageText );							\
}

#define RECOVERABLE_ERROR_GENERIC																			\
{																											\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	engineErrorManager.recoverableError( __FILE__, __LINE__, "Unspecified error" );							\
}


// Credit to Squirrel Eiserloh From Gypsy Engine for following Macro
/////////////////////////////////////////////////////////////////////////////////////////////////
// RECOVERABLE_GUARANTEE
//
// Present in all builds.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs a warning to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Continues execution
//
#define RECOVERABLE_GUARANTEE( condition, errorMessageText )												\
{																											\
	if( !(condition) )																						\
	{																										\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	const char* conditionText = #condition;																	\
	std::string conditionTextAsString( conditionText );														\
	engineErrorManager.recoverableError( __FILE__, __LINE__, errorMessageText, conditionTextAsString );		\
	}																				\
}

#define RECOVERABLE_GUARANTEE_GENERIC( condition )															\
{																											\
	if( !(condition) )																						\
	{																										\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	const char* conditionText = #condition;																	\
	std::string conditionTextAsString( conditionText );														\
	engineErrorManager.recoverableError( __FILE__, __LINE__, "Unmet guarantee", conditionTextAsString );						\
	}																										\
}

// Credit to Squirrel Eiserloh From Gypsy Engine for following Macro
/////////////////////////////////////////////////////////////////////////////////////////////////
// RECOVERABLE_ASSERTION
//
// Removed if GYPSY_DISABLE_ASSERTS is enabled, typically in a Final build configuration.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs a warning to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Continues execution
//
#if defined( CBENGINE_DISABLE_ASSERTS )
#define RECOVERABLE_ASSERTION( condition, errorMessageText ) { (void)( condition ); }
#else
#define RECOVERABLE_ASSERTION( condition, errorMessageText )												\
{																											\
	if( !(condition) )																						\
	{																										\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	const char* conditionText = #condition;																	\
	std::string conditionTextAsString( conditionText );														\
	engineErrorManager.recoverableError( __FILE__, __LINE__, errorMessageText, conditionTextAsString );		\
	}																										\
}
#endif

#if defined( GYPSY_DISABLE_ASSERTS )
#define RECOVERABLE_ASSERTION_GENERIC( condition ) { (void)( condition ); }
#else
#define RECOVERABLE_ASSERTION_GENERIC( condition )															\
{																											\
	if( !(condition) )																						\
	{																										\
	EngineErrorManager& engineErrorManager = EngineErrorManager::getEngineErrorManager();					\
	const char* conditionText = #condition;																	\
	std::string conditionTextAsString( conditionText );														\
	engineErrorManager.recoverableError( __FILE__, __LINE__, "Unmet assertion", conditionTextAsString );	\
	}																										\
}
#endif


/* (Paul: May implement this soon)
//-----------------------------------------------------------------------------------------------
void DebuggerPrintf( const char* messageFormat, ... )
{
const int MESSAGE_MAX_LENGTH = 2048;
char messageLiteral[ MESSAGE_MAX_LENGTH ];
va_list variableArgumentList;
va_start( variableArgumentList, messageFormat );
vsnprintf_s( messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList );
va_end( variableArgumentList );
messageLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

#if defined( PLATFORM_WINDOWS )
if( IsDebuggerAvailable() )
{
OutputDebugStringA( messageLiteral );
}
#endif

std::cout << messageLiteral;
}
*/


#endif