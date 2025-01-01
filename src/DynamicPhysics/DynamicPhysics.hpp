#pragma once

#include "../StaticPhysics/StaticPhysics.hpp"

class DynamicPhysics
{
private:
    StaticPhysics m_static_physics;
    
    std::size_t m_iteration { 0 };
    std::size_t& m_numSteps;
public:
    DynamicPhysics(std::size_t& dim, double& bound, std::size_t& numPoints, std::size_t& numSteps);

    void run(std::vector<ChargedParticle2D>& particles, std::vector<InfiniteWire2D>& wires);

    void evolve(std::vector<ChargedParticle2D>& particles);

    void RK4();

    /*
    To get the force on each particle: 
        1. don't calculate foce by accumulating E-field contributions at each charged particle location
        2. instead, find nearest grid point and use that E-field data as the `ma = qE` and then use RK4 to solve and update to next step
        3. the reason for this is so that we can have E-waves propagating at the speed of light which in this case is the frame evolution
        4. if we did this the other way, perhaps the "naive approach", then unless special care is taken, the force vectors would basically be instantaneously updated (boring)
    */
};
