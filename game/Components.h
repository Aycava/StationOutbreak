#pragma once
#include <SFML/Graphics.hpp>

struct TransformComponent {
    sf::Vector2f position;
    sf::Vector2f size;
};

struct VelocityComponent {
    sf::Vector2f velocity;
};

struct HealthComponent {
    int health;
};

struct GraphicsComponent {
    sf::RectangleShape shape;
};

struct FleeComponent {
    float speed;
    float fleeRadius;
};

struct ShootingComponent {
    float shootInterval;
    float shootTimer;
};

struct BulletComponent {
    bool isPlayerBullet;
    BulletComponent(bool isPlayer) : isPlayerBullet(isPlayer) {}
};

struct SeekComponent {
    float speed;
    SeekComponent(float spd) : speed(spd) {}
};

struct MeleeComponent {
    float damage;
    float attackRadius;
    float attackCooldown;
    float currentCooldown;
    
    MeleeComponent(float dmg = 1.0f, float radius = 60.0f, float cooldown = 1.0f)
        : damage(dmg), attackRadius(radius), attackCooldown(cooldown), currentCooldown(0.0f) {}
};

struct StateMachineComponent {
    enum State { Idle, Seek } currentState;
    float triggerRadius;
    float speed;

    StateMachineComponent(float radius, float spd)
        : currentState(Idle), triggerRadius(radius), speed(spd) {}
};


	
	