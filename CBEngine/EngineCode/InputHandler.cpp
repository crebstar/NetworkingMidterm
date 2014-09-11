#include "InputHandler.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

void InputHandler::registerWindowsInputCallbackFunction( WNDCLASSEX& windowClassDescription ) {

	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>( windowsMessageHandlingProcedure );
}


bool InputHandler::callCallbackFunctionForInputEventSubscribers( unsigned char inputEventKey ) {

	bool wasProcessed = false;
	std::unordered_map<unsigned char,std::vector<EventSubscriberBase*>>::iterator itSub;
	itSub = m_inputEventSubscribers.find( inputEventKey );

	if ( itSub != m_inputEventSubscribers.end() ) {

		std::vector<EventSubscriberBase*>& inputEventSubVec = itSub->second;
		for ( size_t i = 0; i < inputEventSubVec.size(); ++i ) {

			NamedProperties inputEventProperty;
			EventSubscriberBase* inputEventSub = inputEventSubVec[i];
			inputEventSub->callCallbackFunction( inputEventProperty );
			wasProcessed = true;
		}
	}

	return wasProcessed;
}


bool InputHandler::processKeyDownEvent( unsigned char virtualKeyCode ) {

	m_isKeyDown[virtualKeyCode] = true;

	return false;
}


bool InputHandler::processKeyUpEvent( unsigned char virtualKeyCode ) {

	m_isKeyDown[virtualKeyCode] = false;

	return false;
}


void InputHandler::processWMCHARKeyEvent( unsigned char wmCharKey ) {

	m_wmCharKeys.push( wmCharKey );
}


void InputHandler::emptyWMCharMessageQueue() {

	while ( !m_wmCharKeys.empty() ) {
		m_wmCharKeys.pop();
	}
}


const bool * InputHandler::getKeyboardStatus() const {

	return m_isKeyDown;
}


int	InputHandler::getNumVirtualKeys() const {

	return NUM_INPUT_VIRTUAL_KEYS;
}


std::queue<unsigned char>& InputHandler::getWmCharMessageQueue() {

	return m_wmCharKeys;
}


void InputHandler::setDefaultValuesForInputKeys() {

	for ( int i = 0; i < NUM_INPUT_VIRTUAL_KEYS; ++i ) {
		m_isKeyDown[i] = false;
	}
}

LRESULT CALLBACK windowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam ) {

	InputHandler& sharedInputHandler = InputHandler::getSharedInputHandler();

	// wParam and lParam is bonus info
	switch( wmMessageCode ) {

	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		//sharedDirector->m_isQuiting = true;
		return 0;
	
	case WM_KEYDOWN: {
		unsigned char asKey = (unsigned char) wParam;

		/*
		if( asKey == VK_ESCAPE ) {
			sharedDirector->m_isQuiting = true;
			return 0;
		}

		*/
		
		bool wasProcessed = sharedInputHandler.processKeyDownEvent( asKey );

		if (wasProcessed) {
			return 0;
		}

	} // end case key down

	break;

	case WM_CHAR: {
		unsigned char asKey = (unsigned char) wParam;
		sharedInputHandler.processWMCHARKeyEvent( asKey );
	 }

	break;
	
	case WM_KEYUP: {

		unsigned char asKey = (unsigned char) wParam;
		/*
		if ( asKey == VK_ESCAPE ) {
			sharedDirector->m_isQuiting = true;
			return 0;
		}
		*/

		bool wasProcessed = sharedInputHandler.processKeyUpEvent( asKey );

		if (wasProcessed) {
			return 0;
		}

	} // end key up case

	break;

	default:

		break;
	} // end switch

	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );

} // end input function