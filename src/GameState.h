#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState {
public:
	~GameState(void);

	enum STATE {
		START,
		RUN,
		CREDITS,
		UNKNOWN,
	};

	static STATE currentState;
	static STATE previousState;

private:
	GameState(void);
};

#endif
