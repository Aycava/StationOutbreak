#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"
#include <SFML/Graphics.hpp>

Entity makeRangedEnemy(EntityManager& entityManager,
                       ComponentManager<TransformComponent>& transforms,
                       ComponentManager<GraphicsComponent>& graphics,
                       ComponentManager<HealthComponent>& healths,
                       ComponentManager<FleeComponent>& flees,
                       ComponentManager<ShootingComponent>& shootings) {
    Entity enemy = entityManager.createEntity();

    transforms.addComponent(enemy, { {500, 300}, {40, 40} });

    sf::RectangleShape shape({ 40, 40 });
    shape.setFillColor(sf::Color::Green); 
    graphics.addComponent(enemy, { shape });

    healths.addComponent(enemy, { 1 });

    flees.addComponent(enemy, { 100.f, 200.f }); 
    shootings.addComponent(enemy, { 1.0f, 0.0f }); 

    return enemy;
}

Entity makeAggressiveEnemy(EntityManager& entityManager,
                          ComponentManager<TransformComponent>& transforms,
                          ComponentManager<GraphicsComponent>& graphics,
                          ComponentManager<HealthComponent>& healths,
                          ComponentManager<SeekComponent>& seekers,
                          ComponentManager<MeleeComponent>& melees) {  
    Entity enemy = entityManager.createEntity();

    transforms.addComponent(enemy, { {700, 300}, {40, 40} });

    sf::RectangleShape shape({40, 40});
    shape.setFillColor(sf::Color::Red);
    graphics.addComponent(enemy, {shape});

    healths.addComponent(enemy, {2});
    seekers.addComponent(enemy, {150.f});
    melees.addComponent(enemy, MeleeComponent(1.0f, 60.0f, 1.0f));  

    return enemy;
}

Entity makeIdleSeekEnemy(EntityManager& entityManager,
                         ComponentManager<TransformComponent>& transforms,
                         ComponentManager<GraphicsComponent>& graphics,
                         ComponentManager<HealthComponent>& healths,
                         ComponentManager<StateMachineComponent>& stateMachines,
                         ComponentManager<MeleeComponent>& melees) {  
    Entity enemy = entityManager.createEntity();

    transforms.addComponent(enemy, { {1500, 800}, {40, 40} });

    sf::RectangleShape shape({40, 40});
    shape.setFillColor(sf::Color::Yellow);
    graphics.addComponent(enemy, {shape});

    healths.addComponent(enemy, {2});
    stateMachines.addComponent(enemy, {150.f, 200.f});
    melees.addComponent(enemy, MeleeComponent(1.0f, 60.0f, 1.0f));  

    return enemy;
}
