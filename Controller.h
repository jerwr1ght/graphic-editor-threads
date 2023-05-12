#pragma once
#include "Figure.h"
#include "Composite.h"

class Memento;
class EventHandler;

class Controller
{
private:
	int figures_count;
	Figure** figures;

	Figure* current_figure;
	Composite* current_composite;

	static Controller* controller_instance; // ������� ������� Singleton
public:
	Controller();
	~Controller();
	Figure* operator[](int index);
	void add_figure(Figure* figure);
	void remove_figure_by_index(int index);
	bool contains(Figure* figure);
	int get_figure_index(Figure* figure);
	bool check_intersections_with_figure(Figure* figure);
	int get_figures_count();
	bool is_empty();
	void clear();

	// ������ � �������� �������
	Figure* get_active_figure();
	void set_active_figure(int index);
	void set_active_figure(Figure* figure);
	void set_next_figure_active();
	void set_previous_figure_active();
	
	// ������ � ����������
	Composite* get_composite();
	void set_composite(Composite* composite);
	void add_composite(Composite* composite);

	// ������ � ��������� ����� ����������
	static Controller* get_instance();

	
};

