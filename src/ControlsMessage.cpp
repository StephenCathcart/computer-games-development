#include "ControlsMessage.h"

ControlsMessage::ControlsMessage(bool isActive) {
	m_type		= MessageType::CONTROLS;
	m_isActive	= isActive;
}

ControlsMessage::~ControlsMessage(void) {
}
