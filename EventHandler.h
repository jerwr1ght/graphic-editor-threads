#pragma once
#include "Controller.h"
#include "Line.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Star.h"
#include "Square.h"
#include "Composite.h"
#include <thread>

// Константні розміри фігур
const sf::Vector2f LINE_SIZE(50, 5);
const sf::Vector2f RECTANGLE_SIZE(30, 20);
const float CIRCLE_SIZE = 30;
const float SQUARE_SIZE = 35;
const float TRIANGLE_SIZE = 30;

class EventHandler
{
private:
	sf::Text label;
	sf::Text status;
	
	int max_random_figures_count;


	int get_random_number(int min, int max);
	sf::Color get_random_color();
	void set_status(const sf::String& status_message);
	
	// Для створення фігур на екрані
	Figure* create_figure_by_int(int num);

	// Threading functions
	void create_random_figures(int x_size, int y_size);
	void set_figures_targets();
	void move_by_targets(float delta_time);
	void destroy_targets();
public:
	EventHandler(sf::Text& _label, 
		sf::Text& _status, sf::Font& _font, int figures_count = 10);
	~EventHandler();
	
	// Методи для роботи з діями користувача
	void poll_action_event(sf::RenderWindow& window, float delta_time);

	// Методи для малювання об'єктів
	void poll_draw_event(sf::RenderWindow& window);

};

