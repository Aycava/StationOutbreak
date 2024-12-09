#pragma once
#include <unordered_map>
#include <memory>

template <typename Component>
class ComponentManager {
public:
	void addComponent(Entity entity, Component component) {
		components[entity] = std::make_shared<Component>(std::move(component));
	}

	std::shared_ptr<Component> getComponent(Entity entity) {
		if (components.find(entity) != components.end()) {
			return components[entity];
		}
		return nullptr;
	}

	void removeComponent(Entity entity) {
		components.erase(entity);
	}

	const std::unordered_map<Entity, std::shared_ptr<Component>>& getComponents() const {
		return components;
	}

private:
	std::unordered_map<Entity, std::shared_ptr<Component>> components;
};	