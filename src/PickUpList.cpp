#include "PickUpList.h"
#include "Paths.h"
#include "ScoreComponent.h"
#include "PickUpComponent.h"
#include "RenderComponent.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"

vector<GameEntity*> PickUpList::pickUpList;
const float	PickUpList::GAME_Z_DISTANCE = -200.0f;

PickUpList::PickUpList(void) {
}


PickUpList::~PickUpList(void) {
}

void PickUpList::create(sf::RenderWindow &win, GameEntityManager &eMgr, GameResourceManager &rMgr, int amount) {
	for(int i = 0; i < amount ;i++) {
		int r = rand() % 4;

		/** Create crystal entity **/
		GameEntity* crystalEntity = new GameEntity;
		crystalEntity->addComponent(new ScoreComponent(10000));
		crystalEntity->addComponent(new PickUpComponent(false, (PickUpType)r));
		crystalEntity->addComponent(new RenderComponent(*rMgr.getMesh(Paths::OBJ_CRYSTAL), *rMgr.getShader(Paths::SHADER_BASIC)));
		crystalEntity->addComponent(new PositionComponent(glm::vec3(0.0f, -20.0f, GAME_Z_DISTANCE)));
		crystalEntity->addComponent(new VelocityComponent);
		/**	End crystal entity **/

		pickUpList.push_back(crystalEntity);
		eMgr.addEntity(crystalEntity);
	}
}

GameEntity* PickUpList::getPickUp() {
	for(std::vector<GameEntity*>::iterator it = pickUpList.begin(); it != pickUpList.end(); ++it) {
		PickUpComponent* pComp = (PickUpComponent*)(*it)->getComponent<PickUpComponent>();
		if (!pComp->getIsActive()) {
			pComp->setIsActive(true);
			return *it;
		}
	}
	return NULL;
}