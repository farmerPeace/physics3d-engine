#include "physics/math/matrix3d.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

const float PI = 3.14159265358979323846f;

// Función para imprimir matriz formateada
void printMatrix(const std::string& name, const Matrix3D& m) {
    std::cout << name << " =\n";
    for (int i = 0; i < 3; i++) {
        std::cout << "  [ ";
        for (int j = 0; j < 3; j++) {
            std::cout << std::fixed << std::setprecision(4) << std::setw(8) << m(i, j) << " ";
        }
        std::cout << "]\n";
    }
    std::cout << std::endl;
}

// Función para imprimir vector
void printVector(const std::string& name, const Vector3D& v) {
    std::cout << name << " = (" 
              << v.x() << ", " << v.y() << ", " << v.z() << ")\n";
}

void example1_basic_operations() {
    std::cout << "========================================\n";
    std::cout << "EXAMPLE 1: Basic Matrix Operations\n";
    std::cout << "========================================\n\n";
    
    // Crear matrices
    Matrix3D A(1, 2, 3,
              4, 5, 6,
              7, 8, 9);
    
    Matrix3D B(9, 8, 7,
              6, 5, 4,
              3, 2, 1);
    
    printMatrix("Matrix A", A);
    printMatrix("Matrix B", B);
    
    // Suma
    Matrix3D C = A + B;
    printMatrix("A + B", C);
    
    // Resta
    Matrix3D D = A - B;
    printMatrix("A - B", D);
    
    // Multiplicación por escalar
    Matrix3D E = A * 2.5f;
    printMatrix("A * 2.5", E);
    
    // Negativo
    Matrix3D F = -A;
    printMatrix("-A", F);
}

void example2_matrix_multiplication() {
    std::cout << "\n========================================\n";
    std::cout << "EXAMPLE 2: Matrix Multiplication\n";
    std::cout << "========================================\n\n";
    
    Matrix3D A(1, 0, 2,
              3, 1, 0,
              0, 2, 1);
    
    Matrix3D B(2, 1, 1,
              0, 1, 0,
              1, 0, 2);
    
    printMatrix("Matrix A", A);
    printMatrix("Matrix B", B);
    
    // Multiplicación de matrices
    Matrix3D C = A * B;
    printMatrix("A * B", C);
    
    // Multiplicación por identidad
    Matrix3D I = Matrix3D::Identity();
    Matrix3D D = A * I;
    std::cout << "A * Identity = A? " << (D == A ? "YES" : "NO") << "\n\n";
    
    // Multiplicación por cero
    Matrix3D Zero = Matrix3D::Zero();
    Matrix3D E = A * Zero;
    std::cout << "A * Zero is zero matrix? " << (E.IsZero() ? "YES" : "NO") << "\n\n";
}

void example3_transformations() {
    std::cout << "\n========================================\n";
    std::cout << "EXAMPLE 3: Geometric Transformations\n";
    std::cout << "========================================\n\n";
    
    // 1. Escalado
    Matrix3D scale = Matrix3D::Scale(2.0f, 3.0f, 1.5f);
    printMatrix("Scale(2, 3, 1.5)", scale);
    
    // 2. Rotaciones básicas
    float angle30 = PI / 6.0f;  // 30 grados
    float angle45 = PI / 4.0f;  // 45 grados
    
    Matrix3D rotX = Matrix3D::RotationX(angle30);
    printMatrix("RotationX(30°)", rotX);
    
    Matrix3D rotY = Matrix3D::RotationY(angle45);
    printMatrix("RotationY(45°)", rotY);
    
    Matrix3D rotZ = Matrix3D::RotationZ(PI/2);  // 90 grados
    printMatrix("RotationZ(90°)", rotZ);
    
    // 3. Rotación alrededor de eje arbitrario
    Vector3D axis(1, 1, 0);
    Matrix3D rotAxis = Matrix3D::Rotation(angle45, axis.Normalized());
    printMatrix("Rotation(45°, axis(1,1,0))", rotAxis);
    
    // 4. Composición de transformaciones
    Matrix3D transform = rotZ * scale;
    printMatrix("RotationZ(90°) * Scale(2,3,1.5)", transform);
}

void example4_inverse_and_transpose() {
    std::cout << "\n========================================\n";
    std::cout << "EXAMPLE 4: Inverse and Transpose\n";
    std::cout << "========================================\n\n";
    
    // Matriz diagonal (fácil de invertir)
    Matrix3D diag = Matrix3D::Scale(2, 3, 4);
    printMatrix("Diagonal Matrix D", diag);
    
    Matrix3D diagInv = diag.Inverted();
    printMatrix("Inverse of D", diagInv);
    
    // Verificar: D * D⁻¹ = I
    Matrix3D shouldBeIdentity = diag * diagInv;
    std::cout << "D * D⁻¹ is identity? " << (shouldBeIdentity.IsIdentity() ? "YES" : "NO") << "\n\n";
    
    // Transposición
    Matrix3D A(1, 2, 3,
              4, 5, 6,
              7, 8, 9);
    
    printMatrix("Matrix A", A);
    
    Matrix3D AT = A.Transposed();
    printMatrix("Transpose of A", AT);
    
    // Doble transposición
    Matrix3D ATT = AT.Transposed();
    std::cout << "(Aᵀ)ᵀ = A? " << (ATT == A ? "YES" : "NO") << "\n\n";
    
    // Transposición in-place
    Matrix3D B = A;
    B.Transpose();
    std::cout << "In-place transpose works? " << (B == AT ? "YES" : "NO") << "\n\n";
}

void example5_matrix_properties() {
    std::cout << "\n========================================\n";
    std::cout << "EXAMPLE 5: Matrix Properties\n";
    std::cout << "========================================\n\n";
    
    // 1. Matriz identidad
    Matrix3D I = Matrix3D::Identity();
    std::cout << "Identity matrix:\n";
    std::cout << "  IsIdentity? " << (I.IsIdentity() ? "YES" : "NO") << "\n";
    std::cout << "  IsOrthogonal? " << (I.IsOrthogonal() ? "YES" : "NO") << "\n";
    std::cout << "  IsSymmetric? " << (I.IsSymmetric() ? "YES" : "NO") << "\n";
    std::cout << "  Determinant: " << I.Determinant() << "\n";
    std::cout << "  IsInvertible? " << (I.IsInvertible() ? "YES" : "NO") << "\n\n";
    
    // 2. Matriz de rotación (ortogonal)
    Matrix3D rot = Matrix3D::RotationZ(PI/4);
    std::cout << "Rotation matrix (45° around Z):\n";
    std::cout << "  IsIdentity? " << (rot.IsIdentity() ? "YES" : "NO") << "\n";
    std::cout << "  IsOrthogonal? " << (rot.IsOrthogonal() ? "YES" : "NO") << "\n";
    std::cout << "  IsSymmetric? " << (rot.IsSymmetric() ? "YES" : "NO") << "\n";
    std::cout << "  Determinant: " << rot.Determinant() << "\n";
    std::cout << "  IsInvertible? " << (rot.IsInvertible() ? "YES" : "NO") << "\n\n";
    
    // 3. Matriz simétrica
    Matrix3D sym(1, 2, 3,
                2, 4, 5,
                3, 5, 6);
    std::cout << "Symmetric matrix:\n";
    std::cout << "  IsSymmetric? " << (sym.IsSymmetric() ? "YES" : "NO") << "\n";
    std::cout << "  IsOrthogonal? " << (sym.IsOrthogonal() ? "YES" : "NO") << "\n";
    std::cout << "  Determinant: " << sym.Determinant() << "\n\n";
    
    // 4. Matriz cero
    Matrix3D zero = Matrix3D::Zero();
    std::cout << "Zero matrix:\n";
    std::cout << "  IsZero? " << (zero.IsZero() ? "YES" : "NO") << "\n";
    std::cout << "  IsInvertible? " << (zero.IsInvertible() ? "YES" : "NO") << "\n\n";
}

void example6_vector_operations() {
    std::cout << "\n========================================\n";
    std::cout << "EXAMPLE 6: Matrix-Vector Operations\n";
    std::cout << "========================================\n\n";
    
    // Crear matriz de transformación
    Matrix3D transform = Matrix3D::RotationZ(PI/4) * Matrix3D::Scale(2, 1, 1);
    printMatrix("Transformation Matrix", transform);
    
    // Crear vector
    Vector3D point(1, 0, 0);
    printVector("Original point", point);
    
    // Obtener columnas de la matriz
    std::cout << "\nMatrix columns:\n";
    Vector3D col0 = transform.GetColumn(0);
    Vector3D col1 = transform.GetColumn(1);
    Vector3D col2 = transform.GetColumn(2);
    
    printVector("  Column 0 (X axis)", col0);
    printVector("  Column 1 (Y axis)", col1);
    printVector("  Column 2 (Z axis)", col2);
    
    // Obtener filas de la matriz
    std::cout << "\nMatrix rows:\n";
    Vector3D row0 = transform.GetRow(0);
    Vector3D row1 = transform.GetRow(1);
    Vector3D row2 = transform.GetRow(2);
    
    printVector("  Row 0", row0);
    printVector("  Row 1", row1);
    printVector("  Row 2", row2);
    
    // Modificar columnas
    Matrix3D modified = transform;
    modified.SetColumn(0, Vector3D(1, 0, 0));
    printMatrix("\nModified (first column changed)", modified);
}

void example7_interpolation() {
    std::cout << "\n========================================\n";
    std::cout << "EXAMPLE 7: Matrix Interpolation\n";
    std::cout << "========================================\n\n";
    
    // Interpolación entre dos matrices
    Matrix3D start = Matrix3D::Identity();
    Matrix3D end = Matrix3D::Scale(3, 3, 3);
    
    printMatrix("Start matrix (Identity)", start);
    printMatrix("End matrix (Scale 3x)", end);
    
    std::cout << "Interpolation results:\n";
    for (float t = 0.0f; t <= 1.01f; t += 0.25f) {
        Matrix3D interpolated = Matrix3D::Lerp(start, end, t);
        std::cout << "  t = " << t << ": ";
        std::cout << "diag = (" << interpolated(0,0) << ", " 
                  << interpolated(1,1) << ", " << interpolated(2,2) << ")\n";
    }
    
    std::cout << "\nClamping test:\n";
    Matrix3D before = Matrix3D::Lerp(start, end, -0.5f);
    Matrix3D after = Matrix3D::Lerp(start, end, 1.5f);
    
    std::cout << "  t = -0.5 (clamped to 0): ";
    std::cout << "diag = (" << before(0,0) << ", " << before(1,1) << ", " << before(2,2) << ")\n";
    
    std::cout << "  t = 1.5 (clamped to 1): ";
    std::cout << "diag = (" << after(0,0) << ", " << after(1,1) << ", " << after(2,2) << ")\n";
}

void example8_practical_physics_use() {
    std::cout << "\n========================================\n";
    std::cout << "EXAMPLE 8: Practical Physics Applications\n";
    std::cout << "========================================\n\n";
    
    // 1. Matriz de inercia (simplificada)
    std::cout << "1. Inertia Tensor (simplified cube)\n";
    float mass = 10.0f;
    float size = 2.0f;
    float I = (mass * size * size) / 6.0f;  // Para cubo uniforme
    
    Matrix3D inertia = Matrix3D::Scale(I, I, I);
    printMatrix("Inertia Tensor", inertia);
    
    // 2. Matriz de rotación para cambio de coordenadas
    std::cout << "2. Coordinate System Rotation\n";
    Matrix3D localToWorld = Matrix3D::RotationY(PI/3);
    printMatrix("Local to World rotation", localToWorld);
    
    // La inversa (World to Local) es la transpuesta para matrices ortogonales
    Matrix3D worldToLocal = localToWorld.Inverted();
    std::cout << "World to Local (inverse) = Transpose? ";
    std::cout << (worldToLocal == localToWorld.Transposed() ? "YES" : "NO") << "\n\n";
    
    // 3. Deformación por esfuerzo cortante
    std::cout << "3. Shear Deformation\n";
    Matrix3D shear(1.0f, 0.2f, 0.0f,
                  0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 1.0f);
    printMatrix("Shear Matrix", shear);
    std::cout << "Determinant (volume preservation): " << shear.Determinant() << "\n\n";
    
    // 4. Composición de transformaciones rígidas
    std::cout << "4. Composite Rigid Transformation\n";
    Matrix3D scale = Matrix3D::Scale(0.5f, 2.0f, 1.0f);
    Matrix3D rotate = Matrix3D::RotationZ(PI/6);
    Matrix3D composite = rotate * scale;  // Primero escala, luego rota
    
    printMatrix("Scale", scale);
    printMatrix("Rotate", rotate);
    printMatrix("Composite (Rotate * Scale)", composite);
    
    std::cout << "Composite is orthogonal? " << (composite.IsOrthogonal() ? "YES" : "NO") << "\n";
    std::cout << "Determinant: " << composite.Determinant() << "\n";
}

int main() {
    std::cout << "MATRIX3 LIBRARY EXAMPLES\n";
    std::cout << "=========================\n\n";
    
    std::cout << "This file demonstrates various uses of the Matrix3D class\n";
    std::cout << "for 3D physics and graphics applications.\n\n";
    
    example1_basic_operations();
    example2_matrix_multiplication();
    example3_transformations();
    example4_inverse_and_transpose();
    example5_matrix_properties();
    example6_vector_operations();
    example7_interpolation();
    example8_practical_physics_use();
    
    std::cout << "\n========================================\n";
    std::cout << "ALL EXAMPLES COMPLETED SUCCESSFULLY!\n";
    std::cout << "========================================\n";
    
    return 0;
}