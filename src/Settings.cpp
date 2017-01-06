#include "Settings.h"
#include "Paths.h"
#include <fstream>

bool Settings::hasSound	= true;
std::vector<Settings::scorePair> Settings::highscores;
bool Settings::isPaused = false;
bool Settings::isGameOver = false;

Settings::Settings(void) {
}

Settings::~Settings(void) {
}

void Settings::loadHighscores() {
	std::ifstream file(Paths::SETTINGS);

	Settings::scorePair p;
	for(int i = 0; i < 10; i++) {
		file >> p.first >> p.second;
		highscores.push_back(p);
	}
	file >> Settings::hasSound;
}

void Settings::saveHighscores(bool isInGame, float score) {
	if(isInGame) {
		for (std::vector<Settings::scorePair>::iterator i = Settings::highscores.begin(); i != Settings::highscores.end(); ++i) {
			if(i->second < score) {
				Settings::scorePair p("Ste", score);
				i = Settings::highscores.insert(i, p);
				break;
			}
		}
	}

	std::ofstream file(Paths::SETTINGS);
	Settings::scorePair p;
	for(int i = 0; i < 10; i++) {
		file << highscores[i].first << " " << highscores[i].second << std::endl;
	}
	file << Settings::hasSound;
}