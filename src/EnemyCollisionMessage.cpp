#include "EnemyCollisionMessage.h"


/// <summary>
/// Initializes a new instance of the <see cref="EnemyCollisionEvent"/> class.
/// </summary>
/// <param name="entity">The entity.</param>
EnemyCollisionMessage::EnemyCollisionMessage(GameEntity* entity) {
	m_type = MessageType::ENEMYCOLLISION;
	m_entity = entity;
}


/// <summary>
/// Finalizes an instance of the <see cref="EnemyCollisionEvent"/> class.
/// </summary>
EnemyCollisionMessage::~EnemyCollisionMessage(void) {
}
