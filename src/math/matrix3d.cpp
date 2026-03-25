#include "physics/math/matrix3d.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <assert.h>

Matrix3D::Matrix3D() {
    matrix[0] = 1.0f; matrix[3] = 0.0f; matrix[6] = 0.0f;
    matrix[1] = 0.0f; matrix[4] = 1.0f; matrix[7] = 0.0f;
    matrix[2] = 0.0f; matrix[5] = 0.0f; matrix[8] = 1.0f;
}

Matrix3D::Matrix3D(real m00, real m01, real m02, real m10, real m11, real m12, real m20, real m21, real m22) {
    matrix[0] = m00; matrix[3] = m01; matrix[6] = m02;
    matrix[1] = m10; matrix[4] = m11; matrix[7] = m12;
    matrix[2] = m20; matrix[5] = m21; matrix[8] = m22;
}

Matrix3D::Matrix3D(const real data[9]) {
    matrix[0] = data[0]; matrix[3] = data[1]; matrix[6] = data[2];
    matrix[1] = data[3]; matrix[4] = data[4]; matrix[7] = data[5];
    matrix[2] = data[6]; matrix[5] = data[7]; matrix[8] = data[8];
}

Matrix3D::Matrix3D(const Vector3D &col1, const Vector3D &col2, const Vector3D &col3) {
    // Primer columna
    matrix[0] = col1.x();
    matrix[1] = col1.y();
    matrix[2] = col1.z();

    // Segunda columna
    matrix[3] = col2.x();
    matrix[4] = col2.y();
    matrix[5] = col2.z();

    // Tercer columna
    matrix[6] = col3.x();
    matrix[7] = col3.y();
    matrix[8] = col3.z();
}

// Métodos estaticos de matrices especiales
Matrix3D Matrix3D::Identity() {
    Matrix3D matrix_identity (
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
        
    return matrix_identity;
}

Matrix3D Matrix3D::Zero() {
    Matrix3D matrix_zero (
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    );
        
    return matrix_zero;
}

// Métodos estaticos para escalares y rotación
Matrix3D Matrix3D::Scale(real sx, real sy, real sz) {
    return Matrix3D (
        sx, 0.0f, 0.0f,
        0.0f, sy, 0.0f,
        0.0f, 0.0f, sz
    );
}

Matrix3D Matrix3D::Scale(real uniformScale) {
    return Matrix3D (
        uniformScale, 0.0f, 0.0f,
        0.0f, uniformScale, 0.0f,
        0.0f, 0.0f, uniformScale
    );
}

// Métodos de rotación
Matrix3D Matrix3D::RotationX(real angle) {
    return Matrix3D (
        1.0f, 0.0f, 0.0f,
        0.0f, cos(angle), -sin(angle),
        0.0f, sin(angle), cos(angle)
    );
}

Matrix3D Matrix3D::RotationY(real angle) {
    return Matrix3D (
        cos(angle), 0.0f, sin(angle),
        0.0f, 1.0f, 0.0f,
        -sin(angle), 0.0f, cos(angle)
    );
}

Matrix3D Matrix3D::RotationZ(real angle) {
    return Matrix3D (
       cos(angle), -sin(angle), 0.0f,
       sin(angle), cos(angle), 0.0f,
       0.0f, 0.0f, 1.0f
    );
}

Matrix3D Matrix3D::Rotation(real angle, const Vector3D &axis) {
    Vector3D axis_norm = axis.Normalized();
    
    real c = cos(angle);
    real s = sin(angle);
    real t = 1 -  c;

    real x = axis_norm.x();
    real y = axis_norm.y();
    real z = axis_norm.z();

    return Matrix3D(
        t * x * x + c, t * x * y + s * z, t * x * z - s * y,  
        t * x * y - s * z, t * y * y + c, t * y * z + s * x, 
        t * x * z + s * y, t * y * z - s * x, t * z * z + c   
    );
}

// Acceso por indices
real Matrix3D::operator () (int row, int col) const {
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3D fuera de rango");
    }
    
    return matrix[col * 3 + row];
}

real &Matrix3D::operator () (int row, int col) {
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3D fuera de rango");
    }
    
    return matrix[col * 3 + row];
}

// Acceso por vectores/columnas
Vector3D Matrix3D::GetColumn(int col) const {
    if (col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3D fuera de rango");
    }
    return Vector3D(matrix[col * 3 + 0], matrix[col * 3 + 1], matrix[col * 3 + 2]);
}

void Matrix3D::SetColumn(int col, const Vector3D &vector) {
    if (col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3D fuera de rango");
    }

    matrix[col * 3 + 0] = vector.x();
    matrix[col * 3 + 1] = vector.y();
    matrix[col * 3 + 2] = vector.z();
}

// Acceso por vectores/filas
Vector3D Matrix3D::GetRow(int row) const {
    if (row < 0 || row > 2) {
        throw std::out_of_range("Indice Matrix3D fuera de rango");
    }
    return Vector3D(matrix[0 * 3 + row], matrix[1 * 3 + row], matrix[2 * 3 + row]);
}

void Matrix3D::SetRow(int row, const Vector3D &vector) {
    if (row < 0 || row > 2) {
        throw std::out_of_range("Indice Matrix3D fuera de rango");
    }

    matrix[0 * 3 + row] = vector.x();
    matrix[1 * 3 + row] = vector.y();
    matrix[2 * 3 + row] = vector.z();
}

// Operaciones arimeticas
Matrix3D Matrix3D::operator + (const Matrix3D &rhs) const {
    return Matrix3D (
        matrix[0] + rhs.matrix[0], matrix[3] + rhs.matrix[3], matrix[6] + rhs.matrix[6],
        matrix[1] + rhs.matrix[1], matrix[4] + rhs.matrix[4], matrix[7] + rhs.matrix[7],
        matrix[2] + rhs.matrix[2], matrix[5] + rhs.matrix[5], matrix[8] + rhs.matrix[8]
    );
}

Matrix3D Matrix3D::operator - (const Matrix3D &rhs) const {
    return Matrix3D (
        matrix[0] - rhs.matrix[0], matrix[3] - rhs.matrix[3], matrix[6] - rhs.matrix[6],
        matrix[1] - rhs.matrix[1], matrix[4] - rhs.matrix[4], matrix[7] - rhs.matrix[7],
        matrix[2] - rhs.matrix[2], matrix[5] - rhs.matrix[5], matrix[8] - rhs.matrix[8]
    );
}

Matrix3D Matrix3D::operator * (real scalar) const {
    return Matrix3D (
        matrix[0] * scalar, matrix[3] * scalar, matrix[6] * scalar,
        matrix[1] * scalar, matrix[4] * scalar, matrix[7] * scalar,
        matrix[2] * scalar, matrix[5] * scalar, matrix[8] * scalar
    );
}

Matrix3D Matrix3D::operator / (real scalar) const {
    const real EPSILON = 1e-6f;
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por cero");
    }

    real inv = 1.0f / scalar;

    return Matrix3D (
        matrix[0] * inv, matrix[3] * inv, matrix[6] * inv,
        matrix[1] * inv, matrix[4] * inv, matrix[7] * inv,
        matrix[2] * inv, matrix[5] * inv, matrix[8] * inv
    );
}

Matrix3D &Matrix3D::operator += (const Matrix3D &rhs) {
    matrix[0] += rhs.matrix[0]; matrix[3] += rhs.matrix[3]; matrix[6] += rhs.matrix[6];
    matrix[1] += rhs.matrix[1]; matrix[4] += rhs.matrix[4]; matrix[7] += rhs.matrix[7];
    matrix[2] += rhs.matrix[2]; matrix[5] += rhs.matrix[5]; matrix[8] += rhs.matrix[8];

    return *this;
}

Matrix3D &Matrix3D::operator -= (const Matrix3D &rhs) {
    matrix[0] -= rhs.matrix[0]; matrix[3] -= rhs.matrix[3]; matrix[6] -= rhs.matrix[6];
    matrix[1] -= rhs.matrix[1]; matrix[4] -= rhs.matrix[4]; matrix[7] -= rhs.matrix[7];
    matrix[2] -= rhs.matrix[2]; matrix[5] -= rhs.matrix[5]; matrix[8] -= rhs.matrix[8];

    return *this;
}

Matrix3D &Matrix3D::operator *= (real scalar) {
    matrix[0] *= scalar; matrix[3] *= scalar; matrix[6] *= scalar;
    matrix[1] *= scalar; matrix[4] *= scalar; matrix[7] *= scalar;
    matrix[2] *= scalar; matrix[5] *= scalar; matrix[8] *= scalar;

    return *this;
}

Matrix3D &Matrix3D::operator /= (real scalar) {
    const real EPSILON = 1e-6f;
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por cero");
    }

    real inv = 1.0f / scalar;

    matrix[0] *= inv; matrix[3] *= inv; matrix[6] *= inv;
    matrix[1] *= inv; matrix[4] *= inv; matrix[7] *= inv;
    matrix[2] *= inv; matrix[5] *= inv; matrix[8] *= inv;

    return *this;   
}

Matrix3D Matrix3D::operator - () const {
    return Matrix3D (
        -matrix[0], -matrix[3], -matrix[6],
        -matrix[1], -matrix[4], -matrix[7],
        -matrix[2], -matrix[5], -matrix[8]
    );
}

// Multiplicación de matrices
Matrix3D Matrix3D::operator*(const Matrix3D &rhs) const {
    Matrix3D result = Matrix3D::Zero();

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            real sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += matrix[k * 3 + row] * rhs.matrix[col * 3 + k];
            }
            result.matrix[col * 3 + row] = sum;
        }
    }

    return result;
}

Matrix3D &Matrix3D::operator*=(const Matrix3D &rhs) {
    // Guardar valores originales
    real temp[9];
    for (int i = 0; i < 9; i++) {
        temp[i] = matrix[i];
    }

    // Multiplicación correcta
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            real sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += temp[k * 3 + row] * rhs.matrix[col * 3 + k];
            }
            matrix[col * 3 + row] = sum; 
        }
    }

    return *this;
}

real Matrix3D::Trace() const {
    return matrix[0] + matrix[4] + matrix[8];
}

Vector3D Matrix3D::operator*(const Vector3D &vec) const {
    return Vector3D (
        matrix[0] * vec.x() + matrix[3] * vec.y() + matrix[6] * vec.z(),
        matrix[1] * vec.x() + matrix[4] * vec.y() + matrix[7] * vec.z(),
        matrix[2] * vec.x() + matrix[5] * vec.y() + matrix[8] * vec.z()
    );
}

bool Matrix3D::operator == (const Matrix3D &rhs) const {
    const real EPSILON = 1e-6f;
    for (int i = 0; i < 9; i++) {
        if (std::abs(matrix[i] - rhs.matrix[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool Matrix3D::operator != (const Matrix3D &rhs) const {
    return !(*this == rhs);
}

Matrix3D Matrix3D::Transposed () const {
    return Matrix3D(
        (*this)(0,0), (*this)(1,0), (*this)(2,0),
        (*this)(0,1), (*this)(1,1), (*this)(2,1),
        (*this)(0,2), (*this)(1,2), (*this)(2,2)
    );
}

Matrix3D &Matrix3D::Transpose () {
    std::swap(matrix[1], matrix[3]);
    std::swap(matrix[2], matrix[6]);
    std::swap(matrix[5], matrix[7]);

    return *this;
}

real Matrix3D::Determinant () const {
    return matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]) -
           matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6]) +
           matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
}

bool Matrix3D::IsInvertible () const {
    const real EPSILON = 1e-6f;
    return std::abs(Determinant()) > EPSILON;
}

Matrix3D Matrix3D::Inverted() const {
    
        const real EPSILON = 1e-6f;
    real det = Determinant();
    if (std::abs(det) < EPSILON)
        throw std::runtime_error("Matriz no invertible: el determinante es cero");
 
    real invDet = 1.0f / det;
 
    // Layout column-major. Alias para claridad:
    //   a = matrix[0]   b = matrix[3]   c = matrix[6]
    //   d = matrix[1]   e = matrix[4]   f = matrix[7]
    //   g = matrix[2]   h = matrix[5]   i = matrix[8]
    //
    // La inversa es (1/det) * adj(M), donde adj(M) = C^T
    // (C = matriz de cofactores).  result(row,col) = cofactor(col,row)/det
    // almacenado en result.matrix[col*3+row].
 
    Matrix3D result;
 
    // Columna 0 del resultado
    result.matrix[0] = (matrix[4]*matrix[8] - matrix[5]*matrix[7]) * invDet; // e*i - h*f
    result.matrix[1] = (matrix[7]*matrix[2] - matrix[1]*matrix[8]) * invDet; // f*g - d*i
    result.matrix[2] = (matrix[1]*matrix[5] - matrix[4]*matrix[2]) * invDet; // d*h - e*g
 
    // Columna 1 del resultado
    result.matrix[3] = (matrix[6]*matrix[5] - matrix[3]*matrix[8]) * invDet; // c*h - b*i
    result.matrix[4] = (matrix[0]*matrix[8] - matrix[6]*matrix[2]) * invDet; // a*i - c*g
    result.matrix[5] = (matrix[3]*matrix[2] - matrix[0]*matrix[5]) * invDet; // b*g - a*h
 
    // Columna 2 del resultado
    result.matrix[6] = (matrix[3]*matrix[7] - matrix[6]*matrix[4]) * invDet; // b*f - c*e
    result.matrix[7] = (matrix[6]*matrix[1] - matrix[0]*matrix[7]) * invDet; // c*d - a*f
    result.matrix[8] = (matrix[0]*matrix[4] - matrix[3]*matrix[1]) * invDet; // a*e - b*d
 
    return result;
}

Matrix3D& Matrix3D::Invert() {
    const real EPSILON = 1e-6f;
    real det = Determinant();
    if (std::abs(det) < EPSILON)
        throw std::runtime_error("Matriz no invertible: el determinante es cero");
 
    real invDet = 1.0f / det;
 
    // Guardar valores originales antes de sobreescribir
    const real a = matrix[0], b = matrix[3], c = matrix[6];
    const real d = matrix[1], e = matrix[4], f = matrix[7];
    const real g = matrix[2], h = matrix[5], i = matrix[8];
 
    // Columna 0
    matrix[0] = (e*i - h*f) * invDet;
    matrix[1] = (f*g - d*i) * invDet;
    matrix[2] = (d*h - e*g) * invDet;
 
    // Columna 1
    matrix[3] = (c*h - b*i) * invDet;
    matrix[4] = (a*i - c*g) * invDet;
    matrix[5] = (b*g - a*h) * invDet;
 
    // Columna 2
    matrix[6] = (b*f - c*e) * invDet;
    matrix[7] = (c*d - a*f) * invDet;
    matrix[8] = (a*e - b*d) * invDet;
 
    return *this;
}

bool Matrix3D::IsIdentity () const {
    const real EPSILON = 1e-6f;
    return (
        std::abs(matrix[0] - 1.0f) < EPSILON && std::abs(matrix[3]) < EPSILON && std::abs(matrix[6]) < EPSILON &&
        std::abs(matrix[1]) < EPSILON && std::abs(matrix[4] - 1.0f) < EPSILON && std::abs(matrix[7]) < EPSILON &&
        std::abs(matrix[2]) < EPSILON && std::abs(matrix[5]) < EPSILON && std::abs(matrix[8] - 1.0f) < EPSILON
    );
}

bool Matrix3D::IsZero () const {
    const real EPSILON = 1e-6f;
    for (int i = 0; i < 9; i++) {
        if (std::abs(matrix[i]) >= EPSILON) return false;
    }
    return true;
}

bool Matrix3D::IsSymmetric () const {
    const real EPSILON = 1e-6f;
    return (
        std::abs(matrix[1] - matrix[3]) < EPSILON &&
        std::abs(matrix[2] - matrix[6]) < EPSILON &&
        std::abs(matrix[5] - matrix[7]) < EPSILON
    );
}

bool Matrix3D::IsOrthogonal() const {
    const real EPSILON = 1e-6f;
    
    // Verificar que cada columna sea vector unitario
    for (int col = 0; col < 3; col++) {
        real dot = 0;
        for (int row = 0; row < 3; row++) {
            real val = matrix[col * 3 + row];
            dot += val * val;
        }
        if (std::abs(dot - 1.0f) > EPSILON) {
            return false;
        }
    }
    
    // Verificar que columnas sean ortogonales entre sí
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            real dot = 0;
            for (int row = 0; row < 3; row++) {
                dot += matrix[i * 3 + row] * matrix[j * 3 + row];
            }
            if (std::abs(dot) > EPSILON) {
                return false;
            }
        }
    }
    
    return true;
}

Matrix3D Matrix3D::Lerp(const Matrix3D &a, const Matrix3D &b, real t) {
    // Asegurar que t esté en [0, 1]
    t = std::max(0.0f, std::min(1.0f, t));
    
    // Interpolación lineal componente a componente
    Matrix3D result;
    
    for (int i = 0; i < 9; i++) {
        result.matrix[i] = a.matrix[i] + t * (b.matrix[i] - a.matrix[i]);
    }
    
    return result;
}