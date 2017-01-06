#ifndef HIGHSCOREMESSAGE_H
#define HIGHSCOREMESSAGE_H

#include "BaseMessage.h"

class HighScoreMessage : public BaseMessage {
public:
	HighScoreMessage(bool isActive);
	~HighScoreMessage(void);

	MessageType getMessageType() const { return m_type; }
	bool		getPayload() const { return m_isActive; }

private:
	MessageType m_type;
	bool		m_isActive;
};

#endif

