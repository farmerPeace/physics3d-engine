#include "physics/math/quaternion.hpp"
#include "physics/math/vector3d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/matrix4d.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

constexpr float EPS = 1e-5f;
const float PI = 3.14159265358979323846f;

bool float_equal(float a, float b, float eps = EPS) {
    return std::abs(a - b) < eps;
}

bool vec3_equal(const Vector3D &a, const Vector3D &b, float eps = EPS) {
    return float_equal(a.x(), b.x(), eps) &&
           float_equal(a.y(), b.y(), eps) &&
           float_equal(a.z(), b.z(), eps);
}

// ─────────────────────────────────────────────
void test_constructors() {
    // Constructor por defecto → identidad (1, 0, 0, 0)
    Quaternion q;
    assert(float_equal(q[0], 1.0f)); // w
    assert(float_equal(q[1], 0.0f)); // x
    assert(float_equal(q[2], 0.0f)); // y
    assert(float_equal(q[3], 0.0f)); // z

    // Constructor con 4 floats
    Quaternion q2(0.5f, 0.1f, 0.2f, 0.3f);
    assert(float_equal(q2[0], 0.5f));
    assert(float_equal(q2[1], 0.1f));
    assert(float_equal(q2[2], 0.2f));
    assert(float_equal(q2[3], 0.3f));

    // Constructor desde escalar + Vector3D
    Vector3D v(1, 2, 3);
    Quaternion q3(0.5f, v);
    assert(float_equal(q3[0], 0.5f));
    assert(float_equal(q3[1], 1.0f));
    assert(float_equal(q3[2], 2.0f));
    assert(float_equal(q3[3], 3.0f));

    std::cout << "test_constructors: PASSED\n";
}

// ─────────────────────────────────────────────
void test_static_factories() {
    // Identity
    Quaternion id = Quaternion::Identity();
    assert(float_equal(id[0], 1.0f));
    assert(float_equal(id[1], 0.0f));
    assert(float_equal(id[2], 0.0f));
    assert(float_equal(id[3], 0.0f));
    assert(id.IsUnit());

    // FromAxisAngle: 180° alrededor de Z → (0, 0, 0, 1) normalizado
    Vector3D z_axis(0, 0, 1);
    Quaternion q180 = Quaternion::FromAxisAngle(z_axis, PI);
    assert(float_equal(q180[0], 0.0f, 1e-4f)); // cos(PI/2) = 0
    assert(float_equal(q180[3], 1.0f, 1e-4f)); // sin(PI/2) = 1
    assert(q180.IsUnit());

    // FromAxisAngle: 0° → identidad
    Quaternion q0 = Quaternion::FromAxisAngle(z_axis, 0.0f);
    assert(q0 == Quaternion::Identity());

    // FromEulerAngles: sin rotación → identidad
    Quaternion qe = Quaternion::FromEulerAngles(0.0f, 0.0f, 0.0f);
    assert(qe == Quaternion::Identity());

    // FromRotationMatrix: identidad → cuaternión identidad
    Matrix3D m_id = Matrix3D::Identity();
    Quaternion qm = Quaternion::FromRotationMatrix(m_id);
    assert(qm == Quaternion::Identity());

    std::cout << "test_static_factories: PASSED\n";
}

// ─────────────────────────────────────────────
void test_index_accessor() {
    Quaternion q(1, 2, 3, 4);
    assert(float_equal(q[0], 1.0f));
    assert(float_equal(q[1], 2.0f));
    assert(float_equal(q[2], 3.0f));
    assert(float_equal(q[3], 4.0f));

    // Escritura
    q[0] = 10.0f;
    q[3] = 40.0f;
    assert(float_equal(q[0], 10.0f));
    assert(float_equal(q[3], 40.0f));

    // Excepción fuera de rango
    bool caught = false;
    try { q[4]; } catch (const std::out_of_range &) { caught = true; }
    assert(caught);

    std::cout << "test_index_accessor: PASSED\n";
}

// ─────────────────────────────────────────────
void test_arithmetic() {
    Quaternion a(1, 2, 3, 4);
    Quaternion b(5, 6, 7, 8);

    // Suma
    Quaternion sum = a + b;
    assert(float_equal(sum[0], 6));
    assert(float_equal(sum[1], 8));
    assert(float_equal(sum[2], 10));
    assert(float_equal(sum[3], 12));

    // Resta
    Quaternion diff = b - a;
    assert(float_equal(diff[0], 4));
    assert(float_equal(diff[1], 4));
    assert(float_equal(diff[2], 4));
    assert(float_equal(diff[3], 4));

    // Negación
    Quaternion neg = -a;
    assert(float_equal(neg[0], -1));
    assert(float_equal(neg[1], -2));
    assert(float_equal(neg[2], -3));
    assert(float_equal(neg[3], -4));

    // In-place
    Quaternion c = a;
    c += b;
    assert(c == sum);

    c = b;
    c -= a;
    assert(c == diff);

    std::cout << "test_arithmetic: PASSED\n";
}

// ─────────────────────────────────────────────
void test_scalar_operations() {
    Quaternion q(1, 2, 3, 4);

    // Multiplicación
    Quaternion scaled = q * 2.0f;
    assert(float_equal(scaled[0], 2));
    assert(float_equal(scaled[1], 4));
    assert(float_equal(scaled[2], 6));
    assert(float_equal(scaled[3], 8));

    // Conmutativa
    Quaternion scaled2 = 2.0f * q;
    assert(scaled == scaled2);

    // División
    Quaternion divided = scaled / 2.0f;
    assert(divided == q);

    // In-place
    Quaternion q2 = q;
    q2 *= 2.0f;
    assert(q2 == scaled);
    q2 /= 2.0f;
    assert(q2 == q);

    // División por cero
    bool caught = false;
    try { q / 0.0f; } catch (const std::domain_error &) { caught = true; }
    assert(caught);

    std::cout << "test_scalar_operations: PASSED\n";
}

// ─────────────────────────────────────────────
void test_quaternion_multiplication() {
    // Identidad: q * Identity = q
    Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
    Quaternion id = Quaternion::Identity();
    assert(q * id == q);
    assert(id * q == q);

    // Anticonmutatividad con ejes puros
    // i * j = k,  j * i = -k
    Quaternion qi(0, 1, 0, 0); // i
    Quaternion qj(0, 0, 1, 0); // j
    Quaternion qk(0, 0, 0, 1); // k

    Quaternion ij = qi * qj;
    assert(float_equal(ij[0], 0.0f)); // w
    assert(float_equal(ij[1], 0.0f)); // x
    assert(float_equal(ij[2], 0.0f)); // y
    assert(float_equal(ij[3], 1.0f)); // z  → k

    Quaternion ji = qj * qi;
    assert(float_equal(ji[3], -1.0f)); // → -k

    // In-place
    Quaternion q2 = qi;
    q2 *= qj;
    assert(q2 == ij);

    std::cout << "test_quaternion_multiplication: PASSED\n";
}

// ─────────────────────────────────────────────
void test_dot_product() {
    Quaternion a(1, 2, 3, 4);
    Quaternion b(5, 6, 7, 8);

    float dot = a.Dot(b);
    float expected = 1*5 + 2*6 + 3*7 + 4*8;
    assert(float_equal(dot, expected));

    // Dot de un cuaternión unitario consigo mismo = 1
    Quaternion n = a.Normalized();
    assert(float_equal(n.Dot(n), 1.0f));

    std::cout << "test_dot_product: PASSED\n";
}

// ─────────────────────────────────────────────
void test_conjugate_and_inverse() {
    Quaternion q(1, 2, 3, 4);

    // Conjugado: misma parte escalar, vectorial negada
    Quaternion conj = q.Conjugate();
    assert(float_equal(conj[0],  1.0f));
    assert(float_equal(conj[1], -2.0f));
    assert(float_equal(conj[2], -3.0f));
    assert(float_equal(conj[3], -4.0f));

    // q * conj(q) = |q|² * Identity (solo parte escalar)
    Quaternion prod = q * conj;
    assert(float_equal(prod[1], 0.0f));
    assert(float_equal(prod[2], 0.0f));
    assert(float_equal(prod[3], 0.0f));

    // Para unitario: inv = conj
    Quaternion u = q.Normalized();
    Quaternion inv = u.Inverse();
    assert(u.Conjugate() == inv);

    // q * q⁻¹ = Identity
    Quaternion should_be_id = u * inv;
    assert(should_be_id == Quaternion::Identity());

    // Inverso de magnitud cero lanza excepción
    bool caught = false;
    try { Quaternion(0,0,0,0).Inverse(); } catch (const std::domain_error &) { caught = true; }
    assert(caught);

    std::cout << "test_conjugate_and_inverse: PASSED\n";
}

// ─────────────────────────────────────────────
void test_magnitude_and_normalization() {
    Quaternion q(1, 2, 3, 4);

    float expected_mag = std::sqrt(1.0f + 4.0f + 9.0f + 16.0f);
    assert(float_equal(q.Magnitude(), expected_mag));
    assert(float_equal(q.SquareMagnitude(), 30.0f));

    // Normalizado tiene magnitud 1
    Quaternion n = q.Normalized();
    assert(float_equal(n.Magnitude(), 1.0f));
    assert(n.IsUnit());

    // In-place
    Quaternion q2 = q;
    q2.Normalize();
    assert(q2 == n);

    // Normalizar magnitud cero lanza excepción
    bool caught = false;
    try { Quaternion(0,0,0,0).Normalize(); } catch (const std::domain_error &) { caught = true; }
    assert(caught);

    std::cout << "test_magnitude_and_normalization: PASSED\n";
}

// ─────────────────────────────────────────────
void test_rotate_vector() {
    // 90° alrededor de Y: (1,0,0) → (0,0,-1)
    Quaternion q = Quaternion::FromAxisAngle(Vector3D(0,1,0), PI / 2.0f);
    Vector3D v(1, 0, 0);
    Vector3D result = q.Rotate(v);

    assert(float_equal(result.x(),  0.0f, 1e-4f));
    assert(float_equal(result.y(),  0.0f, 1e-4f));
    assert(float_equal(result.z(), -1.0f, 1e-4f));

    // 180° alrededor de Z: (1,0,0) → (-1,0,0)
    Quaternion q180 = Quaternion::FromAxisAngle(Vector3D(0,0,1), PI);
    Vector3D v2 = q180.Rotate(Vector3D(1, 0, 0));
    assert(float_equal(v2.x(), -1.0f, 1e-4f));
    assert(float_equal(v2.y(),  0.0f, 1e-4f));
    assert(float_equal(v2.z(),  0.0f, 1e-4f));

    // Identidad no rota el vector
    Vector3D v3(3, 4, 5);
    Vector3D v3r = Quaternion::Identity().Rotate(v3);
    assert(vec3_equal(v3, v3r, 1e-4f));

    // La magnitud del vector se preserva
    Vector3D v4(1, 2, 3);
    Quaternion qr = Quaternion::FromAxisAngle(Vector3D(1,1,1).Normalized(), PI / 3.0f);
    Vector3D v4r = qr.Rotate(v4);
    assert(float_equal(v4.Magnitude(), v4r.Magnitude(), 1e-4f));

    std::cout << "test_rotate_vector: PASSED\n";
}

// ─────────────────────────────────────────────
void test_to_axis_angle() {
    float angle_in = PI / 3.0f;
    Vector3D axis_in = Vector3D(0, 1, 0);

    Quaternion q = Quaternion::FromAxisAngle(axis_in, angle_in);

    Vector3D axis_out;
    float angle_out;
    q.ToAxisAngle(axis_out, angle_out);

    assert(float_equal(angle_out, angle_in, 1e-4f));
    assert(vec3_equal(axis_out, axis_in, 1e-4f));

    // Cuaternión identidad → ángulo ≈ 0, eje arbitrario (no crash)
    Vector3D axis_id;
    float angle_id;
    Quaternion::Identity().ToAxisAngle(axis_id, angle_id);
    assert(float_equal(angle_id, 0.0f, 1e-4f));

    std::cout << "test_to_axis_angle: PASSED\n";
}

// ─────────────────────────────────────────────
void test_slerp() {
    Quaternion a = Quaternion::Identity();
    Quaternion b = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PI / 2.0f);

    // t=0 → a
    assert(Quaternion::Slerp(a, b, 0.0f) == a);

    // t=1 → b
    assert(Quaternion::Slerp(a, b, 1.0f) == b);

    // t=0.5 → mitad del ángulo (PI/4)
    Quaternion mid = Quaternion::Slerp(a, b, 0.5f);
    Quaternion expected = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PI / 4.0f);
    assert(mid == expected);

    // El resultado siempre es unitario
    for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
        assert(Quaternion::Slerp(a, b, t).IsUnit());
    }

    // Clamp fuera de [0,1]
    assert(Quaternion::Slerp(a, b, -0.5f) == a);
    assert(Quaternion::Slerp(a, b,  1.5f) == b);

    // Camino corto: si dot < 0 debe tomar el arco más corto
    Quaternion b_neg(-b[0], -b[1], -b[2], -b[3]);
    Quaternion s1 = Quaternion::Slerp(a, b,     0.5f);
    Quaternion s2 = Quaternion::Slerp(a, b_neg, 0.5f);
    assert(s1 == s2);

    std::cout << "test_slerp: PASSED\n";
}

// ─────────────────────────────────────────────
void test_nlerp() {
    Quaternion a = Quaternion::Identity();
    Quaternion b = Quaternion::FromAxisAngle(Vector3D(0, 0, 1), PI / 2.0f);

    // t=0 → a,  t=1 → b
    assert(Quaternion::Nlerp(a, b, 0.0f) == a);
    assert(Quaternion::Nlerp(a, b, 1.0f) == b);

    // El resultado siempre es unitario
    for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
        assert(Quaternion::Nlerp(a, b, t).IsUnit());
    }

    std::cout << "test_nlerp: PASSED\n";
}

// ─────────────────────────────────────────────
void test_derivative() {
    // Con cuaternión identidad y velocidad angular (0,1,0):
    // dq/dt = 0.5 * (0,0,1,0) * (1,0,0,0) = (0, 0, 0.5, 0)
    Quaternion q  = Quaternion::Identity();
    Vector3D omega(0, 1, 0);
    Quaternion dq = q.Derivative(omega);

    assert(float_equal(dq[0], 0.0f));
    assert(float_equal(dq[1], 0.0f));
    assert(float_equal(dq[2], 0.5f));
    assert(float_equal(dq[3], 0.0f));

    std::cout << "test_derivative: PASSED\n";
}

// ─────────────────────────────────────────────
void test_to_rotation_matrix() {
    // Identidad → matriz identidad
    Matrix3D m_id = Quaternion::Identity().ToRotationMatrix();
    assert(m_id == Matrix3D::Identity());

    // 90° alrededor de Z: la misma rotación que Matrix3D::RotationZ
    Quaternion qz = Quaternion::FromAxisAngle(Vector3D(0, 0, 1), PI / 2.0f);
    Matrix3D   mz = qz.ToRotationMatrix();
    Matrix3D   mz_ref = Matrix3D::RotationZ(PI / 2.0f);
    assert(mz == mz_ref);

    // El resultado debe ser ortogonal con determinante 1
    assert(mz.IsOrthogonal());
    assert(float_equal(std::abs(mz.Determinant()), 1.0f));

    // Ida y vuelta: FromRotationMatrix → ToRotationMatrix = original
    Matrix3D m_orig = Matrix3D::RotationX(PI / 5.0f);
    Quaternion q_trip = Quaternion::FromRotationMatrix(m_orig);
    Matrix3D  m_trip  = q_trip.ToRotationMatrix();
    assert(m_orig == m_trip);

    std::cout << "test_to_rotation_matrix: PASSED\n";
}

// ─────────────────────────────────────────────
void test_to_transform_matrix() {
    // Identidad → Matrix4D identidad
    Matrix4D m_id = Quaternion::Identity().ToTransformMatrix();
    assert(m_id == Matrix4D::Identity());

    // La submatriz 3×3 debe coincidir con ToRotationMatrix
    Quaternion q = Quaternion::FromAxisAngle(Vector3D(1, 0, 0), PI / 4.0f);
    Matrix4D m4  = q.ToTransformMatrix();
    Matrix3D m3  = q.ToRotationMatrix();

    assert(m4.GetUpper3x3() == m3);

    // Última fila/columna = identidad (sin traslación)
    assert(float_equal(m4(3, 0), 0.0f));
    assert(float_equal(m4(3, 1), 0.0f));
    assert(float_equal(m4(3, 2), 0.0f));
    assert(float_equal(m4(3, 3), 1.0f));
    assert(float_equal(m4(0, 3), 0.0f));
    assert(float_equal(m4(1, 3), 0.0f));
    assert(float_equal(m4(2, 3), 0.0f));

    std::cout << "test_to_transform_matrix: PASSED\n";
}

// ─────────────────────────────────────────────
void test_to_euler_angles() {
    // Identidad → (0, 0, 0)
    Vector3D euler_id = Quaternion::Identity().ToEulerAngles();
    assert(vec3_equal(euler_id, Vector3D(0, 0, 0), 1e-4f));

    // Ida y vuelta: FromEulerAngles → ToEulerAngles ≈ original
    // (válido para ángulos fuera de la singularidad del gimbal lock)
    float pitch = PI / 6.0f;
    float yaw   = PI / 4.0f;
    float roll  = PI / 3.0f;

    Quaternion q = Quaternion::FromEulerAngles(pitch, yaw, roll);
    Vector3D   e = q.ToEulerAngles();

    assert(float_equal(e.x(), pitch, 1e-4f));
    assert(float_equal(e.y(), yaw,   1e-4f));
    assert(float_equal(e.z(), roll,  1e-4f));

    std::cout << "test_to_euler_angles: PASSED\n";
}

// ─────────────────────────────────────────────
void test_is_unit_and_is_pure() {
    // IsUnit
    assert(Quaternion::Identity().IsUnit());
    assert(!Quaternion(2, 0, 0, 0).IsUnit());

    Quaternion q(1, 2, 3, 4);
    assert(!q.IsUnit());
    assert(q.Normalized().IsUnit());

    // IsPure: parte escalar ≈ 0
    assert(!Quaternion::Identity().IsPure());
    assert(Quaternion(0, 1, 0, 0).IsPure());
    assert(Quaternion(0, 0, 0, 0).IsPure());
    assert(!Quaternion(0.5f, 1, 0, 0).IsPure());

    std::cout << "test_is_unit_and_is_pure: PASSED\n";
}

// ─────────────────────────────────────────────
void test_comparison() {
    Quaternion a(1, 2, 3, 4);
    Quaternion b(1, 2, 3, 4);
    Quaternion c(5, 6, 7, 8);

    assert(a == b);
    assert(!(a == c));
    assert(a != c);
    assert(!(a != b));

    // Tolerancia numérica
    Quaternion a2(1.000001f, 2.000001f, 3.000001f, 4.000001f);
    assert(a == a2);

    std::cout << "test_comparison: PASSED\n";
}

// ─────────────────────────────────────────────
void test_exponential_and_logarithm() {
    // exp(log(q)) ≈ q para cuaternión unitario
    Quaternion q = Quaternion::FromAxisAngle(Vector3D(1, 0, 0), PI / 4.0f);
    Quaternion log_q = q.Logarithm();
    Quaternion exp_log_q = log_q.Exponential();

    assert(float_equal(exp_log_q[0], q[0], 1e-4f));
    assert(float_equal(exp_log_q[1], q[1], 1e-4f));
    assert(float_equal(exp_log_q[2], q[2], 1e-4f));
    assert(float_equal(exp_log_q[3], q[3], 1e-4f));

    // log(Identity): parte vectorial = 0, w = log(1) = 0
    Quaternion log_id = Quaternion::Identity().Logarithm();
    assert(float_equal(log_id[0], 0.0f, 1e-4f));
    assert(float_equal(log_id[1], 0.0f, 1e-4f));
    assert(float_equal(log_id[2], 0.0f, 1e-4f));
    assert(float_equal(log_id[3], 0.0f, 1e-4f));

    // Logaritmo de magnitud cero lanza excepción
    bool caught = false;
    try { Quaternion(0,0,0,0).Logarithm(); } catch (const std::domain_error &) { caught = true; }
    assert(caught);

    std::cout << "test_exponential_and_logarithm: PASSED\n";
}

// ─────────────────────────────────────────────
void test_data_pointer() {
    Quaternion q(1, 2, 3, 4);
    const float *data = q.Data();

    // El orden en memoria debe ser w, x, y, z
    assert(float_equal(data[0], 1.0f));
    assert(float_equal(data[1], 2.0f));
    assert(float_equal(data[2], 3.0f));
    assert(float_equal(data[3], 4.0f));

    // Escritura a través del puntero no-const
    float *data_w = q.Data();
    data_w[0] = 10.0f;
    assert(float_equal(q[0], 10.0f));

    std::cout << "test_data_pointer: PASSED\n";
}

// ─────────────────────────────────────────────
int main() {
    std::cout << "Running Quaternion tests...\n\n";

    try {
        test_constructors();
        test_static_factories();
        test_index_accessor();
        test_arithmetic();
        test_scalar_operations();
        test_quaternion_multiplication();
        test_dot_product();
        test_conjugate_and_inverse();
        test_magnitude_and_normalization();
        test_rotate_vector();
        test_to_axis_angle();
        test_slerp();
        test_nlerp();
        test_derivative();
        test_to_rotation_matrix();
        test_to_transform_matrix();
        test_to_euler_angles();
        test_is_unit_and_is_pure();
        test_comparison();
        test_exponential_and_logarithm();
        test_data_pointer();

        std::cout << "\nTodos los tests de Quaternion pasaron correctamente.\n";
        return 0;

    } catch (const std::exception &e) {
        std::cerr << "Test FAILED: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Fallo desconocido en los tests.\n";
        return 1;
    }
}