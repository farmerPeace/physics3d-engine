#include <cmath>
#include <cstring>
#include <stdexcept>
 
#include "physics/math/matrix4d.hpp"
#include "physics/math/vector3d.hpp"
#include "physics/math/vector4d.hpp"
#include "physics/math/matrix3d.hpp"
 
#define EPSILON 1e-6f
 
// ─────────────────────────────────────────────
// Helpers internos
// ─────────────────────────────────────────────
 
// Determinante de una matriz 3×3 dada por sus 9 elementos fila-mayor
static float det3(
    float a00, float a01, float a02,
    float a10, float a11, float a12,
    float a20, float a21, float a22)
{
    return a00 * (a11 * a22 - a12 * a21)
         - a01 * (a10 * a22 - a12 * a20)
         + a02 * (a10 * a21 - a11 * a20);
}
 
// ─────────────────────────────────────────────
// Constructores
// ─────────────────────────────────────────────
 
Matrix4D::Matrix4D() {
    // Identidad en column-major: matrix[col*4+row]
    matrix[0]  = 1.0f; matrix[1]  = 0.0f; matrix[2]  = 0.0f; matrix[3]  = 0.0f;
    matrix[4]  = 0.0f; matrix[5]  = 1.0f; matrix[6]  = 0.0f; matrix[7]  = 0.0f;
    matrix[8]  = 0.0f; matrix[9]  = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
}
 
Matrix4D::Matrix4D(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33)
{
    // Entrada fila-mayor → almacenamiento columna-mayor
    matrix[0]  = m00; matrix[4]  = m01; matrix[8]  = m02; matrix[12] = m03;
    matrix[1]  = m10; matrix[5]  = m11; matrix[9]  = m12; matrix[13] = m13;
    matrix[2]  = m20; matrix[6]  = m21; matrix[10] = m22; matrix[14] = m23;
    matrix[3]  = m30; matrix[7]  = m31; matrix[11] = m32; matrix[15] = m33;
}
 
Matrix4D::Matrix4D(const float data[16]) {
    // Igual que el constructor de 16 floats: interpreta data en fila-mayor
    matrix[0]  = data[0];  matrix[4]  = data[1];  matrix[8]  = data[2];  matrix[12] = data[3];
    matrix[1]  = data[4];  matrix[5]  = data[5];  matrix[9]  = data[6];  matrix[13] = data[7];
    matrix[2]  = data[8];  matrix[6]  = data[9];  matrix[10] = data[10]; matrix[14] = data[11];
    matrix[3]  = data[12]; matrix[7]  = data[13]; matrix[11] = data[14]; matrix[15] = data[15];
}
 
Matrix4D::Matrix4D(const Matrix3D &rot) {
    // Copia la submatriz 3×3 y rellena el resto como identidad
    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            matrix[col * 4 + row] = rot(row, col);
        }
        matrix[col * 4 + 3] = 0.0f; // fila 3 de las primeras 3 columnas
    }
    // Cuarta columna
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}
 
Matrix4D::Matrix4D(const Matrix3D &rot, const Vector3D &translation) {
    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            matrix[col * 4 + row] = rot(row, col);
        }
        matrix[col * 4 + 3] = 0.0f;
    }
    matrix[12] = translation.x();
    matrix[13] = translation.y();
    matrix[14] = translation.z();
    matrix[15] = 1.0f;
}
 
// ─────────────────────────────────────────────
// Matrices especiales
// ─────────────────────────────────────────────
 
Matrix4D Matrix4D::Identity() {
    return Matrix4D();
}
 
Matrix4D Matrix4D::Zero() {
    Matrix4D m;
    for (int i = 0; i < 16; ++i) m.matrix[i] = 0.0f;
    return m;
}
 
Matrix4D Matrix4D::Translation(float tx, float ty, float tz) {
    Matrix4D m;           // parte de rotación = identidad
    m.matrix[12] = tx;
    m.matrix[13] = ty;
    m.matrix[14] = tz;
    return m;
}
 
Matrix4D Matrix4D::Translation(const Vector3D &t) {
    return Translation(t.x(), t.y(), t.z());
}
 
Matrix4D Matrix4D::Scale(float sx, float sy, float sz) {
    Matrix4D m = Zero();
    m.matrix[0]  = sx;
    m.matrix[5]  = sy;
    m.matrix[10] = sz;
    m.matrix[15] = 1.0f;
    return m;
}
 
Matrix4D Matrix4D::Scale(float u) {
    return Scale(u, u, u);
}
 
Matrix4D Matrix4D::RotationX(float angle) {
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return Matrix4D(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,    c,   -s, 0.0f,
        0.0f,    s,    c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
 
Matrix4D Matrix4D::RotationY(float angle) {
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return Matrix4D(
           c, 0.0f,    s, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
          -s, 0.0f,    c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
 
Matrix4D Matrix4D::RotationZ(float angle) {
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return Matrix4D(
           c,   -s, 0.0f, 0.0f,
           s,    c, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
 
Matrix4D Matrix4D::Rotation(float angle, const Vector3D &axis) {
    // Embebe la Matrix3D de Rodrigues en la 4×4
    Matrix3D rot3 = Matrix3D::Rotation(angle, axis);
    return Matrix4D(rot3);
}
 
Matrix4D Matrix4D::TRS(const Vector3D &t, const Matrix3D &r, const Vector3D &s) {
    // Resultado = T * R * S  (primero escala, luego rota, luego traslada)
    Matrix4D result(r);
    // Aplicar escala a las primeras 3 columnas
    for (int col = 0; col < 3; ++col) {
        const float scale_factor = (col == 0) ? s.x() : (col == 1) ? s.y() : s.z();
        result.matrix[col * 4 + 0] *= scale_factor;
        result.matrix[col * 4 + 1] *= scale_factor;
        result.matrix[col * 4 + 2] *= scale_factor;
    }
    // Columna de traslación
    result.matrix[12] = t.x();
    result.matrix[13] = t.y();
    result.matrix[14] = t.z();
    result.matrix[15] = 1.0f;
    return result;
}
 
Matrix4D Matrix4D::Perspective(float fovY, float aspect, float near, float far) {
    if (std::abs(aspect) < EPSILON) {
        throw std::domain_error("Perspective: aspect ratio no puede ser cero");
    }
    if (std::abs(far - near) < EPSILON) {
        throw std::domain_error("Perspective: near y far no pueden ser iguales");
    }
 
    const float tan_half = std::tan(fovY * 0.5f);
    const float range    = far - near;
 
    Matrix4D m = Zero();
    m.matrix[0]  =  1.0f / (aspect * tan_half);  // (0,0)
    m.matrix[5]  =  1.0f / tan_half;             // (1,1)
    m.matrix[10] = -(far + near) / range;         // (2,2)
    m.matrix[11] = -1.0f;                         // (3,2) — w_clip = -z_eye
    m.matrix[14] = -(2.0f * far * near) / range;  // (2,3)
    // m.matrix[15] = 0 — ya está en cero
    return m;
}
 
Matrix4D Matrix4D::Orthographic(
    float left, float right,
    float bottom, float top,
    float near, float far)
{
    if (std::abs(right - left)   < EPSILON ||
        std::abs(top   - bottom) < EPSILON ||
        std::abs(far   - near)   < EPSILON)
    {
        throw std::domain_error("Orthographic: dimensiones degeneradas (division por cero)");
    }
 
    const float rl = right - left;
    const float tb = top   - bottom;
    const float fn = far   - near;
 
    Matrix4D m = Zero();
    m.matrix[0]  =  2.0f / rl;                    // (0,0)
    m.matrix[5]  =  2.0f / tb;                    // (1,1)
    m.matrix[10] = -2.0f / fn;                    // (2,2)
    m.matrix[12] = -(right + left)   / rl;        // (0,3)
    m.matrix[13] = -(top   + bottom) / tb;        // (1,3)
    m.matrix[14] = -(far   + near)   / fn;        // (2,3)
    m.matrix[15] =  1.0f;                         // (3,3)
    return m;
}
 
Matrix4D Matrix4D::LookAt(const Vector3D &eye, const Vector3D &target, const Vector3D &up) {
    const Vector3D f = (target - eye).Normalized();   // forward
    const Vector3D r = f.Cross3D(up).Normalized();    // right
    const Vector3D u = r.Cross3D(f);                  // recomputed up (ortogonal)
 
    // Matriz de vista: rotación transpuesta + traslación negada
    return Matrix4D(
         r.x(),  r.y(),  r.z(), -r.Dot3D(eye),
         u.x(),  u.y(),  u.z(), -u.Dot3D(eye),
        -f.x(), -f.y(), -f.z(),  f.Dot3D(eye),
         0.0f,   0.0f,   0.0f,   1.0f
    );
}
 
// ─────────────────────────────────────────────
// Acceso por índice
// ─────────────────────────────────────────────
 
float Matrix4D::operator () (int row, int col) const {
    if (row < 0 || row > 3 || col < 0 || col > 3) {
        throw std::out_of_range("Indice Matrix4D fuera de rango");
    }
    return matrix[col * 4 + row];
}
 
float &Matrix4D::operator () (int row, int col) {
    if (row < 0 || row > 3 || col < 0 || col > 3) {
        throw std::out_of_range("Indice Matrix4D fuera de rango");
    }
    return matrix[col * 4 + row];
}
 
Vector4D Matrix4D::GetColumn(int col) const {
    if (col < 0 || col > 3) {
        throw std::out_of_range("Indice Matrix4D fuera de rango");
    }
    const int base = col * 4;
    return Vector4D(matrix[base], matrix[base + 1], matrix[base + 2], matrix[base + 3]);
}
 
void Matrix4D::SetColumn(int col, const Vector4D &v) {
    if (col < 0 || col > 3) {
        throw std::out_of_range("Indice Matrix4D fuera de rango");
    }
    const int base = col * 4;
    matrix[base]     = v.x();
    matrix[base + 1] = v.y();
    matrix[base + 2] = v.z();
    matrix[base + 3] = v.w();
}
 
Vector4D Matrix4D::GetRow(int row) const {
    if (row < 0 || row > 3) {
        throw std::out_of_range("Indice Matrix4D fuera de rango");
    }
    return Vector4D(
        matrix[0 * 4 + row],
        matrix[1 * 4 + row],
        matrix[2 * 4 + row],
        matrix[3 * 4 + row]
    );
}
 
void Matrix4D::SetRow(int row, const Vector4D &v) {
    if (row < 0 || row > 3) {
        throw std::out_of_range("Indice Matrix4D fuera de rango");
    }
    matrix[0 * 4 + row] = v.x();
    matrix[1 * 4 + row] = v.y();
    matrix[2 * 4 + row] = v.z();
    matrix[3 * 4 + row] = v.w();
}
 
Matrix3D Matrix4D::GetUpper3x3() const {
    return Matrix3D(
        (*this)(0,0), (*this)(0,1), (*this)(0,2),
        (*this)(1,0), (*this)(1,1), (*this)(1,2),
        (*this)(2,0), (*this)(2,1), (*this)(2,2)
    );
}
 
void Matrix4D::SetUpper3x3(const Matrix3D &m) {
    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            matrix[col * 4 + row] = m(row, col);
        }
    }
}
 
Vector3D Matrix4D::GetTranslation() const {
    return Vector3D(matrix[12], matrix[13], matrix[14]);
}
 
void Matrix4D::SetTranslation(const Vector3D &t) {
    matrix[12] = t.x();
    matrix[13] = t.y();
    matrix[14] = t.z();
}
 
// ─────────────────────────────────────────────
// Aritmética
// ─────────────────────────────────────────────
 
Matrix4D Matrix4D::operator + (const Matrix4D &rhs) const {
    Matrix4D result;
    for (int i = 0; i < 16; ++i) result.matrix[i] = matrix[i] + rhs.matrix[i];
    return result;
}
 
Matrix4D Matrix4D::operator - (const Matrix4D &rhs) const {
    Matrix4D result;
    for (int i = 0; i < 16; ++i) result.matrix[i] = matrix[i] - rhs.matrix[i];
    return result;
}
 
Matrix4D Matrix4D::operator * (float scalar) const {
    Matrix4D result;
    for (int i = 0; i < 16; ++i) result.matrix[i] = matrix[i] * scalar;
    return result;
}
 
Matrix4D Matrix4D::operator / (float scalar) const {
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por (casi) cero en Matrix4D");
    }
    const float inv = 1.0f / scalar;
    Matrix4D result;
    for (int i = 0; i < 16; ++i) result.matrix[i] = matrix[i] * inv;
    return result;
}
 
Matrix4D Matrix4D::operator - () const {
    Matrix4D result;
    for (int i = 0; i < 16; ++i) result.matrix[i] = -matrix[i];
    return result;
}
 
Matrix4D &Matrix4D::operator += (const Matrix4D &rhs) {
    for (int i = 0; i < 16; ++i) matrix[i] += rhs.matrix[i];
    return *this;
}
 
Matrix4D &Matrix4D::operator -= (const Matrix4D &rhs) {
    for (int i = 0; i < 16; ++i) matrix[i] -= rhs.matrix[i];
    return *this;
}
 
Matrix4D &Matrix4D::operator *= (float scalar) {
    for (int i = 0; i < 16; ++i) matrix[i] *= scalar;
    return *this;
}
 
Matrix4D &Matrix4D::operator /= (float scalar) {
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por (casi) cero en Matrix4D");
    }
    const float inv = 1.0f / scalar;
    for (int i = 0; i < 16; ++i) matrix[i] *= inv;
    return *this;
}
 
// Multiplicación de matrices
Matrix4D Matrix4D::operator * (const Matrix4D &rhs) const {
    Matrix4D result = Zero();
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += matrix[k * 4 + row] * rhs.matrix[col * 4 + k];
            }
            result.matrix[col * 4 + row] = sum;
        }
    }
    return result;
}
 
Matrix4D &Matrix4D::operator *= (const Matrix4D &rhs) {
    float temp[16];
    for (int i = 0; i < 16; ++i) temp[i] = matrix[i];
 
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += temp[k * 4 + row] * rhs.matrix[col * 4 + k];
            }
            matrix[col * 4 + row] = sum;
        }
    }
    return *this;
}
 
// Multiplicación por Vector4D
Vector4D Matrix4D::operator * (const Vector4D &v) const {
    return Vector4D(
        matrix[0] * v.x() + matrix[4] * v.y() + matrix[8]  * v.z() + matrix[12] * v.w(),
        matrix[1] * v.x() + matrix[5] * v.y() + matrix[9]  * v.z() + matrix[13] * v.w(),
        matrix[2] * v.x() + matrix[6] * v.y() + matrix[10] * v.z() + matrix[14] * v.w(),
        matrix[3] * v.x() + matrix[7] * v.y() + matrix[11] * v.z() + matrix[15] * v.w()
    );
}
 
// Conmutativa global
Matrix4D operator * (float scalar, const Matrix4D &m) {
    return m * scalar;
}
 
// ─────────────────────────────────────────────
// Comparación
// ─────────────────────────────────────────────
 
bool Matrix4D::operator == (const Matrix4D &rhs) const {
    for (int i = 0; i < 16; ++i) {
        if (std::abs(matrix[i] - rhs.matrix[i]) > EPSILON) return false;
    }
    return true;
}
 
bool Matrix4D::operator != (const Matrix4D &rhs) const {
    return !(*this == rhs);
}
 
// ─────────────────────────────────────────────
// Transpuesta
// ─────────────────────────────────────────────
 
Matrix4D Matrix4D::Transposed() const {
    return Matrix4D(
        (*this)(0,0), (*this)(1,0), (*this)(2,0), (*this)(3,0),
        (*this)(0,1), (*this)(1,1), (*this)(2,1), (*this)(3,1),
        (*this)(0,2), (*this)(1,2), (*this)(2,2), (*this)(3,2),
        (*this)(0,3), (*this)(1,3), (*this)(2,3), (*this)(3,3)
    );
}
 
Matrix4D &Matrix4D::Transpose() {
    std::swap(matrix[1],  matrix[4]);
    std::swap(matrix[2],  matrix[8]);
    std::swap(matrix[3],  matrix[12]);
    std::swap(matrix[6],  matrix[9]);
    std::swap(matrix[7],  matrix[13]);
    std::swap(matrix[11], matrix[14]);
    return *this;
}
 
// ─────────────────────────────────────────────
// Determinante e inversa
// ─────────────────────────────────────────────
 
float Matrix4D::Determinant() const {
    // Elementos por nombre: mRC = elemento en fila R, columna C
    // matrix[col*4+row]
    const float m00 = matrix[0],  m10 = matrix[1],  m20 = matrix[2],  m30 = matrix[3];
    const float m01 = matrix[4],  m11 = matrix[5],  m21 = matrix[6],  m31 = matrix[7];
    const float m02 = matrix[8],  m12 = matrix[9],  m22 = matrix[10], m32 = matrix[11];
    const float m03 = matrix[12], m13 = matrix[13], m23 = matrix[14], m33 = matrix[15];
 
    // Expansión por cofactores a lo largo de la primera fila
    const float c00 = det3(m11, m12, m13,
                            m21, m22, m23,
                            m31, m32, m33);
 
    const float c01 = det3(m10, m12, m13,
                            m20, m22, m23,
                            m30, m32, m33);
 
    const float c02 = det3(m10, m11, m13,
                            m20, m21, m23,
                            m30, m31, m33);
 
    const float c03 = det3(m10, m11, m12,
                            m20, m21, m22,
                            m30, m31, m32);
 
    return m00 * c00 - m01 * c01 + m02 * c02 - m03 * c03;
}
 
bool Matrix4D::IsInvertible() const {
    return std::abs(Determinant()) > EPSILON;
}
 
Matrix4D Matrix4D::Inverted() const {
    const float det = Determinant();
    if (std::abs(det) < EPSILON) {
        throw std::runtime_error("Matrix4D no invertible: el determinante es cero");
    }
 
    const float inv_det = 1.0f / det;
 
    // Extraer los 16 elementos para legibilidad
    const float m00 = matrix[0],  m10 = matrix[1],  m20 = matrix[2],  m30 = matrix[3];
    const float m01 = matrix[4],  m11 = matrix[5],  m21 = matrix[6],  m31 = matrix[7];
    const float m02 = matrix[8],  m12 = matrix[9],  m22 = matrix[10], m32 = matrix[11];
    const float m03 = matrix[12], m13 = matrix[13], m23 = matrix[14], m33 = matrix[15];
 
    // Cofactores C(row, col) — la adjugada es la transpuesta de la matriz de cofactores
    // result(row, col) = (-1)^(row+col) * minor(col, row) / det
    Matrix4D result = Zero();
 
    result.matrix[0]  = inv_det * det3(m11,m12,m13, m21,m22,m23, m31,m32,m33);
    result.matrix[1]  = inv_det * -det3(m10,m12,m13, m20,m22,m23, m30,m32,m33);
    result.matrix[2]  = inv_det * det3(m10,m11,m13, m20,m21,m23, m30,m31,m33);
    result.matrix[3]  = inv_det * -det3(m10,m11,m12, m20,m21,m22, m30,m31,m32);
 
    result.matrix[4]  = inv_det * -det3(m01,m02,m03, m21,m22,m23, m31,m32,m33);
    result.matrix[5]  = inv_det * det3(m00,m02,m03, m20,m22,m23, m30,m32,m33);
    result.matrix[6]  = inv_det * -det3(m00,m01,m03, m20,m21,m23, m30,m31,m33);
    result.matrix[7]  = inv_det * det3(m00,m01,m02, m20,m21,m22, m30,m31,m32);
 
    result.matrix[8]  = inv_det * det3(m01,m02,m03, m11,m12,m13, m31,m32,m33);
    result.matrix[9]  = inv_det * -det3(m00,m02,m03, m10,m12,m13, m30,m32,m33);
    result.matrix[10] = inv_det * det3(m00,m01,m03, m10,m11,m13, m30,m31,m33);
    result.matrix[11] = inv_det * -det3(m00,m01,m02, m10,m11,m12, m30,m31,m32);
 
    result.matrix[12] = inv_det * -det3(m01,m02,m03, m11,m12,m13, m21,m22,m23);
    result.matrix[13] = inv_det * det3(m00,m02,m03, m10,m12,m13, m20,m22,m23);
    result.matrix[14] = inv_det * -det3(m00,m01,m03, m10,m11,m13, m20,m21,m23);
    result.matrix[15] = inv_det * det3(m00,m01,m02, m10,m11,m12, m20,m21,m22);
 
    return result;
}
 
Matrix4D &Matrix4D::Invert() {
    *this = Inverted();
    return *this;
}
 
// ─────────────────────────────────────────────
// Propiedades
// ─────────────────────────────────────────────
 
float Matrix4D::Trace() const {
    return matrix[0] + matrix[5] + matrix[10] + matrix[15];
}
 
bool Matrix4D::IsIdentity() const {
    const Matrix4D id;
    return *this == id;
}
 
bool Matrix4D::IsZero() const {
    for (int i = 0; i < 16; ++i) {
        if (std::abs(matrix[i]) >= EPSILON) return false;
    }
    return true;
}
 
bool Matrix4D::IsOrthogonal() const {
    // Una matriz 4×4 es ortogonal si M * M^T = I
    // Verificamos solo la submatriz 3×3 superior (la parte de rotación/escala)
    for (int col = 0; col < 3; ++col) {
        // Cada columna 3D debe ser unitaria
        float dot = 0.0f;
        for (int row = 0; row < 3; ++row) {
            const float v = matrix[col * 4 + row];
            dot += v * v;
        }
        if (std::abs(dot - 1.0f) > EPSILON) return false;
    }
 
    // Columnas 3D deben ser mutuamente ortogonales
    for (int i = 0; i < 3; ++i) {
        for (int j = i + 1; j < 3; ++j) {
            float dot = 0.0f;
            for (int row = 0; row < 3; ++row) {
                dot += matrix[i * 4 + row] * matrix[j * 4 + row];
            }
            if (std::abs(dot) > EPSILON) return false;
        }
    }
 
    return true;
}
 
// ─────────────────────────────────────────────
// Interpolación
// ─────────────────────────────────────────────
 
Matrix4D Matrix4D::Lerp(const Matrix4D &a, const Matrix4D &b, float t) {
    t = std::max(0.0f, std::min(1.0f, t));
    Matrix4D result;
    for (int i = 0; i < 16; ++i) {
        result.matrix[i] = a.matrix[i] + t * (b.matrix[i] - a.matrix[i]);
    }
    return result;
}
 
// ─────────────────────────────────────────────
// Acceso a datos raw
// ─────────────────────────────────────────────
 
const float *Matrix4D::Data() const {
    return matrix;
}
 
float *Matrix4D::Data() {
    return matrix;
}