#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <SFML/Graphics.hpp>

class Camera {
public :
	Camera (sf::RenderWindow &window);

	Camera (sf::RenderWindow &window, float pitch ,float yaw ,glm::vec3 position);

	~Camera(void){};

	void updateCamera(float msec = 10.0f);

	glm::mat4 buildViewMatrix();

	glm::vec3 getPosition() const { return position; }
	void setPosition(glm::vec3 val) { position = val; }

	float getYaw()const { return yaw ; }
	void setYaw(float y) { yaw = y; }

	float getPitch() const { return pitch;}
	void setPitch(float p) { pitch = p; }

	void keyW();
	void keyA();
	void keyS();
	void keyD();
	void keyShift();
	void keySpace();

protected :
	float yaw;
	float pitch;
	float msec;
	glm::vec3 position; // Set to 0,0,0 by Vector3 constructor ;)
	sf::RenderWindow &window;
};