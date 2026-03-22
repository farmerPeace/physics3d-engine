#include "physics/math/vector3d.hpp"
#include <cmath>

// Constructores
Vector3D::Vector3D(): x_(0), y_(0), z_(0) {}
Vector3D::Vector3D(float x, float y, float z): x_(x), y_(y), z_(z) {}
Vector3D::Vector3D(const float arr[3]): x_(arr[0]), y_(arr[1]), z_(arr[2]) {}

// Getters
float Vector3D::x() const { return x_; }
float Vector3D::y() const { return y_; }
float Vector3D::z() const { return z_; }

// Setters
void Vector3D::set_x(float x) { x_ = x; }
void Vector3D::set_y(float y) { y_ = y; }
void Vector3D::set_z(float z) { z_ = z; }

// Operadores de asignación (suma y resta)
Vector3D &Vector3D::operator += (const Vector3D &rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
}

Vector3D &Vector3D::operator -= (const Vector3D &rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;        
}

// Operadores arimeticos (suma y resta)
Vector3D Vector3D::operator + (const Vector3D &rhs) const {
    return Vector3D(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
}

Vector3D Vector3D::operator - (const Vector3D &rhs) const {
    return Vector3D(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_);
}

Vector3D Vector3D::operator - () const {
    return Vector3D(-x_, -y_, -z_);
}

// Operadores de comparación
bool Vector3D::operator == (const Vector3D &rhs) const {
    const float EPS = 1e-6f;
    return std::abs(x_ - rhs.x_) <= EPS &&
           std::abs(y_ - rhs.y_) <= EPS &&
           std::abs(z_ - rhs.z_) <= EPS;
}

bool Vector3D::operator != (const Vector3D &rhs) const {
    return !(*this == rhs);
}

// Indices
float Vector3D::operator[] (int index) const {
    if (index == 0) return x_;
    if (index == 1) return y_;
    if (index == 2) return z_;
    throw std::out_of_range("Indice Vector3D fuera de rango");
}

float &Vector3D::operator [] (int index) {
    if (index == 0) return x_;
    if (index == 1) return y_;
    if (index == 2) return z_;
    throw std::out_of_range("Indice Vector3D fuera de rango");
}

// Producto punto y producto cruz
float Vector3D::Dot3D (const Vector3D &rhs) const {
    return (
        x_ * rhs.x_ +
        y_ * rhs.y_ +
        z_ * rhs.z_
    );
}

Vector3D Vector3D::Cross3D(const Vector3D &rhs) const {
    return Vector3D (
    y_ * rhs.z_ - z_ * rhs.y_,
    z_ * rhs.x_ - x_ * rhs.z_,
    x_ * rhs.y_ - y_ * rhs.x_
    );
}

// Magnitud y magnitud al cuadrado
float Vector3D::Magnitude() const {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);     
}

float Vector3D::SquareMagnitude() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

// Normalización
Vector3D &Vector3D::Normalize() {
    const float sq_mag = SquareMagnitude();

    if (sq_mag > std::numeric_limits<float>::epsilon()) {
        const float inv_mag = 1.0f / std::sqrt(sq_mag);
        
        x_ *= inv_mag;
        y_ *= inv_mag;
        z_ *= inv_mag;
        
    } else {
        x_ = 0.0f;
        y_ = 0.0f;
        z_ = 0.0f;
    }

    return *this;
}

Vector3D Vector3D::Normalized() const {
    Vector3D result = *this;
    result.Normalize();
    return result;
}

// Distancia y distancia al cuadrado
float Vector3D::Distance(const Vector3D& a, const Vector3D& b) {
    return (b - a).Magnitude();
}

float Vector3D::SquaredDistance(const Vector3D& a, const Vector3D& b) {
    return (b - a).SquareMagnitude();
}