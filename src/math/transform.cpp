#include "physics/math/transform.hpp"

Transform::Transform(const Vector3D &position, const Quaternion &rotation, const Vector3D &scale)
    : position_(position), rotation_(rotation), scale_(scale) {}

Transform Transform::Identity() {
    return Transform();
}

Matrix4D Transform::ToMatrix4D() const {
    Matrix3D rotMatrix = rotation_.ToRotationMatrix();
    return Matrix4D::TRS(position_, rotMatrix, scale_);
}

Transform Transform::Combine(const Transform &parent, const Transform &child) {
    Vector3D newPosition = parent.TransformPoint(child.GetPosition());
    Quaternion newRotation = parent.GetRotation() * child.GetRotation();
    
    const Vector3D &ps = parent.GetScale();
    const Vector3D &cs = child.GetScale();
    Vector3D newScale(ps.x() * cs.x(), ps.y() * cs.y(), ps.z() * cs.z());

    return Transform(newPosition, newRotation, newScale);
}

Transform Transform::Inverted() const {
    // Nota: la inversa exacta de un TRS con escala no uniforme no es otro TRS.
    // Este resultado es exacto solo para escala uniforme. Para el caso general,
    // usar InverseTransformPoint directamente.
    PHYS_ASSERT_MSG(std::abs(scale_.x()) > PHYS_EPSILON, "Escala X es cero");
    PHYS_ASSERT_MSG(std::abs(scale_.y()) > PHYS_EPSILON, "Escala Y es cero");
    PHYS_ASSERT_MSG(std::abs(scale_.z()) > PHYS_EPSILON, "Escala Z es cero");

    Quaternion invRotation = rotation_.Conjugate(); // para unitarios, Conjugate == Inverse
    Vector3D invScale(1.0f / scale_.x(), 1.0f / scale_.y(), 1.0f / scale_.z());
    Vector3D invPosition = invRotation.Rotate(
            Vector3D(-position_.x() * invScale.x(),
                    -position_.y() * invScale.y(),
                    -position_.z() * invScale.z())
        );
    return Transform(invPosition, invRotation, invScale);
}

Transform Transform::Lerp(const Transform& a, const Transform& b, real t) {
    Vector3D newPos = a.GetPosition() + (b.GetPosition() - a.GetPosition()) * t;
    Quaternion newRot = Quaternion::Slerp(a.GetRotation(), b.GetRotation(), t);
    Vector3D newScale = a.GetScale() + (b.GetScale() - a.GetScale()) * t;
    return Transform(newPos, newRot, newScale);
}