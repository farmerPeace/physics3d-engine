#pragma once
#include "physics/math/vector3d.hpp"

#include "physics/core/config.hpp"
#include "physics/core/macros.hpp"

class Matrix3D {
private:
    real matrix[9];

public:
    Matrix3D();

    Matrix3D(real m00, real m01, real m02,
            real m10, real m11, real m12,
            real m20, real m21, real m22);
    
    explicit Matrix3D(const real data[9]);

    Matrix3D(const Vector3D &col1, const Vector3D &col2, const Vector3D &col3);

    // Matrices Especiales
    PHYS_NODISCARD static Matrix3D Identity();
    PHYS_NODISCARD static Matrix3D Zero();
    PHYS_NODISCARD static Matrix3D Scale(real sx, real sy, real sz);
    PHYS_NODISCARD static Matrix3D Scale(real uniformScale);
    PHYS_NODISCARD static Matrix3D RotationX(real angle);
    PHYS_NODISCARD static Matrix3D RotationY(real angle);
    PHYS_NODISCARD static Matrix3D RotationZ(real angle);
    PHYS_NODISCARD static Matrix3D Rotation(real angle, const Vector3D &axis);

    // Acceso por inidce
    PHYS_NODISCARD real operator () (int row, int col) const;
    real &operator () (int row, int col);

    // Acceso por vectores/columnas
    PHYS_NODISCARD Vector3D GetColumn(int col) const;
    void SetColumn(int col, const Vector3D &vector);

    // Acceso por vectores/fila
    PHYS_NODISCARD Vector3D GetRow(int row) const;
    void SetRow(int row, const Vector3D &vector);

    // Operaciones aritmeticas
    PHYS_NODISCARD Matrix3D operator + (const Matrix3D &rhs) const;
    PHYS_NODISCARD Matrix3D operator - (const Matrix3D &rhs) const;
    PHYS_NODISCARD Matrix3D operator * (real scalar) const;
    PHYS_NODISCARD Matrix3D operator / (real scalar) const;

    Matrix3D &operator += (const Matrix3D &rhs);
    Matrix3D &operator -= (const Matrix3D &rhs);
    Matrix3D &operator *= (real scalar);
    Matrix3D &operator /= (real scalar);

    PHYS_NODISCARD Matrix3D operator - () const;

    // Multiplicacion de matrices
    PHYS_NODISCARD Matrix3D operator * (const Matrix3D &rhs) const;
    Matrix3D &operator *= (const Matrix3D &rhs);

    // Multiplicación vector-matrix
    PHYS_NODISCARD Vector3D operator * (const Vector3D &vector) const;

    // Operador comparación
    PHYS_NODISCARD bool operator == (const Matrix3D &rhs) const;
    PHYS_NODISCARD bool operator != (const Matrix3D &rhs) const;

    // Transpuesta
    PHYS_NODISCARD Matrix3D Transposed() const;
    Matrix3D &Transpose();

    PHYS_NODISCARD real Determinant() const;

    PHYS_NODISCARD bool IsInvertible() const;
    PHYS_NODISCARD Matrix3D Inverted() const;
    Matrix3D &Invert();

    // Otras funciones
    PHYS_NODISCARD real Trace() const;

    PHYS_NODISCARD bool IsIdentity() const;
    PHYS_NODISCARD bool IsZero() const;
    PHYS_NODISCARD bool IsSymmetric() const;
    PHYS_NODISCARD bool IsOrthogonal() const;

    // Interpolación
    PHYS_NODISCARD static Matrix3D Lerp(const Matrix3D &a, const Matrix3D &b, real t);
};