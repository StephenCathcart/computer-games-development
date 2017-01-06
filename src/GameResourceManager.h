#ifndef GAMERESOURCEMANAGER_H
#define GAMERESOURCEMANAGER_H

#include <map>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "SOIL.h"

using std::string;
using std::map;

/// <summary>
/// Container for all Mesh, Shader, SoundBuffer and Music resources.
/// </summary>
class GameResourceManager {
public:
	GameResourceManager(void);
	~GameResourceManager(void);

	bool				addObjMesh(const string &objFile, const string &texFile);
	Mesh*				getMesh(const string &meshPath);
	bool				removeMesh(const string &meshPath);

	bool				addShader(const string &shaderName, const string &vertex, 
								  const string &fragment,   const string &geometry = "", 
								  const string &tcs = "",   const string &tes = "");
	Shader*				getShader(const string &shaderName);
	bool				removeShader(const string &shaderName);

	bool				addSoundBuffer(const string fileName);
	sf::SoundBuffer*	getSoundBuffer(const string fileName) const;
	bool				removeSoundBuffer(const string fileName);

	bool				addTexture(const string fileName);
	sf::Texture*		getTexture(const string fileName) const;
	bool				removeTexture(const string fileName);

	bool				loadMusic(const string fileName);
	sf::Music&			getMusic() const;

	void				clear();

private:
	map<const string, Mesh*>*				m_meshes;
	map<const string, Shader*>*				m_shaders;
	map<const string, sf::SoundBuffer*>*	m_sounds;
	map<const string, sf::Texture*>*		m_textures;
	sf::Music*								m_music;
};

#endif

