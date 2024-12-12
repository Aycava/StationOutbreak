#include "Renderer.h"
#include <queue>

using namespace std;
using namespace sf;
//global queue to hold drawable objects
static queue<const Drawable *> sprites;
//pointer to render window
static RenderWindow *rw;
//initializes renderer with given render window
void Renderer::initialise(sf::RenderWindow &r) { rw = &r; }
//retrieves currently initialized render window
sf::RenderWindow &Renderer::getWindow() { return *rw; }
//closes renderer and clears drawable queue
void Renderer::close() {
	while (!sprites.empty())
		sprites.pop();
}
//renders all drawable objects in queue to render window
void Renderer::render() {
	if (rw == nullptr) {
		throw std::runtime_error("Render window not set");
	}
	while (!sprites.empty()) {
		rw->draw(*sprites.front());
		sprites.pop();
	}
	
}
//add drawable object to queue
void Renderer::addDrawable(const sf::Drawable* drawable) { 
	sprites.push(drawable); 
}
	
