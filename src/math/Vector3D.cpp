#include "physics/math/vector3d.hpp"
#include <cmath>

// Constructores
Vector3D::Vector3D(): x_(0), y_(0), z_(0) {}
Vector3D::Vector3D(real x, real y, real z): x_(x), y_(y), z_(z) {}
Vector3D::Vector3D(const real arr[3]): x_(arr[0]), y_(arr[1]), z_(arr[2]) {}

// Indices
real Vector3D::operator[] (int index) const {
    if (index == 0) return x_;
    if (index == 1) return y_;
    if (index == 2) return z_;
    throw std::out_of_range("Indice Vector3D fuera de rango");
}

real &Vector3D::operator [] (int index) {
    if (index == 0) return x_;
    if (index == 1) return y_;
    if (index == 2) return z_;
    throw std::out_of_range("Indice Vector3D fuera de rango");
}

// Producto punto y producto cruz
Vector3D Vector3D::Cross3D(const Vector3D &rhs) const {
    return Vector3D (
    y_ * rhs.z_ - z_ * rhs.y_,
    z_ * rhs.x_ - x_ * rhs.z_,
    x_ * rhs.y_ - y_ * rhs.x_
    );
}

// Magnitud y magnitud al cuadrado
real Vector3D::Magnitude() const {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);     
}

// Normalización
Vector3D &Vector3D::Normalize() {
    const real sq_mag = SquareMagnitude();

    if (sq_mag > PHYS_EPSILON) {
        const real inv_mag = 1.0f / std::sqrt(sq_mag);
        
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
real Vector3D::Distance(const Vector3D& a, const Vector3D& b) {
    return (b - a).Magnitude();
}

real Vector3D::SquaredDistance(const Vector3D& a, const Vector3D& b) {
    return (b - a).SquareMagnitude();
}