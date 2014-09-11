
#include "XBOXController.hpp"

#include <cassert>
#include <math.h>

#include "MathUtil.hpp"
#include "Vector2.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::XBOXController;
using cbengine::Thumb_Stick_Data;

XBOXController::~XBOXController() {



} // end destructor

XBOXController::XBOXController(int controllerNum) {

	this->m_controller_Number = controllerNum;

} // end constructor

// Static Functions
bool XBOXController::isThumbStickInDeadZone( const Thumb_Stick_Data & thumb_Stick_Data ) {

	assert(!thumb_Stick_Data.normalized);
	// Assume left for now
	if (thumb_Stick_Data.isLeftThumbStick) {
		// Left
		if (thumb_Stick_Data.magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {

			return true;
		} // end inner if

	} else  {
		// Right
		if (thumb_Stick_Data.magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {

			return true;
		} // end inner if
		
	} // end outer if
	return false;
} // end isThumbStickInDeadZone

void XBOXController::normalizeThumbStickData( Thumb_Stick_Data & thumb_Stick_Data ) {

	// Assumes left for Asteroids at the moment
	thumb_Stick_Data.xComp = thumb_Stick_Data.xComp / thumb_Stick_Data.magnitude;
	thumb_Stick_Data.yComp = thumb_Stick_Data.yComp / thumb_Stick_Data.magnitude;

	//thumb_Stick_Data.magnitude = thumb_Stick_Data.magnitude - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	// Constrain the range between 0 and 1.0
	thumb_Stick_Data.magnitude = thumb_Stick_Data.magnitude / XBOX_CONTROLLER_MAX_THUMBSTICK_MAGNITUDE;

	thumb_Stick_Data.normalized = true;

} // endNormalizeThumbStickData


XINPUT_STATE XBOXController::getInputState() {

	this->resetInputState();

	XInputGetState( this->m_controller_Number, &(this->m_controller_State));

	return this->m_controller_State;

} // end getInputState

void XBOXController::cycleInputState() {

	this->resetInputState();

	XInputGetState( this->m_controller_Number, &(this->m_controller_State));

}

void XBOXController::resetInputState() {

	ZeroMemory( &(this->m_controller_State), sizeof(XINPUT_STATE) );

}

// Predicates
bool XBOXController::isXboxControllerConnected() {

	// Zero out the XboxController State
	ZeroMemory( &(this->m_controller_State), sizeof(XINPUT_STATE) );

	//Store the XInputGetState value inside result
	DWORD result = XInputGetState( this->m_controller_Number, &(this->m_controller_State));
	//Check if the controller is disconnected
	return  result == ERROR_DEVICE_NOT_CONNECTED ? false : true;  

} // end isXboxControllerconnected

bool XBOXController::hasThumbStickMoved( Thumb_Stick_Data * thumbStickData) const {

	if (thumbStickData == nullptr) {
		// Test Left 
		float xCompL = this->m_controller_State.Gamepad.sThumbLX;
		float yCompL = this->m_controller_State.Gamepad.sThumbLY;

		float magL = sqrt( (xCompL * xCompL) + (yCompL * yCompL));

		if (magL > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) return true;

		// Test Right
		float xCompR = this->m_controller_State.Gamepad.sThumbRX;
		float yCompR = this->m_controller_State.Gamepad.sThumbRY;

		float magR = sqrt( (xCompR * xCompR) + (yCompR * yCompR));

		return (magR > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	} else {
		
		if (thumbStickData->isLeftThumbStick) {

			return (thumbStickData->magnitude > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + XBOX_ADJUTED_DEADZONE) );

		} else {

			return (thumbStickData->magnitude > (XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + XBOX_ADJUTED_DEADZONE) );

		} // end inner if

	} // end if
} // end hasThumbStickMoved

bool XBOXController::getLeftThumbStickData( Thumb_Stick_Data * dataToPopulate /* = nullptr */ ) const {

	dataToPopulate->isLeftThumbStick = true;

	dataToPopulate->xComp = this->m_controller_State.Gamepad.sThumbLX;
	dataToPopulate->yComp = this->m_controller_State.Gamepad.sThumbLY;

	dataToPopulate->magnitude = sqrt( (dataToPopulate->xComp * dataToPopulate->xComp) + 
		(dataToPopulate->yComp * dataToPopulate->yComp) );

	// Check to see if it passed the 'Dead Zone'
	if( !this->hasThumbStickMoved(dataToPopulate) )  {
		dataToPopulate->dirty = true;	
		return false;
	} // end if

	// TODO refactor for atan2f
	float angleInRads = atan2f( dataToPopulate->yComp , dataToPopulate->xComp );

	float angleInDegrees = cbengine::radiansToDegrees( angleInRads );
	dataToPopulate->degrees = angleInDegrees;

	return true;

}

bool XBOXController::getRightThumbStickData( Thumb_Stick_Data * dataToPopulate /* = nullptr */ ) const {

	dataToPopulate->isLeftThumbStick = false;

	dataToPopulate->xComp = this->m_controller_State.Gamepad.sThumbRX;
	dataToPopulate->yComp = this->m_controller_State.Gamepad.sThumbRY;

	dataToPopulate->magnitude = sqrt( (dataToPopulate->xComp * dataToPopulate->xComp) + 
		(dataToPopulate->yComp * dataToPopulate->yComp) );

	// Check to see if it passed the 'Dead Zone'
	if( !this->hasThumbStickMoved(dataToPopulate) )  {
		dataToPopulate->dirty = true;	
		return false;
	} // end if

	// TODO refactor for atan2f
	float angleInRads = atan( dataToPopulate->yComp / dataToPopulate->xComp );

	float angleInDegrees = cbengine::radiansToDegrees( angleInRads );
	dataToPopulate->degrees = angleInDegrees;

	// Adjust for constrained range of atan which is pi/2 to -pi/2
	if ( dataToPopulate->xComp < 0) {

		dataToPopulate->degrees += 180;

	} else if (dataToPopulate->degrees < 0) {

		dataToPopulate->degrees = dataToPopulate->degrees + 360;

	} // end if

	return true;

}

bool XBOXController::getThumbStickData( Thumb_Stick_Data * dataToPopulate ) {

	// For Asteroids we only care about the left stick (Change this later)
	dataToPopulate->isLeftThumbStick = true;

	dataToPopulate->xComp = this->m_controller_State.Gamepad.sThumbLX;
	dataToPopulate->yComp = this->m_controller_State.Gamepad.sThumbLY;

	dataToPopulate->magnitude = sqrt( (dataToPopulate->xComp * dataToPopulate->xComp) + 
		(dataToPopulate->yComp * dataToPopulate->yComp) );

	// Check to see if it passed the 'Dead Zone'
	if( !this->hasThumbStickMoved(dataToPopulate) )  {
		dataToPopulate->dirty = true;	
		return false;
	} // end if

	// TODO refactor for atan2f
	float angleInRads = atan( dataToPopulate->yComp / dataToPopulate->xComp );

	float angleInDegrees = cbengine::radiansToDegrees( angleInRads );
	dataToPopulate->degrees = angleInDegrees;

	// Adjust for constrained range of atan which is pi/2 to -pi/2
	if ( dataToPopulate->xComp < 0) {
		
		dataToPopulate->degrees += 180;

	} else if (dataToPopulate->degrees < 0) {

		dataToPopulate->degrees = dataToPopulate->degrees + 360;

	} // end if
	
	return true;

} // end getThumbStickData


bool XBOXController::isButtonPressedDown(unsigned int xboxButton) const {

	return (xboxButton == this->m_controller_State.Gamepad.wButtons);

} // endIsButtonPressedDown