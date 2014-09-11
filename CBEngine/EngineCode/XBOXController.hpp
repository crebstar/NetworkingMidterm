


#ifndef included_XBOXController
#define included_XBOXController
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "XInput.lib")

namespace cbengine {

// Conceptual Const Implementation
struct Thumb_Stick_Data {

public:

	virtual ~Thumb_Stick_Data() {}

	Thumb_Stick_Data() :
	  xComp(0.0f),
		  yComp(0.0f),
		  magnitude(0.0f),
		  degrees(0.0f),
		  isLeftThumbStick(true),
		  normalized(false),
		  dirty(false)
	  {

	  } // end Struct Ctor

	float xComp;
	float yComp;
	float magnitude;
	float degrees;
	bool isLeftThumbStick;
	bool normalized;
	mutable bool dirty;

protected:

private:

}; // End Struct Representation of ThumbStick Data

const float XBOX_CONTROLLER_MAX_THUMBSTICK_MAGNITUDE = 32800.0f;
const short XBOX_ADJUTED_DEADZONE = 1000;

class Vector2;

class XBOXController {

public:

	// Static Helper Functions
	static bool isThumbStickInDeadZone( const Thumb_Stick_Data & thumb_Stick_Data );
	static void normalizeThumbStickData( Thumb_Stick_Data & thumb_Stick_Data );

	// Ctor and Dtor
	virtual ~XBOXController();
	explicit XBOXController(int controllerNum = 0);
	
	// Input State Functions
	virtual void resetInputState();
	virtual void cycleInputState();

	// Resets input state before sending the current state
	XINPUT_STATE getInputState();

	// Predicates
	virtual bool isXboxControllerConnected();
	virtual bool hasThumbStickMoved( Thumb_Stick_Data * thumbStickData = nullptr ) const;
	virtual bool isButtonPressedDown( unsigned int xboxButton ) const;

	// Mutators
	virtual bool getLeftThumbStickData( Thumb_Stick_Data * dataToPopulate = nullptr ) const;
	virtual bool getRightThumbStickData( Thumb_Stick_Data * dataToPopulate = nullptr ) const;

	// ThumbStick
	// Returns false if neither is above the deadzone :: Otherwise it populates the given parameter with data
	bool getThumbStickData( Thumb_Stick_Data * dataToPopulate );

	virtual inline void setPlayerNumber( int number ) {
		m_controller_Number = number;
	}

protected:

private:

	// Private Instance Variables
	XINPUT_STATE		m_controller_State;
	int					m_controller_Number;

};

} // end namspace

#endif