#pragma once
 
#include "physics/math/vector3d.hpp"
#include "physics/math/vector4d.hpp"
#include "physics/math/matrix3d.hpp"

#include "physics/core/config.hpp"
#include "physics/core/macros.hpp"
 
class Matrix4D {
private:
    // Almacenamiento en columna mayor (column-major), igual que Matrix3D.
    // Acceso: matrix[col * 4 + row]
    real matrix[16];
 
public:
    // ─────────────────────────────────────────────
    // Constructores
    // ─────────────────────────────────────────────
 
    // Identidad por defecto
    Matrix4D();
 
    // Parámetros en orden fila-mayor (misma convención que Matrix3D)
    Matrix4D(
        real m00, real m01, real m02, real m03,
        real m10, real m11, real m12, real m13,
        real m20, real m21, real m22, real m23,
        real m30, real m31, real m32, real m33
    );
 
    // Array de 16 floats interpretado en orden fila-mayor
    explicit Matrix4D(const real data[16]);
 
    // Desde Matrix3D — rota sin traslación (última fila/columna = identidad)
    explicit Matrix4D(const Matrix3D &rot);
 
    // Desde Matrix3D + vector traslación
    Matrix4D(const Matrix3D &rot, const Vector3D &translation);
 
    // ─────────────────────────────────────────────
    // Matrices especiales (estáticas)
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD static Matrix4D Identity();
    PHYS_NODISCARD static Matrix4D Zero();
 
    // Transformaciones básicas
    PHYS_NODISCARD static Matrix4D Translation(real tx, real ty, real tz);
    PHYS_NODISCARD static Matrix4D Translation(const Vector3D &t);
    PHYS_NODISCARD static Matrix4D Scale(real sx, real sy, real sz);
    PHYS_NODISCARD static Matrix4D Scale(real uniformScale);
 
    // Rotaciones elementales
    PHYS_NODISCARD static Matrix4D RotationX(real angle);
    PHYS_NODISCARD static Matrix4D RotationY(real angle);
    PHYS_NODISCARD static Matrix4D RotationZ(real angle);
    PHYS_NODISCARD static Matrix4D Rotation(real angle, const Vector3D &axis);
 
    // Compone traslación × rotación × escala en una sola llamada
    PHYS_NODISCARD static Matrix4D TRS(const Vector3D &translation,
                        const Matrix3D &rotation,
                        const Vector3D &scale);
 
    // Proyecciones (convención OpenGL: mano derecha, NDC [-1, 1])
    PHYS_NODISCARD static Matrix4D Perspective(real fovY, real aspect, real near, real far);
    PHYS_NODISCARD static Matrix4D Orthographic(real left, real right,
                                 real bottom, real top,
                                 real near,  real far);
 
    // Vista
    PHYS_NODISCARD static Matrix4D LookAt(const Vector3D &eye,
                           const Vector3D &target,
                           const Vector3D &up);
 
    // ─────────────────────────────────────────────
    // Acceso por índice
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD real  operator () (int row, int col) const;
    real &operator () (int row, int col);
 
    // Columnas y filas como Vector4D
    PHYS_NODISCARD Vector4D GetColumn(int col) const;
    void     SetColumn(int col, const Vector4D &v);
 
    PHYS_NODISCARD Vector4D GetRow(int row) const;
    void     SetRow(int row, const Vector4D &v);
 
    // Submatriz 3×3 superior-izquierda (rotación/escala)
    PHYS_NODISCARD Matrix3D GetUpper3x3() const;
    void     SetUpper3x3(const Matrix3D &m);
 
    // Componente de traslación (columna 3, filas 0-2)
    PHYS_NODISCARD Vector3D GetTranslation() const;
    void     SetTranslation(const Vector3D &t);
 
    // ─────────────────────────────────────────────
    // Aritmética
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD Matrix4D  operator +  (const Matrix4D &rhs) const;
    PHYS_NODISCARD Matrix4D  operator -  (const Matrix4D &rhs) const;
    PHYS_NODISCARD Matrix4D  operator *  (real scalar)         const;
    PHYS_NODISCARD Matrix4D  operator /  (real scalar)         const;
    PHYS_NODISCARD Matrix4D  operator -  ()                     const;
 
    Matrix4D &operator += (const Matrix4D &rhs);
    Matrix4D &operator -= (const Matrix4D &rhs);
    Matrix4D &operator *= (real scalar);
    Matrix4D &operator /= (real scalar);
 
    // Multiplicación de matrices
    PHYS_NODISCARD Matrix4D  operator *  (const Matrix4D &rhs) const;
    Matrix4D &operator *= (const Matrix4D &rhs);
 
    // Multiplicación por vector (transforma un Vector4D)
    PHYS_NODISCARD Vector4D operator * (const Vector4D &v) const;
 
    // ─────────────────────────────────────────────
    // Comparación
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD bool operator == (const Matrix4D &rhs) const;
    PHYS_NODISCARD bool operator != (const Matrix4D &rhs) const;
 
    // ─────────────────────────────────────────────
    // Transpuesta
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD Matrix4D  Transposed() const;
    Matrix4D &Transpose();
 
    // ─────────────────────────────────────────────
    // Determinante e inversa
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD real    Determinant()  const;
    PHYS_NODISCARD bool     IsInvertible() const;
    PHYS_NODISCARD Matrix4D Inverted()     const;
    Matrix4D &Invert();
 
    // ─────────────────────────────────────────────
    // Propiedades
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD real Trace()        const;
    PHYS_NODISCARD bool  IsIdentity()   const;
    PHYS_NODISCARD bool  IsZero()       const;
    PHYS_NODISCARD bool  IsOrthogonal() const;
 
    // ─────────────────────────────────────────────
    // Interpolación
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD static Matrix4D Lerp(const Matrix4D &a, const Matrix4D &b, real t);
 
    // ─────────────────────────────────────────────
    // Acceso a datos raw (interop con OpenGL, buffers)
    // ─────────────────────────────────────────────
 
    PHYS_NODISCARD const real *Data() const;
    real       *Data();
};
 
// Conmutativa: scalar * M
PHYS_NODISCARD Matrix4D operator * (real scalar, const Matrix4D &m);