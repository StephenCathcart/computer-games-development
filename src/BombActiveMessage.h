#ifndef BOMBACTIVEMESSAGE_H
#define BOMBACTIVEMESSAGE_H

#include "BaseMessage.h"

class BombActiveMessage : public BaseMessage {
public:
	BombActiveMessage(void);
	~BombActiveMessage(void);

	MessageType	getMessageType() const { return m_type; }

private:
	MessageType m_type;
};

#endif