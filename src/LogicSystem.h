#ifndef LOGICSYSTEM_H
#define LOGICSYSTEM_H

#include "GameSystem.h"
#include <SFML/Graphics.hpp>
#include "GameResourceManager.h"
#include "GameSystemManager.h"
#include "GameState.h"
#include "InputSystem.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "RenderingSystem.h"
#include "BulletList.h"
#include "PickUpList.h"
#include "EnemyList.h"
#include "PickUpCollisionMessage.h"

/// <summary>
/// System responsible for game logic such as scores and multiplies.
/// </summary>
class LogicSystem : public GameSystem {
public:
	LogicSystem(sf::RenderWindow &window);
	~LogicSystem(void);

	void		update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager);
	void		receiveMessage(BaseMessage* msg);

private:
	void		receiveStartMessage(BaseMessage* msg);
	void		receiveRunMessage(BaseMessage* msg);

	void		initialiseLoading();
	void		updateLoading(int percent);
	void		finishLoading();

	void		initialiseStart();
	void		initialiseRun();
	void		initialisePause();
	void		initialiseCredits();
	void		resetFramework();

	void		updateStart();
	void		updateRun(float delta);
	void		updatePause();
	void		updateGameOver();
	void		updateCredits();

	void		updateMenuItems(bool isUp);
	void		handleEnter();
	void		startGame();
	void		startOptions();
	void		startHighScores();
	void		startControls();
	void		clearResources();
	void		switchSound();
	void		addHighScoreEntities();
	void		addOptionsEntities();
	void		addDefaultEntities();
	void		updateSoundText();
	void		showDefault();

	void		handlePause();
	void		startPause();
	void		unPause();
	void		addPauseEntities();
	void		removePauseEntities();
	void		handlePauseEnter();
	void		quitToMenu();

	void		updateBomb(bool isIncrement);
	void		handlePickUpMessage(BaseMessage* msg);
	void		dropPickUp(GameEntity* enemy);

	void		updateScore(int points);
	void		updateMultiplier(float amount);
	void		handleGameOver();
	void		addGameOverEntities();
	void		handleGameOverEnter();

	void		pickUpScore(PickUpCollisionMessage* e);
	void		pickUpBomb(PickUpCollisionMessage* e);
	void		pickUpWeapon(PickUpCollisionMessage* e);
	void		pickUpShield(PickUpCollisionMessage* e);

	sf::RenderWindow&		m_window;
	GameResourceManager*	m_resourceManager;
	GameSystemManager*		m_systemManager;
	GameEntityManager*		m_entityManager;

	// START
	int						m_menuItemSelected;
	enum StartState {
		DEFAULT, OPTIONS, HIGHSCORES, CONTROLS
	} m_startState;

	bool					m_previousUp;
	bool					m_previousDown;
	sf::Sound*				m_shootSound;
	sf::Sound*				m_bombSound;
	sf::Sound*				m_menuUpSound;
	sf::Sound*				m_menuDownSound;
	sf::Music*				m_music;
	bool					m_hasLoaded;
	int						m_score;
	float					m_multiplier;
	bool					m_isShooting;
	bool					m_isBombing;
	static const float		BORDER_OFFSET;
	static const int		TRAILING_ZEROS;
	static const float		GAME_Z_DISTANCE;
};

#endif