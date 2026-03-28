#include "physics/math/quaternion.hpp"
#include "physics/math/vector3d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/matrix4d.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

constexpr float EPS  = 1e-5f;
constexpr float EPS3 = 1e-3f;
const float PI = 3.14159265358979323846f;

bool float_eq(float a, float b, float eps = EPS) {
    return std::abs(a - b) < eps;
}
bool vec3_eq(const Vector3D &a, const Vector3D &b, float eps = EPS) {
    return float_eq(a.x(), b.x(), eps) &&
           float_eq(a.y(), b.y(), eps) &&
           float_eq(a.z(), b.z(), eps);
}
bool quat_eq(const Quaternion &a, const Quaternion &b, float eps = EPS) {
    return float_eq(a[0], b[0], eps) && float_eq(a[1], b[1], eps) &&
           float_eq(a[2], b[2], eps) && float_eq(a[3], b[3], eps);
}

void test_constructors() {
    Quaternion q;
    assert(float_eq(q[0], 1.0f)); assert(float_eq(q[1], 0.0f));
    assert(float_eq(q[2], 0.0f)); assert(float_eq(q[3], 0.0f));

    Quaternion q2(0.5f, 0.1f, 0.2f, 0.3f);
    assert(float_eq(q2[0], 0.5f)); assert(float_eq(q2[1], 0.1f));
    assert(float_eq(q2[2], 0.2f)); assert(float_eq(q2[3], 0.3f));

    Vector3D v(1, 2, 3);
    Quaternion q3(0.5f, v);
    assert(float_eq(q3[0], 0.5f)); assert(float_eq(q3[1], 1.0f));
    assert(float_eq(q3[2], 2.0f)); assert(float_eq(q3[3], 3.0f));

    std::cout << "test_constructors: PASSED\n";
}

void test_identity() {
    Quaternion id = Quaternion::Identity();
    assert(float_eq(id[0], 1.0f)); assert(float_eq(id[1], 0.0f));
    assert(float_eq(id[2], 0.0f)); assert(float_eq(id[3], 0.0f));
    assert(id.IsUnit());
    std::cout << "test_identity: PASSED\n";
}

void test_index_accessor() {
    Quaternion q(1, 2, 3, 4);
    assert(float_eq(q[0], 1.0f)); assert(float_eq(q[1], 2.0f));
    assert(float_eq(q[2], 3.0f)); assert(float_eq(q[3], 4.0f));

    q[0] = 10.0f; q[3] = 40.0f;
    assert(float_eq(q[0], 10.0f)); assert(float_eq(q[3], 40.0f));

    bool caught = false;
    try { q[4]; } catch (const std::out_of_range &) { caught = true; }
    assert(caught);
    std::cout << "test_index_accessor: PASSED\n";
}

void test_from_axis_angle() {
    // 0 grados -> identidad
    assert(quat_eq(Quaternion::FromAxisAngle(Vector3D(0,0,1), 0.0f), Quaternion::Identity()));

    // 180 grados alrededor de Z -> w=0, z=1
    Quaternion q180 = Quaternion::FromAxisAngle(Vector3D(0,0,1), PI);
    assert(float_eq(q180[0], 0.0f, 1e-4f));
    assert(float_eq(q180[3], 1.0f, 1e-4f));
    assert(q180.IsUnit());

    // El eje se normaliza automaticamente
    Quaternion qa = Quaternion::FromAxisAngle(Vector3D(0,0,2), PI / 2.0f);
    Quaternion qb = Quaternion::FromAxisAngle(Vector3D(0,0,1), PI / 2.0f);
    assert(quat_eq(qa, qb));

    std::cout << "test_from_axis_angle: PASSED\n";
}

// Firma: FromEulerAngles(pitch, yaw, roll)
// Composicion: Rz(roll) * Ry(yaw) * Rx(pitch)
void test_from_euler_angles() {
    assert(quat_eq(Quaternion::FromEulerAngles(0, 0, 0), Quaternion::Identity()));

    const float a = PI / 4.0f;
    const Vector3D v(1, 2, 3);

    // Solo pitch (X)
    {
        Quaternion qi = Quaternion::FromEulerAngles(a, 0.0f, 0.0f);
        Quaternion qr = Quaternion::FromAxisAngle(Vector3D(1,0,0), a);
        assert(vec3_eq(qi.Rotate(v), qr.Rotate(v), EPS3));
    }
    // Solo yaw (Y)
    {
        Quaternion qi = Quaternion::FromEulerAngles(0.0f, a, 0.0f);
        Quaternion qr = Quaternion::FromAxisAngle(Vector3D(0,1,0), a);
        assert(vec3_eq(qi.Rotate(v), qr.Rotate(v), EPS3));
    }
    // Solo roll (Z)
    {
        Quaternion qi = Quaternion::FromEulerAngles(0.0f, 0.0f, a);
        Quaternion qr = Quaternion::FromAxisAngle(Vector3D(0,0,1), a);
        assert(vec3_eq(qi.Rotate(v), qr.Rotate(v), EPS3));
    }
    // Multi-eje: equivalente a qz * qy * qx
    {
        const float p = PI/6.0f, y = PI/5.0f, r = PI/4.0f;
        Quaternion qe = Quaternion::FromEulerAngles(p, y, r);
        Quaternion composed = Quaternion::FromAxisAngle(Vector3D(0,0,1), r)
                            * Quaternion::FromAxisAngle(Vector3D(0,1,0), y)
                            * Quaternion::FromAxisAngle(Vector3D(1,0,0), p);
        assert(vec3_eq(qe.Rotate(v), composed.Rotate(v), EPS3));
    }
    std::cout << "test_from_euler_angles: PASSED\n";
}

void test_from_rotation_matrix() {
    assert(quat_eq(Quaternion::FromRotationMatrix(Matrix3D::Identity()),
                   Quaternion::Identity()));

    // Ida y vuelta por efecto de rotacion
    Quaternion q_orig = Quaternion::FromAxisAngle(Vector3D(1,1,0).Normalized(), PI/5.0f);
    Quaternion q_trip = Quaternion::FromRotationMatrix(q_orig.ToRotationMatrix());
    Vector3D v(3, 1, 2);
    assert(vec3_eq(q_orig.Rotate(v), q_trip.Rotate(v), EPS3));

    std::cout << "test_from_rotation_matrix: PASSED\n";
}

void test_arithmetic() {
    Quaternion a(1,2,3,4), b(5,6,7,8);

    Quaternion sum = a + b;
    assert(float_eq(sum[0],6)); assert(float_eq(sum[1],8));
    assert(float_eq(sum[2],10)); assert(float_eq(sum[3],12));

    Quaternion diff = b - a;
    assert(float_eq(diff[0],4)); assert(float_eq(diff[3],4));

    Quaternion neg = -a;
    assert(float_eq(neg[0],-1)); assert(float_eq(neg[3],-4));

    Quaternion c = a; c += b; assert(quat_eq(c, sum));
    c = b;           c -= a; assert(quat_eq(c, diff));

    std::cout << "test_arithmetic: PASSED\n";
}

void test_scalar_operations() {
    Quaternion q(1,2,3,4);
    Quaternion scaled = q * 2.0f;
    assert(float_eq(scaled[0],2)); assert(float_eq(scaled[3],8));

    assert(quat_eq(2.0f * q, scaled));  // conmutativa
    assert(quat_eq(scaled / 2.0f, q));

    Quaternion q2 = q; q2 *= 2.0f; assert(quat_eq(q2, scaled));
    q2 /= 2.0f;                    assert(quat_eq(q2, q));

    bool caught = false;
    try { PHYS_UNUSED(q / 0.0f); } catch (const std::domain_error &) { caught = true; }
    assert(caught);

    std::cout << "test_scalar_operations: PASSED\n";
}

void test_quaternion_multiplication() {
    Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
    assert(quat_eq(q * Quaternion::Identity(), q));
    assert(quat_eq(Quaternion::Identity() * q, q));

    // i*j = k,  j*i = -k
    Quaternion qi(0,1,0,0), qj(0,0,1,0);
    Quaternion ij = qi * qj;
    assert(float_eq(ij[0], 0.0f)); assert(float_eq(ij[3], 1.0f));
    assert(float_eq((qj * qi)[3], -1.0f));

    Quaternion q2 = qi; q2 *= qj; assert(quat_eq(q2, ij));

    std::cout << "test_quaternion_multiplication: PASSED\n";
}

void test_dot_product() {
    Quaternion a(1,2,3,4), b(5,6,7,8);
    assert(float_eq(a.Dot(b), 1*5+2*6+3*7+4*8));
    assert(float_eq(a.Normalized().Dot(a.Normalized()), 1.0f));
    std::cout << "test_dot_product: PASSED\n";
}

void test_conjugate_and_inverse() {
    Quaternion q(1,2,3,4);
    Quaternion conj = q.Conjugate();
    assert(float_eq(conj[0], 1.0f)); assert(float_eq(conj[1],-2.0f));
    assert(float_eq(conj[2],-3.0f)); assert(float_eq(conj[3],-4.0f));

    // parte vectorial de q*conj = 0
    Quaternion prod = q * conj;
    assert(float_eq(prod[1],0.0f)); assert(float_eq(prod[2],0.0f)); assert(float_eq(prod[3],0.0f));

    Quaternion u = q.Normalized();
    assert(quat_eq(u.Conjugate(), u.Inverse()));
    assert(quat_eq(u * u.Inverse(), Quaternion::Identity()));

    bool caught = false;
    try { PHYS_UNUSED(Quaternion(0,0,0,0).Inverse()); } catch (const std::domain_error &) { caught = true; }
    assert(caught);
    std::cout << "test_conjugate_and_inverse: PASSED\n";
}

void test_magnitude_and_normalization() {
    Quaternion q(1,2,3,4);
    assert(float_eq(q.SquareMagnitude(), 30.0f));
    assert(float_eq(q.Magnitude(), std::sqrt(30.0f)));

    Quaternion n = q.Normalized();
    assert(float_eq(n.Magnitude(), 1.0f));
    assert(n.IsUnit());

    Quaternion q2 = q; q2.Normalize(); assert(quat_eq(q2, n));

    bool caught = false;
    try { Quaternion(0,0,0,0).Normalize(); } catch (const std::domain_error &) { caught = true; }
    assert(caught);
    std::cout << "test_magnitude_and_normalization: PASSED\n";
}

void test_rotate_vector() {
    // 90 grados Y: (1,0,0) -> (0,0,-1)
    Quaternion qy = Quaternion::FromAxisAngle(Vector3D(0,1,0), PI/2.0f);
    Vector3D r1 = qy.Rotate(Vector3D(1,0,0));
    assert(float_eq(r1.x(), 0.0f,1e-4f)); assert(float_eq(r1.z(),-1.0f,1e-4f));

    // 180 grados Z: (1,0,0) -> (-1,0,0)
    Quaternion qz = Quaternion::FromAxisAngle(Vector3D(0,0,1), PI);
    assert(float_eq(qz.Rotate(Vector3D(1,0,0)).x(), -1.0f, 1e-4f));

    // Identity no rota
    Vector3D v(3,4,5);
    assert(vec3_eq(Quaternion::Identity().Rotate(v), v, 1e-4f));

    // Magnitud preservada
    Vector3D v2(1,2,3);
    Quaternion qr = Quaternion::FromAxisAngle(Vector3D(1,1,1).Normalized(), PI/3.0f);
    assert(float_eq(v2.Magnitude(), qr.Rotate(v2).Magnitude(), 1e-4f));

    std::cout << "test_rotate_vector: PASSED\n";
}

void test_to_axis_angle() {
    const float angle_in = PI / 3.0f;
    const Vector3D axis_in(0,1,0);
    Quaternion q = Quaternion::FromAxisAngle(axis_in, angle_in);
    Vector3D ax; float ag;
    q.ToAxisAngle(ax, ag);
    assert(float_eq(ag, angle_in, 1e-4f));
    assert(vec3_eq(ax, axis_in,   1e-4f));

    // Identidad no debe crashear
    Quaternion::Identity().ToAxisAngle(ax, ag);
    assert(float_eq(ag, 0.0f, 1e-4f));
    std::cout << "test_to_axis_angle: PASSED\n";
}

void test_slerp() {
    Quaternion a = Quaternion::Identity();
    Quaternion b = Quaternion::FromAxisAngle(Vector3D(0,1,0), PI/2.0f);

    assert(quat_eq(Quaternion::Slerp(a, b, 0.0f), a));
    assert(quat_eq(Quaternion::Slerp(a, b, 1.0f), b));
    assert(quat_eq(Quaternion::Slerp(a, b, 0.5f),
                   Quaternion::FromAxisAngle(Vector3D(0,1,0), PI/4.0f), 1e-4f));

    for (float t = 0.0f; t <= 1.0f; t += 0.1f)
        assert(Quaternion::Slerp(a, b, t).IsUnit());

    assert(quat_eq(Quaternion::Slerp(a, b, -0.5f), a));
    assert(quat_eq(Quaternion::Slerp(a, b,  1.5f), b));

    Quaternion b_neg(-b[0],-b[1],-b[2],-b[3]);
    assert(quat_eq(Quaternion::Slerp(a, b, 0.5f),
                   Quaternion::Slerp(a, b_neg, 0.5f), 1e-4f));

    std::cout << "test_slerp: PASSED\n";
}

void test_nlerp() {
    Quaternion a = Quaternion::Identity();
    Quaternion b = Quaternion::FromAxisAngle(Vector3D(0,0,1), PI/2.0f);
    assert(quat_eq(Quaternion::Nlerp(a, b, 0.0f), a));
    assert(quat_eq(Quaternion::Nlerp(a, b, 1.0f), b));
    for (float t = 0.0f; t <= 1.0f; t += 0.1f)
        assert(Quaternion::Nlerp(a, b, t).IsUnit());
    std::cout << "test_nlerp: PASSED\n";
}

void test_derivative() {
    // Identity + omega=(0,1,0) -> dq/dt = (0, 0, 0.5, 0)
    Quaternion dq = Quaternion::Identity().Derivative(Vector3D(0,1,0));
    assert(float_eq(dq[0], 0.0f)); assert(float_eq(dq[1], 0.0f));
    assert(float_eq(dq[2], 0.5f)); assert(float_eq(dq[3], 0.0f));
    std::cout << "test_derivative: PASSED\n";
}

void test_to_rotation_matrix() {
    assert(Quaternion::Identity().ToRotationMatrix() == Matrix3D::Identity());

    Quaternion qz = Quaternion::FromAxisAngle(Vector3D(0,0,1), PI/2.0f);
    Matrix3D mz = qz.ToRotationMatrix();
    assert(mz == Matrix3D::RotationZ(PI/2.0f));
    assert(mz.IsOrthogonal());
    assert(float_eq(std::abs(mz.Determinant()), 1.0f));

    Matrix3D m_orig = Matrix3D::RotationX(PI/5.0f);
    assert(Quaternion::FromRotationMatrix(m_orig).ToRotationMatrix() == m_orig);

    std::cout << "test_to_rotation_matrix: PASSED\n";
}

void test_to_transform_matrix() {
    assert(Quaternion::Identity().ToTransformMatrix() == Matrix4D::Identity());

    Quaternion q = Quaternion::FromAxisAngle(Vector3D(1,0,0), PI/4.0f);
    Matrix4D m4 = q.ToTransformMatrix();
    assert(m4.GetUpper3x3() == q.ToRotationMatrix());

    assert(float_eq(m4(3,0),0.0f)); assert(float_eq(m4(3,3),1.0f));
    assert(float_eq(m4(0,3),0.0f)); assert(float_eq(m4(2,3),0.0f));

    std::cout << "test_to_transform_matrix: PASSED\n";
}

// Construye cuaterniones directamente para no depender de FromEulerAngles.
// ToEulerAngles retorna Vector3D(pitch, yaw, roll).
void test_to_euler_angles() {
    assert(vec3_eq(Quaternion::Identity().ToEulerAngles(), Vector3D(0,0,0), 1e-4f));

    const float a = PI / 4.0f, ha = a * 0.5f;

    // Pure pitch (X): q = (cos(a/2), sin(a/2), 0, 0)
    {
        Quaternion q(std::cos(ha), std::sin(ha), 0.0f, 0.0f);
        Vector3D e = q.ToEulerAngles();
        assert(float_eq(e.x(), a,    1e-4f));
        assert(float_eq(e.y(), 0.0f, 1e-4f));
        assert(float_eq(e.z(), 0.0f, 1e-4f));
    }
    // Pure yaw (Y): q = (cos(a/2), 0, sin(a/2), 0)
    {
        Quaternion q(std::cos(ha), 0.0f, std::sin(ha), 0.0f);
        Vector3D e = q.ToEulerAngles();
        assert(float_eq(e.x(), 0.0f, 1e-4f));
        assert(float_eq(e.y(), a,    1e-4f));
        assert(float_eq(e.z(), 0.0f, 1e-4f));
    }
    // Pure roll (Z): q = (cos(a/2), 0, 0, sin(a/2))
    {
        Quaternion q(std::cos(ha), 0.0f, 0.0f, std::sin(ha));
        Vector3D e = q.ToEulerAngles();
        assert(float_eq(e.x(), 0.0f, 1e-4f));
        assert(float_eq(e.y(), 0.0f, 1e-4f));
        assert(float_eq(e.z(), a,    1e-4f));
    }
    std::cout << "test_to_euler_angles: PASSED\n";
}

void test_is_unit_and_is_pure() {
    assert( Quaternion::Identity().IsUnit());
    assert(!Quaternion(2, 0, 0, 0).IsUnit());
    assert( Quaternion(1,2,3,4).Normalized().IsUnit());

    assert(!Quaternion::Identity().IsPure());
    assert( Quaternion(0, 1, 0, 0).IsPure());
    assert( Quaternion(0, 0, 0, 0).IsPure());
    assert(!Quaternion(0.5f, 1, 0, 0).IsPure());

    std::cout << "test_is_unit_and_is_pure: PASSED\n";
}

void test_comparison() {
    Quaternion a(1,2,3,4), b(1,2,3,4), c(5,6,7,8);
    assert(a == b); assert(!(a == c));
    assert(a != c); assert(!(a != b));
    // Tolerancia numerica
    assert(a == Quaternion(1.000001f, 2.000001f, 3.000001f, 4.000001f));
    std::cout << "test_comparison: PASSED\n";
}

void test_exponential_and_logarithm() {
    // exp(log(q)) ~ q  para cuaternion unitario
    Quaternion q = Quaternion::FromAxisAngle(Vector3D(1,0,0), PI/4.0f);
    assert(quat_eq(q.Logarithm().Exponential(), q, 1e-4f));

    // log(Identity) ~ (0,0,0,0)
    Quaternion log_id = Quaternion::Identity().Logarithm();
    for (int i = 0; i < 4; ++i) assert(float_eq(log_id[i], 0.0f, 1e-4f));

    // log de magnitud cero lanza excepcion
    bool caught = false;
    try { PHYS_UNUSED(Quaternion(0,0,0,0).Logarithm()); } catch (const std::domain_error &) { caught = true; }
    assert(caught);

    std::cout << "test_exponential_and_logarithm: PASSED\n";
}

void test_data_pointer() {
    Quaternion q(1,2,3,4);
    const float *data = q.Data();
    assert(float_eq(data[0],1.0f)); assert(float_eq(data[1],2.0f));
    assert(float_eq(data[2],3.0f)); assert(float_eq(data[3],4.0f));

    q.Data()[0] = 10.0f;
    assert(float_eq(q[0], 10.0f));
    std::cout << "test_data_pointer: PASSED\n";
}

int main() {
    std::cout << "Running Quaternion tests...\n\n";
    try {
        test_constructors();
        test_identity();
        test_index_accessor();
        test_from_axis_angle();
        test_from_euler_angles();
        test_from_rotation_matrix();
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