#include "physics/math/matrix4d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/vector3d.hpp"
#include "physics/math/vector4d.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
 
constexpr float EPS = 1e-5f;
const float PI = 3.14159265358979323846f;
 
bool float_equal(float a, float b, float eps = EPS) {
    return std::abs(a - b) < eps;
}
 
// ─────────────────────────────────────────────
void test_constructors_and_access() {
    // Constructor por defecto → identidad
    Matrix4D id;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            assert(float_equal(id(i, j), i == j ? 1.0f : 0.0f));
        }
    }
 
    // Constructor de 16 floats (entrada fila-mayor)
    Matrix4D m(
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    );
    assert(float_equal(m(0,0),  1)); assert(float_equal(m(0,3),  4));
    assert(float_equal(m(1,0),  5)); assert(float_equal(m(2,2), 11));
    assert(float_equal(m(3,0), 13)); assert(float_equal(m(3,3), 16));
 
    // Constructor desde Matrix3D
    Matrix3D rot3 = Matrix3D::RotationZ(PI / 4.0f);
    Matrix4D m4(rot3);
    assert(float_equal(m4(0, 0), rot3(0, 0)));
    assert(float_equal(m4(1, 1), rot3(1, 1)));
    assert(float_equal(m4(3, 3), 1.0f));
    assert(float_equal(m4(0, 3), 0.0f));
 
    // Constructor desde Matrix3D + traslación
    Vector3D t(3, 5, 7);
    Matrix4D m4t(rot3, t);
    assert(float_equal(m4t(0, 3), 3.0f));
    assert(float_equal(m4t(1, 3), 5.0f));
    assert(float_equal(m4t(2, 3), 7.0f));
    assert(float_equal(m4t(3, 3), 1.0f));
 
    std::cout << "test_constructors_and_access: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_identity_and_zero() {
    Matrix4D id   = Matrix4D::Identity();
    Matrix4D zero = Matrix4D::Zero();
 
    assert(id.IsIdentity());
    assert(zero.IsZero());
    assert(!id.IsZero());
    assert(!zero.IsIdentity());
 
    std::cout << "test_identity_and_zero: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_static_transforms() {
    // Traslación
    Matrix4D T = Matrix4D::Translation(1, 2, 3);
    assert(float_equal(T(0, 3), 1.0f));
    assert(float_equal(T(1, 3), 2.0f));
    assert(float_equal(T(2, 3), 3.0f));
    assert(float_equal(T(3, 3), 1.0f));
    assert(float_equal(T(0, 0), 1.0f)); // diagonal = identidad
 
    // Escala
    Matrix4D S = Matrix4D::Scale(2, 3, 4);
    assert(float_equal(S(0, 0), 2.0f));
    assert(float_equal(S(1, 1), 3.0f));
    assert(float_equal(S(2, 2), 4.0f));
    assert(float_equal(S(3, 3), 1.0f));
    assert(float_equal(S(0, 1), 0.0f));
 
    // Escala uniforme
    Matrix4D Su = Matrix4D::Scale(5.0f);
    assert(float_equal(Su(0, 0), 5.0f));
    assert(float_equal(Su(1, 1), 5.0f));
    assert(float_equal(Su(2, 2), 5.0f));
 
    // Rotación — matrices ortogonales
    Matrix4D Rx = Matrix4D::RotationX(PI / 3.0f);
    Matrix4D Ry = Matrix4D::RotationY(PI / 4.0f);
    Matrix4D Rz = Matrix4D::RotationZ(PI / 6.0f);
 
    assert(Rx.IsOrthogonal());
    assert(Ry.IsOrthogonal());
    assert(Rz.IsOrthogonal());
 
    std::cout << "test_static_transforms: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_trs() {
    Vector3D t(1, 2, 3);
    Matrix3D r = Matrix3D::RotationY(PI / 2.0f);
    Vector3D s(2, 2, 2);
 
    Matrix4D trs = Matrix4D::TRS(t, r, s);
 
    // Traslación preservada en la cuarta columna
    assert(float_equal(trs(0, 3), 1.0f));
    assert(float_equal(trs(1, 3), 2.0f));
    assert(float_equal(trs(2, 3), 3.0f));
    assert(float_equal(trs(3, 3), 1.0f));
 
    // Escala × rotación: columna 0 de TRS debería tener magnitud 2
    float col0_mag = std::sqrt(trs(0,0)*trs(0,0) + trs(1,0)*trs(1,0) + trs(2,0)*trs(2,0));
    assert(float_equal(col0_mag, 2.0f));
 
    std::cout << "test_trs: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_arithmetic() {
    Matrix4D a = Matrix4D::Scale(2, 2, 2);
    Matrix4D b = Matrix4D::Scale(3, 3, 3);
 
    // Suma
    Matrix4D sum = a + b;
    assert(float_equal(sum(0, 0), 5.0f));
    assert(float_equal(sum(3, 3), 2.0f)); // 1+1 de la diagonal inferior
 
    // Resta
    Matrix4D diff = b - a;
    assert(float_equal(diff(0, 0), 1.0f));
 
    // Escalar
    Matrix4D scaled = a * 2.0f;
    assert(float_equal(scaled(0, 0), 4.0f));
 
    // División
    Matrix4D div = scaled / 2.0f;
    assert(div == a);
 
    // Negación
    Matrix4D neg = -a;
    assert(float_equal(neg(0, 0), -2.0f));
    assert(float_equal(neg(3, 3), -1.0f));
 
    std::cout << "test_arithmetic: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_matrix_multiplication() {
    // T * S: primero escala, luego traslada un punto (1,1,1,1)
    Matrix4D T = Matrix4D::Translation(5, 0, 0);
    Matrix4D S = Matrix4D::Scale(2, 2, 2);
    Matrix4D TS = T * S;
 
    Vector4D p(1, 1, 1, 1);
    Vector4D result = TS * p;
 
    // Escala primero: (2,2,2,1), luego traslada: (7,2,2,1)
    assert(float_equal(result.x(), 7.0f));
    assert(float_equal(result.y(), 2.0f));
    assert(float_equal(result.z(), 2.0f));
    assert(float_equal(result.w(), 1.0f));
 
    // M * I = M
    Matrix4D I = Matrix4D::Identity();
    assert((TS * I) == TS);
 
    // M * M⁻¹ = I  (con matriz invertible)
    Matrix4D inv = TS.Inverted();
    Matrix4D should_be_id = TS * inv;
    assert(should_be_id.IsIdentity());
 
    std::cout << "test_matrix_multiplication: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_vector_transform() {
    // Rotar 90° alrededor de Z: (1,0,0) → (0,1,0)
    Matrix4D R = Matrix4D::RotationZ(PI / 2.0f);
    Vector4D v(1, 0, 0, 0);  // dirección (w=0)
    Vector4D rv = R * v;
 
    assert(float_equal(rv.x(),  0.0f));
    assert(float_equal(rv.y(),  1.0f));
    assert(float_equal(rv.z(),  0.0f));
 
    // Trasladar un punto (w=1)
    Matrix4D T = Matrix4D::Translation(3, 4, 5);
    Vector4D p(1, 1, 1, 1);
    Vector4D tp = T * p;
 
    assert(float_equal(tp.x(), 4.0f));
    assert(float_equal(tp.y(), 5.0f));
    assert(float_equal(tp.z(), 6.0f));
 
    // Trasladar una dirección (w=0) NO debe cambiar
    Vector4D d(1, 0, 0, 0);
    Vector4D td = T * d;
    assert(float_equal(td.x(), 1.0f));
    assert(float_equal(td.y(), 0.0f));
    assert(float_equal(td.z(), 0.0f));
 
    std::cout << "test_vector_transform: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_transpose() {
    Matrix4D m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
       13, 14, 15, 16
    );
 
    Matrix4D t = m.Transposed();
    assert(float_equal(t(0, 1), m(1, 0)));
    assert(float_equal(t(0, 3), m(3, 0)));
    assert(float_equal(t(2, 3), m(3, 2)));
 
    // Doble transpuesta = original
    assert(t.Transposed() == m);
 
    // (A*B)^T = B^T * A^T
    Matrix4D a = Matrix4D::Translation(1, 2, 3);
    Matrix4D b = Matrix4D::Scale(2, 3, 4);
    Matrix4D lhs = (a * b).Transposed();
    Matrix4D rhs = b.Transposed() * a.Transposed();
    assert(lhs == rhs);
 
    std::cout << "test_transpose: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_determinant_and_inverse() {
    // Matriz identidad
    assert(float_equal(Matrix4D::Identity().Determinant(), 1.0f));
    assert(float_equal(Matrix4D::Zero().Determinant(),     0.0f));
 
    // Escala: det = sx * sy * sz * 1
    Matrix4D S = Matrix4D::Scale(2, 3, 4);
    assert(float_equal(S.Determinant(), 24.0f));
 
    // Inversa
    Matrix4D T  = Matrix4D::Translation(1, 2, 3);
    Matrix4D Ti = T.Inverted();
    assert((T * Ti).IsIdentity());
 
    // IsInvertible
    assert(T.IsInvertible());
    assert(!Matrix4D::Zero().IsInvertible());
 
    // Invert in-place
    Matrix4D R = Matrix4D::RotationX(PI / 5.0f);
    Matrix4D R_copy = R;
    R_copy.Invert();
    assert((R * R_copy).IsIdentity());
 
    std::cout << "test_determinant_and_inverse: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_column_row_accessors() {
    Matrix4D m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
       13, 14, 15, 16
    );
 
    // Columnas
    Vector4D col0 = m.GetColumn(0);
    assert(float_equal(col0.x(), 1.0f));
    assert(float_equal(col0.y(), 5.0f));
    assert(float_equal(col0.z(), 9.0f));
    assert(float_equal(col0.w(), 13.0f));
 
    // Filas
    Vector4D row1 = m.GetRow(1);
    assert(float_equal(row1.x(), 5.0f));
    assert(float_equal(row1.y(), 6.0f));
    assert(float_equal(row1.z(), 7.0f));
    assert(float_equal(row1.w(), 8.0f));
 
    // SetColumn
    Matrix4D m2 = m;
    m2.SetColumn(0, Vector4D(10, 20, 30, 40));
    assert(float_equal(m2(0,0), 10.0f));
    assert(float_equal(m2(3,0), 40.0f));
 
    // Upper3x3 y traslación
    Matrix3D r3 = Matrix3D::RotationZ(1.0f);
    Vector3D t(7, 8, 9);
    Matrix4D mt(r3, t);
 
    Matrix3D extracted = mt.GetUpper3x3();
    assert(extracted == r3);
 
    Vector3D et = mt.GetTranslation();
    assert(float_equal(et.x(), 7.0f));
    assert(float_equal(et.y(), 8.0f));
    assert(float_equal(et.z(), 9.0f));
 
    std::cout << "test_column_row_accessors: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_projection_matrices() {
    // Perspective: columna 3 debe tener w_clip = -z_eye  →  m(3,2) = -1
    Matrix4D P = Matrix4D::Perspective(PI / 2.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    assert(float_equal(P(3, 2), -1.0f));
    assert(float_equal(P(3, 3),  0.0f));
 
    // Orthographic: m(3,3) = 1
    Matrix4D O = Matrix4D::Orthographic(-1, 1, -1, 1, 0.1f, 100.0f);
    assert(float_equal(O(3, 3), 1.0f));
    assert(float_equal(O(0, 0), 1.0f)); // 2/(r-l) = 2/2 = 1
 
    std::cout << "test_projection_matrices: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_lookat() {
    Vector3D eye(0, 0, 5);
    Vector3D target(0, 0, 0);
    Vector3D up(0, 1, 0);
 
    Matrix4D V = Matrix4D::LookAt(eye, target, up);
 
    // La vista transforma el ojo al origen
    Vector4D eye_h(0, 0, 5, 1);
    Vector4D in_view = V * eye_h;
    assert(float_equal(in_view.x(), 0.0f));
    assert(float_equal(in_view.y(), 0.0f));
    assert(float_equal(in_view.z(), 0.0f));
 
    std::cout << "test_lookat: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_lerp() {
    Matrix4D a = Matrix4D::Identity();
    Matrix4D b = Matrix4D::Scale(3, 3, 3);
 
    Matrix4D mid = Matrix4D::Lerp(a, b, 0.5f);
    assert(float_equal(mid(0, 0), 2.0f));
    assert(float_equal(mid(1, 1), 2.0f));
    assert(float_equal(mid(2, 2), 2.0f));
    assert(float_equal(mid(3, 3), 1.0f)); // 1 + 0.5*(1-1) = 1
 
    // Clamp t fuera de [0,1]
    assert(Matrix4D::Lerp(a, b, -1.0f) == a);
    assert(Matrix4D::Lerp(a, b,  2.0f) == b);
 
    std::cout << "test_lerp: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_trace_and_properties() {
    Matrix4D id = Matrix4D::Identity();
    assert(float_equal(id.Trace(), 4.0f));
 
    Matrix4D S = Matrix4D::Scale(2, 3, 4);
    assert(float_equal(S.Trace(), 10.0f)); // 2+3+4+1
 
    Matrix4D R = Matrix4D::RotationZ(PI / 3.0f);
    assert(R.IsOrthogonal());
 
    Matrix4D T = Matrix4D::Translation(1, 2, 3);
    assert(!T.IsOrthogonal()); // columna de traslación rompe la ortogonalidad
 
    std::cout << "test_trace_and_properties: PASSED\n";
}
 
// ─────────────────────────────────────────────
void test_data_pointer() {
    Matrix4D m = Matrix4D::Identity();
    const float *data = m.Data();
 
    // column-major: primer elemento = m(0,0) = 1
    assert(float_equal(data[0], 1.0f));
    assert(float_equal(data[5], 1.0f));
    assert(float_equal(data[1], 0.0f));
 
    std::cout << "test_data_pointer: PASSED\n";
}
 
// ─────────────────────────────────────────────
int main() {
    std::cout << "Running Matrix4D tests...\n\n";
 
    try {
        test_constructors_and_access();
        test_identity_and_zero();
        test_static_transforms();
        test_trs();
        test_arithmetic();
        test_matrix_multiplication();
        test_vector_transform();
        test_transpose();
        test_determinant_and_inverse();
        test_column_row_accessors();
        test_projection_matrices();
        test_lookat();
        test_lerp();
        test_trace_and_properties();
        test_data_pointer();
 
        std::cout << "\nTodos los tests de Matrix4D pasaron correctamente.\n";
        return 0;
 
    } catch (const std::exception &e) {
        std::cerr << "Test FAILED: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Fallo desconocido en los tests.\n";
        return 1;
    }
}