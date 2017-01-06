#include "JoyStickMessage.h"

/// <summary>
/// Initializes a new instance of the <see cref="JoyStickMessage"/> class.
/// </summary>
/// <param name="joyDirection">The joystick direction.</param>
/// <param name="isActive">if set to <c>true</c> [is active].</param>
JoyStickMessage::JoyStickMessage(JoyButton joyButton, bool isActive) {
	m_type			= MessageType::JOYEVENT;
	m_joyButton		= joyButton;
	m_isActive		= isActive;
}

/// <summary>
/// Finalizes an instance of the <see cref="JoyStickMessage"/> class.
/// </summary>
JoyStickMessage::~JoyStickMessage(void) {
}
