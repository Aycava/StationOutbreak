#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"


Entity makePlayer(EntityManager& entityManager,
                 ComponentManager<TransformComponent>& transforms,
                 ComponentManager<GraphicsComponent>& graphics,
                 ComponentManager<VelocityComponent>& velocities,
                 ComponentManager<HealthComponent>& healths) {  
    Entity player = entityManager.createEntity();
    
    transforms.addComponent(player, { {960, 540}, {50,50} });
    
    sf::RectangleShape shape({50,50});
    shape.setFillColor(sf::Color::Cyan);
    graphics.addComponent(player, { shape });
    
    velocities.addComponent(player, { {0,0} });
    
    healths.addComponent(player, { 3 });  
    
    return player;
}	