#include "physics/vector3d.hpp"
#include <cassert>
#include <iostream>

void test_vector_basics() {
    Vector3D a (1, 2, 3);
    Vector3D b (4, 5, 6);

    // Suma
    Vector3D c = a + b;
    assert(c.x() == 5 && c.y() == 7 && c.z() == 9);

    // Resta
    Vector3D d = b - a;
    assert(d.x() == 3 && d.y() == 3 && d.z() == 3);

    // Producto punto
    float dot = a.Dot(b);
    assert(dot == 32);

    std::cout << "Test Basicos Pasados" << std::endl;
}

void test_normalization() {
    Vector3D v(3, 4, 0);
    float mag = v.Magnitude();
    assert(mag >= 4.999f && mag <= 5.001f);

    v.Normalize();
    mag = v.Magnitude();
    assert(mag >= 0.999f && mag <= 1.001f);

    std::cout << "Test normalizacion paso" << std::endl;
}

void test_cross_product() {
    Vector3D i (1, 0, 0);
    Vector3D j (0, 1, 0);
    Vector3D k = i.Cross(j);

    assert(k.x() == 0 && k.y() == 0 && k.z() == 1);

    std::cout << "Test producto cruz paso" << std::endl;

}

int main() {
    test_vector_basics();
    test_normalization();
    test_cross_product();

    std::cout << "Todos los tests pasados" << std::endl;
    return 0;
}