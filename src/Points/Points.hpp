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

    // Setters
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }

    // Methods
    double distanceTo(const Point2D& other) const
    {
        return std::sqrt(
              (m_x - other.x()) * (m_x - other.x())
            + (m_y - other.y()) * (m_y - other.y())
        );
    }
    
    Point2D operator-(const Point2D& other) const
    {
        return Point2D(m_x - other.x(), m_y - other.y());
    }

    Point2D operator+(const Point2D& other) const
    {
        return Point2D(m_x + other.x(), m_y + other.y());
    }

    Point2D operator*(const double scalar)
    {
        return Point2D(scalar * m_x, scalar * m_y);
    }

    Point2D operator/(const double scalar)
    {
        return Point2D(m_x / scalar, m_y / scalar);
    }

    bool operator==(const Point2D& other) const
    {
        return m_x == other.x() && m_y == other.y();
    }

    void operator+=(const Point2D& other)
    {
        m_x += other.x();
        m_y += other.y();
        // return Point2D(m_x + other.x(), m_y + other.y());
    }
    
    void operator-=(const Point2D& other)
    {
        m_x -= other.x();
        m_y -= other.y();
        // return Point2D(m_x + other.x(), m_y + other.y());
    }

    void normalize()
    {
        double magnitude { this->magnitude() };
        m_x /= magnitude;
        m_y /= magnitude;
    }

    double magnitude() { return std::sqrt(m_x*m_x + m_y*m_y);}
};

// overload scalar multiplication, but to make it work in `scalar * Point2D` direction we define it outside the Point2D class
Point2D operator*(const double scalar, const Point2D& point);

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

    // Setters
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }
    void setZ(double z) { m_z = z; }

    // Methods
    double distanceTo(const Point3D& other) const
    {
        return std::sqrt(
              (m_x - other.x()) * (m_x - other.x())
            + (m_y - other.y()) * (m_y - other.y())
            + (m_z - other.z()) * (m_z - other.z())
        );
    }

    Point3D operator-(const Point3D& other) const
    {
        return Point3D(m_x - other.x(), m_y - other.y(), m_z - other.z());
    }

    Point3D operator+(const Point3D& other) const
    {
        return Point3D(m_x + other.x(), m_y + other.y(), m_z + other.z());
    }


    Point3D operator*(const double scalar)
    {
        return Point3D(scalar * m_x, scalar * m_y, scalar * m_z);
    }

    Point3D operator/(const double scalar)
    {
        return Point3D(m_x / scalar, m_y / scalar, m_z / scalar);
    }

    bool operator==(const Point3D& other) const
    {
        return m_x == other.x() && m_y == other.y() && m_z == other.z();
    }

    void operator+=(const Point3D& other)
    {
        m_x += other.x();
        m_y += other.y();
        m_z += other.z();
    }

    void operator-=(const Point3D& other)
    {
        m_x -= other.x();
        m_y -= other.y();
        m_z -= other.z();
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

// overload scalar multiplication, but to make it commutative we define it outside the Point2D class
Point3D operator*(const double scalar, const Point3D& point);

// Structs below

// this struct allows the user to place a charged particle in the domain and holds the charge and 2D position of the particle
struct ChargedParticle2D
{
    const double charge; // C
    const double mass; // kg
    Point2D position; // m
    Point3D velocity; // m/s

    bool operator==(const ChargedParticle2D& other) const
    {
        return charge == other.charge &&
                mass == other.mass &&
                position == other.position &&
                velocity == other.velocity;
    }
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
    const double mass; // kg
    Point3D position; // m
    Point3D velocity; // m/s

    bool operator==(const ChargedParticle3D& other) const
    {
        return charge == other.charge &&
                mass == other.mass &&
                position == other.position &&
                velocity == other.velocity;
    }
};
