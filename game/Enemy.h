#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"
#include <SFML/Graphics.hpp>
//creates ranged enemy with fleeing and shooting behavior
inline Entity makeRangedEnemy(EntityManager& entityManager,
                       ComponentManager<TransformComponent>& transforms,
                       ComponentManager<GraphicsComponent>& graphics,
                       ComponentManager<HealthComponent>& healths,
                       ComponentManager<FleeComponent>& flees,
                       ComponentManager<ShootingComponent>& shootings) {
    Entity enemy = entityManager.createEntity();
    //add transform component that defines initial position and size of entity
    transforms.addComponent(enemy, { {500, 300}, {40, 40} });
    //add graphics component for visual representation
    sf::RectangleShape shape({ 40, 40 });
    shape.setFillColor(sf::Color::Green); 
    graphics.addComponent(enemy, { shape });
    //add health component defining enemy as having 1 health
    healths.addComponent(enemy, { 1 });
    //add flee component for fleeing behavior
    flees.addComponent(enemy, { 100.f, 200.f }); 
    //add shooting component for shooting behavior, shoots once per second
    shootings.addComponent(enemy, { 1.0f, 0.0f }); 

    return enemy;
}
//creates aggressive enemy with seeking and melee attack behavior
inline Entity makeAggressiveEnemy(EntityManager& entityManager,
                          ComponentManager<TransformComponent>& transforms,
                          ComponentManager<GraphicsComponent>& graphics,
                          ComponentManager<HealthComponent>& healths,
                          ComponentManager<SeekComponent>& seekers,
                          ComponentManager<MeleeComponent>& melees) {  
    Entity enemy = entityManager.createEntity();
    //add transform component to define initial position and size
    transforms.addComponent(enemy, { {700, 300}, {40, 40} });
    //graphics component for visual representation
    sf::RectangleShape shape({40, 40});
    shape.setFillColor(sf::Color::Red);
    graphics.addComponent(enemy, {shape});
    //health component setting health to 2
    healths.addComponent(enemy, {2});
    //seek component for seeking behavior
    seekers.addComponent(enemy, {150.f});
    //melee component
    melees.addComponent(enemy, MeleeComponent(1.0f, 60.0f, 1.0f));  

    return enemy;
}
//makes idle seek enemy with state machine and melee attack behavior
inline Entity makeIdleSeekEnemy(EntityManager& entityManager,
                         ComponentManager<TransformComponent>& transforms,
                         ComponentManager<GraphicsComponent>& graphics,
                         ComponentManager<HealthComponent>& healths,
                         ComponentManager<StateMachineComponent>& stateMachines,
                         ComponentManager<MeleeComponent>& melees) {  
    Entity enemy = entityManager.createEntity();
    //transform component for initial enemy position and size
    transforms.addComponent(enemy, { {1500, 800}, {40, 40} });
    //graphics component for visual representation
    sf::RectangleShape shape({40, 40});
    shape.setFillColor(sf::Color::Yellow);
    graphics.addComponent(enemy, {shape});
    //health component with 2 health
    healths.addComponent(enemy, {2});
    //state machine for state based behavior, trigger radius 150 speed 200
    stateMachines.addComponent(enemy, {150.f, 200.f});
    //melee component for melee attack behavior
    melees.addComponent(enemy, MeleeComponent(1.0f, 60.0f, 1.0f));  

    return enemy;
}
//creates boss enemy with high health and frequent shooting
inline Entity makeBossEnemy(EntityManager& entityManager,
                    ComponentManager<TransformComponent>& transforms,
                    ComponentManager<GraphicsComponent>& graphics,
                    ComponentManager<HealthComponent>& healths,
                    ComponentManager<ShootingComponent>& shootings) {
    Entity enemy = entityManager.createEntity();

    // Make boss much larger (200x200) and position near top center
    transforms.addComponent(enemy, { {860, 100}, {200, 200} });

	// When creating the boss
	shootings.addComponent(enemy, { 2.0f, 0.0f }); // Shoots every 2 seconds

    sf::RectangleShape shape({200, 200});
    shape.setFillColor(sf::Color::Magenta); // Different color to distinguish boss
    graphics.addComponent(enemy, { shape });

    healths.addComponent(enemy, { 20 }); // Much more health than regular enemies

    // Shoots more frequently than regular enemies
    shootings.addComponent(enemy, { 0.5f, 0.0f }); // Shoots every 0.5 seconds

    return enemy;
}
