#include "Utilities.hpp"

namespace Utilities
{
    void initMessage()
    {
        std::cout << '\n' << "############################################" << '\n';
        std::cout << "#            Initialized values            #" << '\n';
        std::cout << "############################################" << "\n\n";
        std::cout << "dim: " << Utilities::dim << '\n' << "bound: " << Utilities::bound << '\n' << "numPoints: " << Utilities::numPoints << std::endl;
        std::cout << '\n' << "############################################" << "\n\n";

        #ifdef _OPENMP
            std::cout << "OpenMP is enabled. Running in parallel mode.\n";
        #else
            std::cout << "OpenMP is not enabled. Running in sequential mode.\n";
        #endif
        
        std::cout << '\n' << "############################################" << "\n\n";
    };

    Point2D r_prime(const Point2D& p1, const Point2D& p2)
    {
        // Minimum image convention
        // https://www.researchgate.net/profile/Ulrich-Deiters/publication/235933545_Efficient_Coding_of_the_Minimum_Image_Convention/links/5955135d458515bbaa21e73e/Efficient-Coding-of-the-Minimum-Image-Convention.pdf

        Point2D r_prime { p1 - p2 };
        
        if (periodic)
        {
            const double& dx { r_prime.x() };
            const double& dy { r_prime.y() };
            r_prime.setX( dx - static_cast<int>(dx / bound)*2*Utilities::bound );
            r_prime.setY( dy - static_cast<int>(dy / bound)*2*Utilities::bound );
        }

        return r_prime;
    }

    
    template <typename FileStream>
    void checkFileOpen(const FileStream& file)
    {
        if (!file.is_open()) {
            throw std::ios_base::failure("Failed to open file!");
        }
    };

    void appendToEndOfLine(const std::string& filename, const std::string& ext, const std::string& delimiter, const std::vector<Field2D>& data)
    {
        if (data.empty()) return;
        
        // Assumes that `data` is already in the format: {magnitude, unit vector component 1, unit vector component 2}
        // assumes file path is /Users/max/ClassicalEM++/outputs/
        std::string inputFilename = "/Users/max/ClassicalEM++/outputs/" + filename + "." + ext;
        std::string tempFilename = "/Users/max/ClassicalEM++/outputs/temp." + ext;

        std::ifstream inputFile(inputFilename, std::ios::in);
        // ensure file is open (needs to caught in a try catch block)
        checkFileOpen<std::ifstream>(inputFile);

        // Create a temporary file to store modified content
        std::ofstream tempFile(tempFilename, std::ios::out);
        checkFileOpen<std::ofstream>(tempFile);

        // Read each line from the original file and append the data to the end of the line
        std::string line;
        std::size_t idx = 0; // To keep track of corresponding data index

        while (std::getline(inputFile, line))
        {
            if (idx < data.size()) // Ensure we have a corresponding data entry
            {
                // Format the current data entry as ",a,b,c"
                const Field2D& datum = data[idx];
                // Append formatted data to the line
                line += delimiter + std::to_string(datum.magnitude) + delimiter + std::to_string(datum.direction.x()) + delimiter + std::to_string(datum.direction.y());
                ++idx; // Move to the next data entry
            }

            // Write the modified line to the temp file
            tempFile << line << '\n';
        }

        // Close the files
        inputFile.close();
        tempFile.close();

        // Replace the original file with the modified temporary file
        std::filesystem::remove(inputFilename.c_str());  // Delete the original file
        std::filesystem::rename(tempFilename.c_str(), inputFilename.c_str());  // Rename temp file to original filename
    };

    void readJsonFile(const std::string& filename)
    {
        nlohmann::json _j;

        std::ifstream file("/Users/max/ClassicalEM++/" + filename);
        checkFileOpen<std::ifstream>(file);

        file >> _j;

        // outputFilename = _j.value("output filename", std::filesystem::path(filename).replace_extension(".txt").string());
        outputFilename = _j.value("output filename", "output");
        dim = _j["dim"];
        bound = _j["bound"];
        periodic = _j.value("periodic", false);
        numPoints = _j["numPoints"];
        numSteps = static_cast<std::size_t>(_j.value("numSteps", 1));
        dt = _j.value("dt", 0.01);

        /*
        particles is setup like this in json file:
        "particles": [
            {"charge": 1, "x": 0.0, "y": 0.0},
            {"charge": -1, "x": 1.0, "y": 1.0}
        ]
        */

        // get particles from json file if "particles" key exists
        if (_j.contains("particles"))
        {
            particles.reserve(_j["particles"].size());
            for (const auto& particle : _j["particles"]) // use `auto` here because `nlohmann::json::object_t` might not be right and looks ugly in my opinion
            {
                if (!checkPointWithinBounds(particle["x"], particle["y"])) 
                {
                    std::cerr << "Particle out of bounds! Ignoring..." << '\n' << "x\t" << particle["x"] << '\n' << "y\t" << particle["y"] << std::endl;
                    continue;
                };

                Point3D velocity {0.0, 0.0, 0.0};
                if (particle.contains("vx") && particle.contains("vy") && particle.contains("vz")) {
                    velocity = Point3D{particle["vx"], particle["vy"], particle["vz"]};
                }

                particles.emplace_back(ChargedParticle2D{particle["charge"], abs(particle.value("mass", 1.0)), Point2D{particle["x"], particle["y"]}, velocity});
            };
        }

        // get infinite wires from json file if "wires" key exists
        if (_j.contains("wires"))
        {
            wires.reserve(_j["wires"].size());
            for (const auto& wire : _j["wires"])
            {
                Point3D direction_Point3D { Point3D{wire["direction"]["x"], wire["direction"]["y"], wire["direction"]["z"]} };
                direction_Point3D.normalize();

                wires.emplace_back(InfiniteWire2D
                {
                    wire["current"], 
                    Point2D{wire["x"], wire["y"]}, 
                    direction_Point3D
                });
            };
        }
    };

    bool checkPointWithinBounds(const double& x, const double& y)
    {
        return abs(x) <= bound && abs(y) <= bound;
    };

    bool checkPointWithinBounds(Point2D& point)
    {
        return abs(point.x()) <= bound && abs(point.y()) <= bound;
    };

    std::size_t findNearestGridPointIndex(const Point2D& point)
    {
        double step_size { 2 * bound / (numPoints + 1) };
        // since `bound` is positive, we have -(-bound) = +bound
        std::size_t x_idx { static_cast<std::size_t>(std::round((point.x() + bound) / step_size)) };
        std::size_t y_idx { static_cast<std::size_t>(std::round((point.y() + bound) / step_size)) };

        return static_cast<std::size_t>( (numPoints+1) * x_idx + y_idx );
    };
};
