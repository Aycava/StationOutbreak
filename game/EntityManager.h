#pragma once
#include <vector>

using Entity = std::size_t;
//class for managing entities
class EntityManager {
public:
	//creates new entity and assigns unique id
	Entity createEntity() {
		Entity entity = nextEntityId;
		activeEntities.push_back(entity);
		return nextEntityId++;
	}
	//adds entity to destroyed list
	void destroyEntity(Entity entity) {
		destroyedEntities.push_back(entity);

		auto it = std::find(activeEntities.begin(), activeEntities.end(), entity);
		if (it != activeEntities.end()) {
			activeEntities.erase(it);
		}
	}
	//retrieves list of destroyed entities
	const std::vector<Entity>& getDestroyedEntities() const {
		return destroyedEntities;
	}
	//clears list of destroyed entities
	void clearDestroyedEntities() {
		destroyedEntities.clear();
	}

	std::size_t size() const {
		return activeEntities.size();
	}
	
private:
	//trakcs unique ids
	Entity nextEntityId = 0;
	//stores destroyed entity ids
	std::vector<Entity> destroyedEntities;
	std::vector<Entity> activeEntities;
};	