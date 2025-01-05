#include "Geometry.hpp"

Geometry::Geometry(const std::size_t& dim, const double& bound, const std::size_t& numPoints) : m_dim{dim}, m_bound{bound}, m_numPoints{numPoints}
{
    checkDomainDimension(m_dim);
    
    // Create a grid of points (either 2D or 3D)
    constructWorld();
};

int Geometry::checkDomainDimension(const std::size_t& dim)
{
    if (dim != 2 && dim != 3)
    {
        std::cerr << "Currently only implemented for 2D and 3D domains." << std::endl;
        return -1;
    }

    return 0;
};

void Geometry::constructWorld()
{
    if (m_dim == 2)
    {
        std::cout << "Constructing 2D world..." << std::endl;
        std::vector<Point2D>& grid {std::get<std::vector<Point2D>>(m_grid)};
        grid.reserve(m_numPoints * m_numPoints);
        double dx = 2*m_bound / m_numPoints;
        for (std::size_t i = 0; i < m_numPoints+1; ++i)
        {
            for (std::size_t j = 0; j < m_numPoints+1; ++j)
            {
                grid.emplace_back(Point2D(-m_bound + i*dx, -m_bound + j*dx));
            }
        }
    }
    else if (m_dim == 3)
    {
        std::cout << "Constructing 3D world..." << std::endl;
        std::vector<Point3D>& grid {std::get<std::vector<Point3D>>(m_grid)};
        grid.reserve(m_numPoints * m_numPoints * m_numPoints);
        double dx = 2*m_bound / m_numPoints;
        for (std::size_t i = 0; i < m_numPoints+1; ++i)
        {
            for (std::size_t j = 0; j < m_numPoints+1; ++j)
            {
                for (std::size_t k = 0; k < m_numPoints+1; ++k)
                {
                    grid.emplace_back(Point3D(-m_bound + i*dx, -m_bound + j*dx, -m_bound + k*dx));
                }
            }
        }
    }
    // Don't need an else statement since checkDomainDimension() should catch any other cases
};

void Geometry::writeGrid(const std::string& filename, const std::string ext, const std::string delimiter)
{
    // Currently for macOS, will change to $HOME for Linux
    std::ofstream file("/Users/max/ClassicalEM++/outputs/" + filename + "." + ext);

    // ensure file is open for writing (needs to caught in a try catch block)
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open file for writing");
    }
    
    // do not write headers for now
    // file << 'x' << delimiter << 'y' << '\n';
    if (m_dim == 2)
    {
        const std::vector<Point2D> grid = std::get<std::vector<Point2D>>(m_grid);
        for (const Point2D& point : grid)
        {
            file << point.x() << delimiter << point.y() << std::endl;
        }
    }
    else if (m_dim == 3)
    {
        const std::vector<Point3D> grid = std::get<std::vector<Point3D>>(m_grid);
        for (const Point3D& point : grid)
        {
            file << point.x() << delimiter << point.y() << delimiter << point.z() << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: Dimension not supported." << std::endl;
    }
};
