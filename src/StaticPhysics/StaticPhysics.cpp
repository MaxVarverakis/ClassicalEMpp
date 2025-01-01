#include "StaticPhysics.hpp"

StaticPhysics::StaticPhysics(std::size_t& dim, double& bound, std::size_t& numPoints): m_geometry{dim, bound, numPoints} {};

void StaticPhysics::calculateElectricField(std::vector<ChargedParticle2D>& particles)
{
    if (particles.empty()) { return; };

    // initialize the electric field at each point in the domain/grid
    m_E_field.reserve(m_geometry.grid2D().size() * particles.size());

    // accumulate the electric field at each point in the domain/grid coming from each charged particle
    for (std::size_t idx = 0; idx < m_geometry.grid2D().size(); ++idx)
    {
        m_E_field.emplace_back(Field2D {0., Point2D {0., 0.}});
        
        for (ChargedParticle2D& particle : particles)
        {
            const Point2D point { m_geometry.grid2D()[idx] };
            double r { point.distanceTo(particle.position) };;
            int sign { particle.charge < 0 ? -1 : 1 }; // negative charge means the electric field points towards the charge
            m_E_field[idx].magnitude += particle.charge / (r*r);
            
            Point2D r_prime { point - particle.position };
            m_E_field[idx].direction.setX( m_E_field[idx].direction.x() + sign * r_prime.x() / r );
            m_E_field[idx].direction.setY( m_E_field[idx].direction.y() + sign * r_prime.y() / r );
        };

        // normalize the unit vector
        m_E_field[idx].direction.normalize();
    };
};

void StaticPhysics::calculateInfiniteWireMagneticField(std::vector<InfiniteWire2D>& wires)
{
    if (wires.empty()) { return; };

    // initialize the magnetic field at each point in the domain/grid
    m_B_field.reserve(m_geometry.grid2D().size());

    // accumulate the magnetic field at each point in the domain/grid coming from each wire
    for (std::size_t idx = 0; idx < m_geometry.grid2D().size(); ++idx)
    {
        m_B_field.emplace_back(Field2D {0., Point2D {0., 0.}});
        
        for (InfiniteWire2D wire : wires)
        {
        const Point2D point { m_geometry.grid2D()[idx] };
            double r { wire.position.distanceTo(point) };

            Point2D r_prime_2D { (point - wire.position) };
            Point3D components { Utilities::crossProduct(wire.direction, Point3D {r_prime_2D.x()/r_prime_2D.magnitude(), r_prime_2D.y()/r_prime_2D.magnitude(), 0.}) };

            m_B_field[idx].magnitude += wire.current / (2 * Constants::pi * r) * components.magnitude();
            m_B_field[idx].direction.setX( m_B_field[idx].direction.x() + components.x() / r );
            m_B_field[idx].direction.setY( m_B_field[idx].direction.y() + components.y() / r );
        };
        // normalize the unit vector
        m_B_field[idx].direction.normalize();

    };
};

void StaticPhysics::writeFields(const std::string& filename, const std::string ext, const std::string delimiter)
{
    // first, write the grid points to file
    m_geometry.writeGrid(filename, ext, delimiter);

    Utilities::appendToEndOfLine(filename, ext, delimiter, m_E_field);
    Utilities::appendToEndOfLine(filename, ext, delimiter, m_B_field);

};

void StaticPhysics::run(std::vector<ChargedParticle2D>& particles, std::vector<InfiniteWire2D>& wires)
{
    std::cout << "Run starting!" << std::endl;

    calculateElectricField(particles);
    calculateInfiniteWireMagneticField(wires);
    writeFields(Utilities::outputFilename);

    std::cout << "Run complete!" << std::endl;
};
