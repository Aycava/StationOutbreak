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
	//Removes entity from active entity list
	void destroyEntity(Entity entity) {
		auto it = std::find(activeEntities.begin(), activeEntities.end(), entity);
		if (it != activeEntities.end()) {
			activeEntities.erase(it);
		}
	}

	std::size_t size() const {
		return activeEntities.size();
	}
	
private:
	//trakcs unique ids
	Entity nextEntityId = 0;
	std::vector<Entity> activeEntities;
};	