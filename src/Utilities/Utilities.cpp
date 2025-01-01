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
    };
    
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
        numPoints = _j["numPoints"];
        numSteps = static_cast<std::size_t>(_j.value("numSteps", 1));

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
                Point3D velocity {0.0, 0.0, 0.0};
                if (particle.contains("vx") && particle.contains("vy") && particle.contains("vz")) {
                    velocity = Point3D{particle["vx"], particle["vy"], particle["vz"]};
                }

                particles.emplace_back(ChargedParticle2D{particle["charge"], _j.value("mass", 0.0), Point2D{particle["x"], particle["y"]}, velocity});
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

    Point3D crossProduct(const Point3D& a, const Point3D& b)
    {
        return {
            a.y() * b.z() - a.z() * b.y(),
            a.z() * b.x() - a.x() * b.z(),
            a.x() * b.y() - a.y() * b.x()
        };
    };

    std::size_t findNearestGridPointIndex(const Geometry& grid, const Point2D& point)
    {
        double minDistance { 3*bound };
        std::size_t idx { 0 };

        const std::vector<Point2D>& grid_points { grid.grid2D() };

        for (std::size_t i = 0; i < grid_points.size(); ++i)
        {
            const double distance { point.distanceTo(grid_points[i]) };
            if (distance < minDistance)
            {
                idx = i;
                minDistance = distance;
            }
        }

        return idx;

        // return grid[std::distance(v.begin(), std::min_element(v.begin(), v.end()))]
    };
};
