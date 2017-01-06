#include "LogicSystem.h"
#include "Paths.h"
#include "KeyEventMessage.h"
#include "JoyStickMessage.h"
#include "BulletCollisionMessage.h"
#include "EnemyCollisionMessage.h"
#include "LoadingMessage.h"
#include "HighScoreMessage.h"
#include "BulletMessage.h"
#include "PickUpMessage.h"
#include "ControlsMessage.h"
#include "BombActiveMessage.h"
#include "TextComponent.h"
#include "MultiplierComponent.h"
#include "CrateComponent.h"
#include "HealthComponent.h"
#include "RenderComponent.h"
#include "EnemyComponent.h"
#include "PickUpComponent.h"
#include "ScoreComponent.h"
#include "SFRenderComponent.h"
#include "MenuItemComponent.h"
#include "PositionComponent.h"
#include "BombComponent.h"
#include "BulletComponent.h"
#include "ShieldComponent.h"
#include "EnemyTimer.h"
#include "Settings.h"

#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

const float	LogicSystem::BORDER_OFFSET = 20.0f;
const int	LogicSystem::TRAILING_ZEROS = 10;

// As game is capped to XY coordinates, set default Z value
const float	LogicSystem::GAME_Z_DISTANCE = -200.0f;

/// <summary>
/// Initializes a new instance of the <see cref="LogicSystem" /> class.
/// </summary>
/// <param name="window">The window.</param>
LogicSystem::LogicSystem(sf::RenderWindow &window) : m_window(window) {
	m_previousUp		= false;
	m_previousDown		= false;	
	m_bombSound			= new sf::Sound;
	m_shootSound		= new sf::Sound;
	m_menuUpSound		= new sf::Sound;
	m_menuDownSound		= new sf::Sound;
	m_music				= new sf::Music;
	m_hasLoaded			= false;
	m_isShooting		= false;
	m_isBombing			= false;
	m_score				= 0;
	m_multiplier		= 0.00f,
	m_menuItemSelected	= 0;
	m_startState		= StartState::DEFAULT;

	/* initialize random seed: */
	srand(time(NULL));
}

/// <summary>
/// Finalizes an instance of the <see cref="LogicSystem"/> class.
/// </summary>
LogicSystem::~LogicSystem(void) {
	delete m_shootSound;
	delete m_bombSound;
	delete m_menuUpSound;
	delete m_menuDownSound;
	delete m_music;
}

/// <summary>
/// Updates various gameplay logic.
/// </summary>
/// <param name="delta">The delta.</param>
/// <param name="manager">The manager.</param>
/// <param name="systemManager">The system manager.</param>
void LogicSystem::update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager) {
	m_resourceManager	= &resourceManager;
	m_systemManager		= &systemManager;
	m_entityManager		= &entityManager;
	
	switch (GameState::currentState) {
		case GameState::START:
			if(GameState::currentState != GameState::previousState) {
				GameState::previousState = GameState::currentState;
				initialiseStart();
			}
			updateStart();
			break;
		case GameState::RUN:
			if(GameState::currentState != GameState::previousState) {
				GameState::previousState = GameState::currentState;
				initialiseRun();
			}
			if(Settings::isGameOver) {
				updateGameOver();
			}else if(!Settings::isPaused) {
				updateRun(delta);
			} else {
				updatePause();
			}
			break;
		case GameState::CREDITS:
			break;
		  default:
			cout << "STATE is unknown" << endl;
	}
}

void LogicSystem::updateMenuItems(bool isUp) {
	if(Settings::hasSound) {
		if(isUp) {
			m_menuUpSound->play();
		} else  {
			m_menuDownSound->play();
		}
	}
	int min = 0;
	int max = 0;
	if(m_startState == StartState::DEFAULT) {
		min = 0;
		max = 4;
	} else if(m_startState == StartState::OPTIONS) {
		min = 0;
		max = 1;
	} else if(m_startState == StartState::HIGHSCORES) {
		min = 0;
		max = 0;
	}

	if(Settings::isPaused || Settings::isGameOver) {
		min = 0;
		max = 1;
	}

	if(isUp) {
		m_menuItemSelected = --m_menuItemSelected < min ? max : m_menuItemSelected;
	}else {
		m_menuItemSelected = ++m_menuItemSelected > max ? min : m_menuItemSelected;
	}

	vector<GameEntity*> menuItemEntities = m_entityManager->getEntitiesWithComponentType<MenuItemComponent>();
	for (vector<GameEntity*>::iterator i = menuItemEntities.begin(); i != menuItemEntities.end(); ++i) {
		MenuItemComponent* menuItemComponent = (MenuItemComponent*)(*i)->getComponent<MenuItemComponent>();
		menuItemComponent->setIsActive(false);
	}
	MenuItemComponent* menuItemComponent = (MenuItemComponent*)menuItemEntities[m_menuItemSelected]->getComponent<MenuItemComponent>();
	menuItemComponent->setIsActive(true);
}

void LogicSystem::handlePause() {
	Settings::isPaused = !Settings::isPaused;
	if(Settings::isPaused) {
		startPause();
	} else {
		unPause();
	}
}

void LogicSystem::handleGameOver() {
	m_menuItemSelected = 0;
	addGameOverEntities();
}

void LogicSystem::startPause() {
	m_menuItemSelected = 0;
	addPauseEntities();
}

void LogicSystem::unPause() {
	removePauseEntities();
}

void LogicSystem::addGameOverEntities() {
	/** Create replay entity **/
	GameEntity* replayEntity = new GameEntity;
	replayEntity->addComponent(new MenuItemComponent(true));
	TextComponent* replayTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_RESTART);
	replayTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2));
	replayEntity->addComponent(replayTextComponent);
	/**	End replay entity **/

	/** Create exit entity **/
	GameEntity* exitEntity	= new GameEntity;
	exitEntity->addComponent(new MenuItemComponent(false));
	TextComponent* exitTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_EXIT);
	exitTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 3));
	exitEntity->addComponent(exitTextComponent);
	/**	End exit entity **/

	m_entityManager->addEntity(replayEntity);
	m_entityManager->addEntity(exitEntity);
}

void LogicSystem::addPauseEntities() {
	/** Create resume entity **/
	GameEntity* resumeEntity = new GameEntity;
	resumeEntity->addComponent(new MenuItemComponent(true));
	TextComponent* resumeTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_RESUME);
	resumeTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2));
	resumeEntity->addComponent(resumeTextComponent);
	/**	End start entity **/

	/** Create exit entity **/
	GameEntity* exitEntity	= new GameEntity;
	exitEntity->addComponent(new MenuItemComponent(false));
	TextComponent* exitTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_EXIT);
	exitTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 3));
	exitEntity->addComponent(exitTextComponent);
	/**	End start entity **/

	m_entityManager->addEntity(resumeEntity);
	m_entityManager->addEntity(exitEntity);
}

void LogicSystem::removePauseEntities() {
	vector<GameEntity*> menuItemEntities = m_entityManager->getEntitiesWithComponentType<MenuItemComponent>();
	for (vector<GameEntity*>::iterator i = menuItemEntities.begin(); i != menuItemEntities.end(); ++i) {
		m_entityManager->removeEntity(*i);
	}
}

void LogicSystem::handleEnter() {
	// Default screen
	if(m_startState == StartState::DEFAULT) {
		switch (m_menuItemSelected) {
			case 0:
				startGame();
				break;
			case 1:
				startHighScores();
				break;
			case 2:
				startOptions();
				break;
			case 3:
				startControls();
				break;
			case 4:
				m_systemManager->setIsActive(false);
				break;
		}
	}else if(m_startState == StartState::OPTIONS) {
		switch (m_menuItemSelected) {
			case 0:
				switchSound();
				break;
			case 1:
				Settings::saveHighscores(false, m_score);
				showDefault();
				break;
		}
	}else if(m_startState == StartState::HIGHSCORES) {
		switch (m_menuItemSelected) {
			case 0:
				m_menuItemSelected = 1;
				m_systemManager->sendMessage(&HighScoreMessage(false));
				showDefault();
				break;
		}
	}else if(m_startState == StartState::CONTROLS) {
		switch (m_menuItemSelected) {
			case 0:
				m_menuItemSelected = 1;
				m_systemManager->sendMessage(&ControlsMessage(false));
				showDefault();
				break;
		}
	}
}

void LogicSystem::handlePauseEnter() {
	switch (m_menuItemSelected) {
		case 0:
			handlePause();
			break;
		case 1:
			quitToMenu();
			break;
	}
}

void LogicSystem::handleGameOverEnter() {
	switch (m_menuItemSelected) {
		case 0:
			startGame();
			break;
		case 1:
			quitToMenu();
			break;
	}
}

void LogicSystem::switchSound() {
	Settings::hasSound = !Settings::hasSound;
	Settings::hasSound ? m_music->play() : m_music->pause();
	updateSoundText();
}

void LogicSystem::updateSoundText() {
	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();
	TextComponent* textComponent = (TextComponent*)textEntities[m_menuItemSelected]->getComponent<TextComponent>();

	string boolStr = Settings::hasSound ? " Yes" :  " No";
	string soundStr = Paths::STR_SOUND;
	string newStr = soundStr.append(boolStr);
	textComponent->getText().setString(newStr);
}

void LogicSystem::showDefault() {
	m_startState = StartState::DEFAULT;
	updateMenuItems(true);
	addDefaultEntities();
}

void LogicSystem::startGame() {
	clearResources();
	Settings::isPaused = false;
	Settings::isGameOver = false;
	m_hasLoaded = false;
	m_score = 0;
	m_multiplier = 0;
	GameState::previousState	= GameState::START;
	GameState::currentState		= GameState::RUN;
}

void LogicSystem::quitToMenu() {
	clearResources();
	m_startState = StartState::DEFAULT;
	Settings::isPaused = false;
	Settings::isGameOver = false;
	m_menuItemSelected = 0;
	m_hasLoaded = false;
	GameState::previousState	= GameState::RUN;
	GameState::currentState		= GameState::START;
}

void LogicSystem::startHighScores() {
	m_startState = StartState::HIGHSCORES;
	m_menuItemSelected = 1;
	updateMenuItems(true);
	addHighScoreEntities();
	m_systemManager->sendMessage(&HighScoreMessage(true));
}

void LogicSystem::startControls() {
	m_startState = StartState::CONTROLS;
	m_menuItemSelected = 1;
	updateMenuItems(true);
	addHighScoreEntities();
	m_systemManager->sendMessage(&ControlsMessage(true));
}

void LogicSystem::startOptions() {
	m_startState = StartState::OPTIONS;
	m_menuItemSelected = 1;
	updateMenuItems(true);
	addOptionsEntities();
	updateSoundText();
}

void LogicSystem::addHighScoreEntities() {
	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		m_entityManager->removeEntity(*i);
	}

	/** Create back entity **/
	GameEntity* backEntity	= new GameEntity;
	backEntity->addComponent(new MenuItemComponent(true));
	TextComponent* backTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_BACK);
	backTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2));
	backEntity->addComponent(backTextComponent);
	/**	End back entity **/

	m_entityManager->addEntity(backEntity);
}

void LogicSystem::addOptionsEntities() {
	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		m_entityManager->removeEntity(*i);
	}

	/** Create sound entity **/
	GameEntity* soundEntity	= new GameEntity;
	soundEntity->addComponent(new MenuItemComponent(true));
	TextComponent* soundTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_SOUND);
	soundTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2));
	soundEntity->addComponent(soundTextComponent);
	/**	End start entity **/

	/** Create back entity **/
	GameEntity* backEntity	= new GameEntity;
	backEntity->addComponent(new MenuItemComponent(false));
	TextComponent* backTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_BACK);
	backTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 3));
	backEntity->addComponent(backTextComponent);
	/**	End start entity **/

	m_entityManager->addEntity(soundEntity);
	m_entityManager->addEntity(backEntity);
}

void LogicSystem::addDefaultEntities() {
	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		m_entityManager->removeEntity(*i);
	}

	/** Create start entity **/
	GameEntity* startEntity	= new GameEntity;
	startEntity->addComponent(new MenuItemComponent(true));
	TextComponent* startTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_START);
	startTextComponent->getText().setPosition(BORDER_OFFSET, m_window.getSize().y / 2);
	startEntity->addComponent(startTextComponent);
	/**	End start entity **/

	/** Create highscore entity **/
	GameEntity* highscoreEntity	= new GameEntity;
	highscoreEntity->addComponent(new MenuItemComponent(false));
	TextComponent* highscoreTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_HIGHSCORES);
	highscoreTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 3));
	highscoreEntity->addComponent(highscoreTextComponent);
	/**	End start entity **/

	/** Create options entity **/
	GameEntity* optionsEntity	= new GameEntity;
	optionsEntity->addComponent(new MenuItemComponent(false));
	TextComponent* optionsTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_OPTIONS);
	optionsTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 6));
	optionsEntity->addComponent(optionsTextComponent);
	/**	End start entity **/

	/** Create control entity **/
	GameEntity* controlEntity	= new GameEntity;
	controlEntity->addComponent(new MenuItemComponent(false));
	TextComponent* controlTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_CONTROLS);
	controlTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 9));
	controlEntity->addComponent(controlTextComponent);
	/**	End start entity **/

	/** Create exit entity **/
	GameEntity* exitEntity	= new GameEntity;
	exitEntity->addComponent(new MenuItemComponent(false));
	TextComponent* exitTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_EXIT);
	exitTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 12));
	exitEntity->addComponent(exitTextComponent);
	/**	End start entity **/

	m_entityManager->addEntity(startEntity);
	m_entityManager->addEntity(highscoreEntity);
	m_entityManager->addEntity(optionsEntity);
	m_entityManager->addEntity(controlEntity);
	m_entityManager->addEntity(exitEntity);
}

void LogicSystem::clearResources() {
	// CLEAR RESOURCES
	m_resourceManager->clear();

	// CLEAR ENTITIES
	m_entityManager->clear();

	// Clear resource lists
	BulletList::clear();
	PickUpList::clear();
	EnemyList::clear();
	EnemyTimer::restart();
}

/// <summary>
/// Receives Messages from other Systems.
/// </summary>
/// <param name="msg">The MSG.</param>
void LogicSystem::receiveMessage(BaseMessage* msg) {
	switch (GameState::currentState) {
		case GameState::START:
			receiveStartMessage(msg);
			break;
		case GameState::RUN:
			receiveRunMessage(msg);
			break;
		case GameState::CREDITS:
			break;
		  default:
			cout << "LOGIC MESSAGE STATE is unknown" << endl;
	}
}

void LogicSystem::receiveStartMessage(BaseMessage* msg) {
	/** KEYBOARD EVENTS **/
	if(msg->getMessageType() == MessageType::KEYEVENT) {
		KeyEventMessage* e = dynamic_cast<KeyEventMessage*>(msg);

		// Close by clicking window
		if(e->getPayload().type == sf::Event::Closed) {
			m_systemManager->setIsActive(false);
		}
		// Key Pressed
		if (e->getPayload().type == sf::Event::KeyPressed) {
			if (e->getPayload().key.code == sf::Keyboard::Escape) {
				m_systemManager->setIsActive(false);
			}
			if (e->getPayload().key.code == sf::Keyboard::W ||
				e->getPayload().key.code == sf::Keyboard::Up) {
				updateMenuItems(true);
			}
			if (e->getPayload().key.code == sf::Keyboard::S ||
				e->getPayload().key.code == sf::Keyboard::Down) {
				updateMenuItems(false);
			}
			if (e->getPayload().key.code == sf::Keyboard::Space ||
				e->getPayload().key.code == sf::Keyboard::Return) {
				handleEnter();
			}
		}
	}
	/** JOYSTICK EVENTS **/
	if(msg->getMessageType() == MessageType::JOYEVENT) {
		JoyStickMessage* e = dynamic_cast<JoyStickMessage*>(msg);
		// JoyStick is active 
		if(e->getIsActive()) {
			if(e->getJoyButton() == JoyButton::UP && !m_previousUp) {
				updateMenuItems(true);
				m_previousUp = true;
			}
			if(e->getJoyButton() == JoyButton::DOWN && !m_previousDown) {
				updateMenuItems(false);
				m_previousDown = true;
			}
			if(e->getJoyButton() == JoyButton::A) {
				// Play shooting sound
				handleEnter();
			}
		}else {
			if(e->getJoyButton() == JoyButton::UP) {
				m_previousUp = false;
			}
			if(e->getJoyButton() == JoyButton::DOWN) {
				m_previousDown = false;
			}
		}
	}
}

void LogicSystem::receiveRunMessage(BaseMessage* msg) {
	/** JOYSTICK EVENTS **/
	if(msg->getMessageType() == MessageType::JOYEVENT) {
		JoyStickMessage* e = dynamic_cast<JoyStickMessage*>(msg);
		// JoyStick is active
		if(e->getIsActive()) {
			if(e->getJoyButton() == JoyButton::A) {
				// Play shooting sound
				m_isShooting = true;
			}
			if(e->getJoyButton() == JoyButton::X) {
				m_isBombing = true;
				updateBomb(false);
			}
			if(e->getJoyButton() == JoyButton::START) {
				handlePause();
			}
			if(Settings::isPaused) {
				if(e->getJoyButton() == JoyButton::UP && !m_previousUp) {
					updateMenuItems(true);
					m_previousUp = true;
				}
				if(e->getJoyButton() == JoyButton::DOWN && !m_previousDown) {
					updateMenuItems(false);
					m_previousDown = true;
				}
				if(e->getJoyButton() == JoyButton::A) {
					handlePauseEnter();
				}
			}
			if(Settings::isGameOver) {
				if (e->getJoyButton() == JoyButton::UP && !m_previousUp) {
					updateMenuItems(true);
					m_previousUp = true;
				}
				if (e->getJoyButton() == JoyButton::DOWN && !m_previousDown) {
					updateMenuItems(false);
					m_previousDown = true;
				}
				if (e->getJoyButton() == JoyButton::A) {
					handleGameOverEnter();
				}
			}
		}else {
			if(e->getJoyButton() == JoyButton::A) {
				// Stop shooting sound
				m_isShooting = false;
			}
			if(e->getJoyButton() == JoyButton::X) {
				m_isBombing = false;
			}
			if(Settings::isPaused || Settings::isGameOver) {
				if(e->getJoyButton() == JoyButton::UP) {
					m_previousUp = false;
				}
				if(e->getJoyButton() == JoyButton::DOWN) {
					m_previousDown = false;
				}
			}
		}
	}
	/** KEYBOARD EVENTS **/
	if(msg->getMessageType() == MessageType::KEYEVENT) {
		KeyEventMessage* e = dynamic_cast<KeyEventMessage*>(msg);
		// Close by clicking window
		if(e->getPayload().type == sf::Event::Closed) {
			m_systemManager->setIsActive(false);
		}
		if (e->getPayload().type == sf::Event::KeyPressed) {
			if (e->getPayload().key.code == sf::Keyboard::Space) {
				m_isShooting = true;
			}
			if(e->getPayload().key.code == sf::Keyboard::E) {
				m_isBombing = true;
				updateBomb(false);
			}
			if(Settings::isPaused) {
				if (e->getPayload().key.code == sf::Keyboard::W ||
					e->getPayload().key.code == sf::Keyboard::Up) {
					updateMenuItems(true);
				}
				if (e->getPayload().key.code == sf::Keyboard::S ||
					e->getPayload().key.code == sf::Keyboard::Down) {
					updateMenuItems(false);
				}
				if (e->getPayload().key.code == sf::Keyboard::Space ||
					e->getPayload().key.code == sf::Keyboard::Return) {
					handlePauseEnter();
				}
			}
			if(Settings::isGameOver) {
				if (e->getPayload().key.code == sf::Keyboard::W ||
					e->getPayload().key.code == sf::Keyboard::Up) {
					updateMenuItems(true);
				}
				if (e->getPayload().key.code == sf::Keyboard::S ||
					e->getPayload().key.code == sf::Keyboard::Down) {
					updateMenuItems(false);
				}
				if (e->getPayload().key.code == sf::Keyboard::Space ||
					e->getPayload().key.code == sf::Keyboard::Return) {
					handleGameOverEnter();
				}
			}
			if (e->getPayload().key.code == sf::Keyboard::Escape && !Settings::isGameOver) {
				handlePause();
			}
		}
		if (e->getPayload().type == sf::Event::KeyReleased) {
			if (e->getPayload().key.code == sf::Keyboard::Space) {
				m_isShooting = false;
			}
			if (e->getPayload().key.code == sf::Keyboard::E) {
				m_isBombing = false;
			}
		}
	}
	/** Updates score **/
	if(msg->getMessageType() == MessageType::PICKUPCOLLISION) {
		handlePickUpMessage(msg);
	}

	/** Player Enemy collision **/
	if(msg->getMessageType() == MessageType::ENEMYCOLLISION) {
		vector<GameEntity*> playerEntity = m_entityManager->getEntitiesWithComponentType<PlayerComponent>();
		
		EnemyCollisionMessage* e = dynamic_cast<EnemyCollisionMessage*>(msg);
		EnemyComponent* eC = (EnemyComponent*)(e->getEntity())->getComponent<EnemyComponent>();
		HealthComponent* ehC = (HealthComponent*)(e->getEntity())->getComponent<HealthComponent>();
		HealthComponent* phC = (HealthComponent*)(playerEntity[0])->getComponent<HealthComponent>();

		// DROP PICKUP
		dropPickUp(e->getEntity());

		eC->setIsActive(false);
		ehC->setCurrentHP(0);
		phC->setCurrentHP(phC->getCurrentHP() - 1);
		
		// RESET MULTIPLIER
		updateMultiplier(-m_multiplier);
		updateScore(Paths::ENEMYDEATHPOINT);
	}

	/** Bullet enemy collision **/
	if(msg->getMessageType() == MessageType::BULLETCOLLISION) {
		BulletCollisionMessage* e = dynamic_cast<BulletCollisionMessage*>(msg);
		BulletComponent* bC = (BulletComponent*)(e->getBulletEntity())->getComponent<BulletComponent>();
		EnemyComponent* eC = (EnemyComponent*)(e->getEnemyEntity())->getComponent<EnemyComponent>();
		HealthComponent* ehC = (HealthComponent*)(e->getEnemyEntity())->getComponent<HealthComponent>();
		PositionComponent* epC = (PositionComponent*)(e->getEnemyEntity())->getComponent<PositionComponent>();

		bC->setIsActive(false);
		ehC->setCurrentHP( ehC->getCurrentHP() - bC->getDamage());

		updateScore(Paths::ENEMYCOLLISIONPOINT);
		updateMultiplier(0.01f);
		if(!ehC->getIsAlive()) {
			// DROP PICKUP
			dropPickUp(e->getEnemyEntity());

			epC->setPosX(200.0f);
			updateScore(Paths::ENEMYDEATHPOINT);
			updateMultiplier(0.2f);
		}
	}

	/** Updates players used bomb **/
	if(msg->getMessageType() == MessageType::BOMBUSED) {
		updateScore(Paths::BOMBPOINT);
		// Check collision with Enemies
		vector<GameEntity*> enemyEntities = m_entityManager->getEntitiesWithComponentType<EnemyComponent>();
		for(int i = 0; i < enemyEntities.size(); i++) {
			EnemyComponent* eC = (EnemyComponent*)(enemyEntities[i])->getComponent<EnemyComponent>();
			HealthComponent* ehC = (HealthComponent*)(enemyEntities[i])->getComponent<HealthComponent>();
			if(ehC->getIsAlive() && eC->getIsActive()) {
				ehC->setCurrentHP( ehC->getCurrentHP() - Paths::BOMBDAMAGE );
				if(!ehC->getIsAlive()) {
					updateScore(Paths::ENEMYDEATHPOINT);

					// DROP PICKUP
					dropPickUp(enemyEntities[i]);
				}
			}
		}
		// RESET MULTIPLIER
		updateMultiplier(-m_multiplier);
	}
}

void LogicSystem::dropPickUp(GameEntity* enemy) {
	// Roll chance to drop
	bool shouldDrop = false;
	int r = rand() % 10;
	if(r >=0 && r <= 2) {
		shouldDrop = true;
	}
	if(shouldDrop) {
		m_systemManager->sendMessage(&PickUpMessage(PickUpList::getPickUp(), enemy));
	}
}

void LogicSystem::handlePickUpMessage(BaseMessage* msg) {
	PickUpCollisionMessage* e = dynamic_cast<PickUpCollisionMessage*>(msg);
	PickUpComponent* p = (PickUpComponent*)(e->getEntity())->getComponent<PickUpComponent>();

	switch (p->getPickUpType()) {
		case PickUpType::POINTS: 
			pickUpScore(e);
			break;
		case PickUpType::BOMB:
			pickUpBomb(e);
			break;
		case PickUpType::SHIELD:
			pickUpShield(e);
			break;
		case PickUpType::WEAPON:
			break;
		  default:
			cout << "PICKUP TYPE is unknown" << endl;
	}

	// DELETE HERE
	p->setIsActive(false);
}

void LogicSystem::pickUpScore(PickUpCollisionMessage* e) {
	ScoreComponent* s = (ScoreComponent*)e->getEntity()->getComponent<ScoreComponent>();
	updateScore(s->getValue());
}

void LogicSystem::pickUpBomb(PickUpCollisionMessage* e) {
	vector<GameEntity*> bombEntities = m_entityManager->getEntitiesWithComponentType<BombComponent>();
	BombComponent*	bC = (BombComponent*)(bombEntities[0])->getComponent<BombComponent>();
	bC->increment();
}

void LogicSystem::pickUpShield(PickUpCollisionMessage* e) {
	vector<GameEntity*> playerEntities = m_entityManager->getEntitiesWithComponentType<PlayerComponent>();
	HealthComponent* hC = (HealthComponent*)(playerEntities[0])->getComponent<HealthComponent>();
	hC->setCurrentHP( hC->getCurrentHP() + 1 );
}

void LogicSystem::pickUpWeapon(PickUpCollisionMessage* e) {

}

void LogicSystem::updateScore(int points) {
	float multiplierExtra = points * m_multiplier;
	m_score += ((float)points + multiplierExtra);
}

void LogicSystem::updateMultiplier(float amount) {
	m_multiplier += amount;

	if(m_multiplier > 10.00f) {
		m_multiplier = 10.00f;
	}
}

void LogicSystem::initialiseLoading() {

}

void LogicSystem::initialiseStart() {
	updateLoading(0);

	// RESOURCES
	m_resourceManager->addTexture(Paths::SFTEX_SPLASH);
	updateLoading(5);
	m_resourceManager->addSoundBuffer(Paths::SOUND_UP);
	updateLoading(10);
	m_resourceManager->addSoundBuffer(Paths::SOUND_DOWN);
	updateLoading(15);
	m_resourceManager->loadMusic(Paths::MUSIC_STARSHIP);
	updateLoading(20);
	m_menuUpSound->setBuffer(*m_resourceManager->getSoundBuffer(Paths::SOUND_UP));
	updateLoading(25);
	m_menuDownSound->setBuffer(*m_resourceManager->getSoundBuffer(Paths::SOUND_DOWN));
	updateLoading(30);

	// ENTITIES
	/** Create splash entity **/
	GameEntity* splashEntity = new GameEntity;
	splashEntity->addComponent(new SFRenderComponent(*m_resourceManager->getTexture(Paths::SFTEX_SPLASH)));
	/**	End splash entity **/
	updateLoading(40);

	/** Create start entity **/
	GameEntity* startEntity	= new GameEntity;
	startEntity->addComponent(new MenuItemComponent(true));
	TextComponent* startTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_START);
	startTextComponent->getText().setPosition(BORDER_OFFSET, m_window.getSize().y / 2);
	startEntity->addComponent(startTextComponent);
	/**	End start entity **/
	updateLoading(45);

	/** Create highscore entity **/
	GameEntity* highscoreEntity	= new GameEntity;
	highscoreEntity->addComponent(new MenuItemComponent(false));
	TextComponent* highscoreTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_HIGHSCORES);
	highscoreTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 3));
	highscoreEntity->addComponent(highscoreTextComponent);
	/**	End start entity **/
	updateLoading(55);

	/** Create options entity **/
	GameEntity* optionsEntity	= new GameEntity;
	optionsEntity->addComponent(new MenuItemComponent(false));
	TextComponent* optionsTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_OPTIONS);
	optionsTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 6));
	optionsEntity->addComponent(optionsTextComponent);
	/**	End start entity **/
	updateLoading(65);

	/** Create control entity **/
	GameEntity* controlEntity	= new GameEntity;
	controlEntity->addComponent(new MenuItemComponent(false));
	TextComponent* controlTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_CONTROLS);
	controlTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 9));
	controlEntity->addComponent(controlTextComponent);
	/**	End start entity **/
	updateLoading(67);

	/** Create exit entity **/
	GameEntity* exitEntity	= new GameEntity;
	exitEntity->addComponent(new MenuItemComponent(false));
	TextComponent* exitTextComponent = new TextComponent(Paths::FONT_STAR, Paths::STR_EXIT);
	exitTextComponent->getText().setPosition(BORDER_OFFSET, (m_window.getSize().y / 2) + (BORDER_OFFSET * 12));
	exitEntity->addComponent(exitTextComponent);
	/**	End start entity **/
	updateLoading(70);

	m_entityManager->addEntity(splashEntity);
	updateLoading(75);
	m_entityManager->addEntity(startEntity);
	updateLoading(80);
	m_entityManager->addEntity(highscoreEntity);
	updateLoading(85);
	m_entityManager->addEntity(optionsEntity);
	updateLoading(90);
	m_entityManager->addEntity(controlEntity);
	updateLoading(92);
	m_entityManager->addEntity(exitEntity);
	updateLoading(95);
	
	// START MUSIC
	m_music = &m_resourceManager->getMusic();
	m_music->setLoop(true);
	if(Settings::hasSound) {
		m_music->play();
	}
	updateLoading(100);
}

void LogicSystem::initialiseRun() { 
	updateLoading(0);

	// RESOURCES
	m_resourceManager->addTexture(Paths::SFTEX_BG);
	updateLoading(1);
	m_resourceManager->addTexture(Paths::SFTEX_SHIELD);
	updateLoading(2);
	m_resourceManager->addTexture(Paths::SFTEX_BOMB);
	updateLoading(5);
	m_resourceManager->addObjMesh(Paths::OBJ_SHIP_PLAYER	, Paths::TEX_SHIP_PLAYER);
	updateLoading(10);
	m_resourceManager->addObjMesh(Paths::OBJ_BULLET			, "");
	updateLoading(15);
	m_resourceManager->addObjMesh(Paths::OBJ_SHIP_LARGE		, Paths::TEX_SHIP_LARGE);
	updateLoading(25);
	m_resourceManager->addObjMesh(Paths::OBJ_SHIP_MEDIUM	, Paths::TEX_SHIP_MEDIUM);
	updateLoading(25);
	m_resourceManager->addObjMesh(Paths::OBJ_SHIP_SMALL		, Paths::TEX_SHIP_SMALL);
	updateLoading(25);
	m_resourceManager->addObjMesh(Paths::OBJ_CRATE			, Paths::TEX_CRATE);
	updateLoading(40);
	m_resourceManager->addObjMesh(Paths::OBJ_CRYSTAL		, Paths::TEX_CRYSTAL);
	updateLoading(55);

	// Load shaders
	m_resourceManager->addShader(Paths::SHADER_BASIC, Paths::SHADER_VERT_BASIC, Paths::SHADER_FRAG_BASIC);
	updateLoading(57);
	// Load shaders
	m_resourceManager->addShader(Paths::SHADER_BULLET, Paths::SHADER_VERT_BULLET, Paths::SHADER_FRAG_BULLET);
	updateLoading(58);

	// Load sound buffers
	m_resourceManager->addSoundBuffer(Paths::SOUND_LASER);
	updateLoading(59);
	m_resourceManager->addSoundBuffer(Paths::SOUND_BOMB);
	updateLoading(61);
	m_resourceManager->addSoundBuffer(Paths::SOUND_UP);
	updateLoading(63);
	m_resourceManager->addSoundBuffer(Paths::SOUND_DOWN);
	updateLoading(64);
	m_menuUpSound->setBuffer(*m_resourceManager->getSoundBuffer(Paths::SOUND_UP));
	updateLoading(67);
	m_menuDownSound->setBuffer(*m_resourceManager->getSoundBuffer(Paths::SOUND_DOWN));
	updateLoading(70);

	// Load music
	m_resourceManager->loadMusic(Paths::MUSIC_SONG);
	updateLoading(71);

	// ENTITIES
	/** Create bulletlist to store bullets **/
	BulletList bulletList;
	bulletList.create(m_window, *m_entityManager, *m_resourceManager, 30);
	updateLoading(72);

	/** Create pickuplist to store pickups **/
	PickUpList pickUpList;
	pickUpList.create(m_window, *m_entityManager, *m_resourceManager, 10);
	updateLoading(73);

	/** Create enemylist to store enemys **/
	EnemyList enemyList;
	enemyList.create(m_window, *m_entityManager, *m_resourceManager, 6);
	updateLoading(74);

	/** Create shieldIcon entity **/
	GameEntity* shieldIconEntity = new GameEntity;
	float w = m_resourceManager->getTexture(Paths::SFTEX_BOMB)->getSize().x;
	shieldIconEntity->addComponent(new PositionComponent(glm::vec3(BORDER_OFFSET + (w * 3), BORDER_OFFSET, 0.0f)));
	shieldIconEntity->addComponent(new SFRenderComponent(*m_resourceManager->getTexture(Paths::SFTEX_SHIELD)));
	/**	End shieldIcon entity **/
	updateLoading(75);

	/** Create bg entity **/
	GameEntity* bgEntity = new GameEntity;
	bgEntity->addComponent(new SFRenderComponent(*m_resourceManager->getTexture(Paths::SFTEX_BG)));
	/**	End bg entity **/
	updateLoading(76);

	/** Create bombIcon entity **/
	GameEntity* bombIconEntity = new GameEntity;
	bombIconEntity->addComponent(new PositionComponent(glm::vec3(BORDER_OFFSET, BORDER_OFFSET, 0.0f)));
	bombIconEntity->addComponent(new SFRenderComponent(*m_resourceManager->getTexture(Paths::SFTEX_BOMB)));
	/**	End bombIconEntity entity **/
	updateLoading(77);

	/** Create player entity **/
	GameEntity* playerEntity = new GameEntity;
	playerEntity->addComponent(new PlayerComponent);
	playerEntity->addComponent(new HealthComponent(2, 99));
	playerEntity->addComponent(new RenderComponent(*m_resourceManager->getMesh(Paths::OBJ_SHIP_PLAYER), *m_resourceManager->getShader(Paths::SHADER_BASIC)));
	playerEntity->addComponent(new PositionComponent(glm::vec3(0.0f, 0.0f, GAME_Z_DISTANCE)));
	playerEntity->addComponent(new VelocityComponent);
	updateLoading(78);
	/**	End player entity **/

	/** Create crate entity **/
	GameEntity* crateEntity = new GameEntity;
	crateEntity->addComponent(new CrateComponent);
	crateEntity->addComponent(new RenderComponent(*m_resourceManager->getMesh(Paths::OBJ_CRATE), *m_resourceManager->getShader(Paths::SHADER_BASIC)));
	crateEntity->addComponent(new PositionComponent(glm::vec3(80.0f, -25.0f, -230.0f)));
	crateEntity->addComponent(new VelocityComponent);
	updateLoading(85);
	/**	End crate entity **/

	/** Create crate2 entity **/
	GameEntity* crateEntity2 = new GameEntity;
	crateEntity2->addComponent(new CrateComponent);
	crateEntity2->addComponent(new RenderComponent(*m_resourceManager->getMesh(Paths::OBJ_CRATE), *m_resourceManager->getShader(Paths::SHADER_BASIC)));
	crateEntity2->addComponent(new PositionComponent(glm::vec3(0.0f, -40.0f, -230.0f)));
	crateEntity2->addComponent(new VelocityComponent);
	updateLoading(87);
	/**	End crate2 entity **/

	/** Create crate3 entity **/
	GameEntity* crateEntity3 = new GameEntity;
	crateEntity3->addComponent(new CrateComponent);
	crateEntity3->addComponent(new RenderComponent(*m_resourceManager->getMesh(Paths::OBJ_CRATE), *m_resourceManager->getShader(Paths::SHADER_BASIC)));
	crateEntity3->addComponent(new PositionComponent(glm::vec3(-100.0f, 50.0f, -230.0f)));
	crateEntity3->addComponent(new VelocityComponent);
	updateLoading(90);
	/**	End crate3 entity **/

	/** Create multiplier entity **/
	GameEntity* multiplierEntity = new GameEntity;
	multiplierEntity->addComponent(new MultiplierComponent);
	multiplierEntity->addComponent(new TextComponent(Paths::FONT_STAR, ""));
	updateLoading(93);
	/**	End multiplier entity **/

	/** Create score entity **/
	GameEntity* scoreEntity	= new GameEntity;
	scoreEntity->addComponent(new ScoreComponent);
	scoreEntity->addComponent(new TextComponent(Paths::FONT_STAR, ""));
	updateLoading(95);
	/**	End score entity **/

	/** Create bombText entity **/
	GameEntity* bombTextEntity	= new GameEntity;
	bombTextEntity->addComponent(new PositionComponent(glm::vec3((BORDER_OFFSET * 2) + w, BORDER_OFFSET, 0.0f)));
	bombTextEntity->addComponent(new BombComponent(1));
	bombTextEntity->addComponent(new TextComponent(Paths::FONT_STAR, ""));
	updateLoading(97);
	/**	End score entity **/

	/** Create shieldText entity **/
	GameEntity* shieldTextEntity = new GameEntity;
	shieldTextEntity->addComponent(new PositionComponent(glm::vec3((BORDER_OFFSET * 2) + (w * 4), BORDER_OFFSET, 0.0f)));
	shieldTextEntity->addComponent(new ShieldComponent);
	shieldTextEntity->addComponent(new TextComponent(Paths::FONT_STAR, ""));
	updateLoading(98);
	/**	End shieldText entity **/

	m_entityManager->addEntity(bgEntity);
	m_entityManager->addEntity(bombIconEntity);
	m_entityManager->addEntity(shieldIconEntity);
	m_entityManager->addEntity(playerEntity);
	m_entityManager->addEntity(crateEntity);
	m_entityManager->addEntity(crateEntity2);
	m_entityManager->addEntity(crateEntity3);
	m_entityManager->addEntity(multiplierEntity);
	m_entityManager->addEntity(scoreEntity);
	m_entityManager->addEntity(bombTextEntity);
	m_entityManager->addEntity(shieldTextEntity);

	updateLoading(100);
}

void LogicSystem::updateLoading(int percent) {
	if(percent == 100) {
		m_systemManager->sendMessage(&LoadingMessage(false, percent));
	} else {
		m_systemManager->sendMessage(&LoadingMessage(true, percent));
	}
}

void LogicSystem::updateStart() {
	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();

	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
		MenuItemComponent* menuItemComponent = (MenuItemComponent*)(*i)->getComponent<MenuItemComponent>();

		sf::RectangleShape rectangle(sf::Vector2f(textComponent->getText().getLocalBounds().width * 1.2f, 
												  textComponent->getText().getLocalBounds().height * 2));
		rectangle.setPosition(0, textComponent->getText().getPosition().y);
	}
}

void LogicSystem::updateRun(float delta) {
	vector<GameEntity*> playerEntity = m_entityManager->getEntitiesWithComponentType<PlayerComponent>();
	HealthComponent* playerHealthComponent = (HealthComponent*)(playerEntity[0])->getComponent<HealthComponent>();
	if(!playerHealthComponent->getIsAlive()) {
		Settings::isGameOver = true;
		Settings::saveHighscores(true, m_score);
		handleGameOver();
		return;
	}

	if(!m_hasLoaded) {
		m_music = &m_resourceManager->getMusic();
		if(Settings::hasSound) {
			m_music->play();
		}
		m_shootSound->setBuffer(*m_resourceManager->getSoundBuffer(Paths::SOUND_LASER));
		m_bombSound->setBuffer(*m_resourceManager->getSoundBuffer(Paths::SOUND_BOMB));
		m_hasLoaded = true;
	}

	if(m_isShooting){
		if(m_shootSound->getStatus() == sf::Sound::Status::Stopped){
			if(Settings::hasSound) {
				m_shootSound->setVolume(30);
			}else {
				m_shootSound->setVolume(0);
			}
			m_shootSound->play();
			m_systemManager->sendMessage(&BulletMessage(BulletList::getBullet()));
		}
	}

	// UPDATE ENEMY WAVES
	EnemyTimer::checkForNewWave(delta, m_systemManager);

	// Get required entities
	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();

	// Update all text positions
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		if((*i)->hasComponent<MultiplierComponent>()) {
			TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
			if(textComponent->getText().getString().isEmpty()) {
				textComponent->getText().setString("0.00x");
			} else {
				float val = m_multiplier;
				stringstream ss (stringstream::in | stringstream::out);
				ss << val << "x";
				string multiplier = ss.str();
				textComponent->getText().setString(multiplier);
			}
			float width = textComponent->getText().getGlobalBounds().width;
			textComponent->getText().setPosition((m_window.getSize().x / 2) - width / 2, BORDER_OFFSET);
		}
		if((*i)->hasComponent<ScoreComponent>()) {
			TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
			int n = m_score;
			int digits = 0; while (n != 0) { n /= 10; digits++; }
			int leadingZeros = TRAILING_ZEROS - digits;
			std::ostringstream buff;
			(m_score == 0) ? buff << "" : buff << m_score;
			std::string dest = std::string(leadingZeros, '0').append( buff.str() );
			textComponent->getText().setString(dest);

			float width = textComponent->getText().getGlobalBounds().width;
			textComponent->getText().setPosition((m_window.getSize().x) - width - BORDER_OFFSET, BORDER_OFFSET);
		}
	}
}

void LogicSystem::updateBomb(bool isIncrement) {
	vector<GameEntity*> bombEntities = m_entityManager->getEntitiesWithComponentType<BombComponent>();
	if(bombEntities.size() != 0) {
		GameEntity*		bombEntity	= bombEntities[0];
		BombComponent*	bC			= (BombComponent*)(bombEntity)->getComponent<BombComponent>();
		
		if(!isIncrement && bC->getAmount() > 0) {
			if(m_isBombing ){
				if(Settings::hasSound) {
					m_bombSound->setVolume(100);
					m_bombSound->play();
				}
			}
			m_systemManager->sendMessage(&BombActiveMessage());
		}

		isIncrement ? bC->increment() : bC->decrement(); 
	}
}

void LogicSystem::updatePause() {

}

void LogicSystem::updateGameOver() {

}

void LogicSystem::updateCredits() {

}