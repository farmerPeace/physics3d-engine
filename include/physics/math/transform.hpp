#pragma once

#include "physics/core/config.hpp"
#include "physics/core/macros.hpp"

#include "physics/math/vector3d.hpp"
#include "physics/math/matrix4d.hpp"
#include "physics/math/quaternion.hpp"

class Transform {
private:
    Vector3D position_ {0, 0, 0};
    Quaternion rotation_ {1, 0, 0, 0};
    Vector3D scale_ {1, 1, 1};

public:
    // Constructores
    Transform() = default;
    Transform(const Vector3D &position, const Quaternion &rotation, const Vector3D &scale);
    static Transform Identity();

    // Getters
    PHYS_NODISCARD PHYS_FORCE_INLINE const Vector3D &GetPosition() const { return position_; };
    PHYS_NODISCARD PHYS_FORCE_INLINE const Quaternion &GetRotation() const { return rotation_; };
    PHYS_NODISCARD PHYS_FORCE_INLINE const Vector3D &GetScale() const { return scale_; };

    // Setters
    PHYS_FORCE_INLINE void SetPosition(const Vector3D &position) { position_ = position; };
    PHYS_FORCE_INLINE void SetRotation(const Quaternion &rotation) { rotation_ = rotation; };
    PHYS_FORCE_INLINE void SetScale(const Vector3D &scale) {scale_ = scale; };

    // Métodos de transformación
    PHYS_FORCE_INLINE Vector3D TransformPoint(const Vector3D &point) const {
        Vector3D scaled(
            point.x() * scale_.x(),
            point.y() * scale_.y(),
            point.z() * scale_.z()
        );

        Vector3D rotated = rotation_.Rotate(scaled);

        return position_ + rotated;
    }

    PHYS_FORCE_INLINE const Vector3D TransformDirection(const Vector3D &direction) const {
        return rotation_.Rotate(direction);
    }

    PHYS_FORCE_INLINE const Vector3D TransformVector(const Vector3D &vector) const {
        Vector3D scaled(
            vector.x() * scale_.x(),
            vector.y() * scale_.y(),
            vector.z() * scale_.z()
        );

        return rotation_.Rotate(scaled);
    }

    // Metodos de transformación inversa
    PHYS_FORCE_INLINE Vector3D InverseTransformPoint(const Vector3D &point) const {
        // Traslacion inversa
        Vector3D translated (
            point.x() - position_.x(),
            point.y() - position_.y(),
            point.z() - position_.z()
        );

        // Rotacion inversa
        Vector3D rotated = rotation_.Conjugate().Rotate(translated);

        // Escala inversa
        return Vector3D(
            rotated.x() / scale_.x(),
            rotated.y() / scale_.y(),
            rotated.z() / scale_.z()
        );
    }

    PHYS_FORCE_INLINE Vector3D InverseTransformDirection(const Vector3D &direction) const {
        // Unicamente rotacion inversa
        return rotation_.Conjugate().Rotate(direction);
    }

    PHYS_FORCE_INLINE Vector3D InverseTransformVector(const Vector3D &vector) const {
        // Rotación y escala inversas
        Vector3D rotated = rotation_.Conjugate().Rotate(vector);
        return Vector3D(
            rotated.x() / scale_.x(),
            rotated.y() / scale_.y(),
            rotated.z() / scale_.z()
        );
    }

    // Conversión a matriz de transformación
    Matrix4D ToMatrix4D() const;

    // Composición e inversa
    static Transform Combine(const Transform &parent, const Transform &child);
    Transform Inverted() const;
    static Transform Lerp(const Transform &a, const Transform &b, real t);
};
