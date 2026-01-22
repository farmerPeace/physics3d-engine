#pragma once
#include "Vector3D.hpp"

class Matrix3 {
private:
    float matrix[9];

public:
    Matrix3();

    Matrix3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);
    
    explicit Matrix3(const float data[9]);

    Matrix3(Vector3D &col1, Vector3D &col2, Vector3D &col3);

    // Matrices Especiales
    static Matrix3 Identity();
    static Matrix3 Zero();
    static Matrix3 Scale(float sx, float sy, float sz);
    static Matrix3 Scale(float uniformScale);
    static Matrix3 RotationX(float angle);
    static Matrix3 RotationY(float angle);
    static Matrix3 RotationZ(float angle);
    static Matrix3 Rotation(float angle, const Vector3D &axis);

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
    Matrix3 operator + (const Matrix3 &rhs) const;
    Matrix3 operator - (const Matrix3 &rhs) const;
    Matrix3 operator * (float scalar) const;
    Matrix3 operator / (float scalar) const;

    Matrix3 &operator += (const Matrix3 &rhs);
    Matrix3 &operator -= (const Matrix3 &rhs);
    Matrix3 &operator *= (float scalar);
    Matrix3 &operator /= (float scalar);

    Matrix3 operator - () const;

    // Multiplicacion de matrices
    Matrix3 operator * (const Matrix3 &rhs) const;
    Matrix3 &operator *= (const Matrix3 &rhs);

    // Multiplicación vector-matrix
    Vector3D operator * (const Vector3D &vector) const;

    // Operador comparación
    bool operator == (const Matrix3 &rhs) const;
    bool operator != (const Matrix3 &rhs) const;

    // Transpuesta
    Matrix3 Transposed() const;
    Matrix3 &Transpose();

    float Determinant() const;

    bool IsInvertible() const;
    Matrix3 Inverted() const;
    Matrix3 Invert();

    // Otras funciones
    float Trace() const;

    bool IsIdentity() const;
    bool IsZero() const;
    bool IsSymmetric() const;
    bool IsOrthogonal() const;

    // Interpolación
    static Matrix3 Lerp(const Matrix3 &a, const Matrix3 &b, float t);
};