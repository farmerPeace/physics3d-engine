#pragma once

#include <type_traits>
#include <stdexcept>


class Vector3D {
private:
    float x_, y_, z_;

public:
    // Constructores
    Vector3D();
    Vector3D(float x, float y, float z);

    // Getters y 
    float x() const;
    float y() const;
    float z() const;

    // Setters
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);  
    
    // Operadores de asignación (suma y resta)
    Vector3D &operator += (const Vector3D &rhs);
    Vector3D &operator -= (const Vector3D &rhs);

    // Operadores arimeticos (suma y resta)
    Vector3D operator + (const Vector3D& rhs) const;
    Vector3D operator - (const Vector3D& rhs) const;
    Vector3D operator - () const;

    // Multiplicación y división con escalar
    template <typename Scalar>
    Vector3D &operator *= (const Scalar &scalar);

    template <typename Scalar>
    Vector3D &operator /= (const Scalar &scalar);

    // Operadores de comparación
    bool operator == (const Vector3D &rhs) const;
    bool operator != (const Vector3D &rhs) const;

    // Acceso por indice
    float operator [] (int index) const;
    float &operator [] (int index);

    // Producto punto y producto cruz
    float Dot (const Vector3D &rhs) const;
    Vector3D Cross(const Vector3D &rhs) const;

    // Magnitud y magnitud al cuadrado
    float Magnitude() const;
    float SquareMagnitude() const;

    // Normalización
    Vector3D &Normalize ();
    Vector3D Normalized () const;

    // Distancia y distancia cuadrada
    static float Distance(const Vector3D &a, const Vector3D &b);
    static float SquaredDistance(const Vector3D &a, const Vector3D &b);
};

// Funciones globales
template <typename Scalar>
Vector3D operator * (const Scalar &scalar, Vector3D vect);

template <typename Scalar>
Vector3D operator * (Vector3D vect, const Scalar &scalar);

template <typename Scalar>
Vector3D operator / (Vector3D vect, const Scalar &scalar);

#include "Vector3D.inl"