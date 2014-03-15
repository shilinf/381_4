#include "View.h"
#include "Geometry.h"
#include "Utility.h"
#include <cmath>
#include <iostream>

using std::string;
using std::cout; using std::endl;


View::View()
{
    set_defaults();
    cout << "View constructed" << endl;
}


View::~View()
{
    cout << "View destructed" << endl;
}

void View::set_size(int size_)
{
    if (size_ <= 6)
        throw Error("New map size is too small!");
    if (size_ > 30)
        throw Error("New map size is too big!");
    size = size_;
}

void View::set_scale(double scale_)
{
    if (scale_ <= 0.0)
        throw Error("New map scale must be positive!");
    scale = scale_;
}


void View::set_origin(Point origin_)
{
    origin = origin_;
}


void View::set_defaults()
{
    size = 25;
    scale = 2.0;
    origin = Point(-10, -10);
}


void View::update_location(const std::string& name, Point location)
{



}

void View::update_remove(const std::string& name)
{


}

void View::draw()
{



}

void View::clear()
{



}




/* *** Use this function to calculate the subscripts for the cell. */

/* *** This code assumes the specified private member variables. */


// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool View::get_subscripts(int &ix, int &iy, Point location)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will produce integer smaller than even for negative values, 
	// so - 0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

