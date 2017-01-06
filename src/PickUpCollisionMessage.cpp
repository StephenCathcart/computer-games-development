#include "PickUpCollisionMessage.h"


/// <summary>
/// Initializes a new instance of the <see cref="PickUpCollisionEvent" /> class.
/// </summary>
/// <param name="entity">The entity.</param>
PickUpCollisionMessage::PickUpCollisionMessage(GameEntity* entity) {
	m_type = MessageType::PICKUPCOLLISION;
	m_entity = entity;
}


/// <summary>
/// Finalizes an instance of the <see cref="PickUpCollisionEvent"/> class.
/// </summary>
PickUpCollisionMessage::~PickUpCollisionMessage(void) {
}
