#include "View.h"
#include "Geometry.h"
#include "Utility.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>

using std::string;
using std::cout; using std::endl;
using std::make_pair;
using std::vector;
using std::ios; using std::setw;
using std::streamsize;

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
    auto pair_it = points.find(name);
    if (pair_it != points.end())
        points.erase(pair_it);
    points.insert(make_pair(name, location));
}

void View::update_remove(const std::string& name)
{
    points.erase(points.find(name));
}

void View::draw()
{
    cout << "Display size: " <<size << ", scale: " << scale << ", origin: " << origin << endl;
    vector< vector<string> > output(size, vector<string>(size, ". "));
    bool exist_out_of_map = false;
    for (auto map_pair : points) {
        int x, y;
        if (get_subscripts(x, y, map_pair.second)) {
            if (output[x][y] == ". ")
                output[x][y] = map_pair.first.substr(0, 2);
            else
                output[x][y] = "* ";
        }
        else {
            if (exist_out_of_map)
                cout << ", " << map_pair.first;
            else {
                cout << map_pair.first;
                exist_out_of_map = true;
            }
        }
    }
    if (exist_out_of_map)
        cout << " outside the map" << endl;
    ios::fmtflags old_settings = cout.flags(); //save previous format flags
    streamsize old_precision = cout.precision();
    cout.setf(ios::fixed, ios::floatfield); // set fixed floating format
    cout.precision(0);
    for (int i = 0; i < size; i++) {
        if ((size - i) % 3 == 1)
            cout << setw(4) << origin.y + scale * (size - i - 1) << ' ';
        else
            cout << "     ";
        for (int j = 0; j < size; j++)
            cout << output[j][size - i - 1];
        cout << endl;
    }
    for (int i = 0; i <= (size-1)/3 ; i++) {
        cout << "  " << setw(4) << origin.x + 3 * scale * i;
    }
    cout << endl;
    cout.flags(old_settings);
    cout.precision(old_precision);
}

void View::clear()
{
    points.clear();
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

