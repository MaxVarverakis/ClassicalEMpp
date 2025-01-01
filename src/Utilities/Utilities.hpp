#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>
#include <nlohmann/json.hpp>
#include "../Points/Points.hpp"
#include "../Geometry/Geometry.hpp"

namespace Utilities
{
    inline std::string outputFilename;
    inline std::size_t dim;
    inline double bound;
    inline std::size_t numPoints;
    inline std::size_t numSteps;
    inline std::vector<ChargedParticle2D> particles;
    inline std::vector<InfiniteWire2D> wires;

    void initMessage();

    template <typename FileStream>
    void checkFileOpen(const FileStream& file);
    
    void appendToEndOfLine(const std::string& filename, const std::string& ext, const std::string& delimiter, const std::vector<Field2D>& data);

    void readJsonFile(const std::string& filename);

    Point3D crossProduct(const Point3D& a, const Point3D& b);

    std::size_t findNearestGridPointIndex(const Geometry& grid, const Point2D& point);
};
