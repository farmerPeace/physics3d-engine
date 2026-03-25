#pragma once

#include <type_traits>
#include <stdexcept>

#include "physics/core/config.hpp"
#include "physics/core/macros.hpp"

class Vector3D {
private:
    real x_, y_, z_;

public:
    // Constructores
    Vector3D();
    Vector3D(real x, real y, real z);
    Vector3D(const real arr[3]);

    // Getters y 
    PHYS_NODISCARD PHYS_FORCE_INLINE real x() const { return x_; };
    PHYS_NODISCARD PHYS_FORCE_INLINE real y() const { return y_; };
    PHYS_NODISCARD PHYS_FORCE_INLINE real z() const { return z_; };

    // Setters
    PHYS_FORCE_INLINE void set_x(real x) { x_ = x; };
    PHYS_FORCE_INLINE void set_y(real y) { y_ = y; };
    PHYS_FORCE_INLINE void set_z(real z) { z_ = z; };  
    
    // Operadores de asignación (suma y resta)
    PHYS_FORCE_INLINE Vector3D &operator += (const Vector3D &rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }

    PHYS_FORCE_INLINE Vector3D &operator -= (const Vector3D &rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;        
    }

    // Operadores arimeticos (suma y resta)
    PHYS_NODISCARD PHYS_FORCE_INLINE Vector3D operator + (const Vector3D& rhs) const {
        return Vector3D(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
    }

    PHYS_NODISCARD PHYS_FORCE_INLINE Vector3D operator - (const Vector3D& rhs) const {
        return Vector3D(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_);
    }

    PHYS_NODISCARD PHYS_FORCE_INLINE Vector3D operator - () const {
        return Vector3D(-x_, -y_, -z_);
    }

    // Multiplicación y división con escalar
    template <typename Scalar>
    Vector3D &operator *= (const Scalar &scalar);

    template <typename Scalar>
    Vector3D &operator /= (const Scalar &scalar);

    // Operadores de comparación
    PHYS_NODISCARD PHYS_FORCE_INLINE bool operator == (const Vector3D &rhs) const {
        return std::abs(x_ - rhs.x_) <= PHYS_EPSILON &&
            std::abs(y_ - rhs.y_) <= PHYS_EPSILON &&
            std::abs(z_ - rhs.z_) <= PHYS_EPSILON;
    }

    PHYS_NODISCARD PHYS_FORCE_INLINE bool operator != (const Vector3D &rhs) const {
        return !(*this == rhs);
    }

    // Acceso por indice
    PHYS_NODISCARD real operator [] (int index) const;
    real &operator [] (int index);

    // Producto punto y producto cruz
    PHYS_NODISCARD PHYS_FORCE_INLINE real Dot3D (const Vector3D &rhs) const {
        return (
            x_ * rhs.x_ +
            y_ * rhs.y_ +
            z_ * rhs.z_
        );
    }

    PHYS_NODISCARD Vector3D Cross3D(const Vector3D &rhs) const;

    // Magnitud y magnitud al cuadrado
    PHYS_NODISCARD real Magnitude() const;
    
    PHYS_NODISCARD PHYS_FORCE_INLINE real SquareMagnitude() const {
        return x_ * x_ + y_ * y_ + z_ * z_;
    }

    // Normalización
    Vector3D &Normalize ();
    PHYS_NODISCARD Vector3D Normalized () const;

    // Distancia y distancia cuadrada
    PHYS_NODISCARD static real Distance(const Vector3D &a, const Vector3D &b);
    PHYS_NODISCARD static real SquaredDistance(const Vector3D &a, const Vector3D &b);
};

// Funciones globales
template <typename Scalar>
Vector3D operator * (const Scalar &scalar, Vector3D vect);

template <typename Scalar>
Vector3D operator * (Vector3D vect, const Scalar &scalar);

template <typename Scalar>
Vector3D operator / (Vector3D vect, const Scalar &scalar);

#include "vector3d.inl"