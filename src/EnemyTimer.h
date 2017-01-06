#ifndef ENEMYTIMER_H
#define ENEMYTIMER_H

#include "GameSystemManager.h"

class EnemyTimer {
public:
	EnemyTimer(void);
	~EnemyTimer(void);

	static void checkForNewWave(float delta, GameSystemManager* mgr);
	static void restart() { 
		timer = 0.0f; 
		nextWave = 5000.0f;
		totalTicks = 0.0f;
		levelOfDifficulty = 1;
	};

private:
	static void	spawnWave(GameSystemManager* mgr);
	static void createWave(GameSystemManager* mgr, int type, int amount);

	static float timer;
	static float nextWave;
	static int totalTicks;
	static int levelOfDifficulty;
};

#endif

