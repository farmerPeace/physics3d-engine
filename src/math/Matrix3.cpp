#include "physics/Matrix3.hpp"
#include <cmath>
#include <cstring>

Matrix3::Matrix3() {
    matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f;
    matrix[3] = 0.0f; matrix[4] = 1.0f; matrix[5] = 0.0f;
    matrix[6] = 0.0f; matrix[7] = 0.0f; matrix[8] = 1.0f;
}

Matrix3::Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
    matrix[0] = m00; matrix[1] = m01; matrix[2] = m02;
    matrix[3] = m10; matrix[4] = m11; matrix[5] = m12;
    matrix[6] = m20; matrix[7] = m21; matrix[8] = m22;
}

Matrix3::Matrix3(const float data[9]) {
    std::memcpy(matrix, data, 9 * sizeof(float));
}

Matrix3::Matrix3(Vector3D &col1, Vector3D &col2, Vector3D &col3) {
    // Primer columna
    matrix[0] = col1.x();
    matrix[1] = col1.y();
    matrix[2] = col1.z();

    // Cuarta columna
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
        matrix[0] + rhs.matrix[0], matrix[1] + rhs.matrix[1], matrix[2] + rhs.matrix[2],
        matrix[3] + rhs.matrix[3], matrix[4] + rhs.matrix[4], matrix[5] + rhs.matrix[5],
        matrix[6] + rhs.matrix[6], matrix[7] + rhs.matrix[7], matrix[8] + rhs.matrix[8]
    );
}

Matrix3 Matrix3::operator - (const Matrix3 &rhs) const {
    return Matrix3 (
        matrix[0] - rhs.matrix[0], matrix[1] - rhs.matrix[1], matrix[2] - rhs.matrix[2],
        matrix[3] - rhs.matrix[3], matrix[4] - rhs.matrix[4], matrix[5] - rhs.matrix[5],
        matrix[6] - rhs.matrix[6], matrix[7] - rhs.matrix[7], matrix[8] - rhs.matrix[8]
    );
}

Matrix3 Matrix3::operator * (float scalar) const {
    return Matrix3 (
        matrix[0] * scalar, matrix[1] * scalar, matrix[2] * scalar,
        matrix[3] * scalar, matrix[4] * scalar, matrix[5] * scalar,
        matrix[6] * scalar, matrix[7] * scalar, matrix[8] * scalar
    );
}

Matrix3 Matrix3::operator / (float scalar) const {
    const float EPSILON = 1e-6f;
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por cero");
    }

    float inv = 1.0f / scalar;

    return Matrix3 (
        matrix[0] * inv, matrix[1] * inv, matrix[2] * inv,
        matrix[3] * inv, matrix[4] * inv, matrix[5] * inv,
        matrix[6] * inv, matrix[7] * inv, matrix[8] * inv
    );
}

Matrix3 &Matrix3::operator += (const Matrix3 &rhs) {
    matrix[0] += rhs.matrix[0]; matrix[1] += rhs.matrix[1]; matrix[2] += rhs.matrix[2];
    matrix[3] += rhs.matrix[3]; matrix[4] += rhs.matrix[4]; matrix[5] += rhs.matrix[5];
    matrix[6] += rhs.matrix[6]; matrix[7] += rhs.matrix[7]; matrix[8] += rhs.matrix[8];

    return *this;
}

Matrix3 &Matrix3::operator -= (const Matrix3 &rhs) {
    matrix[0] -= rhs.matrix[0]; matrix[1] -= rhs.matrix[1]; matrix[2] -= rhs.matrix[2];
    matrix[3] -= rhs.matrix[3]; matrix[4] -= rhs.matrix[4]; matrix[5] -= rhs.matrix[5];
    matrix[6] -= rhs.matrix[6]; matrix[7] -= rhs.matrix[7]; matrix[8] -= rhs.matrix[8];

    return *this;
}

Matrix3 &Matrix3::operator *= (float scalar) {
    matrix[0] *= scalar; matrix[1] *= scalar; matrix[2] *= scalar;
    matrix[3] *= scalar; matrix[4] *= scalar; matrix[5] *= scalar;
    matrix[6] *= scalar; matrix[7] *= scalar; matrix[8] *= scalar;

    return *this;
}

Matrix3 &Matrix3::operator /= (float scalar) {
    const float EPSILON = 1e-6f;
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por cero");
    }

    float inv = 1.0f / scalar;

    matrix[0] *= inv; matrix[1] *= inv; matrix[2] *= inv;
    matrix[3] *= inv; matrix[4] *= inv; matrix[5] *= inv;
    matrix[6] *= inv; matrix[7] *= inv; matrix[8] *= inv;

    return *this;   
}

Matrix3 Matrix3::operator - () const {
    return Matrix3 (
        -matrix[0], -matrix[1], -matrix[2],
        -matrix[3], -matrix[4], -matrix[5],
        -matrix[6], -matrix[7], -matrix[8]
    );
}

bool Matrix3::operator == (const Matrix3 &rhs) const {
    return std::memcmp(matrix, rhs.matrix, sizeof(matrix)) == 0;
}

bool Matrix3::operator != (const Matrix3 &rhs) const {
    return !(*this == rhs);
}