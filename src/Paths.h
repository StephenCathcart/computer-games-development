#ifndef PATHS_H
#define PATHS_H

#include <string>

using std::string;

/// <summary>
/// Stores the path strings to all resources.
/// </summary>
class Paths {
public:
	~Paths(void);

	/** OBJ **/
	static const string OBJ_SHIP_PLAYER;
	static const string OBJ_SHIP_LARGE;
	static const string OBJ_SHIP_MEDIUM;
	static const string OBJ_SHIP_SMALL;
	static const string OBJ_CRATE;
	static const string OBJ_CRYSTAL;
	static const string OBJ_BULLET;

	/** TEXTURES **/
	static const string TEX_SHIP_PLAYER;
	static const string TEX_SHIP_LARGE;
	static const string TEX_SHIP_MEDIUM;
	static const string TEX_SHIP_SMALL;
	static const string TEX_CRATE;
	static const string TEX_CRYSTAL;

	/** SHADERS **/
	static const string SHADER_VERT_BASIC;
	static const string SHADER_FRAG_BASIC;
	static const string SHADER_VERT_BULLET;
	static const string SHADER_FRAG_BULLET;

	/** SHADERS NAMES**/
	static const string SHADER_BASIC;
	static const string SHADER_BULLET;

	/** SOUND **/
	static const string SOUND_LASER;
	static const string SOUND_BOMB;
	static const string SOUND_DOWN;
	static const string SOUND_UP;

	/** SF TEXTURES **/
	static const string SFTEX_BG;
	static const string SFTEX_SPLASH;
	static const string SFTEX_BOMB;
	static const string SFTEX_SHIELD;

	/** MUSIC **/
	static const string MUSIC_STARSHIP;
	static const string MUSIC_SONG;

	/** FONTS **/
	static const string FONT_STAR;
	static const string FONT_ARIAL;

	/** STRING NAMES **/
	static const string STR_START;
	static const string STR_HIGHSCORES;
	static const string STR_OPTIONS;
	static const string STR_CONTROLS;
	static const string STR_EXIT;
	static const string STR_SOUND;
	static const string STR_BACK;
	static const string STR_RESUME;
	static const string STR_PAUSED;
	static const string STR_GAMEOVER;
	static const string STR_RESTART;

	/** SETTINGS **/
	static const string SETTINGS;

	/** CONST INTS **/
	static const int ENEMYCOLLISIONPOINT;
	static const int ENEMYDEATHPOINT;
	static const int BOMBPOINT;
	static const int BOMBDAMAGE;

	/** CONTROLS **/ 
	static const string CONTROLNAMES[5];
	static const string CONTROLBUTTONS[5];

private:
	Paths(void);
};

#endif