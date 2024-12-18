#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"

//creates player entity with specific components
inline Entity makePlayer(EntityManager& entityManager,
                 ComponentManager<TransformComponent>& transforms,
                 ComponentManager<GraphicsComponent>& graphics,
                 ComponentManager<VelocityComponent>& velocities,
                 ComponentManager<HealthComponent>& healths) {  
    //create new entity for player
    Entity player = entityManager.createEntity();
    //defines players initial position and size
    transforms.addComponent(player, { {960, 540}, {50,50} });
    //graphics component for visual representation
    sf::RectangleShape shape({50,50});
    shape.setFillColor(sf::Color::Cyan);
    graphics.addComponent(player, { shape });
    //add velocity component to handle movement
    velocities.addComponent(player, { {0,0} });
    //add health component with 3 health for player
    healths.addComponent(player, { 3 });  
    
    return player;
}	