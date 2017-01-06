#include "RenderingSystem.h"
#include "Paths.h"
#include "Settings.h"
#include "GameSystemManager.h"
#include "RenderComponent.h"
#include "SFRenderComponent.h"
#include "PositionComponent.h"
#include "PlayerComponent.h"
#include "PickUpComponent.h"
#include "TextComponent.h"
#include "MultiplierComponent.h"
#include "EnemyComponent.h"
#include "HealthComponent.h"
#include "MenuItemComponent.h"
#include "BombComponent.h"
#include "BulletComponent.h"
#include "ShieldComponent.h"
#include "KeyEventMessage.h"
#include "JoyStickMessage.h"
#include "ControlsMessage.h"
#include "LoadingMessage.h"
#include "HighScoreMessage.h"
#include "GameState.h"
#include <sstream>

/// <summary>
/// Initializes a new instance of the <see cref="RenderingSystem"/> class.
/// </summary>
/// <param name="window">The window.</param>
RenderingSystem::RenderingSystem(sf::RenderWindow &window) : m_window(window) {
	m_camera = new Camera(window);
	m_camera->setPosition(glm::vec3(0, 0, 3));
	switchToPerspective(window);
	glEnable(GL_DEPTH_TEST);

	m_loadingPercent = 0;
	m_isHighScoreActive = false;
	m_isControlsActive	= false;

	// FOR DEBUGGING ONLY
	m_isProfiling	= false;
	m_hasCollision	= false;

	if (!m_debugFont.loadFromFile(Paths::FONT_ARIAL)) {
		throw "Error loading font!"; 
	}
	m_debugText.setFont(m_debugFont);
	m_debugText.setScale(0.7f, 0.7f);
	m_debugText.setColor(sf::Color::Yellow);
	m_debugText.setString("FPS");
}

/// <summary>
/// Finalizes an instance of the <see cref="RenderingSystem"/> class.
/// </summary>
RenderingSystem::~RenderingSystem(void) {
	delete m_camera;
}

/// <summary>
/// Renders all Render Components to the window.
/// </summary>
/// <param name="delta">The delta.</param>
/// <param name="manager">The entity manager.</param>
/// <param name="systemManager">The system manager.</param>
void RenderingSystem::update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager) {
	glClearColor( 0.2f ,0.2f ,0.2f ,1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_entityManager	= &entityManager;
	m_FPS.update();
	m_camera->updateCamera(delta);
	m_viewMatrix = m_camera->buildViewMatrix();

	// Render based on game state
	switch (GameState::currentState) {
	case GameState::START:
		renderStart();
		break;
	case GameState::RUN:
		renderRun();
		if(Settings::isPaused) {
			renderPause();
		}
		if(Settings::isGameOver) {
			renderGameOver();
		}
		break;
	case GameState::CREDITS:
		renderCredits();
		break;
	  default:
		cout << "RENDER is unknown" << endl;
	}

	// For debugging
	if(m_isProfiling){
		m_window.pushGLStates();
		m_window.resetGLStates ();
		displayDebug(delta, entityManager);
		m_window.popGLStates();
	}
	m_window.display();
}

void RenderingSystem::displayDebug(float delta, GameEntityManager& entityManager) {
	/******** DRAW TEXT ********/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_debugText.setPosition(10.0f, 10.0f);
	std::ostringstream buffer;
	buffer << m_FPS.getFPS();
	std::string str = "FPS: " + buffer.str();
	m_debugText.setString(str);
	m_window.draw(m_debugText);

	m_debugText.setPosition(10.0f, 40.0f);
	buffer.str( std::string() );
	buffer.clear();
	buffer << delta;
	str = "Delta: " + buffer.str();
	m_debugText.setString(str);
	m_window.draw(m_debugText);

	m_debugText.setPosition(10.0f, 70.0f);
	buffer.str( std::string() );
	buffer.clear();
	buffer << entityManager.getEnitiesAmount();
	str = "Entities: " + buffer.str();
	m_debugText.setString(str);
	m_window.draw(m_debugText);

	m_debugText.setPosition(10.0f, 100.0f);
	buffer.str( std::string() );
	buffer.clear();
	buffer << entityManager.getEntityComponentsAmount();
	str = "Components: " + buffer.str();
	m_debugText.setString(str);
	m_window.draw(m_debugText);

	m_debugText.setPosition(10.0f, 130.0f);
	buffer.str( std::string() );
	buffer.clear();
	if(m_hasCollision) {
		buffer << "True";
	}else {
		buffer << "False";
	}
	str = "Collision: " + buffer.str();
	m_debugText.setString(str);
	m_window.draw(m_debugText);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
}

/// <summary>
/// Receives System messages.
/// </summary>
/// <param name="msg">The MSG.</param>
void RenderingSystem::receiveMessage(BaseMessage* msg) {
	if(msg->getMessageType() == MessageType::KEYEVENT) {
		KeyEventMessage* e = dynamic_cast<KeyEventMessage*>(msg);
		if(e->getPayload().type == sf::Event::Resized) {
			// Adjust the viewport when the window is resized
			glViewport(0, 0, e->getPayload().size.width, e->getPayload().size.height);
			m_window.setView(sf::View(sf::FloatRect(0, 0, e->getPayload().size.width,  e->getPayload().size.height)));
		}
		if(e->getPayload().key.code == sf::Keyboard::Num1) {
			// Show wireframe
			m_isProfiling ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
			m_isProfiling = !m_isProfiling;
		}
	}

	/** JOYSTICK EVENTS **/
	if(msg->getMessageType() == MessageType::JOYEVENT) {
		JoyStickMessage* e = dynamic_cast<JoyStickMessage*>(msg);
		// JoyStick is active
		if(e->getIsActive()) {
			if(e->getJoyButton() == JoyButton::BACK) {
				// Show wireframe
				m_isProfiling ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
				m_isProfiling = !m_isProfiling;
			}
		}
	}

	// LOADING
	if(msg->getMessageType() == MessageType::LOADING) {
		LoadingMessage* e = dynamic_cast<LoadingMessage*>(msg);
		m_loadingPercent = e->getPercent();
		renderLoadingScreen();
	}

	// HIGHSCORE
	if(msg->getMessageType() == MessageType::HIGHSCORE) {
		HighScoreMessage* e = dynamic_cast<HighScoreMessage*>(msg);
		m_isHighScoreActive = e->getPayload();
	}

	// CONTROLS
	if(msg->getMessageType() == MessageType::CONTROLS) {
		ControlsMessage* e = dynamic_cast<ControlsMessage*>(msg);
		m_isControlsActive = e->getPayload();
	}

	// ONLY FOR DEBUGGING
	if(msg->getMessageType() == MessageType::PICKUPCOLLISION ||
	   msg->getMessageType() == MessageType::ENEMYCOLLISION) {
		m_hasCollision = true;
	}else {
		m_hasCollision = false;
	}
}

void RenderingSystem::renderLoadingScreen() {
	glClearColor( 0.f ,0.f ,0.f ,1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sf::Font font;
	sf::Text text;
	if (!font.loadFromFile(Paths::FONT_STAR)) {
		throw "Error loading font!"; 
	}
	text.setFont(font);

	std::ostringstream s;
	s << "LOADING " << m_loadingPercent;
	std::string loadingStr(s.str());

	text.setString(loadingStr);
	text.setColor(sf::Color::White);
	float width = text.getGlobalBounds().width;
	float height = text.getGlobalBounds().height;
	text.setPosition((m_window.getSize().x / 2) - (width / 2), 
					 (m_window.getSize().y / 2) - (height / 2));

	m_window.pushGLStates();
	m_window.resetGLStates();
	m_window.draw(text);
	m_window.popGLStates();

	m_window.display();
}

/// <summary>
/// Updates the shader matrices.
/// </summary>
/// <param name="program">The program.</param>
void RenderingSystem::updateShaderMatrices(GLuint program)	{
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix")  , 1, false, (float*)&m_modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix")   , 1, false, (float*)&m_viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix")   , 1, false, (float*)&m_projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "textureMatrix"), 1, false, (float*)&m_textureMatrix);
}

/// <summary>
/// Switches to perspective mode.
/// </summary>
/// <param name="window">The window.</param>
void RenderingSystem::switchToPerspective(sf::RenderWindow &window) {
	setProjectionMatrix(glm::perspective(45.0f, (float)window.getSize().x / (float)window.getSize().y, 1.0f, 4500.0f));
}

void RenderingSystem::renderStart() {

	glClearColor( 0.f ,0.f ,0.f ,1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_window.pushGLStates();
	m_window.resetGLStates();

	vector<GameEntity*> sfrenderEntities	= m_entityManager->getEntitiesWithComponentType<SFRenderComponent>();
	vector<GameEntity*> textEntities		= m_entityManager->getEntitiesWithComponentType<TextComponent>();

	// Draw all sfml textures to screen
	for (vector<GameEntity*>::iterator i = sfrenderEntities.begin(); i != sfrenderEntities.end(); ++i) {
			SFRenderComponent* rComp = (SFRenderComponent*)(*i)->getComponent<SFRenderComponent>();
			sf::Sprite splashSprite;
			splashSprite.setTexture(rComp->getTexture());
			auto size = splashSprite.getTexture()->getSize();
			m_window.draw(splashSprite);
	}

	const int YBUFFER = 40.0f;
	// Draw all text entities to screen
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
		MenuItemComponent* menuItemComponent = (MenuItemComponent*)(*i)->getComponent<MenuItemComponent>();

		sf::RectangleShape rectangle(sf::Vector2f(textComponent->getText().getLocalBounds().width + YBUFFER, 
												  textComponent->getText().getLocalBounds().height * 2));
		rectangle.setPosition(0, textComponent->getText().getPosition().y);

		if(menuItemComponent->getIsActive()) {
			rectangle.setFillColor(sf::Color::White);
			textComponent->getText().setColor(sf::Color::Black);
		}else {
			rectangle.setFillColor(sf::Color::Black);
			textComponent->getText().setColor(sf::Color::White);
		}

		m_window.draw(rectangle);
		m_window.draw(textComponent->getText());
	}

	if(m_isHighScoreActive) {
		sf::RectangleShape rectangle(sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y));
		rectangle.setPosition(m_window.getSize().x / 2, 0);
		rectangle.setFillColor(sf::Color(0, 0, 0, 100));
		m_window.draw(rectangle);

		sf::Font font;
		sf::Text nameText;
		sf::Text scoreText;
		if (!font.loadFromFile(Paths::FONT_STAR)) {
			throw "Error loading font!"; 
		}
		nameText.setFont(font);
		scoreText.setFont(font);

		const int YOFFSET = 20.0f;
		const int XOFFSET = 10.0f;
		for(int i = 0; i < 10; i++) {
			std::ostringstream s1;
			std::ostringstream s2;

			s1 << Settings::highscores[i].first << ":";
			s2 << Settings::highscores[i].second;
			std::string nameStr(s1.str());
			std::string scoreStr(s2.str());

			nameText.setString(nameStr);
			scoreText.setString(scoreStr);
			nameText.setColor(sf::Color::White);
			scoreText.setColor(sf::Color::White);

			float width1 = nameText.getGlobalBounds().width;
			float height1 = nameText.getGlobalBounds().height;
			float width2 = scoreText.getGlobalBounds().width;
			float height2 = scoreText.getGlobalBounds().height;

			float x1 = (m_window.getSize().x / 2) + (m_window.getSize().x / 4) - width1;
			float y1 = (m_window.getSize().y * 0.02) + (YOFFSET * i * 2);
			float x2 = (m_window.getSize().x / 2) + (m_window.getSize().x / 4) + XOFFSET;
			float y2 = (m_window.getSize().y * 0.02) + (YOFFSET * i * 2);

			nameText.setPosition(x1, y1);
			scoreText.setPosition(x2, y2);

			m_window.draw(nameText);
			m_window.draw(scoreText);
		}
	}

	// CONTROLS
	if(m_isControlsActive) {
		sf::RectangleShape rectangle(sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y));
		rectangle.setPosition(m_window.getSize().x / 2, 0);
		rectangle.setFillColor(sf::Color(0, 0, 0, 100));
		m_window.draw(rectangle);

		sf::Font font;
		sf::Text nameText;
		sf::Text controlText;
		if (!font.loadFromFile(Paths::FONT_STAR)) {
			throw "Error loading font!"; 
		}
		nameText.setFont(font);
		controlText.setFont(font);

		const int YOFFSET = 20.0f;
		const int XOFFSET = 10.0f;
		for(int i = 0; i < 5; i++) {
			std::string nameStr(Paths::CONTROLNAMES[i]);
			std::string scoreStr(Paths::CONTROLBUTTONS[i]);

			nameText.setString(nameStr);
			controlText.setString(scoreStr);
			nameText.setColor(sf::Color::White);
			controlText.setColor(sf::Color::White);

			float width1 = nameText.getGlobalBounds().width;
			float height1 = nameText.getGlobalBounds().height;
			float width2 = controlText.getGlobalBounds().width;
			float height2 = controlText.getGlobalBounds().height;

			float x1 = (m_window.getSize().x / 2) + (m_window.getSize().x / 6) - width1;
			float y1 = (m_window.getSize().y * 0.02) + (YOFFSET * i * 2);
			float x2 = (m_window.getSize().x / 2) + (m_window.getSize().x / 6) + XOFFSET;
			float y2 = (m_window.getSize().y * 0.02) + (YOFFSET * i * 2);

			nameText.setPosition(x1, y1);
			controlText.setPosition(x2, y2);

			m_window.draw(nameText);
			m_window.draw(controlText);
		}
	}
	m_window.popGLStates();
}

void RenderingSystem::renderRun() {
	// Get required Entities
	vector<GameEntity*> shieldEntities	 = m_entityManager->getEntitiesWithComponentType<ShieldComponent>();
	vector<GameEntity*> bombEntities	 = m_entityManager->getEntitiesWithComponentType<BombComponent>();
	vector<GameEntity*> sfrenderEntities = m_entityManager->getEntitiesWithComponentType<SFRenderComponent>();
	vector<GameEntity*> renderEntities	 = m_entityManager->getEntitiesWithComponentType<RenderComponent>();
	vector<GameEntity*> textEntities	 = m_entityManager->getEntitiesWithComponentType<TextComponent>();
	PlayerComponent* playerComponent;
	HealthComponent* playerHealthComponent;

	// Draw bg
	// Push/Pop states to avoid OpenGL and SFML clashing
	m_window.pushGLStates();
	for (vector<GameEntity*>::iterator i = sfrenderEntities.begin(); i != sfrenderEntities.end(); ++i) {
		if(!(*i)->hasComponent<PositionComponent>()) {
			SFRenderComponent* rComp = (SFRenderComponent*)(*i)->getComponent<SFRenderComponent>();

			sf::Sprite sprite;
			sprite.setTexture(rComp->getTexture());
			sprite.setPosition(0, 0);
			m_window.draw(sprite);
		}
	}
	m_window.popGLStates();

	// Draw all renderable entities
	for (vector<GameEntity*>::iterator i = renderEntities.begin(); i != renderEntities.end(); ++i) {
		if((*i)->hasComponent<PositionComponent>()) {
			if((*i)->hasComponent<PlayerComponent>()) {
				playerComponent = (PlayerComponent*)(*i)->getComponent<PlayerComponent>();
				playerHealthComponent = (HealthComponent*)(*i)->getComponent<HealthComponent>();
			}
			if((*i)->hasComponent<BulletComponent>()) {
				BulletComponent* bComp = (BulletComponent*)(*i)->getComponent<BulletComponent>();
				if(!bComp->getIsActive()) {
					continue;
				}
			}
			if((*i)->hasComponent<HealthComponent>()) {
				HealthComponent* hComp = (HealthComponent*)(*i)->getComponent<HealthComponent>();
				if(!hComp->getIsAlive()) {
					continue;
				}
			}
			if((*i)->hasComponent<PickUpComponent>()) {
				PickUpComponent* pComp = (PickUpComponent*)(*i)->getComponent<PickUpComponent>();
				if(!pComp->getIsActive()) {
					continue;
				}
			}
			if((*i)->hasComponent<EnemyComponent>()) {
				EnemyComponent* eComp = (EnemyComponent*)(*i)->getComponent<EnemyComponent>();
				if(!eComp->getIsActive()) {
					continue;
				}
			}
			RenderComponent* rComp = (RenderComponent*)(*i)->getComponent<RenderComponent>();
			PositionComponent* pComp = (PositionComponent*)(*i)->getComponent<PositionComponent>();
			
			m_modelMatrix = glm::translate(glm::mat4(), 
				glm::vec3(pComp->getPosX(), pComp->getPosY(), pComp->getPosZ())) * 
				glm::rotate(glm::mat4(), pComp->getRotX(), glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(), pComp->getRotY(), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(glm::mat4(), pComp->getRotZ(), glm::vec3(0.0f, 0.0f, 1.0f));

			GLuint program = rComp->getShader().getShaderProgram();
			glUseProgram (program);
			
			updateShaderMatrices(program);
			glUniform1i(glGetUniformLocation(program, "colourTex"),	0);

			rComp->getMesh().draw();
			if(m_isProfiling) { // Show bounding boxes
				rComp->getMesh().drawBoundingBox(&rComp->getMesh(), m_modelMatrix);
			}

			glUseProgram(0);
		}
	}

	// Push/Pop states to avoid OpenGL and SFML clashing
	m_window.pushGLStates();
	// Draw all text entities to screen
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		if(m_isProfiling){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
			m_window.draw(textComponent->getText());
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
		}else {
			TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
			m_window.draw(textComponent->getText());
		}
	}
	// Draw sfRenderables
	for (vector<GameEntity*>::iterator i = sfrenderEntities.begin(); i != sfrenderEntities.end(); ++i) {
		if((*i)->hasComponent<PositionComponent>()) {
			SFRenderComponent* rComp		= (SFRenderComponent*)(*i)->getComponent<SFRenderComponent>();
			PositionComponent* pComponent	= (PositionComponent*)(*i)->getComponent<PositionComponent>();

			sf::Sprite sprite;
			sprite.setTexture(rComp->getTexture());
			sprite.setPosition(pComponent->getPosX(), pComponent->getPosY());
			m_window.draw(sprite);
		}
	}
	// Draw bomb text
	if(bombEntities.size() != 0) {
		GameEntity* e = bombEntities[0];
		PositionComponent*  pC	= (PositionComponent*)(e)->getComponent<PositionComponent>();
		BombComponent*		bC	= (BombComponent*)(e)->getComponent<BombComponent>();
		TextComponent*		tC	= (TextComponent*)(e)->getComponent<TextComponent>();

		std::ostringstream buffer;
		buffer << bC->getAmount();

		tC->getText().setString("x " + buffer.str());
		tC->getText().setPosition(pC->getPosX(), pC->getPosY());
		m_window.draw(tC->getText());
	}
	// Draw shield text
	if(shieldEntities.size() != 0) {
		GameEntity* e = shieldEntities[0];
		PositionComponent*  pC	= (PositionComponent*)(e)->getComponent<PositionComponent>();
		TextComponent*		tC	= (TextComponent*)(e)->getComponent<TextComponent>();

		std::ostringstream buffer;
		buffer << playerHealthComponent->getCurrentHP();

		tC->getText().setString("x " + buffer.str());
		tC->getText().setPosition(pC->getPosX(), pC->getPosY());
		m_window.draw(tC->getText());
	}


	// Draw paused and game over state
	if(Settings::isPaused || Settings::isGameOver) {
		const int XBUFFER = 40.0f;
		sf::Font font;
		sf::Text pauseText;
		if (!font.loadFromFile(Paths::FONT_STAR)) {
			throw "Error loading font!"; 
		}
		pauseText.setFont(font);
		if(Settings::isPaused) {
			pauseText.setString(Paths::STR_PAUSED);
		}else {
			pauseText.setString(Paths::STR_GAMEOVER);
		}
		pauseText.setColor(sf::Color::White);

		float w = pauseText.getGlobalBounds().width;
		float h = pauseText.getGlobalBounds().height;
		float x = (m_window.getSize().x / 2) - (w / 2);
		float y = (m_window.getSize().y / 4);
		pauseText.setPosition(x, y);

		sf::RectangleShape rectText(sf::Vector2f(w + XBUFFER * 2, h * 2));
		rectText.setPosition(x - XBUFFER, y);
		rectText.setFillColor(sf::Color::Black);

		sf::RectangleShape rectPause(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
		rectPause.setPosition(0, 0);
		rectPause.setFillColor(sf::Color(0, 0, 0, 100));

		m_window.draw(rectPause);
		m_window.draw(rectText);
		m_window.draw(pauseText);
	}

	m_window.popGLStates();
}

void RenderingSystem::renderPause() {
	m_window.pushGLStates();
	m_window.resetGLStates();

	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();

	const int YBUFFER = 40.0f;
	// Draw all text entities to screen
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		if(!(*i)->hasComponent<MenuItemComponent>()) {
			continue;
		}
		TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
		MenuItemComponent* menuItemComponent = (MenuItemComponent*)(*i)->getComponent<MenuItemComponent>();

		sf::RectangleShape rectangle(sf::Vector2f(textComponent->getText().getLocalBounds().width + YBUFFER, 
												  textComponent->getText().getLocalBounds().height * 2));
		rectangle.setPosition(0, textComponent->getText().getPosition().y);

		if(menuItemComponent->getIsActive()) {
			rectangle.setFillColor(sf::Color::White);
			textComponent->getText().setColor(sf::Color::Black);
		}else {
			rectangle.setFillColor(sf::Color::Black);
			textComponent->getText().setColor(sf::Color::White);
		}

		m_window.draw(rectangle);
		m_window.draw(textComponent->getText());
	}
	m_window.popGLStates();
}

void RenderingSystem::renderGameOver() {
	m_window.pushGLStates();
	m_window.resetGLStates();

	vector<GameEntity*> textEntities = m_entityManager->getEntitiesWithComponentType<TextComponent>();

	const int YBUFFER = 40.0f;
	// Draw all text entities to screen
	for (vector<GameEntity*>::iterator i = textEntities.begin(); i != textEntities.end(); ++i) {
		if(!(*i)->hasComponent<MenuItemComponent>()) {
			continue;
		}
		TextComponent* textComponent = (TextComponent*)(*i)->getComponent<TextComponent>();
		MenuItemComponent* menuItemComponent = (MenuItemComponent*)(*i)->getComponent<MenuItemComponent>();

		sf::RectangleShape rectangle(sf::Vector2f(textComponent->getText().getLocalBounds().width + YBUFFER, 
												  textComponent->getText().getLocalBounds().height * 2));
		rectangle.setPosition(0, textComponent->getText().getPosition().y);

		if(menuItemComponent->getIsActive()) {
			rectangle.setFillColor(sf::Color::White);
			textComponent->getText().setColor(sf::Color::Black);
		}else {
			rectangle.setFillColor(sf::Color::Black);
			textComponent->getText().setColor(sf::Color::White);
		}

		m_window.draw(rectangle);
		m_window.draw(textComponent->getText());
	}
	m_window.popGLStates();
}

void RenderingSystem::renderCredits() {

}