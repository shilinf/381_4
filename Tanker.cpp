#include "Tanker.h"
#include "Ship.h"
#include "Utility.h"
#include "Island.h"
#include <iostream>

using std::string;
using std::cout; using std::endl;


Tanker::Tanker(const std::string& name_, Point position_) : Ship(name_, position_, 100., 10., 2., 0), cargo_capacity(1000.), cargo(0.), tanker_state(NO_CARGO_DESTINATIONS), load_destination(nullptr), unload_destination(nullptr)
{
    cout << "Tanker " << name_ << " constructed" << endl;
}

Tanker::~Tanker()
{
    cout << "Tanker " << get_name() << " destructed" << endl;
}

void Tanker::set_destination_position_and_speed(Point destination, double speed)
{
    check_no_cargo_destination();
    Ship::set_destination_position_and_speed(destination, speed);
}

void Tanker::set_course_and_speed(double course, double speed)
{
    check_no_cargo_destination();
    Ship::set_course_and_speed(course, speed);
}


void Tanker::stop()
{
    Ship::stop();
    load_destination = nullptr;
    unload_destination = nullptr;
    tanker_state = NO_CARGO_DESTINATIONS;
    cout << get_name() << " now has no cargo destinations" << endl;
}

void Tanker::describe() const
{
    cout << "\nTanker ";
    Ship::describe();
    cout << "Cargo: " << cargo << " tons";
    switch (tanker_state) {
        case NO_CARGO_DESTINATIONS:
            cout << ", no cargo destinations";
            break;
        case UNLOADING:
            cout << ", unloading";
            break;
        case MOVING_TO_LOADING:
            cout << ", moving to loading destination";
            break;
        case LOADING:
            cout << ", loading";
            break;
        case MOVING_TO_UNLOADING:
            cout << ", moving to unloading destination";
            break;
        default:
            break;
    }
    cout << endl;
}

void Tanker::set_load_destination(Island* destination)
{
    check_no_cargo_destination();
    load_destination = destination;
    if (destination == unload_destination)
        throw Error("Load and unload cargo destinations are the same!");
    cout <<  get_name() << " will load at " << destination->get_name() << endl;
    if (unload_destination)
        start_cycle();
}



void Tanker::set_unload_destination(Island* destination)
{
    check_no_cargo_destination();
    unload_destination = destination;
    if (destination == load_destination)
        throw Error("Load and unload cargo destinations are the same!");
    cout << get_name() << " will unload at " << destination->get_name() << endl;
    if (load_destination)
        start_cycle();
}

void Tanker::start_cycle()
{
    if (is_docked()) {
        if (get_docked_Island() == load_destination)
            tanker_state = LOADING;
        else if (get_docked_Island() == unload_destination)
            tanker_state = UNLOADING;
    }
    else if (cargo == 0. && can_dock(load_destination))
        tanker_state = LOADING;
    else if (cargo > 0. && can_dock(unload_destination))
        tanker_state = UNLOADING;
    else if (cargo == 0.) {
        Ship::set_destination_position_and_speed(load_destination->get_location(), get_maximum_speed());
        tanker_state = MOVING_TO_LOADING;
    }
    else if (cargo > 0.) {
        Ship::set_destination_position_and_speed(unload_destination->get_location(), get_maximum_speed());
        tanker_state = MOVING_TO_UNLOADING;
    }
}

void Tanker::update()
{
    Ship::update();
    if (!can_move()) {
        tanker_state = NO_CARGO_DESTINATIONS;
        load_destination = nullptr;
        unload_destination = nullptr;
        cout << get_name() << " now has no cargo destinations" << endl;
        return;
    }
    switch (tanker_state) {
        case NO_CARGO_DESTINATIONS:
            break;
        case MOVING_TO_LOADING:
            if (!is_moving() && can_dock(load_destination)) {
                dock(load_destination);
                tanker_state = LOADING;
            }
            break;
        case MOVING_TO_UNLOADING:
            if (!is_moving() && can_dock(unload_destination)) {
                dock(unload_destination);
                tanker_state = UNLOADING;
            }
            break;
        case LOADING:
        {
            Ship::refuel();
            double cargo_needed = cargo_capacity - cargo;
            if (cargo_needed < 0.005) {
                cargo = cargo_capacity;
                Ship::set_destination_position_and_speed(unload_destination->get_location(), get_maximum_speed());
                tanker_state = MOVING_TO_UNLOADING;
            }
            else {
                cargo += load_destination->provide_fuel(cargo_needed);
                cout << get_name() <<  " now has " <<cargo << " of cargo" << endl;
            }
            break;
        }
        case UNLOADING:
            if (cargo == 0.) {
                Ship::set_destination_position_and_speed(load_destination->get_location(), get_maximum_speed());
                tanker_state = MOVING_TO_LOADING;
            }
            else {
                unload_destination->accept_fuel(cargo);
                cargo = 0.;
            }
            break;
        default:
            break;
    }
}

void Tanker::check_no_cargo_destination()
{
    if (tanker_state != NO_CARGO_DESTINATIONS)
        throw Error("Tanker has cargo destinations!");
}