#include "GameResourceManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="GameResourceManager" /> class.
/// </summary>
GameResourceManager::GameResourceManager(void) {
	m_meshes = new map<const string, Mesh*>;
	m_shaders = new map<const string, Shader*>;
	m_sounds = new map<const string, sf::SoundBuffer*>;
	m_textures = new map<const string, sf::Texture*>;
	m_music = new sf::Music;
}

/// <summary>
/// Finalizes an instance of the <see cref="GameResourceManager"/> class.
/// Removes all Meshes, Shaders, SoundBuffers and Music from manager and
/// releases memory.
/// </summary>
GameResourceManager::~GameResourceManager(void) {
	// Delete all meshes
	std::map<const string, Mesh*>::iterator mIt;
	mIt = m_meshes->begin();
	while(mIt != m_meshes->end()) {
	  delete mIt->second;
	  mIt++;
	}
	m_meshes->clear();
	delete m_meshes;

	// Delete all shaders
	std::map<const string, Shader*>::iterator sIt;
	sIt = m_shaders->begin();
	while(sIt != m_shaders->end()) {
	  delete sIt->second;
	  sIt++;
	}
	m_shaders->clear();
	delete m_shaders;

	// Delete all sound buffers
	std::map<const string,  sf::SoundBuffer*>::iterator bIt;
	bIt = m_sounds->begin();
	while(bIt != m_sounds->end()) {
	  delete bIt->second;
	  bIt++;
	}
	m_sounds->clear();
	delete m_sounds;

	// Delete all textures
	std::map<const string,  sf::Texture*>::iterator tIt;
	tIt = m_textures->begin();
	while(tIt != m_textures->end()) {
	  delete tIt->second;
	  tIt++;
	}
	m_textures->clear();
	delete m_textures;

	// Delete music
	delete m_music;
}

/// <summary>
/// Adds an .OBJ Mesh to manager.
/// </summary>
/// <param name="meshName">Name of the mesh.</param>
/// <param name="objFile">The OBJ file.</param>
/// <param name="texFile">The texture file.</param>
bool GameResourceManager::addObjMesh(const string &objFile, const string &texFile) {
	Mesh* mesh = Mesh::importModel(objFile);
	if(!texFile.empty()) {
		mesh->setTexture(SOIL_load_OGL_texture(texFile.c_str(),
			SOIL_LOAD_AUTO , SOIL_CREATE_NEW_ID , 0));
		if (!mesh->getTexture()) {
			return false;
		}	
	}
	(*m_meshes)[objFile] = mesh;
	return true;
}

/// <summary>
/// Gets a Mesh by name.
/// </summary>
/// <param name="meshName">Name of the mesh.</param>
/// <returns></returns>
Mesh* GameResourceManager::getMesh(const string &meshPath) {
	Mesh& m = *(*m_meshes)[meshPath];
	if(&m) {
		return &m;
	}
	return NULL;
}

/// <summary>
/// Removes a Mesh by name and releases memory.
/// </summary>
/// <param name="meshName">Name of the mesh.</param>
bool GameResourceManager::removeMesh(const string &meshPath) {
	for (map<const string, Mesh*>::iterator it = m_meshes->begin(); it != m_meshes->end(); ++it) {
		if (it->second == (*m_meshes)[meshPath]) { 
			delete it->second;
			m_meshes->erase(it);
			return true;
		}
	}
	return false;
}

/// <summary>
/// Adds a Shader to manager.
/// </summary>
/// <param name="shaderName">Name of the shader.</param>
/// <param name="vertex">The vertex shader.</param>
/// <param name="fragment">The fragment shader.</param>
/// <param name="geometry">The geometry shader.</param>
/// <param name="tcs">The TCS shader.</param>
/// <param name="tes">The TES shader.</param>
bool GameResourceManager::addShader(const string &shaderName, const string &vertex, 
			   const string &fragment,   const string &geometry, 
			   const string &tcs,   const string &tes) {
	Shader* shader = new Shader(vertex, fragment, geometry, tcs, tes);
	if (!shader->linkProgram ()) {
		return false;
	}
	(*m_shaders)[shaderName] = shader;
	return true;
}

/// <summary>
/// Gets a Shader by name.
/// </summary>
/// <param name="shaderName">Name of the shader.</param>
/// <returns></returns>
Shader* GameResourceManager::getShader(const string &shaderName) {
	Shader& s = *(*m_shaders)[shaderName];
	if(&s) {
		return &s;
	}
	return NULL;
}

/// <summary>
/// Removes a Shader from manager and releases memory.
/// </summary>
/// <param name="shaderName">Name of the shader.</param>
/// <returns></returns>
bool GameResourceManager::removeShader(const string &shaderName) {
	for (map<const string, Shader*>::iterator it = m_shaders->begin(); it != m_shaders->end(); ++it) {
		if (it->second == (*m_shaders)[shaderName]) { 
			delete it->second;
			m_shaders->erase(it);
			return true;
		}
	}
	return false;
}

/// <summary>
/// Adds a Sound Buffer to the manager.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns></returns>
bool GameResourceManager::addSoundBuffer(const string fileName) {
	// Load sound from file into new buffer
	sf::SoundBuffer* buffer = new sf::SoundBuffer;
    if (!buffer->loadFromFile(fileName)) {
        return false; // error
	}
	// Store new buffer in map
	(*m_sounds)[fileName] = buffer;
	return true;
}

/// <summary>
/// Gets a Sound Buffer by name.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns></returns>
sf::SoundBuffer* GameResourceManager::getSoundBuffer(const string fileName) const {
	sf::SoundBuffer& s = *(*m_sounds)[fileName];
	if(&s) {
		return &s;
	}
	return NULL;
}

/// <summary>
/// Removes a Sound Buffer from manager and releases memory.
/// </summary>
/// <param name="fileName">Name of the file.</param>
bool GameResourceManager::removeSoundBuffer(const string fileName) {
	for (map<const string, sf::SoundBuffer*>::iterator it = m_sounds->begin(); it != m_sounds->end(); ++it) {
		if (it->second == (*m_sounds)[fileName]) { 
			delete it->second;
			m_sounds->erase(it);
			return true;
		}
	}
	return false;
}

/// <summary>
/// Adds a Texture to the manager.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns></returns>
bool GameResourceManager::addTexture(const string fileName) {
	// TODO OpenGL call error on second call to this function
	// Load sound from file into new buffer
	sf::Texture* texture = new sf::Texture;
    if (!texture->loadFromFile(fileName)) {
        return false; // error
	}
	// Store new buffer in map
	(*m_textures)[fileName] = texture;
	return true;
}

/// <summary>
/// Gets a Texture by name.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns></returns>
sf::Texture* GameResourceManager::getTexture(const string fileName) const {
	sf::Texture& t = *(*m_textures)[fileName];
	if(&t) {
		return &t;
	}
	return NULL;
}

/// <summary>
/// Removes a Texture from manager and releases memory.
/// </summary>
/// <param name="fileName">Name of the file.</param>
bool GameResourceManager::removeTexture(const string fileName) {
	for (map<const string, sf::Texture*>::iterator it = m_textures->begin(); it != m_textures->end(); ++it) {
		if (it->second == (*m_textures)[fileName]) { 
			delete it->second;
			m_textures->erase(it);
			return true;
		}
	}
	return false;
}

/// <summary>
/// Loads Music to manager. Only one SFML Music instance can be
/// streamed at once. 
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns></returns>
bool GameResourceManager::loadMusic(const string fileName) {
	return m_music->openFromFile(fileName);
}

/// <summary>
/// Returns the Music stream.
/// </summary>
/// <returns></returns>
sf::Music& GameResourceManager::getMusic() const {
	return *m_music;
}

void GameResourceManager::clear() {
	// Delete all meshes
	std::map<const string, Mesh*>::iterator mIt;
	mIt = m_meshes->begin();
	while(mIt != m_meshes->end()) {
	  delete mIt->second;
	  mIt++;
	}
	m_meshes->clear();

	// Delete all shaders
	std::map<const string, Shader*>::iterator sIt;
	sIt = m_shaders->begin();
	while(sIt != m_shaders->end()) {
	  delete sIt->second;
	  sIt++;
	}
	m_shaders->clear();

	// Delete all sound buffers
	std::map<const string,  sf::SoundBuffer*>::iterator bIt;
	bIt = m_sounds->begin();
	while(bIt != m_sounds->end()) {
	  delete bIt->second;
	  bIt++;
	}
	m_sounds->clear();

	// Delete all textures
	std::map<const string,  sf::Texture*>::iterator tIt;
	tIt = m_textures->begin();
	while(tIt != m_textures->end()) {
	  delete tIt->second;
	  tIt++;
	}
	m_textures->clear();
}