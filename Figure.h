#pragma once
#include <SFML/Graphics.hpp>
using namespace std;


class Figure
{
protected:
	sf::Color default_color;
	bool trail_movement;
	bool visible;
	Figure* target;
public:
	virtual ~Figure() {};
	void set_trail_movement(bool);
	bool get_trail_movement();
	bool is_visible();
	void hide();
	void show();
	Figure* get_target();
	void set_target(Figure* target);
	virtual void reset();
	virtual bool check_intersection(Figure*);
	virtual bool check_intersection(float, float);
	virtual bool check_window_collision(sf::RenderWindow&);
	void auto_move(sf::RenderWindow&, float);
	virtual sf::FloatRect get_global_bounds() = 0;
	virtual sf::Vector2f get_position() = 0;
	virtual void set_scale(float, float) = 0;
	virtual sf::Vector2f get_scale() = 0;
	virtual void draw(sf::RenderWindow&) = 0;
	virtual void move(float, float) = 0;
	virtual void set_position(float, float) = 0;
	virtual void set_color(sf::Color) = 0;
	virtual sf::Color get_color() = 0;
	virtual string get_string() = 0;
	virtual Figure* clone() const = 0;
};

