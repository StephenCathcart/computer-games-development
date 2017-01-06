#include "Paths.h"

/** OBJ **/
const string Paths::OBJ_SHIP_PLAYER		= "Data/Obj/ship_player.obj";
const string Paths::OBJ_SHIP_LARGE		= "Data/Obj/ship_large.obj";
const string Paths::OBJ_SHIP_MEDIUM		= "Data/Obj/ship_medium.obj";
const string Paths::OBJ_SHIP_SMALL		= "Data/Obj/ship_small.obj";
const string Paths::OBJ_CRATE			= "Data/Obj/crate.obj";
const string Paths::OBJ_CRYSTAL			= "Data/Obj/crystal.obj";
const string Paths::OBJ_BULLET			= "Data/Obj/bullet.obj";;

/** TEXTURES **/
const string Paths::TEX_SHIP_PLAYER		= "Data/Textures/ship_player.png";
const string Paths::TEX_SHIP_LARGE		= "Data/Textures/ship_large.jpg";
const string Paths::TEX_SHIP_MEDIUM		= "Data/Textures/ship_medium.png";
const string Paths::TEX_SHIP_SMALL		= "Data/Textures/ship_small.jpg";
const string Paths::TEX_CRATE			= "Data/Textures/crate.jpg";
const string Paths::TEX_CRYSTAL			= "Data/Textures/crystal.jpg";

/** SHADERS **/
const string Paths::SHADER_VERT_BASIC	= "Data/Shaders/Basic/vert.glsl";
const string Paths::SHADER_FRAG_BASIC	= "Data/Shaders/Basic/frag.glsl";
const string Paths::SHADER_VERT_BULLET	= "Data/Shaders/Bullet/vert.glsl";
const string Paths::SHADER_FRAG_BULLET	= "Data/Shaders/Bullet/frag.glsl";

/** SHADERS NAMES**/
const string Paths::SHADER_BASIC		= "BASIC";
const string Paths::SHADER_BULLET		= "BULLET";

/** SOUND **/
const string Paths::SOUND_LASER			= "Data/SFX/laser.wav";
const string Paths::SOUND_BOMB			= "Data/SFX/bomb.wav";
const string Paths::SOUND_DOWN			= "Data/SFX/moveDown.wav";
const string Paths::SOUND_UP			= "Data/SFX/moveUp.wav";

/** SF TEXTURES **/
const string Paths::SFTEX_BG			= "Data/Overlay/bg.jpg";;
const string Paths::SFTEX_SPLASH		= "Data/Overlay/splash.png";
const string Paths::SFTEX_BOMB			= "Data/Overlay/bomb.png";
const string Paths::SFTEX_SHIELD		= "Data/Overlay/shield.png";

/** MUSIC **/
const string Paths::MUSIC_STARSHIP		= "Data/Music/starship.ogg";
const string Paths::MUSIC_SONG			= "Data/Music/sadame.ogg";

/** FONTS **/
const string Paths::FONT_STAR			= "Data/Fonts/starcraft.ttf";
const string Paths::FONT_ARIAL			= "Data/Fonts/arial.ttf";

/** STRING NAMES **/
const string Paths::STR_START			= "START";
const string Paths::STR_HIGHSCORES		= "HIGHSCORES";
const string Paths::STR_OPTIONS			= "OPTIONS";
const string Paths::STR_CONTROLS		= "CONTROLS";
const string Paths::STR_EXIT			= "EXIT";
const string Paths::STR_SOUND			= "SOUND:";
const string Paths::STR_BACK			= "BACK";
const string Paths::STR_RESUME			= "RESUME";
const string Paths::STR_PAUSED			= "PAUSED";
const string Paths::STR_GAMEOVER		= "GAME OVER";
const string Paths::STR_RESTART			= "RESTART";

/** SETTINGS **/
const string Paths::SETTINGS			= "Data/Settings/settings.game";

/** CONST INTS **/
const int Paths::ENEMYCOLLISIONPOINT	= 10;
const int Paths::ENEMYDEATHPOINT		= 500;
const int Paths::BOMBPOINT				= 100;
const int Paths::BOMBDAMAGE				= 100;

const string Paths::CONTROLNAMES[5] = {
	"FIRE:",
	"BOMB:",
	"SELECT:",
	"PAUSE:",
	"MOVE:"
};

const string Paths::CONTROLBUTTONS[5] = {
	"A | SPACE",
	"E | X",
	"SPACE | A",
	"ESC | START",
	"WASD | ANALOG"
};

Paths::Paths(void){
}

Paths::~Paths(void){
}