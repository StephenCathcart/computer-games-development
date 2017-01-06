#ifndef JOYSTICKMESSAGE_H
#define JOYSTICKMESSAGE_H

#include "BaseMessage.h"

/// <summary>
/// JoyDirections let us know what direction(XY) the controller
/// thumbsticks are pointing at.
/// </summary>
enum JoyButton {
	UP, DOWN, LEFT, RIGHT, NULLX, NULLY,
	A, B, X, Y, LB, RB, BACK, START, LEFTANALOG, RIGHTANALOG 
};

/// <summary>
/// Stores joystick thumbstick movement information. Its message type is
/// of JOYEVENT. 
/// </summary>
class JoyStickMessage : public BaseMessage {
public:
	JoyStickMessage(JoyButton joyButton, bool isActive);
	~JoyStickMessage(void);

	MessageType		getMessageType() const { return m_type; }
	JoyButton		getJoyButton() const { return m_joyButton; }
	bool			getIsActive() const { return m_isActive; };

private:
	MessageType		m_type;
	JoyButton		m_joyButton;
	bool			m_isActive;
};

#endif
