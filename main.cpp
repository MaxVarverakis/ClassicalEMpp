// #include "src/Geometry/Geometry.hpp"
#include "src/Physics/Physics.hpp"


int main(int argc, char* argv[])
{
    if (argc < 2)
    {   
        std::cerr << "No input file given! " << std::endl;
        std::cerr << "Usage: " << argv[0] << " </path/to/config.json>" << std::endl;
    };

    // read the json file and set the dim, bound, and numPoints in the Utilities namespace
    Utilities::readJsonFile(argv[1]);
    Physics physics(Utilities::dim, Utilities::bound, Utilities::numPoints);
    physics.run(Utilities::particles, Utilities::wires);

    return 0;
}