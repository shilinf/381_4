#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <map>

/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

/* 
*** This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

// May need these ???????
class Model;
class View;
class Ship;
class Island;
class Controller;
struct Point;

using Command_map_t = std::map<std::string, void(Controller::*)()>;


class Controller {
public:	
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();
    
    
private:
    View *view_ptr;
    Ship *target_ship;
    void discard_input_remainder();
    void quit();
    void load_command_map(Command_map_t &commands_map);
    void restore_default_map();
    void set_map_size();
    void set_map_scale();
    void set_map_origin();
    void draw_map();
    void show_object_status();
    void update_all_objects();
    void create_new_ship();
    
    void set_ship_course();
    void set_ship_to_position();
    void set_ship_destination_island();
    void set_ship_load_island();
    void set_ship_unload_island();
    void set_ship_dock_island();
    void set_ship_attack_target();
    void set_ship_refuel();
    void set_ship_stop();
    void set_ship_stop_attack();
    
    
    Point read_point();
    double read_double();
    double read_check_speed();
    Island *read_get_island();
};

#endif






