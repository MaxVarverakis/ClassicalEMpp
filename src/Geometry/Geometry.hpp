#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <variant>
#include <fstream>
#include <string>

#include "../Points/Points.hpp"

// Currently only implemented for square and cube world volumes

class Geometry
{
private:
    const std::size_t& m_dim;
    const double& m_bound; // maximum value of x and y (and z if applicable)
    const std::size_t& m_numPoints; // number of points (minus 1) in each dimension (should be even integer if you want origin centered in domain)
    
    // only one of these will be used
    std::variant<std::vector<Point2D>, std::vector<Point3D>> m_grid;

public:
    Geometry(const std::size_t& dim, const double& bound, const std::size_t& numPoints);

    // Getters
    // std::size_t dim() const { return m_dim; }
    double bound() const { return m_bound; }
    std::size_t numPoints() const { return m_numPoints; }
    const std::vector<Point2D>& grid2D() const { return std::get<std::vector<Point2D>>(m_grid); }
    const std::vector<Point3D>& grid3D() const { return std::get<std::vector<Point3D>>(m_grid); }

    int checkDomainDimension(const std::size_t& dim);
    void constructWorld();
    void writeGrid(const std::string& filename, const std::string ext="txt", const std::string delimiter=",");
};
