#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "LevelOneScene.h"
#include <memory>
#include <map>

std::map<std::string, std::shared_ptr<Scene>> scenes;
std::shared_ptr<Scene> activeScene;
//function for loading scenes
void loadScenes() {
	scenes["LevelOne"] = std::make_shared<LevelOneScene>();

	for (auto& pair : scenes) {
		const auto& name = pair.first;
		auto& scene = pair.second;
		scene->load();
	}
}

int main() {
	//create window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "StationOutbreak");
	Renderer::initialise(window);
	//load scenes
	loadScenes();
	activeScene = scenes["LevelOne"];
	//main game loop
	sf::Clock clock;
	bool isRunning = true;

	while (isRunning) {
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				isRunning = false;
				window.close();
			}
		}
		//update active scene
		if (activeScene) {
			activeScene->update(deltaTime);
		}

		window.clear();
		//render active scene
		if (activeScene) {
			activeScene->render();
		}
		Renderer::render();
		window.display();
	}

	Renderer::close();
	return 0;
}