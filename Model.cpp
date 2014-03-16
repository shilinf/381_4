#include "Model.h"
#include "Island.h"
#include "Ship.h"
#include "Ship_factory.h"
#include "Utility.h"
#include "View.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

using std::string;
using std::cout; using std::endl;
using std::merge; using std::inserter;
using std::pair; using std::make_pair;
using std::mem_fn; using std::bind;
using std::placeholders::_1; using std::ref;
using std::map; using std::vector;

Model* g_Model_ptr = nullptr;


/* create some islands and ships using the following code in the Model constructor.
Do not change the execution order of these code fragments. You should delete this comment. */
Model::Model() : time(0){
	island_container["Ex"] = new Island("Exxon", Point(10, 10), 1000, 200);
	island_container["Sh"] = new Island("Shell", Point(0, 30), 1000, 200);
	island_container["Be"] = new Island("Bermuda", Point(20, 20));
	
	ship_container["Aj"] = create_ship("Ajax", "Cruiser", Point (15, 15));
	ship_container["Xe"] = create_ship("Xerxes", "Cruiser", Point (25, 25));
	ship_container["Va"] = create_ship("Valdez", "Tanker", Point (30, 30));
    
    
    // check whether this merge works fine
    copy(island_container.begin(), island_container.end(), inserter(object_container, object_container.begin()));
    copy(ship_container.begin(), ship_container.end(), inserter(object_container, object_container.begin()));
	cout << "Model constructed" << endl;
}

Model::~Model()
{
    // not sure whether this can work....
    for_each(object_container.begin(), object_container.end(), [](pair<string, Sim_object *> object_pair) {delete object_pair.second;});
    cout << "Model destructed" << endl;
}

bool Model::is_name_in_use(const std::string& name) const
{
    return object_container.find(name.substr(0, 2)) != object_container.end();
}

bool Model::is_island_present(const std::string& name) const
{
    return island_container.find(name.substr(0, 2)) != island_container.end();
}

void Model::add_island(Island* new_island)
{
    string key = new_island->get_name().substr(0,2);
    island_container.insert(make_pair(key, new_island));
    object_container.insert(make_pair(key, new_island));
    new_island->broadcast_current_state();
}

Island* Model::get_island_ptr(const std::string& name) const
{
    if (!is_island_present(name))
        throw Error("Island not found!");
    return island_container.find(name.substr(0, 2))->second;
}

bool Model::is_ship_present(const std::string& name) const
{
    return ship_container.find(name.substr(0, 2)) != ship_container.end();
}

// how to update view? ????
void Model::add_ship(Ship* new_ship)
{
    string key = new_ship->get_name().substr(0, 2);
    ship_container.insert(make_pair(key, new_ship));
    object_container.insert(make_pair(key, new_ship));
    new_ship->broadcast_current_state();
}


// consider efficiency
Ship* Model::get_ship_ptr(const std::string& name) const
{
    if (!is_ship_present(name))
        throw Error("Ship not found!");
    return ship_container.find(name.substr(0, 2))->second;
}
                              
void Model::describe() const
{
    for_each(object_container.begin(), object_container.end(), bind(&Sim_object::describe, bind(& map<string, Sim_object *>::value_type::second, _1)));
}

void Model::update()
{
    ++time;
    for_each(object_container.begin(), object_container.end(), bind(&Sim_object::update, bind(& map<string, Sim_object *>::value_type::second, _1)));
    vector<string> sunk_ships;
    for (auto ship_pair : ship_container) {
        Ship * ship_ptr = ship_pair.second;
        if (ship_ptr->is_on_the_bottom()) {
            string key = ship_ptr->get_name().substr(0, 2);
            sunk_ships.push_back(key);
            delete ship_ptr;
        }
    }
    for (string key : sunk_ships) {
        ship_container.erase(ship_container.find(key));
        object_container.erase(object_container.find(key));
    }
}

void Model::attach(View* view)
{
    view_container.insert(view);
    for_each(object_container.begin(), object_container.end(), bind(&Sim_object::broadcast_current_state, bind(& map<string, Sim_object *>::value_type::second, _1)));
}


void Model::detach(View* view)
{
    view_container.erase(view_container.find(view));
}


void Model::notify_location(const std::string& name, Point location)
{
    for_each(view_container.begin(), view_container.end(), bind(&View::update_location, _1, ref(name), ref(location)));
}


void Model::notify_gone(const std::string& name)
{
    for_each(view_container.begin(), view_container.end(), bind(&View::update_remove, _1, ref(name)));
}








