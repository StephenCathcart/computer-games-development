#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <typeinfo>
#include <vector>
#include "GameComponent.h"

using std::vector;

/// <summary>
/// An Entity holds various Components.
/// </summary>
class GameEntity {
public:
	GameEntity(void);
	~GameEntity(void);

	/// <summary>
	/// Returns Component T or NULL if T does not exist.
	/// </summary>
	/// <returns></returns>
	template<typename T> 
	GameComponent* getComponent() {
		for (vector<GameComponent*>::iterator i = m_components.begin(); i != m_components.end(); ++i) {
			if(typeid(*(*i)) == typeid(T)) {
				return *i;
			}
		}
		return NULL;
	}

	/// <summary>
	/// Adds a Component to the entity. 
	/// An entity may only store one of each type of Component.
	/// </summary>
	/// <param name="c">The c.</param>
	bool addComponent(GameComponent* c) { 
		for (vector<GameComponent*>::iterator i = m_components.begin(); i != m_components.end(); ++i) {
			if(typeid(*(*i)) == typeid(*c)) {
				return false;
			}
		}
		m_components.push_back(c); 
		return true;
	}

	/// <summary>
	/// Removes Component T from the entity and releases memory.
	/// </summary>
	template<typename T> 
	bool removeComponent() {
		for (vector<GameComponent*>::iterator i = m_components.begin(); i != m_components.end(); ++i) {
			if(typeid(*(*i)) == typeid(T)) {
				delete m_components[i - m_components.begin()];
				i = m_components.erase(i);
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Determines whether this Entity has Component T.
	/// </summary>
	/// <returns></returns>
	template<typename T> 
	bool hasComponent() {
		for (vector<GameComponent*>::iterator i = m_components.begin(); i != m_components.end(); ++i) {
			if(typeid(*(*i)) == typeid(T)) {
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Gets the amount of components.
	/// </summary>
	/// <returns></returns>
	int getComponentAmount() { return m_components.size();}

private:
	vector<GameComponent*> m_components;
};

#endif