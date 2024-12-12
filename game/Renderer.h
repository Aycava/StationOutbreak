#include <SFML/Graphics.hpp>
#pragma once
//manages rendering
namespace Renderer {
	//initialize renderer with reference to main render window
	void initialise(sf::RenderWindow&);
	//retrieve current render window
	sf::RenderWindow& getWindow();
	//close renderer
	void close();
	//updates renderer
	void update(const double&);
	//adds drawable object
	void addDrawable(const sf::Drawable* drawable);
	//renders all queued drawables to screen
	void render();
};
