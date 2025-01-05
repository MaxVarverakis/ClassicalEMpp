#pragma once

#include "../StaticPhysics/StaticPhysics.hpp"

class DynamicPhysics
{
private:
    StaticPhysics m_static_physics;
    
    std::size_t m_iteration { 0 };
    const std::size_t& m_numSteps;
    const double& m_dt;
    std::vector<Point2D> m_acceleration;

public:
    DynamicPhysics(const std::size_t& dim, const double& bound, const std::size_t& numPoints, const std::size_t& numSteps, const double& dt);

    void run(std::vector<ChargedParticle2D>& particles, std::vector<InfiniteWire2D>& wires);

    void evolve(std::vector<ChargedParticle2D>& particles);

    std::vector<Point2D> calculateAcceleration(std::vector<ChargedParticle2D>& particles);

    // void RK4(ChargedParticle2D& particle, std::vector<ChargedParticle2D>& particles);
};
