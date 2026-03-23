#pragma once
 
#include "physics/math/vector3d.hpp"
#include "physics/math/vector4d.hpp"
#include "physics/math/matrix3d.hpp"
 
class Matrix4D {
private:
    // Almacenamiento en columna mayor (column-major), igual que Matrix3D.
    // Acceso: matrix[col * 4 + row]
    float matrix[16];
 
public:
    // ─────────────────────────────────────────────
    // Constructores
    // ─────────────────────────────────────────────
 
    // Identidad por defecto
    Matrix4D();
 
    // Parámetros en orden fila-mayor (misma convención que Matrix3D)
    Matrix4D(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    );
 
    // Array de 16 floats interpretado en orden fila-mayor
    explicit Matrix4D(const float data[16]);
 
    // Desde Matrix3D — rota sin traslación (última fila/columna = identidad)
    explicit Matrix4D(const Matrix3D &rot);
 
    // Desde Matrix3D + vector traslación
    Matrix4D(const Matrix3D &rot, const Vector3D &translation);
 
    // ─────────────────────────────────────────────
    // Matrices especiales (estáticas)
    // ─────────────────────────────────────────────
 
    static Matrix4D Identity();
    static Matrix4D Zero();
 
    // Transformaciones básicas
    static Matrix4D Translation(float tx, float ty, float tz);
    static Matrix4D Translation(const Vector3D &t);
    static Matrix4D Scale(float sx, float sy, float sz);
    static Matrix4D Scale(float uniformScale);
 
    // Rotaciones elementales
    static Matrix4D RotationX(float angle);
    static Matrix4D RotationY(float angle);
    static Matrix4D RotationZ(float angle);
    static Matrix4D Rotation(float angle, const Vector3D &axis);
 
    // Compone traslación × rotación × escala en una sola llamada
    static Matrix4D TRS(const Vector3D &translation,
                        const Matrix3D &rotation,
                        const Vector3D &scale);
 
    // Proyecciones (convención OpenGL: mano derecha, NDC [-1, 1])
    static Matrix4D Perspective(float fovY, float aspect, float near, float far);
    static Matrix4D Orthographic(float left, float right,
                                 float bottom, float top,
                                 float near,  float far);
 
    // Vista
    static Matrix4D LookAt(const Vector3D &eye,
                           const Vector3D &target,
                           const Vector3D &up);
 
    // ─────────────────────────────────────────────
    // Acceso por índice
    // ─────────────────────────────────────────────
 
    float  operator () (int row, int col) const;
    float &operator () (int row, int col);
 
    // Columnas y filas como Vector4D
    Vector4D GetColumn(int col) const;
    void     SetColumn(int col, const Vector4D &v);
 
    Vector4D GetRow(int row) const;
    void     SetRow(int row, const Vector4D &v);
 
    // Submatriz 3×3 superior-izquierda (rotación/escala)
    Matrix3D GetUpper3x3() const;
    void     SetUpper3x3(const Matrix3D &m);
 
    // Componente de traslación (columna 3, filas 0-2)
    Vector3D GetTranslation() const;
    void     SetTranslation(const Vector3D &t);
 
    // ─────────────────────────────────────────────
    // Aritmética
    // ─────────────────────────────────────────────
 
    Matrix4D  operator +  (const Matrix4D &rhs) const;
    Matrix4D  operator -  (const Matrix4D &rhs) const;
    Matrix4D  operator *  (float scalar)         const;
    Matrix4D  operator /  (float scalar)         const;
    Matrix4D  operator -  ()                     const;
 
    Matrix4D &operator += (const Matrix4D &rhs);
    Matrix4D &operator -= (const Matrix4D &rhs);
    Matrix4D &operator *= (float scalar);
    Matrix4D &operator /= (float scalar);
 
    // Multiplicación de matrices
    Matrix4D  operator *  (const Matrix4D &rhs) const;
    Matrix4D &operator *= (const Matrix4D &rhs);
 
    // Multiplicación por vector (transforma un Vector4D)
    Vector4D operator * (const Vector4D &v) const;
 
    // ─────────────────────────────────────────────
    // Comparación
    // ─────────────────────────────────────────────
 
    bool operator == (const Matrix4D &rhs) const;
    bool operator != (const Matrix4D &rhs) const;
 
    // ─────────────────────────────────────────────
    // Transpuesta
    // ─────────────────────────────────────────────
 
    Matrix4D  Transposed() const;
    Matrix4D &Transpose();
 
    // ─────────────────────────────────────────────
    // Determinante e inversa
    // ─────────────────────────────────────────────
 
    float    Determinant()  const;
    bool     IsInvertible() const;
    Matrix4D Inverted()     const;
    Matrix4D &Invert();
 
    // ─────────────────────────────────────────────
    // Propiedades
    // ─────────────────────────────────────────────
 
    float Trace()        const;
    bool  IsIdentity()   const;
    bool  IsZero()       const;
    bool  IsOrthogonal() const;
 
    // ─────────────────────────────────────────────
    // Interpolación
    // ─────────────────────────────────────────────
 
    static Matrix4D Lerp(const Matrix4D &a, const Matrix4D &b, float t);
 
    // ─────────────────────────────────────────────
    // Acceso a datos raw (interop con OpenGL, buffers)
    // ─────────────────────────────────────────────
 
    const float *Data() const;
    float       *Data();
};
 
// Conmutativa: scalar * M
Matrix4D operator * (float scalar, const Matrix4D &m);