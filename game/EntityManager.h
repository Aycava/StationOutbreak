#pragma once
#include <vector>

using Entity = std::size_t;
//class for managing entities
class EntityManager {
public:
	//creates new entity and assigns unique id
	Entity createEntity() {
		return nextEntityId++;
	}
	//adds entity to destroyed list
	void destroyEntity(Entity entity) {
		destroyedEntities.push_back(entity);
	}
	//retrieves list of destroyed entities
	const std::vector<Entity>& getDestroyedEntities() const {
		return destroyedEntities;
	}
	//clears list of destroyed entities
	void clearDestroyedEntities() {
		destroyedEntities.clear();
	}
	
private:
	//trakcs unique ids
	Entity nextEntityId = 0;
	//stores destroyed entity ids
	std::vector<Entity> destroyedEntities;
};	