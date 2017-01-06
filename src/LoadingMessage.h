#ifndef LOADINGMESSAGE_H
#define LOADINGMESSAGE_H

#include "BaseMessage.h"

class LoadingMessage : public BaseMessage {
public:
	LoadingMessage(bool isLoading, int percent);
	~LoadingMessage(void);

	MessageType getMessageType() const { return m_type; }
	bool		getPayload() const { return m_isLoading; }
	int			getPercent() const { return m_percent; }

private:
	MessageType m_type;
	bool		m_isLoading;
	int			m_percent;
};

#endif
