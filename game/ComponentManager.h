//ComponentManager.h
#pragma once
#include <unordered_map>
#include <memory>
//template class used to manage components
template <typename Component>
class ComponentManager {
public:
	//adds component to array
	void addComponent(Entity entity, Component component) {
		components[entity] = std::make_shared<Component>(std::move(component));
	}
	//gets the component associated with a specific entity
	std::shared_ptr<Component> getComponent(Entity entity) {
		if (components.find(entity) != components.end()) {
			return components[entity];
		}
		return nullptr;
	}
	//removes component associated with specified entity
	void removeComponent(Entity entity) {
		components.erase(entity);
	}
	//retrieves all components associated with this manager
	const std::unordered_map<Entity, std::shared_ptr<Component>>& getComponents() const {
		return components;
	}

private:
	//maps entity ids to components
	std::unordered_map<Entity, std::shared_ptr<Component>> components;
};	