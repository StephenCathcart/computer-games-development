#include "GameState.h"

GameState::STATE GameState::currentState	= STATE::START;
GameState::STATE GameState::previousState	= STATE::UNKNOWN;

GameState::GameState(void) {
}

GameState::~GameState(void) {
}
