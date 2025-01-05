#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>
#include </opt/homebrew/Cellar/nlohmann-json/3.11.3/include/nlohmann/json.hpp>
#include </opt/homebrew/Cellar/libomp//19.1.6/include/omp.h>
#include "../Points/Points.hpp"
#include "../Geometry/Geometry.hpp"

namespace Utilities
{
    inline std::string outputFilename;
    inline std::size_t dim;
    inline double bound;
    inline std::size_t numPoints;
    inline std::size_t numSteps;
    inline double dt;
    inline std::vector<ChargedParticle2D> particles;
    inline std::vector<InfiniteWire2D> wires;

    void initMessage();

    template <typename FileStream>
    void checkFileOpen(const FileStream& file);
    
    template <typename T>
    int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    };

    void appendToEndOfLine(const std::string& filename, const std::string& ext, const std::string& delimiter, const std::vector<Field2D>& data);

    void readJsonFile(const std::string& filename);

    Point3D crossProduct(const Point3D& a, const Point3D& b);

    bool checkPointWithinBounds(const double& x, const double& y);

    bool checkPointWithinBounds(Point2D& point);

    std::size_t findNearestGridPointIndex(const Point2D& point);
};
