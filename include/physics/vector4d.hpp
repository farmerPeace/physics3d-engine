#pragma once
#include "physics/vector3d.hpp"

class Vector4D {
private:
    float x_, y_, z_, w_;

public:
    // Constructores
    Vector4D();
    Vector4D(float x, float y, float z, float w);
    Vector4D(const float arr[4]);
    
    explicit Vector4D(const Vector3D &vect);
    Vector4D(const Vector3D &vect, float w);

    // Getters
    float x() const;
    float y() const;
    float z() const;
    float w() const;

    // Setters
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);
    void set_w(float w);

    // Acceso por indices []
    float operator [] (int index) const;
    float &operator [] (int index);

    // Operadores suma y resta in place entre vectores
    Vector4D &operator += (const Vector4D &rhs);
    Vector4D &operator -= (const Vector4D &rhs);
    
    // Operadores suma y resta entre vectores
    Vector4D operator + (const Vector4D &rhs) const;
    Vector4D operator - (const Vector4D &rhs) const;

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

    // Operador de negacion
    Vector4D operator - () const;

    // Operadores de comparación
    bool operator == (const Vector4D &rhs) const;
    bool operator != (const Vector4D &rhs) const;

    // Producto punto
    float Dot (const Vector3D &rhs) const;

    // Producto cruz
    Vector4D Cross (const Vector4D &rhs) const;

    // Magnitud y magnitud al cuadrado
    float Magnitude () const;
    float SquareMagnitude () const;

    // Normalización
    Vector4D &Normalize ();
    Vector4D Normalized () const;

    // Distancia y distancia cuadrada
    static float Distance(const Vector3D &vecA, const Vector4D &vecB);
    static float SquareDistance(const Vector3D &vecA, const Vector4D &vecB);

    // Conversión a Vector3D
    Vector3D ToVector3D() const;
    Vector3D ToDirection3D() const;

    // Operadores de homogenizacion
    Vector4D &Homogenize ();
    Vector4D Homogenized () const;
    bool IsPoint () const;
    bool IsDirection () const;

    // Modificadores
    Vector4D &MakePoint ();
    Vector4D &MakeDirection ();

    // Obtener data 
    const float *Data () const;
    float *Data ();
};
