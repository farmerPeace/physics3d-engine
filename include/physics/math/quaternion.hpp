#pragma once

#include "physics/math/vector3d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/matrix4d.hpp"

class Quaternion {
private:
    float w_, x_, y_, z_;

public:
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float w, const Vector3D &vect);

    // Data
    const float *Data() const;
    float *Data();

    // Acceso por indices
    float operator [] (int index) const;
    float &operator [] (int index);

    // Operadores suma y resta in place entre cuaterniones
    Quaternion &operator += (const Quaternion &rhs);
    Quaternion &operator -= (const Quaternion &rhs);

    // Operadores suma y resta entre cuaterniones
    Quaternion operator + (const Quaternion &rhs) const;
    Quaternion operator - (const Quaternion &rhs) const;
    Quaternion operator - () const;

    // Operadores multiplicacion y division con escalar in place
    Quaternion &operator *= (const float &scalar);
    Quaternion &operator /= (const float &scalar);
    
    // Operadores multiplicación y división con escalar
    Quaternion operator * (const float &scalar) const;
    Quaternion operator / (const float &scalar) const;

    // Multiplicación de cuaterniones
    Quaternion &operator *= (const Quaternion &rhs);
    Quaternion operator * (const Quaternion &rhs) const;
    
    // Producto punto
    float Dot(const Quaternion &rhs) const;

    // Conjugado e inverso
    Quaternion Conjugate () const;
    Quaternion Inverse () const;

    // Magnitud y magnitud al cuadrado
    float Magnitude () const;
    float SquareMagnitude () const;

    // Normalización
    Quaternion &Normalize ();
    Quaternion Normalized () const;

    // Rotar vector por un cuaternion
    Vector3D Rotate (const Vector3D &vec) const;

    // Interpolaciones
    static Quaternion Slerp (const Quaternion &a, const Quaternion &b, float t);
    static Quaternion Nlerp (const Quaternion &a, const Quaternion &b, float t);

    // Derivada temporal
    Quaternion Derivative (const Vector3D &angularVelocity) const;
 
    // Conversiones
    Matrix3D ToRotationMatrix () const;
    Matrix4D ToTransformMatrix () const;
    Vector3D ToEulerAngles () const;
    void ToAxisAngle (Vector3D &axis, float &angle) const;

    // Verificar unitario
    bool IsUnit () const;
    bool IsPure () const;

    // Operadores de comparación
    bool operator == (const Quaternion &rhs) const;
    bool operator != (const Quaternion &rhs) const;

    // Exponencial y logaritmo
    Quaternion Exponential () const;
    Quaternion Logarithm () const;

    // Metodos estaticos
    static Quaternion Identity();
    static Quaternion FromAxisAngle(const Vector3D &axis, float angle);
    static Quaternion FromEulerAngles(float pitch, float yaw, float roll);
    static Quaternion FromRotationMatrix(const Matrix3D &m);
};

Quaternion operator * (float scalar, const Quaternion &q);
