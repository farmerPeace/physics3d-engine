#pragma once
#include "vector3d.hpp"

class Matrix3D {
private:
    float matrix[9];

public:
    Matrix3D();

    Matrix3D(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);
    
    explicit Matrix3D(const float data[9]);

    Matrix3D(const Vector3D &col1, const Vector3D &col2, const Vector3D &col3);

    // Matrices Especiales
    static Matrix3D Identity();
    static Matrix3D Zero();
    static Matrix3D Scale(float sx, float sy, float sz);
    static Matrix3D Scale(float uniformScale);
    static Matrix3D RotationX(float angle);
    static Matrix3D RotationY(float angle);
    static Matrix3D RotationZ(float angle);
    static Matrix3D Rotation(float angle, const Vector3D &axis);

    // Acceso por inidce
    float operator () (int row, int col) const;
    float &operator () (int row, int col);

    // Acceso por vectores/columnas
    Vector3D GetColumn(int col) const;
    void SetColumn(int col, const Vector3D &vector);

    // Acceso por vectores/fila
    Vector3D GetRow(int row) const;
    void SetRow(int row, const Vector3D &vector);

    // Operaciones aritmeticas
    Matrix3D operator + (const Matrix3D &rhs) const;
    Matrix3D operator - (const Matrix3D &rhs) const;
    Matrix3D operator * (float scalar) const;
    Matrix3D operator / (float scalar) const;

    Matrix3D &operator += (const Matrix3D &rhs);
    Matrix3D &operator -= (const Matrix3D &rhs);
    Matrix3D &operator *= (float scalar);
    Matrix3D &operator /= (float scalar);

    Matrix3D operator - () const;

    // Multiplicacion de matrices
    Matrix3D operator * (const Matrix3D &rhs) const;
    Matrix3D &operator *= (const Matrix3D &rhs);

    // Multiplicación vector-matrix
    Vector3D operator * (const Vector3D &vector) const;

    // Operador comparación
    bool operator == (const Matrix3D &rhs) const;
    bool operator != (const Matrix3D &rhs) const;

    // Transpuesta
    Matrix3D Transposed() const;
    Matrix3D &Transpose();

    float Determinant() const;

    bool IsInvertible() const;
    Matrix3D Inverted() const;
    Matrix3D &Invert();

    // Otras funciones
    float Trace() const;

    bool IsIdentity() const;
    bool IsZero() const;
    bool IsSymmetric() const;
    bool IsOrthogonal() const;

    // Interpolación
    static Matrix3D Lerp(const Matrix3D &a, const Matrix3D &b, float t);
};