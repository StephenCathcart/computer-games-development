#include "LoadingMessage.h"

LoadingMessage::LoadingMessage(bool isLoading, int percent) {
	m_type		= MessageType::LOADING;
	m_isLoading = isLoading;
	m_percent	= percent;
}

LoadingMessage::~LoadingMessage(void) {
}
