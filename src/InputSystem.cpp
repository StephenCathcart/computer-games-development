#include "InputSystem.h"
#include "GameSystemManager.h"
#include "KeyEventMessage.h"
#include "JoyStickMessage.h"
#include "Xbox.h"
#include "Settings.h"

const float InputSystem::JOY_THRESHOLD = 50.0f;

/// <summary>
/// Initializes a new instance of the <see cref="InputSystem"/> class.
/// </summary>
/// <param name="window">The window.</param>
InputSystem::InputSystem(sf::RenderWindow &window) : m_window(window) {
}

/// <summary>
/// Finalizes an instance of the <see cref="InputSystem"/> class.
/// </summary>
InputSystem::~InputSystem(void) {
}

/// <summary>
/// Polls for events.
/// </summary>
/// <param name="delta">The delta.</param>
/// <param name="manager">The manager.</param>
/// <param name="systemManager">The system manager.</param>
void InputSystem::update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager) {
	sf::Event e;
	while (m_window.pollEvent(e)) {
		handleKeyboard(systemManager, e);
		handleJoypad(systemManager, e);
	}
}

/// <summary>
/// Handles keyboard events.
/// </summary>
/// <param name="mgr">The MGR.</param>
/// <param name="e">The e.</param>
void InputSystem::handleKeyboard(GameSystemManager& mgr, sf::Event& e) {
	bool fireMsg = false;

	if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)) {
		//mgr.setIsActive(false);
		fireMsg = true;
	}
	if (e.type == sf::Event::Resized) {
		fireMsg = true;
	}
	if (e.type == sf::Event::KeyPressed) {
		if (e.key.code == sf::Keyboard::Num1 || e.key.code == sf::Keyboard::W ||
			e.key.code == sf::Keyboard::A || e.key.code == sf::Keyboard::S ||
			e.key.code == sf::Keyboard::A || e.key.code == sf::Keyboard::S ||
			e.key.code == sf::Keyboard::D || e.key.code == sf::Keyboard::Space ||
			e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::Down ||
			e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right ||
			e.key.code == sf::Keyboard::Return || e.key.code == sf::Keyboard::E) {
			fireMsg = true;
		}
	}
	if(e.type == sf::Event::KeyReleased) {
		if (e.key.code == sf::Keyboard::W || e.key.code == sf::Keyboard::A ||
			e.key.code == sf::Keyboard::S || e.key.code == sf::Keyboard::D ||
			e.key.code == sf::Keyboard::Space || e.key.code == sf::Keyboard::Up ||
			e.key.code == sf::Keyboard::Down || e.key.code == sf::Keyboard::Left || 
			e.key.code == sf::Keyboard::Right || e.key.code == sf::Keyboard::E) {
			fireMsg = true;
		}
	}
	if(fireMsg) {
		mgr.sendMessage(&KeyEventMessage(e));
	}
}

/// <summary>
/// Handles joypad events.
/// </summary>
/// <param name="mgr">The MGR.</param>
/// <param name="e">The e.</param>
void InputSystem::handleJoypad(GameSystemManager& mgr, sf::Event& e) {
	if(e.type == sf::Event::JoystickButtonPressed) {
		unsigned int joyID = e.joystickMove.joystickId;
		if (sf::Joystick::isConnected(joyID)) { 
			if (sf::Joystick::isButtonPressed(joyID, Xbox::A)) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::A, true));
			}
			if (sf::Joystick::isButtonPressed(joyID, Xbox::BACK)) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::BACK, true));
			}
			if (sf::Joystick::isButtonPressed(joyID, Xbox::START)) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::START, true));
			}
			if (sf::Joystick::isButtonPressed(joyID, Xbox::X)) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::X, true));
			}
		}
	}
	if(e.type == sf::Event::JoystickButtonReleased) {
		unsigned int joyID = e.joystickMove.joystickId;
		if (sf::Joystick::isConnected(joyID)) { 
			if (e.joystickButton.button == Xbox::A) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::A, false));
			}
			if (e.joystickButton.button == Xbox::X) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::X, false));
			}
		}
	}
	if(e.type == sf::Event::JoystickMoved) {
		unsigned int joyID = e.joystickMove.joystickId;
		if (sf::Joystick::isConnected(joyID)) {
			float x = sf::Joystick::getAxisPosition(joyID, sf::Joystick::Axis::X); // Left Thumbstick X
			float y = sf::Joystick::getAxisPosition(joyID, sf::Joystick::Axis::Y); // Left Thumbstick Y

			(x < -JOY_THRESHOLD) ? mgr.sendMessage(&JoyStickMessage(JoyButton::LEFT, true)) :
								   mgr.sendMessage(&JoyStickMessage(JoyButton::LEFT, false));

			(x > JOY_THRESHOLD)  ? mgr.sendMessage(&JoyStickMessage(JoyButton::RIGHT, true)) :
								   mgr.sendMessage(&JoyStickMessage(JoyButton::RIGHT, false));

			(y < -JOY_THRESHOLD) ? mgr.sendMessage(&JoyStickMessage(JoyButton::UP, true)) :
								   mgr.sendMessage(&JoyStickMessage(JoyButton::UP, false));

			(y > JOY_THRESHOLD)  ? mgr.sendMessage(&JoyStickMessage(JoyButton::DOWN, true)) :
								   mgr.sendMessage(&JoyStickMessage(JoyButton::DOWN, false));

			if(y > -JOY_THRESHOLD && y < JOY_THRESHOLD) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::NULLY, false));
			}

			if(x > -JOY_THRESHOLD && x < JOY_THRESHOLD) {
				mgr.sendMessage(&JoyStickMessage(JoyButton::NULLX, false));
			}
		}
	}
}

/// <summary>
/// Receives Messages from other Systems.
/// </summary>
/// <param name="msg">The MSG.</param>
void InputSystem::receiveMessage(BaseMessage* msg) {
}