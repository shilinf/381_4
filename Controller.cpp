#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <iostream>
#include <map>
#include <utility>

using std::string;
using std::cout; using std::cin; using std::endl;
using std::map;
using std::make_pair;

Controller::Controller()
{
    cout << "Controller constructed" << endl;
}

Controller::~Controller()
{
    cout << "Controller destructed" << endl;
}

void Controller::run()
{
    view_ptr = new View();
    g_Model_ptr->attach(view_ptr);
    Command_map_t commands_map;
    load_command_map(commands_map);
    while (true) {
        cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
        string first_word, command;
        cin >> first_word;
        try {
            if (first_word == "quit") {
                quit();
                return;
            }
            else if (g_Model_ptr->is_ship_present(first_word)) {
                target_ship = g_Model_ptr->get_ship_ptr(first_word);
                cin >> command;
            }
            else
                command = first_word;
            auto cfp = commands_map[command];
            if (cfp)
                (this->*cfp)();
            else {
                commands_map.erase(command);
                cout << "Unrecognized command!" << endl;
                discard_input_remainder();
            }
        } catch (Error& error) {
            cout << error.what() << endl;
            discard_input_remainder();
        }
    }
}

void Controller::load_command_map(Command_map_t &commands_map)
{
    commands_map.insert(make_pair("default", &Controller::restore_default_map));
    commands_map.insert(make_pair("size", &Controller::set_map_size));
    commands_map.insert(make_pair("zoom", &Controller::set_map_scale));
    commands_map.insert(make_pair("pan", &Controller::set_map_origin));
    commands_map.insert(make_pair("show", &Controller::draw_map));
    commands_map.insert(make_pair("status", &Controller::show_object_status));
    commands_map.insert(make_pair("go", &Controller::update_all_objects));
    commands_map.insert(make_pair("create", &Controller::create_new_ship));
    
    commands_map.insert(make_pair("course", &Controller::set_ship_course));
    commands_map.insert(make_pair("position", &Controller::set_ship_to_position));
    commands_map.insert(make_pair("destination", &Controller::set_ship_destination_island));
    commands_map.insert(make_pair("load_at", &Controller::set_ship_load_island));
    commands_map.insert(make_pair("unload_at", &Controller::set_ship_unload_island));
    commands_map.insert(make_pair("dock_at", &Controller::set_ship_dock_island));
    commands_map.insert(make_pair("attack", &Controller::set_ship_attack_target));
    commands_map.insert(make_pair("refuel", &Controller::set_ship_refuel));
    commands_map.insert(make_pair("stop", &Controller::set_ship_stop));
    commands_map.insert(make_pair("stop_attack", &Controller::set_ship_stop_attack));
}

void Controller::restore_default_map()
{
    view_ptr->set_defaults();
}

void Controller::set_map_size()
{
    int size;
    if (!(cin >> size))
        throw Error("Expected an integer!");
    view_ptr->set_size(size);
}

void Controller::set_map_scale()
{
    double scale = read_double();
    view_ptr->set_scale(scale);
}

void Controller::set_map_origin()
{
    view_ptr->set_origin(read_point());
}

void Controller::draw_map()
{
    view_ptr->draw();
}

void Controller::show_object_status()
{
    g_Model_ptr->describe();
}

void Controller::update_all_objects()
{
    g_Model_ptr->update();
}

void Controller::create_new_ship()
{
    string name;
    cin >> name;
    if (name.length() < 2)
        throw Error("Name is too short!");
    if (g_Model_ptr->is_name_in_use(name))
        throw Error("Name is already in use!");
    string ship_type;
    cin >> ship_type;
    Ship *new_ship = create_ship(name, ship_type, read_point());
    g_Model_ptr->add_ship(new_ship);
}


// maybe order different??????????
void Controller::set_ship_course()
{
    double course = read_double();
    double speed = read_check_speed();
    if (course < 0.0 || course >= 360.0)
        throw Error("Invalid heading entered!");
    target_ship->set_course_and_speed(course, speed);
}

void Controller::set_ship_to_position()
{
    Point position = read_point();
    double speed = read_check_speed();
    target_ship->set_destination_position_and_speed(position, speed);
}

void Controller::set_ship_destination_island()
{
    Point island_location = read_get_island()->get_location();
    target_ship->set_destination_position_and_speed(island_location, read_check_speed());
}

void Controller::set_ship_load_island()
{
    target_ship->set_load_destination(read_get_island());
}

void Controller::set_ship_unload_island()
{
    target_ship->set_unload_destination(read_get_island());
}

void Controller::set_ship_dock_island()
{
    target_ship->dock(read_get_island());
}

void Controller::set_ship_attack_target()
{
    string ship_name;
    cin >> ship_name;
    Ship *attack_ship = g_Model_ptr->get_ship_ptr(ship_name);
    target_ship->attack(attack_ship);
}

void Controller::set_ship_refuel()
{
    target_ship->refuel();
}

void Controller::set_ship_stop()
{
    target_ship->stop();
}

void Controller::set_ship_stop_attack()
{
    target_ship->stop_attack();
}


void Controller::quit()
{
    g_Model_ptr->detach(view_ptr);
    delete view_ptr;
    cout << "Done" << endl;
}


double Controller::read_check_speed()
{
    double speed = read_double();
    if (speed < 0.0)
        throw Error("Negative speed entered!");
    return speed;
}



// Read to new line
void Controller::discard_input_remainder()
{
    cin.clear();
    while (cin.get() != '\n')
        ;
}

Point Controller::read_point()
{
    double x, y;
    if (!(cin >> x) || !(cin >> y))
        throw Error("Expected a double!");
    return Point(x, y);
}


double Controller::read_double()
{
    double temp;
    if (!(cin >> temp))
        throw Error("Expected a double!");
    return temp;
}

Island* Controller::read_get_island()
{
    string island_name;
    cin >> island_name;
    return g_Model_ptr->get_island_ptr(island_name);
}
