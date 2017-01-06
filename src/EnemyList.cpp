#include "EnemyList.h"
#include "Paths.h"
#include "EnemyComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "RenderComponent.h"
#include "PositionComponent.h"

vector<GameEntity*> EnemyList::enemy1List;
vector<GameEntity*> EnemyList::enemy2List;
vector<GameEntity*> EnemyList::enemy3List;
const float	EnemyList::GAME_Z_DISTANCE = -200.0f;


EnemyList::EnemyList(void) {
}


EnemyList::~EnemyList(void) {
}

void EnemyList::create(sf::RenderWindow &win, GameEntityManager &eMgr, GameResourceManager &rMgr, int amount) {
	for(int i = 0; i < amount ;i++) {
		/** Create enemy entity **/
		GameEntity* enemyEntity = new GameEntity;
		enemyEntity->addComponent(new EnemyComponent(EnemyType::CURVED, 1.0f, false));
		enemyEntity->addComponent(new HealthComponent(1, 1));
		enemyEntity->addComponent(new ScoreComponent(500));
		enemyEntity->addComponent(new RenderComponent(*rMgr.getMesh(Paths::OBJ_SHIP_SMALL), *rMgr.getShader(Paths::SHADER_BASIC)));
		enemyEntity->addComponent(new PositionComponent(glm::vec3(500.0f, 0.0f, GAME_Z_DISTANCE)));
		/**	End enemy entity **/

		enemy1List.push_back(enemyEntity);
		eMgr.addEntity(enemyEntity);
	}

	for(int i = 0; i < amount ;i++) {
		/** Create enemy entity **/
		GameEntity* enemyEntity = new GameEntity;
		enemyEntity->addComponent(new EnemyComponent(EnemyType::STRAIGHT, 1.0f, false));
		enemyEntity->addComponent(new HealthComponent(20, 20));
		enemyEntity->addComponent(new ScoreComponent(1000));
		enemyEntity->addComponent(new RenderComponent(*rMgr.getMesh(Paths::OBJ_SHIP_MEDIUM), *rMgr.getShader(Paths::SHADER_BASIC)));
		enemyEntity->addComponent(new PositionComponent(glm::vec3(500.0f, 0.0f, GAME_Z_DISTANCE)));
		/**	End enemy entity **/

		enemy2List.push_back(enemyEntity);
		eMgr.addEntity(enemyEntity);
	}

	for(int i = 0; i < amount ;i++) {
		/** Create enemy entity **/
		GameEntity* enemyEntity = new GameEntity;
		enemyEntity->addComponent(new EnemyComponent(EnemyType::STRAIGHT, 1.0f, false));
		enemyEntity->addComponent(new HealthComponent(3, 3));
		enemyEntity->addComponent(new ScoreComponent(5000));
		enemyEntity->addComponent(new RenderComponent(*rMgr.getMesh(Paths::OBJ_SHIP_LARGE), *rMgr.getShader(Paths::SHADER_BASIC)));
		enemyEntity->addComponent(new PositionComponent(glm::vec3(500.0f, 0.0f, GAME_Z_DISTANCE)));
		/**	End enemy entity **/

		enemy3List.push_back(enemyEntity);
		eMgr.addEntity(enemyEntity);
	}
}

GameEntity* EnemyList::getEnemy1() {
	for(std::vector<GameEntity*>::iterator it = enemy1List.begin(); it != enemy1List.end(); ++it) {
		EnemyComponent* eComp = (EnemyComponent*)(*it)->getComponent<EnemyComponent>();
		if (!eComp->getIsActive()) {
			eComp->setIsActive(true);
			return *it;
		}
	}
	return NULL;
}

GameEntity* EnemyList::getEnemy2() {
	for(std::vector<GameEntity*>::iterator it = enemy2List.begin(); it != enemy2List.end(); ++it) {
		EnemyComponent* eComp = (EnemyComponent*)(*it)->getComponent<EnemyComponent>();
		if (!eComp->getIsActive()) {
			eComp->setIsActive(true);
			return *it;
		}
	}
	return NULL;
}

GameEntity* EnemyList::getEnemy3() {
	for(std::vector<GameEntity*>::iterator it = enemy3List.begin(); it != enemy3List.end(); ++it) {
		EnemyComponent* eComp = (EnemyComponent*)(*it)->getComponent<EnemyComponent>();
		if (!eComp->getIsActive()) {
			eComp->setIsActive(true);
			return *it;
		}
	}
	return NULL;
}