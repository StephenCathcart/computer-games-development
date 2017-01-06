#ifndef CONTROLSMESSAGE_H
#define CONTROLSMESSAGE_H

#include "BaseMessage.h"

class ControlsMessage : public BaseMessage {
public:
	ControlsMessage(bool isActive);
	~ControlsMessage(void);

	MessageType getMessageType() const { return m_type; }
	bool		getPayload() const { return m_isActive; }

private:
	MessageType m_type;
	bool		m_isActive;
};

#endif