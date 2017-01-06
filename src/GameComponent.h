#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

/// <summary>
/// Abstract base Component class for all Components to derive from.
/// All components are just bags of data ready to be attached to GameEntity and
/// manipulated by GameSystems.
/// </summary>
class GameComponent {
public:
	GameComponent(void);
	~GameComponent(void);

	virtual void update ( float msec ) = 0;
};

#endif