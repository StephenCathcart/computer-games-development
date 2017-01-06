#include "CollisionSystem.h"
#include "GameSystemManager.h"
#include "GameState.h"
#include "PlayerComponent.h"
#include "PositionComponent.h"
#include "RenderComponent.h"
#include "PickUpComponent.h"
#include "EnemyComponent.h"
#include "HealthComponent.h"
#include "BulletComponent.h"
#include "PickUpComponent.h"
#include "PickUpCollisionMessage.h"
#include "EnemyCollisionMessage.h"
#include "BulletCollisionMessage.h"
#include "Settings.h"

/// <summary>
/// Initializes a new instance of the <see cref="CollisionSystem"/> class.
/// </summary>
CollisionSystem::CollisionSystem(void) {
}

/// <summary>
/// Finalizes an instance of the <see cref="CollisionSystem"/> class.
/// </summary>
CollisionSystem::~CollisionSystem(void) {
}

/// <summary>
/// Updates all entities that could collide.
/// </summary>
/// <param name="delta">The delta.</param>
/// <param name="resourceManager">The resource manager.</param>
/// <param name="entityManager">The entity manager.</param>
/// <param name="systemManager">The system manager.</param>
void CollisionSystem::update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager) {
	m_entityManager = &entityManager;
	m_systemManager = &systemManager;

	// Collision based on game state
	switch (GameState::currentState) {
	case GameState::START:
		updateStart();
		break;
	case GameState::RUN:
		if(!Settings::isPaused) {
			updateRun();
		}else {
			updatePause();
		}
		break;
	case GameState::CREDITS:
		updateCredits();
		break;
	  default:
		cout << "COLLISION is unknown" << endl;
	}
}

void CollisionSystem::updateStart() {
}

void CollisionSystem::updateRun() {
	// Player collision update
	PositionComponent* playerPosition;
	vector<GameEntity*> playerEntity = m_entityManager->getEntitiesWithComponentType<PlayerComponent>();
	if(!playerEntity.empty()) {
		if(playerEntity[0]->hasComponent<PositionComponent>()) {
			playerPosition = (PositionComponent*)playerEntity[0]->getComponent<PositionComponent>();
		}
	}else {
		return;
	}

	// Check collision with PickUps
	vector<GameEntity*> pickUpEntities = m_entityManager->getEntitiesWithComponentType<PickUpComponent>();
	for(int i = 0; i < pickUpEntities.size(); i++) {
		PickUpComponent* pComp = (PickUpComponent*)(pickUpEntities[i])->getComponent<PickUpComponent>();
		if(pComp->getIsActive()) {
			if(checkCollision(playerEntity[0], pickUpEntities[i])) {
				m_systemManager->sendMessage(&PickUpCollisionMessage(pickUpEntities[i]));
			}		
		}
	}

	// Check collision with Enemies
	vector<GameEntity*> enemyEntities = m_entityManager->getEntitiesWithComponentType<EnemyComponent>();
	for(int i = 0; i < enemyEntities.size(); i++) {
		EnemyComponent* eC = (EnemyComponent*)(enemyEntities[i])->getComponent<EnemyComponent>();
		HealthComponent* ehC = (HealthComponent*)(enemyEntities[i])->getComponent<HealthComponent>();
		if(ehC->getIsAlive() && eC->getIsActive()) {
			if(checkCollision(playerEntity[0], enemyEntities[i])) {
				m_systemManager->sendMessage(&EnemyCollisionMessage(enemyEntities[i]));
			}
		}
	}

	// Check bullet collisons
	vector<GameEntity*> bulletEntities = m_entityManager->getEntitiesWithComponentType<BulletComponent>();
	for(int i = 0; i < bulletEntities.size(); i++) {
		BulletComponent* b = (BulletComponent*)(bulletEntities[i])->getComponent<BulletComponent>();
		if(b->getIsActive()) {
			for(int j = 0; j < enemyEntities.size(); j++) {
				EnemyComponent* eC = (EnemyComponent*)(enemyEntities[i])->getComponent<EnemyComponent>();
				HealthComponent* ehC = (HealthComponent*)(enemyEntities[j])->getComponent<HealthComponent>();
				if(ehC->getIsAlive() && eC->getIsActive()) {
					if(checkCollision(enemyEntities[j], bulletEntities[i])) {
						m_systemManager->sendMessage(&BulletCollisionMessage(bulletEntities[i], enemyEntities[j]));
					}	
				}
			}
		}
	}
}

void CollisionSystem::updatePause() {
}

void CollisionSystem::updateCredits() {
}

/// <summary>
/// Checks the collision. IMPORTANT:
/// We only calculate collision on XY planes as there is no point in calculating Z
/// as the game is fixed to the XY axis.
/// </summary>
/// <param name="player">The player.</param>
/// <param name="pickup">The pickup.</param>
/// <returns></returns>
bool CollisionSystem::checkCollision(GameEntity* player, GameEntity* pickup) {
	RenderComponent* playerRender = (RenderComponent*)player->getComponent<RenderComponent>();
	RenderComponent* pickupRender = (RenderComponent*)pickup->getComponent<RenderComponent>();
	PositionComponent* playerPosition = (PositionComponent*)player->getComponent<PositionComponent>();
	PositionComponent* pickupPosition = (PositionComponent*)pickup->getComponent<PositionComponent>();

	float playerX = playerPosition->getPosX();
	float playerY = playerPosition->getPosY();
	float playerXSize = playerRender->getMesh().getXSize();
	float playerYSize = playerRender->getMesh().getYSize();

	float pickupX = pickupPosition->getPosX();
	float pickupY = pickupPosition->getPosY();
	float pickupXSize = pickupRender->getMesh().getXSize();
	float pickupYSize = pickupRender->getMesh().getYSize();

	return (abs(playerX - pickupX) * 2 < (playerXSize + pickupXSize)) &&
		   (abs(playerY - pickupY) * 2 < (playerYSize + pickupYSize));

	return true;
}

/// <summary>
/// Receives Messages from other systems.
/// </summary>
/// <param name="msg">The MSG.</param>
void CollisionSystem::receiveMessage(BaseMessage* msg) {

}