#pragma once

#include <type_traits>
#include <stdexcept>

#include "physics/math/vector3d.hpp"
#include "physics/core/config.hpp"
#include "physics/core/macros.hpp"

class Vector4D {
private:
    real x_, y_, z_, w_;

public:
    // Constructores
    Vector4D();
    Vector4D(real x, real y, real z, real w);
    Vector4D(const real arr[4]);
    
    explicit Vector4D(const Vector3D &vect);
    Vector4D(const Vector3D &vect, real w);

    // Getters
    PHYS_NODISCARD PHYS_FORCE_INLINE real x() const { return x_; }
    PHYS_NODISCARD PHYS_FORCE_INLINE real y() const { return y_; }
    PHYS_NODISCARD PHYS_FORCE_INLINE real z() const { return z_; }
    PHYS_NODISCARD PHYS_FORCE_INLINE real w() const { return w_; }

    // Setters
    PHYS_FORCE_INLINE void set_x(real x) { x_ = x; }
    PHYS_FORCE_INLINE void set_y(real y) { y_ = y; }
    PHYS_FORCE_INLINE void set_z(real z) { z_ = z; }
    PHYS_FORCE_INLINE void set_w(real w) { w_ = w; }

    // Acceso por indices []
    PHYS_NODISCARD real operator [] (int index) const;
    real &operator [] (int index);

    // Operadores suma y resta in place entre vectores
    PHYS_FORCE_INLINE Vector4D &operator += (const Vector4D &rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        w_ += rhs.w_;

        return *this;
    }

    PHYS_FORCE_INLINE Vector4D &operator -= (const Vector4D &rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        w_ -= rhs.w_;

        return *this;
    }
    
    // Operadores suma y resta entre vectores
    PHYS_NODISCARD PHYS_FORCE_INLINE Vector4D operator + (const Vector4D &rhs) const {
        return Vector4D(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_, w_ + rhs.w_);
    }

    PHYS_NODISCARD PHYS_FORCE_INLINE Vector4D operator - (const Vector4D &rhs) const {
        return Vector4D(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_, w_ - rhs.w_);
    }

    // Operador de negacion
    PHYS_NODISCARD PHYS_FORCE_INLINE Vector4D operator - () const {
        return Vector4D(-x_, -y_, -z_, -w_);
    }

    // Operadores multiplicación y división con escalar in place
    template <typename Scalar>
    Vector4D &operator *= (const Scalar &scalar);
    template <typename Scalar>
    Vector4D &operator /= (const Scalar &scalar);

    // Operadores multiplicacion y division con escalar
    template <typename Scalar>
    Vector4D operator * (const Scalar &scalar) const;
    template <typename Scalar>
    Vector4D operator / (const Scalar &scalar) const;

    // Operadores de comparación
    PHYS_NODISCARD PHYS_FORCE_INLINE bool operator == (const Vector4D &rhs) const {
        return (std::abs(x_ - rhs.x_) <= PHYS_EPSILON &&
                std::abs(y_ - rhs.y_) <= PHYS_EPSILON &&
                std::abs(z_ - rhs.z_) <= PHYS_EPSILON &&
                std::abs(w_ - rhs.w_) <= PHYS_EPSILON);
    }

    PHYS_NODISCARD PHYS_FORCE_INLINE bool operator != (const Vector4D &rhs) const {
        return !(*this == rhs);
    }

    // Producto punto
    PHYS_NODISCARD PHYS_FORCE_INLINE real Dot4D (const Vector4D &rhs) const {
        return (
            x_ * rhs.x() +
            y_ * rhs.y() +
            z_ * rhs.z() +
            w_ * rhs.w()
        );
    }

    // Producto cruz
    PHYS_NODISCARD Vector4D ComponentWiseMultiply (const Vector4D &rhs) const;

    // Magnitud y magnitud al cuadrado
    PHYS_NODISCARD real Magnitude () const;
    PHYS_NODISCARD real SquareMagnitude () const;

    // Normalización
    Vector4D &Normalize ();
    PHYS_NODISCARD Vector4D Normalized () const;

    // Distancia y distancia cuadrada
    PHYS_NODISCARD static real Distance(const Vector4D &vecA, const Vector4D &vecB);
    PHYS_NODISCARD static real SquareDistance(const Vector4D &vecA, const Vector4D &vecB);

    // Operadores de homogenizacion
    Vector4D &Homogenize ();
    PHYS_NODISCARD Vector4D Homogenized () const;
    PHYS_NODISCARD bool IsPoint () const;
    PHYS_NODISCARD bool IsDirection () const;

    // Conversión a Vector3D
    PHYS_NODISCARD Vector3D ToVector3D() const;
    PHYS_NODISCARD Vector3D ToDirection3D() const;

    // Modificadores
    Vector4D &MakePoint ();
    Vector4D &MakeDirection ();

    // Obtener data 
    PHYS_NODISCARD const real *Data () const;
    real *Data ();
};

#include "vector4d.inl"