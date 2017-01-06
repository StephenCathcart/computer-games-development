#include "BulletList.h"
#include "RenderComponent.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "BulletComponent.h"
#include "Paths.h"

vector<GameEntity*> BulletList::bulletList;
const float	BulletList::GAME_Z_DISTANCE = -200.0f;

BulletList::BulletList(void) {
}


BulletList::~BulletList(void) {
}

void BulletList::create(sf::RenderWindow &win, GameEntityManager &eMgr, GameResourceManager &rMgr, int amount) {
	for(int i = 0; i < amount ;i++) {
		/** Create bullet entity **/
		GameEntity* bulletEntity = new GameEntity;
		bulletEntity->addComponent(new RenderComponent(*rMgr.getMesh(Paths::OBJ_BULLET), *rMgr.getShader(Paths::SHADER_BULLET)));
		bulletEntity->addComponent(new PositionComponent(glm::vec3(0, 0, GAME_Z_DISTANCE)));
		bulletEntity->addComponent(new VelocityComponent(glm::vec3(5.0f, 0.0f, 0.0f)));
		bulletEntity->addComponent(new BulletComponent(false));
		/**	End bullet entity **/

		bulletList.push_back(bulletEntity);
		eMgr.addEntity(bulletEntity);
	}
}

GameEntity* BulletList::getBullet() {
	for(std::vector<GameEntity*>::iterator it = bulletList.begin(); it != bulletList.end(); ++it) {
		BulletComponent* bComp = (BulletComponent*)(*it)->getComponent<BulletComponent>();
		if (!bComp->getIsActive()) {
			bComp->setIsActive(true);
			return *it;
		}
	}
	return NULL;
}