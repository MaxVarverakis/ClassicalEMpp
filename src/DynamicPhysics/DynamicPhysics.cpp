#include "DynamicPhysics.hpp"

DynamicPhysics::DynamicPhysics(const std::size_t& dim, const double& bound, const std::size_t& numPoints, const std::size_t& numSteps, const double& dt)
    : m_static_physics {dim, bound, numPoints}
    , m_numSteps {numSteps}
    , m_dt {dt}
    , m_acceleration { calculateAcceleration(Utilities::particles) }
{
    Utilities::initMessage();
}

void DynamicPhysics::run(std::vector<ChargedParticle2D>& particles, std::vector<InfiniteWire2D>& wires)
{
    std::cout << "Run starting!" << std::endl;
    std::cout << "Current iteration: " << m_iteration << '\n';
    
    m_static_physics.calculateInfiniteWireMagneticField(wires);
    
    if (!particles.empty())
    {
        m_static_physics.calculateElectricField(particles, true);
        m_static_physics.writeFields(Utilities::outputFilename + "_" +  std::to_string(m_iteration));

        while (m_iteration < m_numSteps-1)
        {
            evolve(particles);
            std::cout << "Current iteration: " << m_iteration << '\n';
        }
    }

    std::cout << "Run complete!" << std::endl;
};

std::vector<Point2D> DynamicPhysics::calculateAcceleration(std::vector<ChargedParticle2D>& particles)
{
    const std::size_t& numParticles { particles.size() };
    std::vector<Point2D> acceleration(numParticles, Point2D{ 0.0, 0.0 });

    for (std::size_t i = 0; i < numParticles; ++i)
    {
        for (std::size_t j = i+1; j < numParticles; ++j)
        {
            Point2D r_prime { Utilities::r_prime(particles[i].position, particles[j].position) };

            double r { r_prime.magnitude() };
            acceleration[i] += Point2D { r_prime * particles[i].charge * particles[j].charge / (particles[i].mass * r*r*r) };
            acceleration[j] -= Point2D { r_prime * particles[i].charge * particles[j].charge / (particles[j].mass * r*r*r) };
        }
    }

    return acceleration;
        
        // for (ChargedParticle2D& particle : particles)
        // {
        //     for (ChargedParticle2D& other_particle : particles)
        //     {
        //         if (particle == other_particle) { continue; }

        //         Point2D r_prime { particle.position - other_particle.position };
        //         double r { r_prime.magnitude() };

        //         acceleration += Point2D { r_prime * particle.charge * other_particle.charge / (particle.mass * r*r*r) };
        //     }
        // }

    // temporary fix until annihilation is implemented
    // double max_acceleration { 10 };
    // if (abs(acceleration.x()) > max_acceleration)
    // { 
    //     // std::cout << "ax:\t" << acceleration.x() << '\n' << "ay:\t" << acceleration.y() << std::endl;        
    //     acceleration.setX(Utilities::sign<double>(acceleration.x()) * max_acceleration); 
    //     acceleration.setX(0.);
    // }
    // else if (abs(acceleration.y()) > max_acceleration)
    // {
    //     // acceleration.setY(Utilities::sign<double>(acceleration.y()) * max_acceleration);
    //     acceleration.setY(0.);
    // }
};

void DynamicPhysics::evolve(std::vector<ChargedParticle2D>& particles)
{
    
    #pragma omp parallel for
    for (std::size_t i = 0; i < particles.size(); ++i)
    {
        // std::size_t idx = Utilities::findNearestGridPointIndex(particle.position);
        // const Field2D& E_field = m_static_physics.E_field()[idx];
        // double a { particle.charge * E_field.magnitude / particle.mass };
        // Point2D acceleration { a * E_field.direction.x(), a * E_field.direction.y() };

        ChargedParticle2D& particle { particles[i] };
        const Point2D& acceleration { m_acceleration[i] };

        // update the position according to Verlet integration
        double next_x { particle.position.x() + particle.velocity.x()*m_dt + 0.5 * acceleration.x()*m_dt*m_dt };
        double next_y { particle.position.y() + particle.velocity.y()*m_dt + 0.5 * acceleration.y()*m_dt*m_dt };

        if (abs(next_x) >= Utilities::bound)
        {
            if (Utilities::periodic)
            {
                particle.position.setX( next_x - Utilities::sign<double>(next_x) * 2 * Utilities::bound );
            }
            else
            {
                particle.position.setX( Utilities::sign<double>(next_x) * Utilities::bound );
            }
        }
        else
        {
            particle.position.setX( next_x );
        }

        if (abs(next_y) >= Utilities::bound)
        {
            if (Utilities::periodic)
            {
                particle.position.setY( next_y - Utilities::sign<double>(next_y) * 2 * Utilities::bound );
            }
            else
            {
                particle.position.setY( Utilities::sign<double>(next_y) * Utilities::bound );
            }
        }
        else
        {
            particle.position.setY( next_y );
        }
    }
    
    std::vector<Point2D> new_accelerations { calculateAcceleration(particles) };
    
    #pragma omp parallel for
    for (std::size_t i = 0; i < particles.size(); ++i)
    {
        ChargedParticle2D& particle { particles[i] };
        const Point2D& acceleration { m_acceleration[i] };
        const Point2D& new_acceleration { new_accelerations[i] };

        const double v_limit { m_static_physics.geometry().bound() / (8 * m_dt) }; // max velocity is 1/8-th the domain grid per time step

        /*
        From the position-setting loop, now any particles
        that were headed out of bounds will have their
        appropriate position component equal to the domain bound (if boundaries non-periodic)
        */

        // update the velocity according to Verlet velocity integration
        if (abs(particle.position.x()) == Utilities::bound)
        {
            particle.velocity.setX( -particle.velocity.x() );
        }
        else
        {
            const double& new_v { particle.velocity.x() + 0.5 * (acceleration.x() + new_acceleration.x())*m_dt };
            particle.velocity.setX( (abs(new_v) < v_limit) ? new_v : Utilities::sign<double>(new_v) * v_limit );
        }

        if (abs(particle.position.y()) == Utilities::bound)
        {
            particle.velocity.setY( -particle.velocity.y() );
        }
        else
        {
            const double& new_v { particle.velocity.y() + 0.5 * (acceleration.y() + new_acceleration.y())*m_dt };
            particle.velocity.setY( (abs(new_v) < v_limit) ? new_v : Utilities::sign<double>(new_v) * v_limit );
        }

        m_acceleration[i] = new_accelerations[i];

        // Not in use, but here for nostalgia...
        // use RK4 to update the position and velocity
        // RK4(particle, acceleration);
    }

    ++m_iteration;
    m_static_physics.calculateElectricField(particles, false);
    m_static_physics.writeFields(Utilities::outputFilename + "_" +  std::to_string(m_iteration));
}

// void DynamicPhysics::RK4(ChargedParticle2D& particle, std::vector<ChargedParticle2D>& particles)
// {
//     const Point2D k1 { particle.velocity.x(), particle.velocity.y() };
//     const Point2D l1 { calculateAcceleration(particle, particles) };

//     const Point2D k2 { particle.velocity.x() + 0.5 * m_dt * l1.x(), particle.velocity.y() + 0.5 * m_dt * l1.y() };
//     const Point2D l2 { calculateAcceleration(particle, particles, 0.5 * m_dt * k1) };
// };
