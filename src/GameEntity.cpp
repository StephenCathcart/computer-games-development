#include "GameEntity.h"

/// <summary>
/// Initializes a new instance of the <see cref="GameEntity" /> class.
/// </summary>
GameEntity::GameEntity(void) {
}

/// <summary>
/// Removes all Components from Entity and releases memory.
/// </summary>
GameEntity::~GameEntity(void) {
	for (int i = 0; i < m_components.size(); i++) {       
        delete m_components[i];    
    }    
    m_components.clear(); 
}