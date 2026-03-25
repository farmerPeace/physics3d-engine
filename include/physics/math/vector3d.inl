#ifndef VECTOR3D_INL
#define VECTOR3D_INL

#include <type_traits>
#include <limits>
#include <cmath>

template <typename Scalar>
PHYS_FORCE_INLINE Vector3D &Vector3D::operator *= (const Scalar &scalar) {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar debe ser tipo aritmetico");

    x_ *= static_cast<float>(scalar);
    y_ *= static_cast<float>(scalar);
    z_ *= static_cast<float>(scalar);

    return *this;
}

template <typename Scalar>
PHYS_FORCE_INLINE Vector3D &Vector3D::operator /= (const Scalar &scalar) {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar debe ser tipo aritmetico");
    
    const float scalar_f = static_cast<float>(scalar);

    if (std::abs(scalar_f) < PHYS_EPSILON) {
        throw std::domain_error("División por casi cero en un Vector3D");
    }

    const float inv = 1.0f / scalar_f;
    x_ *= inv;
    y_ *= inv;
    z_ *= inv;

    return *this;
}

// Multiplicación escalar por ambos lados
template <typename Scalar>
PHYS_NODISCARD PHYS_FORCE_INLINE Vector3D operator *(const Scalar &scalar, Vector3D vect) {
    vect *= scalar;
    return vect;
}

template <typename Scalar>
PHYS_NODISCARD PHYS_FORCE_INLINE Vector3D operator *(Vector3D vect, const Scalar &scalar) {
    vect *= scalar;
    return vect;
}

// División escalar
template<typename Scalar>
PHYS_NODISCARD PHYS_FORCE_INLINE Vector3D operator / (Vector3D vect, const Scalar& scalar) {
    vect /= scalar;
    return vect;
}

#endif