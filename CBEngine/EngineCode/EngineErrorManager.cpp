#include "EngineErrorManager.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // ugh
#include <atlstr.h>

#include "GameDirector.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

EngineErrorManager::~EngineErrorManager() {

}


void EngineErrorManager::reportFatalErrorAndTerminateProgram( const std::string& errorTitle, const std::string& errorMessage ) {
	// Note... For now only Windows Error Dialogue Box is supported
	MessageBoxA( 0, (LPCSTR) errorMessage.c_str(), (LPCSTR) errorTitle.c_str(), MB_ICONERROR ); 
	// TODO:: Research a better way to exit the program
	std::exit(0);
}


void EngineErrorManager::outputErrorMessageToConsole( const std::string& consoleMessage ) {

	OutputDebugStringA( consoleMessage.c_str() );
}


void EngineErrorManager::outputErrorMessageAsCStringToConsole( const char* consoleMessage ) {

	OutputDebugStringA( consoleMessage );
}

// Credit to Squirrel Eiserloh for Debuggger Detection
bool EngineErrorManager::isDebuggerAvailable() const {

	// TODO:: Check if the platform is Windows  (( #if defined( PLATFORM_WINDOWS ) ))
	typedef BOOL (CALLBACK IsDebugFuncType)();

	// Obtain handle to Kernel32.DLL (Comes with Windows?)
	HINSTANCE hInstanceKernel32 = GetModuleHandle( TEXT( "KERNEL32" ) );

	if ( !hInstanceKernel32 ) {
		return false;
	}

	// Obtain a Handle to the IsDebuggerPresent() Function in Kernal32.DLL
	IsDebugFuncType* isDebuggerPresentFunc = ( IsDebugFuncType*) GetProcAddress( hInstanceKernel32, "IsDebuggerPresent" );

	if ( !isDebuggerPresentFunc ) {
		return false;
	}

	BOOL isDebuggerAvailableForUse = isDebuggerPresentFunc();
	return ( isDebuggerAvailableForUse == TRUE );
}


// Credit To Squirrel Eiserloh 
// TODO:: Write define check for Windows platform
UINT GetWindowsMessageBoxIconFlagForSeverityLevel( SeverityLevel severity )
{
	switch( severity )
	{
	case SEVERITY_INFORMATION:		return MB_ICONASTERISK;		// blue circle with 'i' in Windows 7
	case SEVERITY_QUESTION:			return MB_ICONQUESTION;		// blue circle with '?' in Windows 7
	case SEVERITY_WARNING:			return MB_ICONEXCLAMATION;	// yellow triangle with '!' in Windows 7
	case SEVERITY_FATAL:			return MB_ICONHAND;			// red circle with 'x' in Windows 7
	default:						return MB_ICONEXCLAMATION;
	}
}

// TODO:: Set up platform specific detectors!

// Credit to Squirrel Eiserloh From Gypsy Engine
void EngineErrorManager::systemDialogueOkay( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity ) {

	ShowCursor( TRUE );
	UINT dialogueIconTypeFlag = GetWindowsMessageBoxIconFlagForSeverityLevel( severity );
	MessageBoxA( NULL, messageText.c_str(), messageTitle.c_str(), MB_OK | dialogueIconTypeFlag | MB_TOPMOST );
	ShowCursor( FALSE );

}


// Credit to Squirrel Eiserloh From Gypsy Engine
bool EngineErrorManager::systemDialogueOkayCancel( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity ) {
	bool isAnswerOkay = true;

	ShowCursor( TRUE );
	UINT dialogueIconTypeFlag = GetWindowsMessageBoxIconFlagForSeverityLevel( severity );
	int buttonClicked = MessageBoxA( NULL, messageText.c_str(), messageTitle.c_str(), MB_OKCANCEL | dialogueIconTypeFlag | MB_TOPMOST );
	isAnswerOkay = (buttonClicked == IDOK);
	ShowCursor( FALSE );

	return isAnswerOkay;
}


// Credit to Squirrel Eiserloh From Gypsy Engine
// Returns true if YES was chosen, false if NO was chosen.

bool EngineErrorManager::systemDialogueYesNo( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity ) {

	bool isAnswerYes = true;

	ShowCursor( TRUE );
	UINT dialogueIconTypeFlag = GetWindowsMessageBoxIconFlagForSeverityLevel( severity );
	int buttonClicked = MessageBoxA( NULL, messageText.c_str(), messageTitle.c_str(), MB_YESNO | dialogueIconTypeFlag | MB_TOPMOST );
	isAnswerYes = (buttonClicked == IDYES);
	ShowCursor( FALSE );

	return isAnswerYes;
}


// Credit to Squirrel Eiserloh From Gypsy Engine
//-----------------------------------------------------------------------------------------------
// Returns 1 if YES was chosen, 0 if NO was chosen, -1 if CANCEL was chosen.
//
int EngineErrorManager::systemDialogueYesNoCancel( const std::string& messageTitle, const std::string& messageText, SeverityLevel severity ) {

	int answerCode = 1;

	ShowCursor( TRUE );
	UINT dialogueIconTypeFlag = GetWindowsMessageBoxIconFlagForSeverityLevel( severity );
	int buttonClicked = MessageBoxA( NULL, messageText.c_str(), messageTitle.c_str(), MB_YESNOCANCEL | dialogueIconTypeFlag | MB_TOPMOST );
	answerCode = (buttonClicked == IDYES ? 1 : (buttonClicked == IDNO ? 0 : -1) );
	ShowCursor( FALSE );
	
	return answerCode;
}


// Credit to Squirrel Eiserloh From Gypsy Engine
void EngineErrorManager::recoverableError( const std::string& cppFileName,
	int cppLineNumber,
	const std::string& errorMessageContent,
	const std::string& conditionText ) 
{
	std::string engineName = "CBEngine";
	std::string fullMessageTitle = engineName + " :: Warning";
	std::string fullMessageText = errorMessageContent;

	bool isDebuggerPresent = ( isDebuggerAvailable() == TRUE );

	if ( isDebuggerPresent ) {

		fullMessageText += "\n\n A Debugger has been detected!\nWould you like to continue running?\n (Cancel to debug, No to quit)\n";
	} else {

		fullMessageText += "\n\nWould you like to continue running?\n  (No to quit)\n";
	}

	if ( !conditionText.empty() ) {

		std::string cppLineNumberAsString = std::to_string( static_cast<long long>( cppLineNumber ) );

		fullMessageText += "\n---------- Debugging Details Follow ----------\n";
		fullMessageText += "\nThis error was caused by a run-time condition check from line ";
		fullMessageText += cppLineNumberAsString;
		fullMessageText += " of:\n";
		fullMessageText += cppFileName;
		fullMessageText += "\n\nThe condition asserted which failed was:\n ";
		fullMessageText += conditionText;
		fullMessageText += "\n";
	}

	// TODO:: Output content to output window
	//DebuggerPrintf( "%s(%d): %s", cppFileName, cppLineNum, fullMessageText.c_str() ); // Use this specific format so Visual Studio users can double-click to jump to file-and-line of error

	if ( isDebuggerPresent ) {

		int answerCode = systemDialogueYesNoCancel( fullMessageTitle, fullMessageText, SEVERITY_WARNING );
		ShowCursor( TRUE );

		if ( answerCode == 0 ) {
			// No Chosen
			std::exit( -1 );
		} else if ( answerCode == -1 ) {
			// Cancel Chosen
			__debugbreak();
		} 

		ShowCursor( FALSE );
	} else {

		bool isAnswerYes = systemDialogueYesNo( fullMessageTitle, fullMessageText, SEVERITY_WARNING );
		ShowCursor( TRUE );

		if ( !isAnswerYes ) {
			std::exit( -1 );
		}
	}
}


// Credit to Squirrel Eiserloh From Gypsy Engine
__declspec( noreturn ) void EngineErrorManager::fatalError( const std::string& cppFileName,
	int cppLineNumber,
	const std::string& errorMessageContent,
	const std::string& conditionText )
{
	std::string engineName = "CBEngine";
	std::string fullMessageTitle = engineName + " :: Error";
	std::string fullMessageText = errorMessageContent;
	fullMessageText += "\n\nThe application will now close.\n";

	bool isDebuggerPresent = ( isDebuggerAvailable() == TRUE );

	if ( !conditionText.empty() ) {

		std::string cppLineNumberAsString = std::to_string( static_cast<long long>( cppLineNumber ) );

		fullMessageText += "\n------------------ Debugging Details Follow -----------------\n";
		fullMessageText += "\nThis error was caused by a run-time condition check from line ";
		fullMessageText += cppLineNumberAsString;
		fullMessageText += " of:\n";
		fullMessageText += cppFileName;
		fullMessageText += "\n\nThe condition asserted which failed was:\n  ";
		fullMessageText += conditionText;
		fullMessageText += "\n";
	}

	// TODO:: Output message details to output window
	//DebuggerPrintf( "%s(%d): %s", cppFileName, cppLineNum, fullMessageText.c_str() ); // Use this specific format so Visual Studio users can double-click to jump to file-and-line of error

	if ( isDebuggerPresent ) {

		fullMessageText += "\n A Debugger has been detected!\nWould you like to break and debug?\n (No to quit)\n";
		bool isAnswerYes = systemDialogueYesNo( fullMessageTitle, fullMessageText, SEVERITY_FATAL );
		ShowCursor( TRUE );

		if ( isAnswerYes ) {

			__debugbreak();
		}

		ShowCursor( FALSE );
	} else {

		systemDialogueOkay( fullMessageTitle, fullMessageText, SEVERITY_FATAL );
		ShowCursor( TRUE );
	}

	std::exit( -1 );
}


