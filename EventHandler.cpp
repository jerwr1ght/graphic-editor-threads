#include "EventHandler.h"
using namespace std;
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>

EventHandler::EventHandler(sf::Text& _label, 
	sf::Text& _status,  sf::Font& font, int figures_count) {
	label = _label;
	status = _status;
	font.loadFromFile("ICTV-Bold.ttf");
	label.setFont(font);
	label.setCharacterSize(25);
	label.setFillColor(sf::Color::White);
	status.setFont(font);
	status.setCharacterSize(15);
	status.setFillColor(sf::Color::White);
	max_random_figures_count = figures_count;
}

EventHandler::~EventHandler() {
	Controller::get_instance()->clear();
}

void EventHandler::poll_action_event(sf::RenderWindow& window,
	float delta_time) {

	std::thread destroy_targets_thread([this]() {
		destroy_targets();
		});

	destroy_targets_thread.join();

	std::thread move_by_targets_thread([&delta_time, this]() {
		move_by_targets(delta_time);
		});

	move_by_targets_thread.join();

	std::thread targets_thread([this]() {
		set_figures_targets();
	});

	targets_thread.join();

	std::thread figures_thread([&window, this]() {
		create_random_figures((int)window.getSize().x, (int)window.getSize().y);
		});

	figures_thread.join();	
}

void EventHandler::poll_draw_event(sf::RenderWindow& window) {
	
	// Розставляємо додадкові елементи на екрані
	window.clear(sf::Color(66, 112, 63));
	label.setPosition((window.getSize().x - label.getGlobalBounds().width) / 2.f, 5);
	status.setPosition(6, (float) window.getSize().y - 26);
	window.draw(label);
	window.draw(status);

	// Встановлюємо кількість фігур на екрані
	label.setString("<" + to_string(Controller::get_instance()->get_figures_count()) + " figures>");

	// Малюємо фігури
	for (int i = 0; i < Controller::get_instance()->get_figures_count(); i++) {
		Controller::get_instance()->operator[](i)->draw(window);
	}

	window.display();
}

sf::Color EventHandler::get_random_color() {
	return sf::Color(get_random_number(0, 255), get_random_number(0, 255), get_random_number(0, 255));
}

void EventHandler::set_status(const sf::String& status_message) {
	status.setString(status_message);
}

Figure* EventHandler::create_figure_by_int(int num) {
	sf::Color default_color = get_random_color();
	Figure* result = nullptr;

	switch (num) {
		case 0:
			result = new Triangle(TRIANGLE_SIZE, default_color);
			break;
		case 1:
			result = new Circle(CIRCLE_SIZE, default_color);
			break;
		case 2:
			result = new Line(LINE_SIZE.x, LINE_SIZE.y, default_color);
			break;
		case 3:
			result = new Rectangle(RECTANGLE_SIZE.x, RECTANGLE_SIZE.y, default_color);
			break;
		case 4:
			result = new Star(default_color);
			break;
		default:
			result = new Square(SQUARE_SIZE, default_color);
			break;
	}

	return result;
}

void EventHandler::create_random_figures(int x_size, int y_size) {
	Controller* instance = Controller::get_instance();

	if (instance->get_figures_count() < max_random_figures_count) {
		Figure* temp = create_figure_by_int(get_random_number(0, 5));
		temp->set_position(float(get_random_number(0, x_size)),
			float(get_random_number(0, y_size)));
		instance->add_figure(temp);
		set_status("Created " + temp->get_string());
	}
}

void EventHandler::set_figures_targets() {
	Controller* instance = Controller::get_instance();
	if (instance->get_figures_count() <= 1)
		return;

	for (int i = 0; i < instance->get_figures_count(); i++) {
		if (instance->operator[](i)->get_target() != nullptr)
			continue;

		int temp_index = get_random_number(0, instance->get_figures_count() - 1);
		while (temp_index == i)
		{
			temp_index = get_random_number(0, instance->get_figures_count() - 1);
		}

		instance->operator[](i)->set_target(instance->operator[](temp_index));
	}
}

void EventHandler::move_by_targets(float delta_time) {
	Controller* instance = Controller::get_instance();
	int figures_count = instance->get_figures_count();
	if (figures_count <= 1)
		return;

	for (int i = 0; i < figures_count; i++) {
		Figure* current_figure = instance->operator[](i);
		Figure* current_target = current_figure->get_target();

		if (current_target == nullptr ||
			current_figure->check_intersection(current_target)) {
			continue;
		}
		
		sf::Vector2f target_position = current_target->get_position();
		sf::Vector2f current_figure_position = current_figure->get_position();


		float x_position = (target_position - current_figure_position).x;
		float y_position = (target_position - current_figure_position).y;

		current_figure->move(x_position * delta_time, y_position * delta_time);
	}

}

void EventHandler::destroy_targets() {
	Controller* instance = Controller::get_instance();
	int figures_count = instance->get_figures_count();
	if (figures_count <= 1)
		return;

	for (int i = 0; i < figures_count; i++) {
		Figure* current_figure = instance->operator[](i);
		if (current_figure == nullptr)
			continue;
		Figure* current_target = current_figure->get_target();

		if (current_target == nullptr) {
			continue;
		} 
		else if (current_figure->check_intersection(current_target)) {
			for (int j = 0; j < figures_count; j++) {
				if (instance->operator[](j)->get_target() == current_target ||
					instance->operator[](j)->get_target() == current_figure) {
					instance->operator[](j)->set_target(nullptr);
				}
			}

			string figure_info = current_target->get_string();
			instance->remove_figure_by_index(instance->
				get_figure_index(current_target));
			set_status("Destroyed " + figure_info);

			figure_info = current_figure->get_string();
			instance->remove_figure_by_index(instance->
				get_figure_index(current_figure));
			set_status("Destroyed " + figure_info);

			
			break;
		}
	}
}


int EventHandler::get_random_number(int min, int max) {
	std::random_device rd;
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rd);
}