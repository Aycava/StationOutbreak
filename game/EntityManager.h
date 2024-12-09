#pragma once
#include <vector>

using Entity = std::size_t;

class EntityManager {
public:
	Entity createEntity() {
		return nextEntityId++;
	}

	void destroyEntity(Entity entity) {
		destroyedEntities.push_back(entity);
	}

	const std::vector<Entity>& getDestroyedEntities() const {
		return destroyedEntities;
	}

	void clearDestroyedEntities() {
		destroyedEntities.clear();
	}
	
private:
	Entity nextEntityId = 0;
	std::vector<Entity> destroyedEntities;
};	