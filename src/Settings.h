#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <utility>
#include <iostream>

class Settings {
public:
	typedef std::pair <std::string, int> scorePair;

	~Settings(void);
	
	static void						loadHighscores();
	static void						saveHighscores(bool isInGame, float score);

	static bool						hasSound;
	static std::vector<scorePair>	highscores;
	static bool						isPaused;
	static bool						isGameOver;

private:
	Settings(void);
};

#endif
