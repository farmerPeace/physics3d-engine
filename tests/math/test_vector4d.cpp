// test_vector4d.cpp
#include <cassert>
#include <cmath>
#include <iostream>
#include "physics/math/vector4d.hpp"
#include "physics/math/vector3d.hpp"

const float TEST_EPSILON = 1e-5f;

bool float_equal(float a, float b, float epsilon = TEST_EPSILON) {
    return std::abs(a - b) <= epsilon;
}

void test_constructors() {
    std::cout << "Testing constructors...\n";
    
    // Constructor por defecto
    Vector4D v1;
    assert(float_equal(v1.x(), 0));
    assert(float_equal(v1.y(), 0));
    assert(float_equal(v1.z(), 0));
    assert(float_equal(v1.w(), 0));
    
    // Constructor con valores
    Vector4D v2(1.5f, 2.5f, 3.5f, 4.5f);
    assert(float_equal(v2.x(), 1.5f));
    assert(float_equal(v2.y(), 2.5f));
    assert(float_equal(v2.z(), 3.5f));
    assert(float_equal(v2.w(), 4.5f));
    
    // Constructor desde Vector3D
    Vector3D v3d(1, 2, 3);
    Vector4D v3(v3d);
    assert(float_equal(v3.x(), 1));
    assert(float_equal(v3.y(), 2));
    assert(float_equal(v3.z(), 3));
    assert(float_equal(v3.w(), 1));
    
    // Constructor desde Vector3D con w
    Vector4D v4(v3d, 0.5f);
    assert(float_equal(v4.x(), 1));
    assert(float_equal(v4.y(), 2));
    assert(float_equal(v4.z(), 3));
    assert(float_equal(v4.w(), 0.5f));
    
    std::cout << "Constructors: PASSED\n";
}

void test_accessors() {
    std::cout << "Testing accessors...\n";
    
    Vector4D v;
    
    // Setters y getters
    v.set_x(1.5f);
    v.set_y(2.5f);
    v.set_z(3.5f);
    v.set_w(4.5f);
    
    assert(float_equal(v.x(), 1.5f));
    assert(float_equal(v.y(), 2.5f));
    assert(float_equal(v.z(), 3.5f));
    assert(float_equal(v.w(), 4.5f));
    
    // Operador []
    assert(float_equal(v[0], 1.5f));
    assert(float_equal(v[1], 2.5f));
    assert(float_equal(v[2], 3.5f));
    assert(float_equal(v[3], 4.5f));
    
    // Modificación con []
    v[0] = 10;
    v[1] = 20;
    v[2] = 30;
    v[3] = 40;
    
    assert(float_equal(v[0], 10));
    assert(float_equal(v[1], 20));
    assert(float_equal(v[2], 30));
    assert(float_equal(v[3], 40));
    
    std::cout << "Accessors: PASSED\n";
}

void test_arithmetic() {
    std::cout << "Testing arithmetic operations...\n";
    
    Vector4D v1(1, 2, 3, 4);
    Vector4D v2(5, 6, 7, 8);
    
    // Suma
    Vector4D sum = v1 + v2;
    assert(float_equal(sum.x(), 6));
    assert(float_equal(sum.y(), 8));
    assert(float_equal(sum.z(), 10));
    assert(float_equal(sum.w(), 12));
    
    // Resta
    Vector4D diff = v2 - v1;
    assert(float_equal(diff.x(), 4));
    assert(float_equal(diff.y(), 4));
    assert(float_equal(diff.z(), 4));
    assert(float_equal(diff.w(), 4));
    
    // Negativo
    Vector4D neg = -v1;
    assert(float_equal(neg.x(), -1));
    assert(float_equal(neg.y(), -2));
    assert(float_equal(neg.z(), -3));
    assert(float_equal(neg.w(), -4));
    
    // += y -=
    Vector4D v3 = v1;
    v3 += v2;
    assert(v3 == sum);
    
    v3 = v2;
    v3 -= v1;
    assert(v3 == diff);
    
    std::cout << "Arithmetic: PASSED\n";
}

void test_scalar_operations() {
    std::cout << "Testing scalar operations...\n";
    
    Vector4D v(1, 2, 3, 4);
    float scalar = 2.0f;
    
    // Multiplicación por escalar
    Vector4D mult = v * scalar;
    assert(float_equal(mult.x(), 2));
    assert(float_equal(mult.y(), 4));
    assert(float_equal(mult.z(), 6));
    assert(float_equal(mult.w(), 8));
    
    // División por escalar
    Vector4D div = mult / scalar;
    assert(float_equal(div.x(), v.x()));
    assert(float_equal(div.y(), v.y()));
    assert(float_equal(div.z(), v.z()));
    assert(float_equal(div.w(), v.w()));
    
    // *= y /=
    Vector4D v2 = v;
    v2 *= scalar;
    assert(v2 == mult);
    
    v2 /= scalar;
    assert(v2 == v);
    
    std::cout << "Scalar operations: PASSED\n";
}

void test_comparisons() {
    std::cout << "Testing comparisons...\n";
    
    Vector4D v1(1, 2, 3, 4);
    Vector4D v2(1, 2, 3, 4);
    Vector4D v3(1.000001f, 2.000001f, 3.000001f, 4.000001f);
    Vector4D v4(5, 6, 7, 8);
    
    // Igualdad exacta
    assert(v1 == v2);
    assert(!(v1 == v4));
    
    // Igualdad con tolerancia
    assert(v1 == v3);  // Dentro de EPSILON
    
    // Desigualdad
    assert(v1 != v4);
    assert(!(v1 != v2));
    
    std::cout << "Comparisons: PASSED\n";
}

void test_homogeneous_operations() {
    std::cout << "Testing homogeneous operations...\n";
    
    // Homogenize con w=2
    Vector4D v1(2, 4, 6, 2);
    Vector4D h1 = v1.Homogenized();
    assert(float_equal(h1.x(), 1));
    assert(float_equal(h1.y(), 2));
    assert(float_equal(h1.z(), 3));
    assert(float_equal(h1.w(), 1));
    
    // Homogenize con w=0 (dirección)
    Vector4D v2(1, 2, 3, 0);
    Vector4D h2 = v2.Homogenized();
    assert(float_equal(h2.x(), 1));
    assert(float_equal(h2.y(), 2));
    assert(float_equal(h2.z(), 3));
    assert(float_equal(h2.w(), 0));
    
    // IsPoint e IsDirection
    Vector4D point(1, 2, 3, 1);
    Vector4D dir(1, 2, 3, 0);
    Vector4D other(1, 2, 3, 0.5f);
    
    assert(point.IsPoint() == true);
    assert(point.IsDirection() == false);
    
    assert(dir.IsPoint() == false);
    assert(dir.IsDirection() == true);
    
    // ToVector3D y ToDirection3D
    Vector3D p3d = point.ToVector3D();
    assert(float_equal(p3d.x(), 1));
    assert(float_equal(p3d.y(), 2));
    assert(float_equal(p3d.z(), 3));
    
    Vector3D d3d = dir.ToDirection3D();
    assert(float_equal(d3d.x(), 1));
    assert(float_equal(d3d.y(), 2));
    assert(float_equal(d3d.z(), 3));
    
    std::cout << "Homogeneous operations: PASSED\n";
}

void test_dot_product() {
    std::cout << "Testing dot product...\n";
    
    Vector4D v1(1, 2, 3, 4);
    Vector4D v2(5, 6, 7, 8);
    
    float dot = v1.Dot4D(v2);
    float expected = 1*5 + 2*6 + 3*7 + 4*8;
    assert(float_equal(dot, expected));
    
    std::cout << "Dot product: PASSED\n";
}

void test_normalization() {
    std::cout << "Testing normalization...\n";
    
    // Vector dirección
    Vector4D dir(3, 0, 4, 0);
    Vector4D norm = dir.Normalized();
    
    float mag = std::sqrt(3*3 + 4*4);
    assert(float_equal(norm.x(), 3/mag));
    assert(float_equal(norm.y(), 0));
    assert(float_equal(norm.z(), 4/mag));
    assert(float_equal(norm.w(), 0));
    
    // Magnitud de dirección
    assert(float_equal(dir.Magnitude(), mag));
    assert(float_equal(dir.SquareMagnitude(), mag*mag));
    
    std::cout << "Normalization: PASSED\n";
}

void test_exceptions() {
    std::cout << "Testing exceptions...\n";
    
    Vector4D point(1, 2, 3, 1);
    Vector4D dir(1, 2, 3, 0);
    
    // División por (casi) cero
    Vector4D v(1, 2, 3, 4);
    bool caught = false;
    try {
        PHYS_UNUSED(v / 0.0f);
    } catch (const std::domain_error&) {
        caught = true;
    }
    assert(caught);
    
    // ToVector3D con dirección
    caught = false;
    try {
        PHYS_UNUSED(dir.ToVector3D());
    } catch (const std::domain_error&) {
        caught = true;
    }
    assert(caught);
    
    std::cout << "Exceptions: PASSED\n";
}

void test_distance() {
    std::cout << "Testing distance...\n";
    
    Vector4D p1(2, 4, 6, 2);  // (1,2,3) homogenizado
    Vector4D p2(6, 8, 10, 2); // (3,4,5) homogenizado
    
    float dist = Vector4D::Distance(p1, p2);
    float expected = std::sqrt((3-1)*(3-1) + (4-2)*(4-2) + (5-3)*(5-3));
    
    assert(float_equal(dist, expected));
    
    std::cout << "Distance: PASSED\n";
}

int main() {
    std::cout << "Running Vector4D tests...\n\n";
    
    try {
        test_constructors();
        test_accessors();
        test_arithmetic();
        test_scalar_operations();
        test_comparisons();
        test_homogeneous_operations();
        test_dot_product();
        test_normalization();
        test_exceptions();
        test_distance();
        
        std::cout << "\nAll tests PASSED!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test FAILED: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown test failure\n";
        return 1;
    }
}