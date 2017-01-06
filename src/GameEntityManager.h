#ifndef GAMEENTITYMANAGER_H
#define GAMEENTITYMANAGER_H

#include <typeinfo>
#include <vector>
#include "GameEntity.h"

using std::vector;

/// <summary>
/// Container for instances of GameEntitys.
/// </summary>
class GameEntityManager {
public:
	GameEntityManager(void);
	~GameEntityManager(void);

	/// <summary>
	/// Adds an entity to the manager.
	/// </summary>
	/// <param name="o">The entity.</param>
	void addEntity(GameEntity* e) { m_entities.push_back(e); }

	/// <summary>
	/// Removes an entity from the manager.
	/// </summary>
	/// <param name="e">The entity.</param>
	/// <returns></returns>
	bool removeEntity(GameEntity* e) {
		for (vector<GameEntity*>::iterator i = m_entities.begin(); i != m_entities.end(); ++i) {
			if(*i == e) {
				delete m_entities[i - m_entities.begin()];
				i = m_entities.erase(i);
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Returns a vector of Entities with a Component of type T.
	/// </summary>
	/// <returns></returns>
	template<typename T> 
	vector<GameEntity*> getEntitiesWithComponentType() {
		vector<GameEntity*> matchedEntites;
		for (vector<GameEntity*>::iterator i = m_entities.begin(); i != m_entities.end(); ++i) {
			if((*i)->hasComponent<T>()) {
				matchedEntites.push_back(*i);
			}
		}
		return matchedEntites;
	}

	/// <summary>
	/// Gets the amount of enities.
	/// </summary>
	/// <returns></returns>
	int getEnitiesAmount() { return m_entities.size(); }

	int getEntityComponentsAmount() {
		float amount = 0;
		for(int i = 0; i < m_entities.size() ;i++) {
			amount += m_entities[i]->getComponentAmount();
		}
		return amount;
	}

	void clear() {
		for (int i = 0; i < m_entities.size(); i++) {       
			delete m_entities[i];    
		}    
		m_entities.clear(); 
	}

private:
	vector<GameEntity*> m_entities;
};

#endif