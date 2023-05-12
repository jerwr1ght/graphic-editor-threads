#include "Figure.h"

void Figure::set_trail_movement(bool status) {
	trail_movement = status;
}

bool Figure::get_trail_movement() {
	return trail_movement;
}

bool Figure::is_visible() {
	return visible;
}

void Figure::hide() {
	visible = false;
}

void Figure::show() {
	visible = true;
}

Figure* Figure::get_target() {
	return target;
}
void Figure::set_target(Figure* target) {
	this->target = target;
}

void Figure::auto_move(sf::RenderWindow& window, float elapsed_time) {
	move(1, sin(elapsed_time + 1));
}

void Figure::reset() {
	set_color(default_color);
	set_scale(1, 1);
}

bool Figure::check_intersection(Figure* another_figure) {
	bool result = get_global_bounds()
		.intersects(another_figure->get_global_bounds());
	/*if (result)
		set_color(another_figure->get_color());*/

	return result;
}

bool Figure::check_intersection(float x, float y) {
	return get_global_bounds().contains(x, y);
}

bool Figure::check_window_collision(sf::RenderWindow& window) {
	if ((get_position().x < 0 || get_position().x > window.getSize().x - get_global_bounds().width)
		|| (get_position().y < 0 || get_position().y > window.getSize().y - get_global_bounds().height))
	{
		return true;
	}

	return false;
}