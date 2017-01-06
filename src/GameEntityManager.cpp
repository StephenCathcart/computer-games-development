#include "GameEntityManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="GameEntityManager" /> class.
/// </summary>
GameEntityManager::GameEntityManager(void) {
}

/// <summary>
/// Finalizes an instance of the <see cref="GameEntityManager" /> class.
/// Removes all entities in manager and releases memory.
/// </summary>
GameEntityManager::~GameEntityManager(void) {
	for (int i = 0; i < m_entities.size(); i++) {       
        delete m_entities[i];    
    }    
    m_entities.clear(); 
}
