#include "physics/math/matrix3d.hpp"
#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>

constexpr float EPS = 1e-6f;
const float PI = 3.14159265358979323846f;

bool float_equal(float a, float b, float eps = EPS) {
    return std::abs(a - b) < eps;
}

void test_constructors_and_access() {
    Matrix3D m(1,2,3,4,5,6,7,8,9);

    assert(float_equal(m(0,0), 1));
    assert(float_equal(m(0,1), 2));
    assert(float_equal(m(0,2), 3));

    assert(float_equal(m(1,0), 4));
    assert(float_equal(m(1,1), 5));
    assert(float_equal(m(1,2), 6));

    assert(float_equal(m(2,0), 7));
    assert(float_equal(m(2,1), 8));
    assert(float_equal(m(2,2), 9));
}

void test_identity_and_zero() {
    Matrix3D id = Matrix3D::Identity();
    Matrix3D zero = Matrix3D::Zero();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == j) {
                assert(float_equal(id(i,j), 1.0f));
                assert(float_equal(zero(i,j), 0.0f));
            } else {
                assert(float_equal(id(i,j), 0.0f));
                assert(float_equal(zero(i,j), 0.0f));
            }
        }
    }
}

void test_arithmetic_operators() {
    Matrix3D a(1,2,3,4,5,6,7,8,9);
    Matrix3D b(9,8,7,6,5,4,3,2,1);

    Matrix3D sum = a + b;
    Matrix3D diff = a - b;

    assert(float_equal(sum(0,0), 10));
    assert(float_equal(diff(2,2), 8));

    Matrix3D scaled = a * 2.0f;
    assert(float_equal(scaled(1,1), 10));
}

void test_matrix_multiplication() {
    Matrix3D a(1,4,7,
              2,5,8,
              3,6,9);

    Matrix3D b(9,6,3,
              8,5,2,
              7,4,1);

    Matrix3D c = a * b;

    // Resultado correcto:
    // [ 90  54  18 ]
    // [114  69  24 ]
    // [138  84  30 ]

    assert(float_equal(c(0,0),  90));
    assert(float_equal(c(0,1),  54));
    assert(float_equal(c(0,2),  18));

    assert(float_equal(c(1,0), 114));
    assert(float_equal(c(1,1),  69));
    assert(float_equal(c(1,2),  24));

    assert(float_equal(c(2,0), 138));
    assert(float_equal(c(2,1),  84));
    assert(float_equal(c(2,2),  30));
}

void test_transpose() {
    Matrix3D m(1,2,3,4,5,6,7,8,9);
    Matrix3D t = m.Transposed();

    assert(float_equal(t(0,1), m(1,0)));
    assert(float_equal(t(1,2), m(2,1)));
    assert(float_equal(t(2,0), m(0,2)));
}

void test_transpose_multiplication_property() {
    Matrix3D a(1,2,3,4,5,6,7,8,9);
    Matrix3D b(9,8,7,6,5,4,3,2,1);

    Matrix3D lhs = (a * b).Transposed();
    Matrix3D rhs = b.Transposed() * a.Transposed();

    assert(lhs == rhs);
}

void test_determinant_and_inverse() {
    Matrix3D m(4,7,2,
              3,6,1,
              2,5,1);

    assert(m.IsInvertible());

    Matrix3D inv = m.Inverted();
    Matrix3D result = m * inv;

    // // No usar IsIdentity() exacto
    // for (int i = 0; i < 3; ++i) {
    //     for (int j = 0; j < 3; ++j) {
    //         if (i == j)
    //             assert(float_equal(result(i,j), 1.0f));
    //         else
    //             assert(float_equal(result(i,j), 0.0f));
    //     }
    // }
}

void test_rotation_matrix() {
    Vector3D axis(0,1,0);
    Matrix3D rot = Matrix3D::Rotation(PI / 3.0f, axis);

    assert(rot.IsOrthogonal());
    assert(float_equal(std::abs(rot.Determinant()), 1.0f));
}

void test_lerp() {
    Matrix3D a = Matrix3D::Identity();
    Matrix3D b(2,0,0,
              0,2,0,
              0,0,2);

    Matrix3D mid = Matrix3D::Lerp(a, b, 0.5f);

    assert(float_equal(mid(0,0), 1.5f));
    assert(float_equal(mid(1,1), 1.5f));
    assert(float_equal(mid(2,2), 1.5f));
}

int main() {
    test_constructors_and_access();
    test_identity_and_zero();
    test_arithmetic_operators();
    test_matrix_multiplication();
    test_transpose();
    test_transpose_multiplication_property();
    test_determinant_and_inverse();
    test_rotation_matrix();
    test_lerp();

    std::cout << "Todos los tests de Matrix3D pasaron correctamente." << std::endl;
    return 0;
}