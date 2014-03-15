#include "Ship_factory.h"
#include "Geometry.h"
#include "Utility.h"
#include "Tanker.h"
#include "Cruiser.h"


Ship *create_ship(const std::string& name, const std::string& type, Point initial_position)
{
    if (type == "Cruiser")
        return new Cruiser(name, initial_position);
    else if (type == "Tanker")
        return new Tanker(name, initial_position);
    else
        throw Error("Trying to create ship of unknown type!");
}
