#include "HighScoreMessage.h"

HighScoreMessage::HighScoreMessage(bool isActive) {
	m_type		= MessageType::HIGHSCORE;
	m_isActive	= isActive;
}

HighScoreMessage::~HighScoreMessage(void) {
}