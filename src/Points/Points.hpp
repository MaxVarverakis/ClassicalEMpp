#pragma once

#include <cmath>

class Point2D
{
private:
    double m_x;
    double m_y;

public:
    Point2D(double x, double y) : m_x{x}, m_y{y} {};

    // Getters
    double x() const { return m_x; }
    double y() const { return m_y; }

    // Setters (for now leave out this feature)
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }

    // Methods
    double distanceTo(Point2D& other)
    {
        return std::sqrt(
              (m_x - other.x()) * (m_x - other.x())
            + (m_y - other.y()) * (m_y - other.y())
        );
    }
    
    Point2D operator-(const Point2D& other)
    {
        return Point2D(m_x - other.x(), m_y - other.y());
    }

    void normalize()
    {
        double magnitude { this->magnitude() };
        m_x /= magnitude;
        m_y /= magnitude;
    }

    double magnitude() { return std::sqrt(m_x*m_x + m_y*m_y);}
};

class Point3D
{
private:
    double m_x;
    double m_y;
    double m_z;

public:
    Point3D(double x, double y, double z) : m_x{x}, m_y{y}, m_z{z} {};

    // Getters
    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }

    // Setters (for now leave out this feature)
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }
    void setZ(double z) { m_z = z; }

    // Methods
    double distanceTo(Point3D& other)
    {
        return std::sqrt(
              (m_x - other.x()) * (m_x - other.x())
            + (m_y - other.y()) * (m_y - other.y())
            + (m_z - other.z()) * (m_z - other.z())
        );
    }

    void normalize()
    {
        double magnitude { this->magnitude() };
        m_x /= magnitude;
        m_y /= magnitude;
        m_z /= magnitude;
    }

    double magnitude() { return std::sqrt(m_x*m_x + m_y*m_y + m_z*m_z);}
};

// Structs below

// this struct allows the user to place a charged particle in the domain and holds the charge and 2D position of the particle
struct ChargedParticle2D
{
    const double charge; // C
    Point2D position; // m
};

// this struct allows the user to place an infinite wire in the domain and holds the current, 2D position of the wire, and the direction of the wire
struct InfiniteWire2D
{
    const double current; // A
    Point2D position; // m
    Point3D direction; // unit vector
};

// this struct holds the magnitude and unit vector of a 2D field
struct Field2D
{
    double magnitude; // V/m or T
    Point2D direction; // unit vector
};

// this struct allows the user to place a charged particle in the domain and holds the charge and 3D position of the particle
struct ChargedParticle3D
{
    const double charge; // C
    Point3D position; // m
};
