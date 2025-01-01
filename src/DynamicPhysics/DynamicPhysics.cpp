#include "DynamicPhysics.hpp"

DynamicPhysics::DynamicPhysics(std::size_t& dim, double& bound, std::size_t& numPoints, std::size_t& numSteps)
    : m_static_physics {dim, bound, numPoints}
    , m_numSteps {numSteps}
{
    Utilities::initMessage();
};

void DynamicPhysics::run(std::vector<ChargedParticle2D>& particles, std::vector<InfiniteWire2D>& wires)
{
    std::cout << "Run starting!" << std::endl;
    std::cout << "Current iteration: " << m_iteration << '\n';
    
    m_static_physics.calculateElectricField(particles);
    m_static_physics.calculateInfiniteWireMagneticField(wires);
    while (m_iteration < m_numSteps)
    {
        evolve(particles);
    }

    m_static_physics.writeFields(Utilities::outputFilename + std::to_string(m_iteration));

    std::cout << "Run complete!" << std::endl;
};

void DynamicPhysics::evolve(std::vector<ChargedParticle2D>& particles)
{
    for (ChargedParticle2D& particle : particles)
    {
        std::size_t idx = Utilities::findNearestGridPointIndex(m_static_physics.geometry(), particle.position);
        const Field2D& E_field = m_static_physics.E_field()[idx];
        double a { particle.charge * E_field.magnitude / particle.mass };
        Point2D acceleration { a * E_field.direction.x(), a * E_field.direction.y() };

        // for now, dt is just one iteration so v*dt = v
        // use RK4 to update the position and velocity
        // void RK4();
    }
    
    ++m_iteration;
}
