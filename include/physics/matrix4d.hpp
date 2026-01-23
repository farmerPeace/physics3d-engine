#pragma once

class Matrix4D {
private:
    float matrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

public:
    // Constructores
    Matrix4D();

    Matrix4D(
        float m00, float m01, float m02, float m04,
        float m10, float m11, float m12, float m14,
        float m20, float m21, float m22, float m24,
        float m30, float m31, float m32, float m34
    );

    explicit Matrix4D(const float data[16]);

    // Acceso por vectores como columnas
    
};