#include "physics/Matrix3.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <assert.h>

Matrix3::Matrix3() {
    matrix[0] = 1.0f; matrix[3] = 0.0f; matrix[6] = 0.0f;
    matrix[1] = 0.0f; matrix[4] = 1.0f; matrix[7] = 0.0f;
    matrix[2] = 0.0f; matrix[5] = 0.0f; matrix[8] = 1.0f;
}

Matrix3::Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
    matrix[0] = m00; matrix[3] = m01; matrix[6] = m02;
    matrix[1] = m10; matrix[4] = m11; matrix[7] = m12;
    matrix[2] = m20; matrix[5] = m21; matrix[8] = m22;
}

Matrix3::Matrix3(const float data[9]) {
    matrix[0] = data[0]; matrix[3] = data[1]; matrix[6] = data[2];
    matrix[1] = data[3]; matrix[4] = data[4]; matrix[7] = data[5];
    matrix[2] = data[6]; matrix[5] = data[7]; matrix[8] = data[8];
}

Matrix3::Matrix3(const Vector3D &col1, const Vector3D &col2, const Vector3D &col3) {
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
Matrix3 Matrix3::Identity() {
    Matrix3 matrix_identity (
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
        
    return matrix_identity;
}

Matrix3 Matrix3::Zero() {
    Matrix3 matrix_zero (
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    );
        
    return matrix_zero;
}

// Métodos estaticos para escalares y rotación
Matrix3 Matrix3::Scale(float sx, float sy, float sz) {
    return Matrix3 (
        sx, 0.0f, 0.0f,
        0.0f, sy, 0.0f,
        0.0f, 0.0f, sz
    );
}

Matrix3 Matrix3::Scale(float uniformScale) {
    return Matrix3 (
        uniformScale, 0.0f, 0.0f,
        0.0f, uniformScale, 0.0f,
        0.0f, 0.0f, uniformScale
    );
}

// Métodos de rotación
Matrix3 Matrix3::RotationX(float angle) {
    return Matrix3 (
        1.0f, 0.0f, 0.0f,
        0.0f, cos(angle), -sin(angle),
        0.0f, sin(angle), cos(angle)
    );
}

Matrix3 Matrix3::RotationY(float angle) {
    return Matrix3 (
        cos(angle), 0.0f, sin(angle),
        0.0f, 1.0f, 0.0f,
        -sin(angle), 0.0f, cos(angle)
    );
}

Matrix3 Matrix3::RotationZ(float angle) {
    return Matrix3 (
       cos(angle), -sin(angle), 0.0f,
       sin(angle), cos(angle), 0.0f,
       0.0f, 0.0f, 1.0f
    );
}

Matrix3 Matrix3::Rotation(float angle, const Vector3D &axis) {
    Vector3D axis_norm = axis.Normalized();
    
    float c = cos(angle);
    float s = sin(angle);
    float t = 1 -  c;

    float x = axis_norm.x();
    float y = axis_norm.y();
    float z = axis_norm.z();

    return Matrix3(
        t * x * x + c, t * x * y + s * z, t * x * z - s * y,  
        t * x * y - s * z, t * y * y + c, t * y * z + s * x, 
        t * x * z + s * y, t * y * z - s * x, t * z * z + c   
    );
}

// Acceso por indices
float Matrix3::operator () (int row, int col) const {
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3 fuera de rango");
    }
    
    return matrix[col * 3 + row];
}

float &Matrix3::operator () (int row, int col) {
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3 fuera de rango");
    }
    
    return matrix[col * 3 + row];
}

// Acceso por vectores/columnas
Vector3D Matrix3::GetColumn(int col) const {
    if (col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3 fuera de rango");
    }
    return Vector3D(matrix[col * 3 + 0], matrix[col * 3 + 1], matrix[col * 3 + 2]);
}

void Matrix3::SetColumn(int col, const Vector3D &vector) {
    if (col < 0 || col > 2) {
        throw std::out_of_range("Indice Matrix3 fuera de rango");
    }

    matrix[col * 3 + 0] = vector.x();
    matrix[col * 3 + 1] = vector.y();
    matrix[col * 3 + 2] = vector.z();
}

// Acceso por vectores/filas
Vector3D Matrix3::GetRow(int row) const {
    if (row < 0 || row > 2) {
        throw std::out_of_range("Indice Matrix3 fuera de rango");
    }
    return Vector3D(matrix[0 * 3 + row], matrix[1 * 3 + row], matrix[2 * 3 + row]);
}

void Matrix3::SetRow(int row, const Vector3D &vector) {
    if (row < 0 || row > 2) {
        throw std::out_of_range("Indice Matrix3 fuera de rango");
    }

    matrix[0 * 3 + row] = vector.x();
    matrix[1 * 3 + row] = vector.y();
    matrix[2 * 3 + row] = vector.z();
}

// Operaciones arimeticas
Matrix3 Matrix3::operator + (const Matrix3 &rhs) const {
    return Matrix3 (
        matrix[0] + rhs.matrix[0], matrix[3] + rhs.matrix[3], matrix[6] + rhs.matrix[6],
        matrix[1] + rhs.matrix[1], matrix[4] + rhs.matrix[4], matrix[7] + rhs.matrix[7],
        matrix[2] + rhs.matrix[2], matrix[5] + rhs.matrix[5], matrix[8] + rhs.matrix[8]
    );
}

Matrix3 Matrix3::operator - (const Matrix3 &rhs) const {
    return Matrix3 (
        matrix[0] - rhs.matrix[0], matrix[3] - rhs.matrix[3], matrix[6] - rhs.matrix[6],
        matrix[1] - rhs.matrix[1], matrix[4] - rhs.matrix[4], matrix[7] - rhs.matrix[7],
        matrix[2] - rhs.matrix[2], matrix[5] - rhs.matrix[5], matrix[8] - rhs.matrix[8]
    );
}

Matrix3 Matrix3::operator * (float scalar) const {
    return Matrix3 (
        matrix[0] * scalar, matrix[3] * scalar, matrix[6] * scalar,
        matrix[1] * scalar, matrix[4] * scalar, matrix[7] * scalar,
        matrix[2] * scalar, matrix[5] * scalar, matrix[8] * scalar
    );
}

Matrix3 Matrix3::operator / (float scalar) const {
    const float EPSILON = 1e-6f;
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por cero");
    }

    float inv = 1.0f / scalar;

    return Matrix3 (
        matrix[0] * inv, matrix[3] * inv, matrix[6] * inv,
        matrix[1] * inv, matrix[4] * inv, matrix[7] * inv,
        matrix[2] * inv, matrix[5] * inv, matrix[8] * inv
    );
}

Matrix3 &Matrix3::operator += (const Matrix3 &rhs) {
    matrix[0] += rhs.matrix[0]; matrix[3] += rhs.matrix[3]; matrix[6] += rhs.matrix[6];
    matrix[1] += rhs.matrix[1]; matrix[4] += rhs.matrix[4]; matrix[7] += rhs.matrix[7];
    matrix[2] += rhs.matrix[2]; matrix[5] += rhs.matrix[5]; matrix[8] += rhs.matrix[8];

    return *this;
}

Matrix3 &Matrix3::operator -= (const Matrix3 &rhs) {
    matrix[0] -= rhs.matrix[0]; matrix[3] -= rhs.matrix[3]; matrix[6] -= rhs.matrix[6];
    matrix[1] -= rhs.matrix[1]; matrix[4] -= rhs.matrix[4]; matrix[7] -= rhs.matrix[7];
    matrix[2] -= rhs.matrix[2]; matrix[5] -= rhs.matrix[5]; matrix[8] -= rhs.matrix[8];

    return *this;
}

Matrix3 &Matrix3::operator *= (float scalar) {
    matrix[0] *= scalar; matrix[3] *= scalar; matrix[6] *= scalar;
    matrix[1] *= scalar; matrix[4] *= scalar; matrix[7] *= scalar;
    matrix[2] *= scalar; matrix[5] *= scalar; matrix[8] *= scalar;

    return *this;
}

Matrix3 &Matrix3::operator /= (float scalar) {
    const float EPSILON = 1e-6f;
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por cero");
    }

    float inv = 1.0f / scalar;

    matrix[0] *= inv; matrix[3] *= inv; matrix[6] *= inv;
    matrix[1] *= inv; matrix[4] *= inv; matrix[7] *= inv;
    matrix[2] *= inv; matrix[5] *= inv; matrix[8] *= inv;

    return *this;   
}

Matrix3 Matrix3::operator - () const {
    return Matrix3 (
        -matrix[0], -matrix[3], -matrix[6],
        -matrix[1], -matrix[4], -matrix[7],
        -matrix[2], -matrix[5], -matrix[8]
    );
}

// Multiplicación de matrices
Matrix3 Matrix3::operator*(const Matrix3 &rhs) const {
    Matrix3 result = Matrix3::Zero();

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            float sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += matrix[k * 3 + row] * rhs.matrix[col * 3 + k];
            }
            result.matrix[col * 3 + row] = sum;
        }
    }

    return result;
}

Matrix3 &Matrix3::operator*=(const Matrix3 &rhs) {
    // Guardar valores originales
    float temp[9];
    for (int i = 0; i < 9; i++) {
        temp[i] = matrix[i];
    }

    // Multiplicación correcta
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            float sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += temp[k * 3 + row] * rhs.matrix[col * 3 + k];
            }
            matrix[col * 3 + row] = sum; 
        }
    }

    return *this;
}

bool Matrix3::operator == (const Matrix3 &rhs) const {
    const float EPSILON = 1e-6f;
    for (int i = 0; i < 9; i++) {
        if (std::abs(matrix[i] - rhs.matrix[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool Matrix3::operator != (const Matrix3 &rhs) const {
    return !(*this == rhs);
}

Matrix3 Matrix3::Transposed () const {
    return Matrix3(
        (*this)(0,0), (*this)(1,0), (*this)(2,0),
        (*this)(0,1), (*this)(1,1), (*this)(2,1),
        (*this)(0,2), (*this)(1,2), (*this)(2,2)
    );
}

Matrix3 &Matrix3::Transpose () {
    std::swap(matrix[1], matrix[3]);
    std::swap(matrix[2], matrix[6]);
    std::swap(matrix[5], matrix[7]);

    return *this;
}

float Matrix3::Determinant () const {
    return matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]) -
           matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6]) +
           matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
}

bool Matrix3::IsInvertible () const {
    const float EPSILON = 1e-6f;
    return std::abs(Determinant()) > EPSILON;
}

Matrix3 Matrix3::Inverted() const {
    
    const float EPSILON = 1e-6f;
    float det = Determinant();
    if (std::abs(det) < EPSILON) {
        std::cerr << "Matriz no invertible" << std::endl;
        assert(false);
    }
    
    float invDet = 1.0f / det;
    
    Matrix3 result;
    
    // Calcular matriz de cofactores
    result.matrix[0] = (matrix[4]*matrix[8] - matrix[5]*matrix[7]) * invDet;
    result.matrix[3] = (matrix[2]*matrix[7] - matrix[1]*matrix[8]) * invDet;
    result.matrix[6] = (matrix[1]*matrix[5] - matrix[2]*matrix[4]) * invDet;
    
    result.matrix[1] = (matrix[5]*matrix[6] - matrix[3]*matrix[8]) * invDet;
    result.matrix[4] = (matrix[0]*matrix[8] - matrix[2]*matrix[6]) * invDet;
    result.matrix[7] = (matrix[2]*matrix[3] - matrix[0]*matrix[5]) * invDet;
    
    result.matrix[2] = (matrix[3]*matrix[7] - matrix[4]*matrix[6]) * invDet;
    result.matrix[5] = (matrix[1]*matrix[6] - matrix[0]*matrix[7]) * invDet;
    result.matrix[8] = (matrix[0]*matrix[4] - matrix[1]*matrix[3]) * invDet;
    
    return result;
}

Matrix3& Matrix3::Invert() {
    const float EPSILON = 1e-6f;
    float det = Determinant();
    if (std::abs(det) < EPSILON) {
        std::cerr << "Matriz no invertible" << std::endl;
        assert(false);
    }
    
    float invDet = 1.0f / det;
    
    // Guardar valores originales
    float m00 = matrix[0], m01 = matrix[1], m02 = matrix[2];
    float m10 = matrix[3], m11 = matrix[4], m12 = matrix[5];
    float m20 = matrix[6], m21 = matrix[7], m22 = matrix[8];
    
    // Calcular directamente sobre la matriz
    matrix[0] = (m11*m22 - m12*m21) * invDet;
    matrix[3] = (m02*m21 - m01*m22) * invDet;
    matrix[6] = (m01*m12 - m02*m11) * invDet;
    
    matrix[1] = (m12*m20 - m10*m22) * invDet;
    matrix[4] = (m00*m22 - m02*m20) * invDet;
    matrix[7] = (m02*m10 - m00*m12) * invDet;
    
    matrix[2] = (m10*m21 - m11*m20) * invDet;
    matrix[5] = (m01*m20 - m00*m21) * invDet;
    matrix[8] = (m00*m11 - m01*m10) * invDet;
    
    return *this;
}

bool Matrix3::IsIdentity () const {
    return (
        matrix[0] == 1 && matrix[3] == 0 && matrix[6] == 0 &&
        matrix[1] == 0 && matrix[4] == 1 && matrix[7] == 0 &&
        matrix[2] == 0 && matrix[5] == 0 && matrix[8] == 1
    );
}

bool Matrix3::IsZero () const {
    return (
        matrix[0] == 0 && matrix[3] == 0 && matrix[6] == 0 &&
        matrix[1] == 0 && matrix[4] == 0 && matrix[7] == 0 &&
        matrix[2] == 0 && matrix[5] == 0 && matrix[8] == 0
    );
}

bool Matrix3::IsSymmetric () const {
    return (
        matrix[1] == matrix[3] &&
        matrix[2] == matrix[6] &&
        matrix[5] == matrix[7]
    );
}

bool Matrix3::IsOrthogonal() const {
    const float EPSILON = 1e-6f;
    
    // Verificar que cada columna sea vector unitario
    for (int col = 0; col < 3; col++) {
        float dot = 0;
        for (int row = 0; row < 3; row++) {
            float val = matrix[col * 3 + row];
            dot += val * val;
        }
        if (std::abs(dot - 1.0f) > EPSILON) {
            return false;
        }
    }
    
    // Verificar que columnas sean ortogonales entre sí
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            float dot = 0;
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

Matrix3 Matrix3::Lerp(const Matrix3 &a, const Matrix3 &b, float t) {
    // Asegurar que t esté en [0, 1]
    t = std::max(0.0f, std::min(1.0f, t));
    
    // Interpolación lineal componente a componente
    Matrix3 result;
    
    for (int i = 0; i < 9; i++) {
        result.matrix[i] = a.matrix[i] + t * (b.matrix[i] - a.matrix[i]);
    }
    
    return result;
}