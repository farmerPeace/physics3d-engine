#include <cmath>
#include <stdexcept>
#include <limits>

#include "physics/vector4d.hpp"
#include "physics/vector3d.hpp"

#define EPSILON 1e-6f

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

// Negativo
Vector4D Vector4D::operator - () const {
    return Vector4D(-x_, -y_, -z_, -w_);
}

// Operadores de comparación
bool Vector4D::operator == (const Vector4D &rhs) const {
    return (std::abs(x_ - rhs.x_) <= EPSILON &&
            std::abs(y_ - rhs.y_) <= EPSILON &&
            std::abs(z_ - rhs.z_) <= EPSILON &&
            std::abs(w_ - rhs.w_) <= EPSILON);
}

bool Vector4D::operator != (const Vector4D &rhs) const {
    return !(*this == rhs);
}

float Vector4D::Dot4D (const Vector4D &rhs) const {
    return (
        x_ * rhs.x() +
        y_ * rhs.y() +
        z_ * rhs.z() +
        w_ * rhs.w()
    );
}

Vector4D Vector4D::ComponentWiseMultiply (const Vector4D &rhs) const {
    return Vector4D ( 
        x_ * rhs.x(),
        y_ * rhs.y(),
        z_ * rhs.z(),
        w_ * rhs.w()
    );
}

float Vector4D::Magnitude () const {
    if (Vector4D::IsPoint()) {
        throw std::domain_error("La magnitud de un punto homogeneo no esta definida");
    }

    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_ );
}

float Vector4D::SquareMagnitude () const {
    if (IsPoint()) {
        throw std::domain_error("La magnitud de un punto homogeneo no esta definida");
    }

    return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
}


Vector4D &Vector4D::Normalize () {
    if (IsPoint()) {
        throw std::domain_error("No se puede normalizar un punto homogeneo");
    }

    const float sq_mag = x_ * x_ + y_ * y_ + z_ * z_;

    if (sq_mag > EPSILON) {
        const float inv_mag = 1.0f / std::sqrt(sq_mag);
        x_ *= inv_mag;
        y_ *= inv_mag;
        z_ *= inv_mag;
    } else {
        x_ = y_ = z_ = 0.0f;
    }

    w_ = 0.0f;
    return *this;
}

Vector4D Vector4D::Normalized () const {
    Vector4D result = *this;
    result.Normalize();
    return result;
}

float Vector4D::Distance(const Vector4D& a, const Vector4D& b) {
    if (!a.IsPoint() || !b.IsPoint()) {
        throw std::domain_error("Distance requiere dos puntos homogeneos");
    }

    Vector4D ah = a.Homogenized();
    Vector4D bh = b.Homogenized();

    const float dx = bh.x_ - ah.x_;
    const float dy = bh.y_ - ah.y_;
    const float dz = bh.z_ - ah.z_;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float Vector4D::SquareDistance(const Vector4D& a, const Vector4D& b) {
    if (!a.IsPoint() || !b.IsPoint()) {
        throw std::domain_error("SquareDistance requiere dos puntos homogeneos");
    }

    Vector4D ah = a.Homogenized();
    Vector4D bh = b.Homogenized();

    const float dx = bh.x_ - ah.x_;
    const float dy = bh.y_ - ah.y_;
    const float dz = bh.z_ - ah.z_;

    return dx * dx + dy * dy + dz * dz;
}

Vector4D &Vector4D::Homogenize () {

    if (std::abs(w_) < EPSILON) {
        return *this;
    }

    const float w_inv = 1.0f / w_;
    x_ *= w_inv;
    y_ *= w_inv;
    z_ *= w_inv;
    w_ = 1.0f;

    return *this;
}

Vector4D Vector4D::Homogenized () const {
    if (std::abs(w_) < EPSILON) {
        return Vector4D(x_, y_, z_, w_);
    }

    const float w_inv = 1.0f / w_;

    return Vector4D (
    x_ * w_inv,
    y_ * w_inv,
    z_ * w_inv,
    1.0f
    );
}

bool Vector4D::IsPoint () const {
    return std::abs(w_) > EPSILON;
}

bool Vector4D::IsDirection () const {
    return !(Vector4D::IsPoint());
}

Vector3D Vector4D::ToVector3D () const {
    if (Vector4D::IsPoint()) {
        return Vector3D(x_/w_, y_/w_, z_/w_);
    } else {
        throw std::domain_error("No se puede convertir");
    }
}

Vector3D Vector4D::ToDirection3D () const {
    return Vector3D(x_, y_, z_);
}

Vector4D& Vector4D::MakePoint() {
    if (IsDirection()) {
        throw std::domain_error("No se puede convertir una direccion en punto sin referencia");
    }
    
    if (std::abs(w_ - 1.0f) > EPSILON) {
        Homogenize();
    }

    return *this;
}

Vector4D& Vector4D::MakeDirection() {
    w_ = 0.0f;
    return *this;;
}

const float *Vector4D::Data () const {
    return &x_;
}

float *Vector4D::Data () {
    return &x_;
}

