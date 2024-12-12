#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Renderer.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"
#include "Systems.h"
#include "Enemy.h"

int main() {
    // initialize game window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Station Outbreak");

    Renderer::initialise(window);
    //initialize managers and systems
    EntityManager entityManager;
    ComponentManager<TransformComponent> transformManager;
    ComponentManager<GraphicsComponent> graphicsManager;
    ComponentManager<VelocityComponent> velocityManager;
    ComponentManager<HealthComponent> healthManager;
    ComponentManager<FleeComponent> fleeManager;
    ComponentManager<ShootingComponent> shootingManager;
    ComponentManager<SeekComponent> seekManager;      
    ComponentManager<BulletComponent> bulletManager;  
    ComponentManager<MeleeComponent> meleeManager;
	ComponentManager<StateMachineComponent> stateMachineManager;

    ControlsSystem controlsSystem;
    RenderSystem renderSystem;
    MovementSystem movementSystem;
    BulletSystem bulletSystem;
    MeleeSystem meleeSystem;
	StateMachineSystem stateMachineSystem;
    //create player and enemy entities
    Entity player = makePlayer(entityManager, transformManager, graphicsManager, velocityManager, healthManager);
    Entity rangedEnemy = makeRangedEnemy(entityManager, transformManager, graphicsManager, healthManager, fleeManager, shootingManager);
    Entity aggressiveEnemy = makeAggressiveEnemy(entityManager, transformManager, graphicsManager,
                                           healthManager, seekManager, meleeManager);
	Entity idleSeekEnemy = makeIdleSeekEnemy(entityManager, transformManager, graphicsManager,
                                        healthManager, stateMachineManager, meleeManager);	
    Entity bossEnemy = makeBossEnemy(entityManager, transformManager, graphicsManager,
                                healthManager, shootingManager);																	   
    //game loop
    sf::Clock clock;
    while (Renderer::getWindow().isOpen()) {
    sf::Event event;
    while (Renderer::getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            Renderer::getWindow().close();
    }

    float deltaTime = clock.restart().asSeconds();
    //input handling
    controlsSystem.handleInput(deltaTime, window, player, velocityManager, transformManager, 
                             graphicsManager, bulletManager, entityManager);
    //update system
    movementSystem.update(deltaTime, transformManager, velocityManager);
    //update fleeing enemy entities
    movementSystem.updateFlee(deltaTime, transformManager, fleeManager, 
                             transformManager.getComponent(player)->position);
    //update seek 
    movementSystem.updateSeek(deltaTime, transformManager, seekManager, 
                             transformManager.getComponent(player)->position);
    //updates bullets
    bulletSystem.update(deltaTime, transformManager, velocityManager, healthManager, 
                       graphicsManager, shootingManager, fleeManager, bulletManager,
                       seekManager, stateMachineManager, meleeManager,
                       entityManager, transformManager.getComponent(player)->position);
    //updates melee stuff
    meleeSystem.update(deltaTime, transformManager, meleeManager, healthManager,
                   seekManager, stateMachineManager, graphicsManager,
                   bulletManager, velocityManager,
                   entityManager,
                   transformManager.getComponent(player)->position, player);
    //updates state machine stuff
    stateMachineSystem.update(deltaTime, transformManager, stateMachineManager,
                             transformManager.getComponent(player)->position);
    //render
    Renderer::getWindow().clear();
    renderSystem.render(Renderer::getWindow(), transformManager, graphicsManager);
    Renderer::getWindow().display();
}
    //close renderer 
    Renderer::close();
    return 0;
}