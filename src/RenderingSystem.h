#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include "GameSystem.h"
#include "glm\glm.hpp"
#include <GL\glew.h>
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "FPS.h"

/// <summary>
/// System responsible for drawing all Entities with a Render Component.
/// </summary>
class RenderingSystem : public GameSystem {
public:
	RenderingSystem(sf::RenderWindow &window);
	~RenderingSystem(void);

	void		update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager);
	void		receiveMessage(BaseMessage* msg);
	void		displayDebug(float delta, GameEntityManager& entityManager);

protected:
	sf::RenderWindow&	m_window;
	GameEntityManager*	m_entityManager;

	glm::mat4	m_projMatrix;		//Projection matrix
	glm::mat4	m_modelMatrix;		//Model matrix
	glm::mat4	m_viewMatrix;		//View matrix
	glm::mat4	m_textureMatrix;	//Texture matrix

	void		renderStart();
	void		renderRun();
	void		renderPause();
	void		renderGameOver();
	void		renderCredits();

	void		renderLoadingScreen();

	void		switchToPerspective(sf::RenderWindow &window);
	void		updateShaderMatrices(GLuint program);
	void		setProjectionMatrix(const glm::mat4 &m) { m_projMatrix = m; }
	void		setViewMatrix(const glm::mat4 &m) { m_viewMatrix = m; }
	void		setTextureMatrix(const glm::mat4 &m) { m_textureMatrix = m; }
	void		setModelMatrix(const glm::mat4 &m) { m_modelMatrix = m; }

	int			m_loadingPercent;
	bool		m_isHighScoreActive;
	bool		m_isControlsActive;

	// DEBUGGING PURPOSES ONLY
	bool		m_isProfiling; // Used for debugging purposes only
	bool		m_hasCollision;
	sf::Font	m_debugFont;
	sf::Text	m_debugText;
	FPS			m_FPS;

	// TODO REMOVE AFTER CHANGE TO COMPONENT ENTITY SYSTEM
	Camera*		m_camera;
};

#endif