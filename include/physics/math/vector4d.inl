#ifndef VECTOR4D_INL
#define VECTOR4D_INL

#include <type_traits>
#include <cmath>

// Operadres multiplicacion y division con escalar in place
template <typename Scalar>
PHYS_FORCE_INLINE Vector4D &Vector4D::operator *= (const Scalar &scalar) {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar debe ser arimetico");

    x_ *= static_cast<float>(scalar);
    y_ *= static_cast<float>(scalar);
    z_ *= static_cast<float>(scalar);
    w_ *= static_cast<float>(scalar);

    return *this;
}

template <typename Scalar>
PHYS_FORCE_INLINE Vector4D &Vector4D::operator /= (const Scalar &scalar) {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar debe ser arimetrico");

    const float scalar_f = static_cast<float>(scalar);

    if (std::abs(scalar_f) < PHYS_EPSILON) {
        throw std::domain_error("Division por (casi) cero en un Vector4D");
    }

    const float inv = 1.0f / scalar_f;

    x_ *= inv;
    y_ *= inv;
    z_ *= inv;
    w_ *= inv;

    return *this;
}

// Operadores multiplicacion y division con escalar 
template <typename Scalar>
PHYS_NODISCARD PHYS_FORCE_INLINE Vector4D Vector4D::operator * (const Scalar &scalar) const {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar debe ser aritmetico");

    return Vector4D(x_ * scalar, y_ * scalar, z_ * scalar, w_ * scalar);
}

template <typename Scalar>
PHYS_NODISCARD PHYS_FORCE_INLINE Vector4D Vector4D::operator / (const Scalar &scalar) const {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar debe ser aritmetico");

    const float scalar_f = static_cast<float>(scalar);

    if (std::abs(scalar_f) < PHYS_EPSILON) {
        throw std::domain_error("Division por (casi) cero en un Vector4D");
    }

    const float inv = 1.0f / scalar_f;

    return Vector4D(x_ * inv, y_ * inv, z_ * inv, w_ * inv);
}


#endif