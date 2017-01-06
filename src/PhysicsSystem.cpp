#include "PhysicsSystem.h"
#include "GameSystemManager.h"
#include "PositionComponent.h"
#include "PickUpComponent.h"
#include "BulletComponent.h"
#include "RenderComponent.h"
#include "EnemyComponent.h"
#include "CrateComponent.h"
#include "HealthComponent.h"
#include "KeyEventMessage.h"
#include "JoyStickMessage.h"
#include "BulletMessage.h"
#include "PickUpMessage.h"
#include "EnemyMessage.h"
#include "GameState.h"
#include "Settings.h"
#include <cmath>

const float PhysicsSystem::ACTIVE_VELOCITY		= 1.0f;
const float PhysicsSystem::INACTIVE_VELOCITY	= 0.0f;
const float PhysicsSystem::DAMPENING			= 0.15f;
const float PhysicsSystem::MAX_ROTATION			= 25.0f;
const float PhysicsSystem::ROTATION_STEP		= 2.0f;
const float PhysicsSystem::BULLET_VELOCITY		= 0.5f;
const float PhysicsSystem::SPRING_AMOUNT		= 0.01f;
const float PhysicsSystem::MAX_X				= 127.0f;
const float PhysicsSystem::MAX_Y				= 76.0f;
const float PhysicsSystem::CRATE_SPEED			= -0.05f;
const float PhysicsSystem::CRATE_X				= 200.f;
const float PhysicsSystem::CRATE_Y				= 70.f;
const float PhysicsSystem::ENEMY_SPEED			= -0.2f;
float		PhysicsSystem::timer				= 0.0f;

/// <summary>
/// Initializes a new instance of the <see cref="PhysicsSystem" /> class.
/// </summary>
PhysicsSystem::PhysicsSystem(void) {
}


/// <summary>
/// Finalizes an instance of the <see cref="PhysicsSystem"/> class.
/// </summary>
PhysicsSystem::~PhysicsSystem(void) {
}

/// <summary>
/// Updates Entity Position and Velocity Components.
/// </summary>
/// <param name="delta">The delta.</param>
/// <param name="manager">The entity manager.</param>
/// <param name="systemManager">The system manager.</param>
void PhysicsSystem::update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager) {
	delta *= DAMPENING;
	m_entityManager = &entityManager;

	// Physics based on game state
	switch (GameState::currentState) {
		case GameState::START:
			updateStart();
			break;
		case GameState::RUN:
			if(!Settings::isPaused && !Settings::isGameOver) {
				updateRun(delta);
			}else {
				updatePause();
			}
			break;
		case GameState::CREDITS:
			updateCredits();
			break;
		  default:
			cout << "PHYSICS is unknown" << endl;
	}
}

void PhysicsSystem::updateStart() {
}

void PhysicsSystem::updateRun(float delta) {
	timer += delta;
	// Player physics update
	vector<GameEntity*> playerEntity = m_entityManager->getEntitiesWithComponentType<PlayerComponent>();
	if(!playerEntity.empty()) {
		if(playerEntity[0]->hasComponent<VelocityComponent>() && 
		   playerEntity[0]->hasComponent<PositionComponent>()) {
			PlayerComponent* player = (PlayerComponent*)playerEntity[0]->getComponent<PlayerComponent>();
			PositionComponent* position = (PositionComponent*)playerEntity[0]->getComponent<PositionComponent>();
			VelocityComponent* velocity = (VelocityComponent*)playerEntity[0]->getComponent<VelocityComponent>();

			// Set players XY position based on velocity
			position->setPosX(position->getPosX() + velocity->getVX() * delta);
			position->setPosY(position->getPosY() + velocity->getVY() * delta);

			// Cap position
			if(position->getPosX() > MAX_X) {
				position->setPosX(MAX_X);
			}
			if(position->getPosX() < -MAX_X) {
				position->setPosX(-MAX_X);
			}
			if(position->getPosY() > MAX_Y) {
				position->setPosY(MAX_Y);
			}
			if(position->getPosY() < -MAX_Y) {
				position->setPosY(-MAX_Y);
			}

			// Handle rotation of player
			if(player->getIsInclining()) {
				if(position->getRotX() < MAX_ROTATION) {
					position->setRotX(position->getRotX() + ROTATION_STEP);
				}else {
					position->setRotX(MAX_ROTATION);
				}
			}else if(player->getIsDeclining()) {
				if(position->getRotX() > -MAX_ROTATION) {
					position->setRotX(position->getRotX() - ROTATION_STEP);
				}else {
					position->setRotX(-MAX_ROTATION);
				}
			}else{
				if(position->getRotX() < ROTATION_STEP && position->getRotX() > -ROTATION_STEP) {
					position->setRotX(0.0f);
				}else if(position->getRotX() > 0) {
					position->setRotX(position->getRotX() - ROTATION_STEP);
				}else {
					position->setRotX(position->getRotX() + ROTATION_STEP);
				}
			}
		}
	} 

	// PickUp physics update
	vector<GameEntity*> pickupEntities = m_entityManager->getEntitiesWithComponentType<PickUpComponent>();
	if(!pickupEntities.empty()) {
		for(int i = 0; i < pickupEntities.size(); i++){
			PickUpComponent* pComp = (PickUpComponent*)(pickupEntities[i])->getComponent<PickUpComponent>();
			if(pComp->getIsActive()) {
				if(pickupEntities[i]->hasComponent<PositionComponent>()) {
					PositionComponent* puP = (PositionComponent*)pickupEntities[i]->getComponent<PositionComponent>();
					PositionComponent* pP = (PositionComponent*)playerEntity[0]->getComponent<PositionComponent>();
					
					// Pickup follows player
					glm::vec2 a = glm::vec2(puP->getPosX(), puP->getPosY());
					glm::vec2 b = glm::vec2(pP->getPosX(), pP->getPosY());
					glm::vec2 edge = b - a;
					glm::vec2 norm = glm::normalize(edge);
					float d = calculateDistance(puP->getPosX(), puP->getPosY(), pP->getPosX(), pP->getPosY());

					glm::vec2 result = a + norm * d;

					puP->setPosX( puP->getPosX() + (result.x * SPRING_AMOUNT) * delta);
					puP->setPosY( puP->getPosY() + (result.y * SPRING_AMOUNT) * delta);

					// Rotate pickup
					puP->setRotX(puP->getRotX() - ROTATION_STEP);
					puP->setRotY(puP->getRotY() - ROTATION_STEP);
				}
			}
		}
	}

	// Bullet physics update
	vector<GameEntity*> bulletEntities = m_entityManager->getEntitiesWithComponentType<BulletComponent>();
	for(int i = 0; i < bulletEntities.size(); i++){
		BulletComponent* b = (BulletComponent*)(bulletEntities[i])->getComponent<BulletComponent>();
		if(b->getIsActive()) {
			PositionComponent* p = (PositionComponent*)bulletEntities[i]->getComponent<PositionComponent>();
			VelocityComponent* v = (VelocityComponent*)bulletEntities[i]->getComponent<VelocityComponent>();

			// Set bullets XY position based on velocity
			p->setPosX(p->getPosX() + v->getVX() * delta);

			if(b->getBulletType() == BulletType::LINEAR) {
				p->setPosY(p->getPosY() + v->getVY() * delta);
			}else if (b->getBulletType() == BulletType::SINE) {
				float mod = sin(timer / 5);
				p->setPosY(p->getPosY() + v->getVY() + mod * delta);
			}

			if(p->getPosX() > 200.0f) {
				b->setIsActive(false);
			}
		}
	}

	// Crate physics update
	vector<GameEntity*> crateEntity = m_entityManager->getEntitiesWithComponentType<CrateComponent>();
		for(int i = 0; i < crateEntity.size(); i++){
		PositionComponent* p = (PositionComponent*)crateEntity[i]->getComponent<PositionComponent>();

		// Set bullets XY position based on velocity
		p->setPosX(p->getPosX() + CRATE_SPEED * delta);

		if(p->getPosX() < -CRATE_X) {
			p->setPosX(CRATE_X);
			float lower = -CRATE_Y, upper = CRATE_Y; 
			int r = rand(); 
			float fraction = ((float) r / RAND_MAX) * (upper - lower); 
			float x = lower + fraction; 
			p->setPosY(x);
		}
	}

	// Enemy physics update
	vector<GameEntity*> enemyEntity = m_entityManager->getEntitiesWithComponentType<EnemyComponent>();
		for(int i = 0; i < enemyEntity.size(); i++) {
		PositionComponent* eP = (PositionComponent*)enemyEntity[i]->getComponent<PositionComponent>();
		EnemyComponent* eC = (EnemyComponent*)enemyEntity[i]->getComponent<EnemyComponent>();

		// Set bullets XY position based on velocity
		eP->setPosX(eP->getPosX() + ENEMY_SPEED * delta);

		if(eP->getPosX() < -CRATE_X) {
			eP->setPosX(CRATE_X);
			eC->setIsActive(false);
		}
	}
}

float PhysicsSystem::calculateDistance(float x1, float y1, float x2, float y2) {
	float x = x1 - x2;
	float y = y1 - y2;
	return sqrt(x*x + y*y); // Urgh
}

void PhysicsSystem::updatePause() {
}

void PhysicsSystem::updateGameOver() {
}

void PhysicsSystem::updateCredits() {
}

/// <summary>
/// Receives messages from other Systems.
/// </summary>
/// <param name="msg">The MSG.</param>
void PhysicsSystem::receiveMessage(BaseMessage* msg) {
	if(GameState::currentState == GameState::RUN) {
		handlePlayerPhysics(msg);
		handleBulletPhysics(msg);
		handlePickUpPhysics(msg);
		handleEnemyPhysics(msg);
	}
}

/// <summary>
/// Handles the player physics.
/// </summary>
/// <param name="msg">The MSG.</param>
void PhysicsSystem::handlePlayerPhysics(BaseMessage* msg) {
	vector<GameEntity*> playerEntity = m_entityManager->getEntitiesWithComponentType<PlayerComponent>();
	if(!playerEntity.empty()) {
		PlayerComponent* player = (PlayerComponent*)playerEntity[0]->getComponent<PlayerComponent>();
		PositionComponent* position = (PositionComponent*)playerEntity[0]->getComponent<PositionComponent>();
		VelocityComponent* velocity = (VelocityComponent*)playerEntity[0]->getComponent<VelocityComponent>();
	
		/** KEYBOARD EVENTS **/
		if(msg->getMessageType() == MessageType::KEYEVENT) {
			KeyEventMessage* e = dynamic_cast<KeyEventMessage*>(msg);
			// Key Pressed
			if (e->getPayload().type == sf::Event::KeyPressed) {
				if (e->getPayload().key.code == sf::Keyboard::W ||
					e->getPayload().key.code == sf::Keyboard::Up) {
					setPlayerY(player, velocity, false, true, ACTIVE_VELOCITY);
				}
				if (e->getPayload().key.code == sf::Keyboard::A ||
					e->getPayload().key.code == sf::Keyboard::Left) {
					setPlayerX(velocity, -ACTIVE_VELOCITY);
				}
				if (e->getPayload().key.code == sf::Keyboard::S ||
					e->getPayload().key.code == sf::Keyboard::Down) {
					setPlayerY(player, velocity, true, false, -ACTIVE_VELOCITY);
				}
				if (e->getPayload().key.code == sf::Keyboard::D ||
					e->getPayload().key.code == sf::Keyboard::Right) {
					setPlayerX(velocity, ACTIVE_VELOCITY);
				}
			}
			// Key Released
			if(e->getPayload().type == sf::Event::KeyReleased) {
				if (e->getPayload().key.code == sf::Keyboard::W ||
					e->getPayload().key.code == sf::Keyboard::S ||
					e->getPayload().key.code == sf::Keyboard::Up ||
					e->getPayload().key.code == sf::Keyboard::Down) {
					setPlayerY(player, velocity, false, false, INACTIVE_VELOCITY);
				}
				if (e->getPayload().key.code == sf::Keyboard::A ||
					e->getPayload().key.code == sf::Keyboard::D ||
					e->getPayload().key.code == sf::Keyboard::Left ||
					e->getPayload().key.code == sf::Keyboard::Right) {
					setPlayerX(velocity, INACTIVE_VELOCITY);
				}
			}
		}

		/** JOYSTICK EVENTS **/
		if(msg->getMessageType() == MessageType::JOYEVENT) {
			JoyStickMessage* e = dynamic_cast<JoyStickMessage*>(msg);
			// JoyStick is active 
			if(e->getIsActive()) {
				if(e->getJoyButton() == JoyButton::UP) {
					setPlayerY(player, velocity, false, true, ACTIVE_VELOCITY);
				}
				if(e->getJoyButton() == JoyButton::LEFT) {
					setPlayerX(velocity, -ACTIVE_VELOCITY);
				}
				if(e->getJoyButton() == JoyButton::DOWN) {
					setPlayerY(player, velocity, true, false, -ACTIVE_VELOCITY);
				}
				if(e->getJoyButton() == JoyButton::RIGHT) {
					setPlayerX(velocity, ACTIVE_VELOCITY);
				}
			}
			// Joystick is not active
			if(!e->getIsActive()) {
				if(e->getJoyButton() == JoyButton::NULLY) {
					setPlayerY(player, velocity, false, false, INACTIVE_VELOCITY);
				}
				if (e->getJoyButton() == JoyButton::NULLX) {
					setPlayerX(velocity, INACTIVE_VELOCITY);
				}
			}
		}
	}
}

void PhysicsSystem::handleBulletPhysics(BaseMessage* msg) {
	if(msg->getMessageType() == MessageType::BULLET) {
		BulletMessage* bE = dynamic_cast<BulletMessage*>(msg);
		if(bE == NULL) {
			return;
		}
		vector<GameEntity*> playerEntity = m_entityManager->getEntitiesWithComponentType<PlayerComponent>();
		PositionComponent* pP	= (PositionComponent*)playerEntity[0]->getComponent<PositionComponent>();
		RenderComponent* pR		= (RenderComponent*)playerEntity[0]->getComponent<RenderComponent>();

		float h = pR->getMesh().getYSize();
		float w = pR->getMesh().getXSize();

		PositionComponent* bP = (PositionComponent*)(bE->getEntity())->getComponent<PositionComponent>();
		bP->setPosX(pP->getPosX() + w / 2);
		bP->setPosY(pP->getPosY() - (h / 4));
	}
}

void PhysicsSystem::handlePickUpPhysics(BaseMessage* msg) {
	if(msg->getMessageType() == MessageType::PICKUP) {
		PickUpMessage* bE = dynamic_cast<PickUpMessage*>(msg);
		if(bE == NULL) {
			return;
		}

		PositionComponent* eP	= (PositionComponent*)(bE->getEnemyEntity())->getComponent<PositionComponent>();
		PositionComponent* pP	= (PositionComponent*)(bE->getPickUpEntity())->getComponent<PositionComponent>();
		PickUpComponent* puP	= (PickUpComponent*)(bE->getPickUpEntity())->getComponent<PickUpComponent>();

		pP->setPosX(eP->getPosX());
		pP->setPosY(eP->getPosY());
	}
}

void PhysicsSystem::handleEnemyPhysics(BaseMessage* msg) {
	if(msg->getMessageType() == MessageType::ENEMY) {
		EnemyMessage* bE = dynamic_cast<EnemyMessage*>(msg);
		PositionComponent* eP = (PositionComponent*)(bE->getEntity())->getComponent<PositionComponent>();
		HealthComponent* eH = (HealthComponent*)(bE->getEntity())->getComponent<HealthComponent>();

		eH->setCurrentHP( eH->getMaxHP() );
		eP->setPosX(CRATE_X);

		float lower = -CRATE_Y, upper = CRATE_Y; 
		int r = rand(); 
		float fraction = ((float) r / RAND_MAX) * (upper - lower); 
		float x = lower + fraction; 
		eP->setPosY(x);
	}
}

/// <summary>
/// Sets the players Y Velocity & Rotation.
/// </summary>
/// <param name="p">The p.</param>
/// <param name="v">The v.</param>
/// <param name="isInclining">if set to <c>true</c> [is inclining].</param>
/// <param name="isDeclining">if set to <c>true</c> [is declining].</param>
/// <param name="vY">The v y.</param>
void PhysicsSystem::setPlayerY(PlayerComponent* p, VelocityComponent* v, bool isInclining, bool isDeclining, float vY) {
	p->setIsInclining(isInclining);
	p->setIsDeclining(isDeclining);
	v->setVY(vY);
}

/// <summary>
/// Sets the player X Velocity & Rotation.
/// </summary>
/// <param name="v">The v.</param>
/// <param name="vX">The v x.</param>
void PhysicsSystem::setPlayerX(VelocityComponent* v, float vX) {
	v->setVX(vX);
}