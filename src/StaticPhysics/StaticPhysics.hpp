#pragma once

#include "../Geometry/Geometry.hpp"
#include "../Utilities/Utilities.hpp"
#include "../Constants/Constants.hpp"

// Idea(?): Make an electrostatics class that has this stuff and then electrodynamics class and then the `Physics` class will instantiate whichever one is needed

class StaticPhysics
{
private:
    Geometry m_geometry;

    // 2D Physics
    std::vector<Field2D> m_E_field; // {magnitude V/m , unit vector components}
    std::vector<Field2D> m_B_field; // {magnitude T , unit vector components}

public:
    // 2D Methods

    // Constructor for 2D
    StaticPhysics(const std::size_t& dim, const double& bound, const std::size_t& numPoints);
    // accumulates the electric field at each point in the domain (grid) for each charged particle
    void calculateElectricField(std::vector<ChargedParticle2D>& particles, const bool& initialize);
    void calculateInfiniteWireMagneticField(std::vector<InfiniteWire2D>& wires);

    // writes the electric/magnetic field to a file along with the grid points
    void writeFields(const std::string& filename, const std::string ext="txt", const std::string delimiter=",");

    void run(std::vector<ChargedParticle2D>& particles, std::vector<InfiniteWire2D>& wires);

    // Getters
    const std::vector<Field2D>& E_field() const { return m_E_field; }
    const std::vector<Field2D>& B_field() const { return m_B_field; }

    // 3D Methods

    // For 3D: just overload the constructor and add a 3D version of ChargedParticle (probably not a good idea)
    StaticPhysics(std::size_t& dim, double& bound, std::size_t& numPoints, std::vector<ChargedParticle3D>& particles);

    // Getters
    const Geometry& geometry() const { return m_geometry; }

};
