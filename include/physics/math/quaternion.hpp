#pragma once
#include <cmath>

#include "physics/math/vector3d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/matrix4d.hpp"

#include "physics/core/config.hpp"
#include "physics/core/macros.hpp"

class Quaternion {
private:
    real w_, x_, y_, z_;

public:
    Quaternion();
    Quaternion(real w, real x, real y, real z);
    Quaternion(real w, const Vector3D &vect);

    // Data
    PHYS_NODISCARD const real *Data() const;
    real *Data();

    // Acceso por indices
    PHYS_NODISCARD real operator [] (int index) const;
    real &operator [] (int index);

    // Operadores suma y resta in place entre cuaterniones
    Quaternion &operator += (const Quaternion &rhs);
    Quaternion &operator -= (const Quaternion &rhs);

    // Operadores suma y resta entre cuaterniones
    PHYS_NODISCARD Quaternion operator + (const Quaternion &rhs) const;
    PHYS_NODISCARD Quaternion operator - (const Quaternion &rhs) const;
    PHYS_NODISCARD Quaternion operator - () const;

    // Operadores multiplicacion y division con escalar in place
    Quaternion &operator *= (const real &scalar);
    Quaternion &operator /= (const real &scalar);
    
    // Operadores multiplicación y división con escalar
    PHYS_NODISCARD Quaternion operator * (const real &scalar) const;
    PHYS_NODISCARD Quaternion operator / (const real &scalar) const;

    // Multiplicación de cuaterniones
    Quaternion &operator *= (const Quaternion &rhs);
    PHYS_NODISCARD Quaternion operator * (const Quaternion &rhs) const;
    
    // Producto punto
    PHYS_NODISCARD PHYS_FORCE_INLINE real Dot(const Quaternion &rhs) const {
        return w_ * rhs.w_ + x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
    }

    // Conjugado e inverso
    PHYS_NODISCARD Quaternion Conjugate () const;
    PHYS_NODISCARD Quaternion Inverse () const;

    // Magnitud y magnitud al cuadrado
    PHYS_NODISCARD real Magnitude () const;
    PHYS_NODISCARD PHYS_FORCE_INLINE real SquareMagnitude () const {
        return w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_;
    }

    // Normalización
    Quaternion &Normalize ();
    PHYS_NODISCARD Quaternion Normalized () const;

    // Rotar vector por un cuaternion
    PHYS_NODISCARD Vector3D Rotate (const Vector3D &vec) const;

    // Interpolaciones
    PHYS_NODISCARD static Quaternion Slerp (const Quaternion &a, const Quaternion &b, real t);
    PHYS_NODISCARD static Quaternion Nlerp (const Quaternion &a, const Quaternion &b, real t);

    // Derivada temporal
    PHYS_NODISCARD Quaternion Derivative (const Vector3D &angularVelocity) const;
 
    // Conversiones
    PHYS_NODISCARD Matrix3D ToRotationMatrix () const;
    PHYS_NODISCARD Matrix4D ToTransformMatrix () const;
    PHYS_NODISCARD Vector3D ToEulerAngles () const;
    void ToAxisAngle (Vector3D &axis, real &angle) const;

    // Verificar unitario
    PHYS_NODISCARD PHYS_FORCE_INLINE bool IsUnit () const {
        return std::abs(SquareMagnitude() - static_cast<real>(1.0)) < PHYS_EPSILON;
    }

    PHYS_NODISCARD PHYS_FORCE_INLINE bool IsPure () const {
        return std::abs(w_) < PHYS_EPSILON;
    }

    // Operadores de comparación
    PHYS_NODISCARD bool operator == (const Quaternion &rhs) const;
    PHYS_NODISCARD bool operator != (const Quaternion &rhs) const;

    // Exponencial y logaritmo
    PHYS_NODISCARD Quaternion Exponential () const;
    PHYS_NODISCARD Quaternion Logarithm () const;

    // Metodos estaticos
    PHYS_NODISCARD static Quaternion Identity();
    PHYS_NODISCARD static Quaternion FromAxisAngle(const Vector3D &axis, real angle);
    PHYS_NODISCARD static Quaternion FromEulerAngles(real pitch, real yaw, real roll);
    PHYS_NODISCARD static Quaternion FromRotationMatrix(const Matrix3D &m);
};

PHYS_NODISCARD Quaternion operator * (real scalar, const Quaternion &q);
