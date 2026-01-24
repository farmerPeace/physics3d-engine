#pragma once
#include "physics/vector3d.hpp"
#include <type_traits>

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
    Vector4D &operator *= (const float &scalar);
    Vector4D &operator /= (const float &scalar);

    // Operadores multiplicacion y division con escalar
    Vector4D operator * (const float &scalar) const;
    Vector4D operator / (const float &scalar) const;

    // Operador de negacion
    Vector4D operator - () const;

    // Operadores de comparación
    bool operator == (const Vector4D &rhs) const;
    bool operator != (const Vector4D &rhs) const;

    // Producto punto
    float Dot4D (const Vector4D &rhs) const;

    // Producto cruz
    Vector4D ComponentWiseMultiply (const Vector4D &rhs) const;

    // Magnitud y magnitud al cuadrado
    float Magnitude () const;
    float SquareMagnitude () const;

    // Normalización
    Vector4D &Normalize ();
    Vector4D Normalized () const;

    // Distancia y distancia cuadrada
    static float Distance(const Vector4D &vecA, const Vector4D &vecB);
    static float SquareDistance(const Vector4D &vecA, const Vector4D &vecB);

    // Operadores de homogenizacion
    Vector4D &Homogenize ();
    Vector4D Homogenized () const;
    bool IsPoint () const;
    bool IsDirection () const;

    // Conversión a Vector3D
    Vector3D ToVector3D() const;
    Vector3D ToDirection3D() const;

    // Modificadores
    Vector4D &MakePoint ();
    Vector4D &MakeDirection ();

    // Obtener data 
    const float *Data () const;
    float *Data ();
};
