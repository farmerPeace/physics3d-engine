#include <cmath>
#include <stdexcept>

#include "physics/math/quaternion.hpp"
#include "physics/math/vector3d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/matrix4d.hpp"

#define EPSILON 1e-6f

// Constructures
Quaternion::Quaternion(): w_(1.0f), x_(0.0f), y_(0.0f), z_(0.0f) {}
Quaternion::Quaternion(float w, float x, float y, float z): w_(w), x_(x), y_(y), z_(z) {}
Quaternion::Quaternion(float w, const Vector3D &vect): w_(w), x_(vect.x()), y_(vect.y()), z_(vect.z()) {}

// Métodos estaticos de construccion
Quaternion Quaternion::Identity () {
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion Quaternion::FromAxisAngle(const Vector3D &axis, float angle) {
    Vector3D norm_axis = axis.Normalized();
    const float half = angle * 0.5f;
    const float sin = std::sin(half);
    const float cos = std::cos(half);

    return Quaternion(cos, norm_axis.x() * sin, norm_axis.y() * sin, norm_axis.z() * sin);
}

Quaternion Quaternion::FromEulerAngles(float roll, float pitch, float yaw) {
    // X -> Pitch, Y -> Yaw, Z -> Roll
    const float half_pitch = pitch * 0.5f;
    const float half_yaw = yaw * 0.5f;
    const float half_roll = roll * 0.5f;

    const float cos_pitch = std::cos(half_pitch), sin_pitch = std::sin(half_pitch);
    const float cos_yaw = std::cos(half_yaw), sin_yaw = std::sin(half_yaw);
    const float cos_roll = std::cos(half_roll), sin_roll = std::sin(half_roll);

    return Quaternion(
        cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw,
        cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw,
        cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw,
        sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw
    );
}

Quaternion Quaternion::FromRotationMatrix(const Matrix3D &m) {
    // Método de Shepperd para estabilidad numérica
    const float trace = m(0,0) + m(1,1) + m(2,2);
 
    if (trace > 0.0f) {
        const float s = 0.5f / std::sqrt(trace + 1.0f);
        return Quaternion(
            0.25f / s,
            (m(2,1) - m(1,2)) * s,
            (m(0,2) - m(2,0)) * s,
            (m(1,0) - m(0,1)) * s
        );
    } else if (m(0,0) > m(1,1) && m(0,0) > m(2,2)) {
        const float s = 2.0f * std::sqrt(1.0f + m(0,0) - m(1,1) - m(2,2));
        return Quaternion(
            (m(2,1) - m(1,2)) / s,
            0.25f * s,
            (m(0,1) + m(1,0)) / s,
            (m(0,2) + m(2,0)) / s
        );
    } else if (m(1,1) > m(2,2)) {
        const float s = 2.0f * std::sqrt(1.0f + m(1,1) - m(0,0) - m(2,2));
        return Quaternion(
            (m(0,2) - m(2,0)) / s,
            (m(0,1) + m(1,0)) / s,
            0.25f * s,
            (m(1,2) + m(2,1)) / s
        );
    } else {
        const float s = 2.0f * std::sqrt(1.0f + m(2,2) - m(0,0) - m(1,1));
        return Quaternion(
            (m(1,0) - m(0,1)) / s,
            (m(0,2) + m(2,0)) / s,
            (m(1,2) + m(2,1)) / s,
            0.25f * s
        );
    }
}
 
// ─────────────────────────────────────────────
// Acceso por índice
// ─────────────────────────────────────────────
 
float Quaternion::operator [] (int index) const {
    if (index == 0) return w_;
    if (index == 1) return x_;
    if (index == 2) return y_;
    if (index == 3) return z_;
    throw std::out_of_range("Indice Quaternion fuera de rango");
}
 
float &Quaternion::operator [] (int index) {
    if (index == 0) return w_;
    if (index == 1) return x_;
    if (index == 2) return y_;
    if (index == 3) return z_;
    throw std::out_of_range("Indice Quaternion fuera de rango");
}
 
// ─────────────────────────────────────────────
// Operadores suma y resta
// ─────────────────────────────────────────────
 
Quaternion &Quaternion::operator += (const Quaternion &rhs) {
    w_ += rhs.w_;
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
}
 
Quaternion &Quaternion::operator -= (const Quaternion &rhs) {
    w_ -= rhs.w_;
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
}
 
Quaternion Quaternion::operator + (const Quaternion &rhs) const {
    return Quaternion(w_ + rhs.w_, x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
}
 
Quaternion Quaternion::operator - (const Quaternion &rhs) const {
    return Quaternion(w_ - rhs.w_, x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_);
}
 
Quaternion Quaternion::operator - () const {
    return Quaternion(-w_, -x_, -y_, -z_);
}
 
// ─────────────────────────────────────────────
// Operadores con escalar
// ─────────────────────────────────────────────
 
Quaternion &Quaternion::operator *= (const float &scalar) {
    w_ *= scalar;
    x_ *= scalar;
    y_ *= scalar;
    z_ *= scalar;
    return *this;
}
 
Quaternion &Quaternion::operator /= (const float &scalar) {
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por (casi) cero en un Quaternion");
    }
    const float inv = 1.0f / scalar;
    w_ *= inv;
    x_ *= inv;
    y_ *= inv;
    z_ *= inv;
    return *this;
}
 
Quaternion Quaternion::operator * (const float &scalar) const {
    return Quaternion(w_ * scalar, x_ * scalar, y_ * scalar, z_ * scalar);
}
 
Quaternion Quaternion::operator / (const float &scalar) const {
    if (std::abs(scalar) < EPSILON) {
        throw std::domain_error("Division por (casi) cero en un Quaternion");
    }
    const float inv = 1.0f / scalar;
    return Quaternion(w_ * inv, x_ * inv, y_ * inv, z_ * inv);
}
 
// Conmutativa global
Quaternion operator * (float scalar, const Quaternion &q) {
    return q * scalar;
}
 
// ─────────────────────────────────────────────
// Multiplicación de cuaterniones  q1 * q2
// ─────────────────────────────────────────────
 
Quaternion &Quaternion::operator *= (const Quaternion &rhs) {
    const float w = w_ * rhs.w_ - x_ * rhs.x_ - y_ * rhs.y_ - z_ * rhs.z_;
    const float x = w_ * rhs.x_ + x_ * rhs.w_ + y_ * rhs.z_ - z_ * rhs.y_;
    const float y = w_ * rhs.y_ - x_ * rhs.z_ + y_ * rhs.w_ + z_ * rhs.x_;
    const float z = w_ * rhs.z_ + x_ * rhs.y_ - y_ * rhs.x_ + z_ * rhs.w_;
 
    w_ = w; x_ = x; y_ = y; z_ = z;
    return *this;
}
 
Quaternion Quaternion::operator * (const Quaternion &rhs) const {
    return Quaternion(
        w_ * rhs.w_ - x_ * rhs.x_ - y_ * rhs.y_ - z_ * rhs.z_,
        w_ * rhs.x_ + x_ * rhs.w_ + y_ * rhs.z_ - z_ * rhs.y_,
        w_ * rhs.y_ - x_ * rhs.z_ + y_ * rhs.w_ + z_ * rhs.x_,
        w_ * rhs.z_ + x_ * rhs.y_ - y_ * rhs.x_ + z_ * rhs.w_
    );
}
 
// ─────────────────────────────────────────────
// Producto punto
// ─────────────────────────────────────────────
 
float Quaternion::Dot (const Quaternion &rhs) const {
    return w_ * rhs.w_ + x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
}
 
// ─────────────────────────────────────────────
// Conjugado e inverso
// ─────────────────────────────────────────────
 
Quaternion Quaternion::Conjugate () const {
    return Quaternion(w_, -x_, -y_, -z_);
}
 
Quaternion Quaternion::Inverse () const {
    const float sq_mag = SquareMagnitude();
    if (sq_mag < EPSILON) {
        throw std::domain_error("No se puede invertir un Quaternion de magnitud cero");
    }
    return Conjugate() / sq_mag;
}
 
// ─────────────────────────────────────────────
// Magnitud
// ─────────────────────────────────────────────
 
float Quaternion::Magnitude () const {
    return std::sqrt(w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_);
}
 
float Quaternion::SquareMagnitude () const {
    return w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_;
}
 
// ─────────────────────────────────────────────
// Normalización
// ─────────────────────────────────────────────
 
Quaternion &Quaternion::Normalize () {
    const float mag = Magnitude();
    if (mag < EPSILON) {
        throw std::domain_error("No se puede normalizar un Quaternion de magnitud cero");
    }
    const float inv = 1.0f / mag;
    w_ *= inv;
    x_ *= inv;
    y_ *= inv;
    z_ *= inv;
    return *this;
}
 
Quaternion Quaternion::Normalized () const {
    Quaternion result = *this;
    result.Normalize();
    return result;
}
 
// ─────────────────────────────────────────────
// Rotar un vector  v' = q * v * q⁻¹
// ─────────────────────────────────────────────
 
Vector3D Quaternion::Rotate (const Vector3D &vec) const {
    // Optimización de Rodrigues sin construir cuaternión intermedio:
    // v' = v + 2w(q_vec × v) + 2(q_vec × (q_vec × v))
    const Vector3D q_vec(x_, y_, z_);
    const Vector3D t = q_vec.Cross3D(vec) * 2.0f;
    return vec + t * w_ + q_vec.Cross3D(t);
}
 
// ─────────────────────────────────────────────
// Eje y ángulo
// ─────────────────────────────────────────────
 
void Quaternion::ToAxisAngle (Vector3D &axis, float &angle) const {
    Quaternion n = Normalized();
    angle = 2.0f * std::acos(std::max(-1.0f, std::min(1.0f, n.w_)));
 
    const float s = std::sqrt(1.0f - n.w_ * n.w_);
    if (s < EPSILON) {
        // Ángulo ≈ 0: eje arbitrario
        axis = Vector3D(1.0f, 0.0f, 0.0f);
    } else {
        const float inv_s = 1.0f / s;
        axis = Vector3D(n.x_ * inv_s, n.y_ * inv_s, n.z_ * inv_s);
    }
}
 
 
// ─────────────────────────────────────────────
// Interpolaciones estáticas
// ─────────────────────────────────────────────
 
Quaternion Quaternion::Slerp (const Quaternion &a, const Quaternion &b, float t) {
    t = std::max(0.0f, std::min(1.0f, t));
 
    Quaternion qb = b;
    float dot = a.Dot(b);
 
    // Elegir el camino más corto
    if (dot < 0.0f) {
        qb  = -b;
        dot = -dot;
    }
 
    // Para ángulos muy pequeños usar Nlerp (más estable)
    if (dot > 1.0f - EPSILON) {
        return Nlerp(a, qb, t);
    }
 
    const float theta_0  = std::acos(dot);
    const float theta    = theta_0 * t;
    const float sin_t    = std::sin(theta);
    const float sin_t0   = std::sin(theta_0);
 
    const float s0 = std::cos(theta) - dot * sin_t / sin_t0;
    const float s1 = sin_t / sin_t0;
 
    return (a * s0 + qb * s1).Normalized();
}
 
Quaternion Quaternion::Nlerp (const Quaternion &a, const Quaternion &b, float t) {
    t = std::max(0.0f, std::min(1.0f, t));
 
    Quaternion qb = b;
    if (a.Dot(b) < 0.0f) {
        qb = -b;
    }
 
    return (a * (1.0f - t) + qb * t).Normalized();
}
 
// ─────────────────────────────────────────────
// Derivada temporal  dq/dt = 0.5 * ω * q
// ─────────────────────────────────────────────
 
Quaternion Quaternion::Derivative (const Vector3D &angularVelocity) const {
    const Quaternion omega(0.0f, angularVelocity.x(),
                                 angularVelocity.y(),
                                 angularVelocity.z());
    return (omega * *this) * 0.5f;
}
 
// ─────────────────────────────────────────────
// Conversiones
// ─────────────────────────────────────────────
 
Matrix3D Quaternion::ToRotationMatrix () const {
    Quaternion n = Normalized();
 
    const float w = n.w_, x = n.x_, y = n.y_, z = n.z_;
    const float x2 = x*x, y2 = y*y, z2 = z*z;
    const float xy = x*y, xz = x*z, yz = y*z;
    const float wx = w*x, wy = w*y, wz = w*z;
 
    return Matrix3D(
        1.0f - 2.0f*(y2 + z2),   2.0f*(xy - wz),         2.0f*(xz + wy),
        2.0f*(xy + wz),           1.0f - 2.0f*(x2 + z2),  2.0f*(yz - wx),
        2.0f*(xz - wy),           2.0f*(yz + wx),          1.0f - 2.0f*(x2 + y2)
    );
}
 
Matrix4D Quaternion::ToTransformMatrix () const {
    Quaternion n = Normalized();
 
    const float w = n.w_, x = n.x_, y = n.y_, z = n.z_;
    const float x2 = x*x, y2 = y*y, z2 = z*z;
    const float xy = x*y, xz = x*z, yz = y*z;
    const float wx = w*x, wy = w*y, wz = w*z;
 
    return Matrix4D(
        1.0f - 2.0f*(y2 + z2),  2.0f*(xy - wz),         2.0f*(xz + wy),         0.0f,
        2.0f*(xy + wz),          1.0f - 2.0f*(x2 + z2),  2.0f*(yz - wx),         0.0f,
        2.0f*(xz - wy),          2.0f*(yz + wx),          1.0f - 2.0f*(x2 + y2), 0.0f,
        0.0f,                    0.0f,                    0.0f,                   1.0f
    );
}
 
Vector3D Quaternion::ToEulerAngles () const {
    Quaternion n = Normalized();
 
    const float w = n.w_, x = n.x_, y = n.y_, z = n.z_;
 
    // Pitch (X)
    const float sin_pitch = 2.0f * (w * x + y * z);
    const float cos_pitch = 1.0f - 2.0f * (x * x + y * y);
    const float pitch = std::atan2(sin_pitch, cos_pitch);
 
    // Yaw (Y) — clamp para evitar NaN en el dominio de asin
    const float sin_yaw = 2.0f * (w * y - z * x);
    const float yaw = std::asin(std::max(-1.0f, std::min(1.0f, sin_yaw)));
 
    // Roll (Z)
    const float sin_roll = 2.0f * (w * z + x * y);
    const float cos_roll = 1.0f - 2.0f * (y * y + z * z);
    const float roll = std::atan2(sin_roll, cos_roll);
 
    return Vector3D(pitch, yaw, roll);
}
 
// ─────────────────────────────────────────────
// Verificaciones
// ─────────────────────────────────────────────
 
bool Quaternion::IsUnit () const {
    return std::abs(SquareMagnitude() - 1.0f) < EPSILON;
}
 
bool Quaternion::IsPure () const {
    return std::abs(w_) < EPSILON;
}
 
// ─────────────────────────────────────────────
// Comparación
// ─────────────────────────────────────────────
 
bool Quaternion::operator == (const Quaternion &rhs) const {
    return (std::abs(w_ - rhs.w_) <= EPSILON &&
            std::abs(x_ - rhs.x_) <= EPSILON &&
            std::abs(y_ - rhs.y_) <= EPSILON &&
            std::abs(z_ - rhs.z_) <= EPSILON);
}
 
bool Quaternion::operator != (const Quaternion &rhs) const {
    return !(*this == rhs);
}
 
// ─────────────────────────────────────────────
// Exponencial y logaritmo
// ─────────────────────────────────────────────
 
Quaternion Quaternion::Exponential () const {
    // exp(q) = e^w * (cos|v| + v_hat * sin|v|)
    const Vector3D v(x_, y_, z_);
    const float v_mag = v.Magnitude();
    const float e_w   = std::exp(w_);
 
    if (v_mag < EPSILON) {
        return Quaternion(e_w, 0.0f, 0.0f, 0.0f);
    }
 
    const float sin_v = std::sin(v_mag);
    const float scale = e_w * sin_v / v_mag;
 
    return Quaternion(e_w * std::cos(v_mag), x_ * scale, y_ * scale, z_ * scale);
}
 
Quaternion Quaternion::Logarithm () const {
    // log(q) = log|q| + v_hat * acos(w / |q|)
    const float mag   = Magnitude();
    if (mag < EPSILON) {
        throw std::domain_error("Logaritmo de Quaternion con magnitud cero no esta definido");
    }
 
    const Vector3D v(x_, y_, z_);
    const float v_mag = v.Magnitude();
    const float log_w = std::log(mag);
 
    if (v_mag < EPSILON) {
        return Quaternion(log_w, 0.0f, 0.0f, 0.0f);
    }
 
    const float angle = std::acos(std::max(-1.0f, std::min(1.0f, w_ / mag)));
    const float scale = angle / v_mag;
 
    return Quaternion(log_w, x_ * scale, y_ * scale, z_ * scale);
}
 
// ─────────────────────────────────────────────
// Data
// ─────────────────────────────────────────────
 
const float *Quaternion::Data () const {
    return &w_;
}
 
float *Quaternion::Data () {
    return &w_;
}