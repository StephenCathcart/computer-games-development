#include "EnemyTimer.h"
#include "EnemyList.h"
#include "EnemyMessage.h"
#include <iostream>

#include "PositionComponent.h"

float	EnemyTimer::timer				= 0.0f;
float	EnemyTimer::nextWave			= 5000.0f;
int		EnemyTimer::totalTicks			= 0.0f;
int		EnemyTimer::levelOfDifficulty	= 1;

EnemyTimer::EnemyTimer(void) {
}

EnemyTimer::~EnemyTimer(void) {
}

void EnemyTimer::checkForNewWave(float delta, GameSystemManager* mgr) {
	timer += delta;
	if(timer > nextWave) {
		totalTicks++;
		if(totalTicks == 10 || totalTicks == 20) {
			nextWave -= 1000.0f;
			++levelOfDifficulty;
		}
		timer = 0.0f;
		spawnWave(mgr);
	}
}

void EnemyTimer::spawnWave(GameSystemManager* mgr) {
	int amountOfEnemies = rand() % 3 + 1; // 1- 4 enemies
	int typeOfEnemy = rand() % levelOfDifficulty; // type of enemy 0-2

	createWave(mgr, typeOfEnemy, amountOfEnemies);
}

void EnemyTimer::createWave(GameSystemManager* mgr, int type, int amount) {
	for(int i = 0; i < amount; i++) {
		GameEntity* e;
		switch (type) {
			case 0:
				e = EnemyList::getEnemy1();
				break;
			case 1:
				e = EnemyList::getEnemy3();
				break;
			case 2:
				e = EnemyList::getEnemy2();
				break;
			default:
				std::cout << "value of type unknown" << std::endl;
		}
		if(e != NULL) {
			mgr->sendMessage(&EnemyMessage(e));
		}
	}
}