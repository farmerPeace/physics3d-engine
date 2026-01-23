#include <cmath>
#include <stdexcept>
#include <limits>

#include "physics/vector4d.hpp"
#include "physics/vector3d.hpp"

#define EPSILON 1e-6f;

// Constructores
Vector4D::Vector4D(): x_(0), y_(0), z_(0), w_(0) {}
Vector4D::Vector4D(float x, float y, float z, float w): x_(x), y_(y), z_(z), w_(w) {}
Vector4D::Vector4D(const float arr[4]): x_(arr[0]), y_(arr[1]), z_(arr[2]), w_(arr[3]) {}

Vector4D::Vector4D(const Vector3D &vect): x_(vect.x()), y_(vect.y()), z_(vect.z()), w_(1) {}
Vector4D::Vector4D(const Vector3D &vect, float w): x_(vect.x()), y_(vect.y()), z_(vect.z()), w_(w) {}

// Getters
float Vector4D::x() const { return x_; }
float Vector4D::y() const { return y_; }
float Vector4D::z() const { return z_; }
float Vector4D::w() const { return w_; }

// Setters
void Vector4D::set_x(float x) { x_ = x; }
void Vector4D::set_y(float y) { y_ = y; }
void Vector4D::set_z(float z) { z_ = z; }
void Vector4D::set_w(float w) { w_ = w; }

// Indices
float Vector4D::operator [] (int index) const {
    if (index == 0) return x_;
    if (index == 1) return y_;
    if (index == 2) return z_;
    if (index == 3) return w_;

    throw std::out_of_range("Indice Vector4D fuera de rango");
}

float &Vector4D::operator [] (int index) {
    if (index == 0) return x_;
    if (index == 1) return y_;
    if (index == 2) return z_;
    if (index == 3) return w_;
    throw std::out_of_range("Indice Vector4D fuera de rango");
}

// Operadores suma y resta in place entre vectores
Vector4D &Vector4D::operator += (const Vector4D &rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    w_ += rhs.w_;

    return *this;
}

Vector4D &Vector4D::operator -= (const Vector4D &rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    w_ -= rhs.w_;

    return *this;
}

// Operadores suma y resta entre vectores
Vector4D Vector4D::operator + (const Vector4D &rhs) const {
    return Vector4D(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_, w_ + rhs.w_);
}

Vector4D Vector4D::operator - (const Vector4D &rhs) const {
    return Vector4D(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_, w_ - rhs.w_);
}

// Operadres multiplicacion y division con escalar in place
template <typename Scalar>
Vector4D &Vector4D::operator *= (const Scalar &scalar) {
    static_assert(std::is_arithmetic<Scalar>, "Scalar debe ser aritmetico");

    x_ *= static_cast<float>(scalar);
    y_ *= static_cast<float>(scalar);
    z_ *= static_cast<float>(scalar);
    w_ *= static_cast<float>(scalar);

    return *this;
}

template <typename Scalar>
Vector4D &Vector4D::operator /= (const Scalar &scalar) {
    static_assert(std::is_arithmetic<Scalar>, "Scalar debe ser aritmetico");

    const float scalar_f = static_cast<float>(scalar);

    if (std::abs(scalar_f) < std::numeric_limits<float>::epsilon()) {
        throw std::domain_error("Division por (casi) cero en un Vector4D");
    }

    const float inv = 1.0f / scalar_f;

    x_ *= inv;
    y_ *= inv;
    z_ *= inv;
    w_ *= inv;

    return *this;
}

// Operadores multiplicacion y division con escalar 
template <typename Scalar>
Vector4D Vector4D::operator * (const Scalar &scalar) const {
    static_assert(std::is_arithmetic<Scalar>, "Scalar debe ser aritmetico");
    return Vector4D(x_ * scalar, y_ * scalar, z_ * scalar, w_ * scalar);
}

template <typename Scalar>
Vector4D Vector4D::operator / (const Scalar &scalar) const {
    static_assert(std::is_arithmetic<Scalar>, "Scalar debe ser aritmetico");

    const float scalar_f = static_cast<float>(scalar);

    if (std::abs(scalar_f) < std::numeric_limits<float>::epsilon()) {
        throw std::domain_error("Division por (casi) cero en un Vector4D");
    }

    const float inv = 1.0f / scalar_f;

    return Vector4D(x_ * inv, y_ * inv, z_ * inv, w_ * inv);
}

// Negativo
Vector4D Vector4D::operator - () const {
    return Vector4D(-x_, -y_, -z_, -w_);
}

// Operadores de comparación
bool Vector4D::operator == (const Vector4D &rhs) const {
    const float epsilon = 1e-5f;
    return (std::abs(x_ - rhs.x_) <= epsilon &&
            std::abs(y_ - rhs.y_) <= epsilon &&
            std::abs(z_ - rhs.z_) <= epsilon &&
            std::abs(w_ - rhs.w_) <= epsilon);
}

bool Vector4D::operator != (const Vector4D &rhs) const {
    return !(*this == rhs);
}