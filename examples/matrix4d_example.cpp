#include "physics/math/matrix4d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/vector3d.hpp"
#include "physics/math/vector4d.hpp"
#include "physics/core/config.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

/*
El archivo cubre 12 ejemplos progresivos:

1. **Constructores y matrices especiales** — los 5 constructores, `Identity()`, `Zero()`
2. **Transformaciones básicas** — traslación, escala, rotaciones elementales y por eje arbitrario
3. **TRS** — `Translation × Rotation × Scale` combinados, transformando puntos y direcciones
4. **Aritmética** — suma, resta, escalar, negación, operadores in-place
5. **Multiplicación y transformación de vectores** — composición de matrices, diferencia entre punto (`w=1`) y dirección (`w=0`)
6. **Transpuesta** — `Transposed()`, `Transpose()` in-place y la propiedad `(A·B)ᵀ = Bᵀ·Aᵀ`
7. **Determinante e inversa** — `Determinant()`, `Inverted()`, `Invert()`, `IsInvertible()`
8. **Columnas, filas y submatrices** — `GetColumn/Row`, `GetUpper3x3`, `GetTranslation`, sus setters
9. **Proyección perspectiva y ortográfica** — con verificación de las propiedades OpenGL esperadas
10. **LookAt** — construcción de la matriz de vista y pipeline MVP completo
11. **Traza, propiedades e interpolación** — `Trace()`, `IsOrthogonal()`, `Lerp()` con clamp
12. **Físicas prácticas** — cuerpo rígido, transformar al espacio local, pipeline Local→Mundo→Cámara, articulaciones encadenadas (hombro→codo→muñeca)
*/


// ─────────────────────────────────────────────
// Utilidades de impresion
// ─────────────────────────────────────────────

void printMatrix(const std::string& name, const Matrix4D& m) {
    std::cout << name << " =\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "  [ ";
        for (int j = 0; j < 4; ++j) {
            std::cout << std::fixed << std::setprecision(4) << std::setw(9) << m(i, j) << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

void printVector4(const std::string& name, const Vector4D& v) {
    std::cout << name << " = ("
              << std::fixed << std::setprecision(4)
              << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")\n";
}

void printVector3(const std::string& name, const Vector3D& v) {
    std::cout << name << " = ("
              << std::fixed << std::setprecision(4)
              << v.x() << ", " << v.y() << ", " << v.z() << ")\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 1: Constructores y matrices especiales
// ─────────────────────────────────────────────

void example1_constructors_and_special_matrices() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 1: Constructores y Matrices Especiales\n";
    std::cout << "========================================\n\n";

    // Constructor por defecto: identidad
    Matrix4D id_default;
    printMatrix("Constructor por defecto (identidad)", id_default);

    // Matrices especiales estaticas
    Matrix4D id = Matrix4D::Identity();
    Matrix4D zero = Matrix4D::Zero();
    printMatrix("Identity()", id);
    printMatrix("Zero()", zero);

    std::cout << "IsIdentity? " << (id.IsIdentity() ? "SI" : "NO") << "\n";
    std::cout << "IsZero?     " << (zero.IsZero() ? "SI" : "NO") << "\n\n";

    // Constructor con 16 floats (orden fila-mayor)
    Matrix4D m16(
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    );
    printMatrix("Constructor 16 floats (fila-mayor)", m16);

    // Constructor desde Matrix3D (solo rotacion, sin traslacion)
    Matrix3D rot3 = Matrix3D::RotationZ(PHYS_PI / 4.0f);
    Matrix4D m_from_rot(rot3);
    printMatrix("Desde Matrix3D (RotacionZ 45°)", m_from_rot);

    // Constructor desde Matrix3D + traslacion
    Vector3D traslacion(3.0f, 5.0f, 7.0f);
    Matrix4D m_trs(rot3, traslacion);
    printMatrix("Desde Matrix3D + traslacion (3, 5, 7)", m_trs);
    printVector3("Traslacion extraida", m_trs.GetTranslation());
    std::cout << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 2: Transformaciones basicas
// ─────────────────────────────────────────────

void example2_basic_transforms() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 2: Transformaciones Basicas\n";
    std::cout << "========================================\n\n";

    // Traslacion
    Matrix4D T = Matrix4D::Translation(4.0f, 2.0f, -3.0f);
    printMatrix("Traslacion (4, 2, -3)", T);

    // Escala no uniforme
    Matrix4D S = Matrix4D::Scale(2.0f, 3.0f, 0.5f);
    printMatrix("Escala (2, 3, 0.5)", S);

    // Escala uniforme
    Matrix4D Su = Matrix4D::Scale(5.0f);
    printMatrix("Escala uniforme (5)", Su);

    // Rotaciones elementales
    Matrix4D Rx = Matrix4D::RotationX(PHYS_PI / 6.0f);  // 30°
    Matrix4D Ry = Matrix4D::RotationY(PHYS_PI / 4.0f);  // 45°
    Matrix4D Rz = Matrix4D::RotationZ(PHYS_PI / 2.0f);  // 90°

    printMatrix("RotationX(30°)", Rx);
    printMatrix("RotationY(45°)", Ry);
    printMatrix("RotationZ(90°)", Rz);

    std::cout << "Rx es ortogonal? " << (Rx.IsOrthogonal() ? "SI" : "NO") << "\n";
    std::cout << "Ry es ortogonal? " << (Ry.IsOrthogonal() ? "SI" : "NO") << "\n";
    std::cout << "Rz es ortogonal? " << (Rz.IsOrthogonal() ? "SI" : "NO") << "\n";
    std::cout << "T  es ortogonal? " << (T.IsOrthogonal()  ? "SI" : "NO") << "\n\n";

    // Rotacion alrededor de eje arbitrario
    Vector3D eje_diagonal(1.0f, 1.0f, 0.0f);
    Matrix4D R_eje = Matrix4D::Rotation(PHYS_PI / 3.0f, eje_diagonal.Normalized());
    printMatrix("Rotation(60°, eje (1,1,0))", R_eje);
}

// ─────────────────────────────────────────────
// EJEMPLO 3: TRS (Translation × Rotation × Scale)
// ─────────────────────────────────────────────

void example3_trs() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 3: TRS (Traslacion × Rotacion × Escala)\n";
    std::cout << "========================================\n\n";

    Vector3D posicion(10.0f, 5.0f, -2.0f);
    Matrix3D rotacion = Matrix3D::RotationY(PHYS_PI / 4.0f);  // 45° en Y
    Vector3D escala(2.0f, 2.0f, 2.0f);

    Matrix4D trs = Matrix4D::TRS(posicion, rotacion, escala);
    printMatrix("TRS(posicion=(10,5,-2), Ry(45°), escala=(2,2,2))", trs);

    printVector3("Traslacion del TRS", trs.GetTranslation());

    // La magnitud de la primera columna debe ser igual a la escala en X
    Vector4D col0 = trs.GetColumn(0);
    float mag_col0 = std::sqrt(col0.x()*col0.x() + col0.y()*col0.y() + col0.z()*col0.z());
    std::cout << "Magnitud columna 0 (escala X = 2): " << mag_col0 << "\n\n";

    // Transformar un punto con TRS
    Vector4D punto_local(1.0f, 0.0f, 0.0f, 1.0f);  // w=1 -> punto
    Vector4D punto_mundo = trs * punto_local;
    printVector4("Punto local  (1, 0, 0, 1)", punto_local);
    printVector4("Punto mundo  (despues de TRS)", punto_mundo);
    std::cout << "\n";

    // Transformar una direccion (w=0, la traslacion no afecta)
    Vector4D dir_local(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4D dir_mundo = trs * dir_local;
    printVector4("Direccion local  (1, 0, 0, 0)", dir_local);
    printVector4("Direccion mundo  (traslacion ignorada)", dir_mundo);
    std::cout << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 4: Operaciones aritmeticas
// ─────────────────────────────────────────────

void example4_arithmetic() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 4: Operaciones Aritmeticas\n";
    std::cout << "========================================\n\n";

    Matrix4D A = Matrix4D::Scale(2.0f, 3.0f, 4.0f);
    Matrix4D B = Matrix4D::Translation(1.0f, 2.0f, 3.0f);

    printMatrix("A (Escala 2,3,4)", A);
    printMatrix("B (Traslacion 1,2,3)", B);

    // Suma y resta
    Matrix4D sum  = A + B;
    Matrix4D diff = A - B;
    printMatrix("A + B", sum);
    printMatrix("A - B", diff);

    // Escalar
    Matrix4D scaled = A * 2.0f;
    printMatrix("A * 2", scaled);

    Matrix4D div = scaled / 2.0f;
    std::cout << "(A * 2) / 2 == A? " << (div == A ? "SI" : "NO") << "\n\n";

    // Negacion
    Matrix4D neg = -A;
    printMatrix("-A", neg);

    // In-place
    Matrix4D C = A;
    C += B;
    std::cout << "C = A; C += B -> igual a A + B? " << (C == sum ? "SI" : "NO") << "\n\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 5: Multiplicacion de matrices y transformacion de vectores
// ─────────────────────────────────────────────

void example5_matrix_multiply_and_transform() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 5: Multiplicacion de Matrices y Transformacion de Vectores\n";
    std::cout << "========================================\n\n";

    // Composicion: primero escala, luego traslada
    // El orden de multiplicacion es: T * S (S se aplica primero)
    Matrix4D T = Matrix4D::Translation(5.0f, 0.0f, 0.0f);
    Matrix4D S = Matrix4D::Scale(2.0f, 2.0f, 2.0f);
    Matrix4D TS = T * S;

    printMatrix("T (Traslacion 5,0,0)", T);
    printMatrix("S (Escala 2,2,2)", S);
    printMatrix("T * S (escala primero, luego traslada)", TS);

    Vector4D p(1.0f, 1.0f, 1.0f, 1.0f);
    Vector4D resultado = TS * p;
    printVector4("Punto (1,1,1,1)", p);
    printVector4("(T*S) * p -> esperado (7, 2, 2, 1)", resultado);
    std::cout << "\n";

    // Rotacion 90° en Z: (1,0,0) -> (0,1,0)
    Matrix4D Rz90 = Matrix4D::RotationZ(PHYS_PI / 2.0f);
    Vector4D eje_x(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4D rotado = Rz90 * eje_x;
    printVector4("Eje X (1,0,0,0) rotado 90° en Z", rotado);

    // Traslacion NO afecta a direcciones (w=0)
    Vector4D dir(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4D dir_trasladada = T * dir;
    printVector4("Direccion (w=0) tras traslacion de 5 en X", dir_trasladada);
    std::cout << "-> La traslacion no afecta direcciones: x sigue siendo " << dir_trasladada.x() << "\n\n";

    // M * I = M
    Matrix4D I = Matrix4D::Identity();
    std::cout << "TS * I == TS? " << ((TS * I) == TS ? "SI" : "NO") << "\n";

    // M * M^-1 = I
    Matrix4D TS_inv = TS.Inverted();
    Matrix4D deberia_ser_id = TS * TS_inv;
    std::cout << "TS * TS^-1 == I? " << (deberia_ser_id.IsIdentity() ? "SI" : "NO") << "\n\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 6: Transpuesta
// ─────────────────────────────────────────────

void example6_transpose() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 6: Transpuesta\n";
    std::cout << "========================================\n\n";

    Matrix4D m(
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    );
    printMatrix("Matriz M", m);

    Matrix4D mt = m.Transposed();
    printMatrix("M transpuesta", mt);

    // Doble transpuesta = original
    std::cout << "(M^T)^T == M? " << (mt.Transposed() == m ? "SI" : "NO") << "\n";

    // Transpuesta in-place
    Matrix4D m2 = m;
    m2.Transpose();
    std::cout << "Transpose() in-place == Transposed()? " << (m2 == mt ? "SI" : "NO") << "\n\n";

    // (A * B)^T = B^T * A^T
    Matrix4D A = Matrix4D::Translation(1.0f, 2.0f, 3.0f);
    Matrix4D B = Matrix4D::Scale(2.0f, 3.0f, 4.0f);
    Matrix4D lhs = (A * B).Transposed();
    Matrix4D rhs = B.Transposed() * A.Transposed();
    std::cout << "(A*B)^T == B^T * A^T? " << (lhs == rhs ? "SI" : "NO") << "\n\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 7: Determinante e inversa
// ─────────────────────────────────────────────

void example7_determinant_and_inverse() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 7: Determinante e Inversa\n";
    std::cout << "========================================\n\n";

    // Identidad: det = 1
    std::cout << "det(I)    = " << Matrix4D::Identity().Determinant() << " (esperado 1)\n";
    std::cout << "det(Zero) = " << Matrix4D::Zero().Determinant()     << " (esperado 0)\n";

    // Escala: det = sx * sy * sz * 1
    Matrix4D S = Matrix4D::Scale(2.0f, 3.0f, 4.0f);
    std::cout << "det(Scale(2,3,4)) = " << S.Determinant()            << " (esperado 24)\n\n";

    // Inversa de traslacion
    Matrix4D T  = Matrix4D::Translation(3.0f, -5.0f, 1.0f);
    Matrix4D Ti = T.Inverted();
    printMatrix("T (Traslacion 3,-5,1)", T);
    printMatrix("T^-1", Ti);
    std::cout << "T * T^-1 == I? " << ((T * Ti).IsIdentity() ? "SI" : "NO") << "\n\n";

    // Inversa de rotacion: para matrices ortogonales, inversa = transpuesta
    Matrix4D R  = Matrix4D::RotationX(PHYS_PI / 5.0f);
    Matrix4D Ri = R.Inverted();
    std::cout << "R^-1 == R^T? " << (Ri == R.Transposed() ? "SI" : "NO") << "\n\n";

    // Verificar IsInvertible
    std::cout << "IsInvertible(T)? " << (T.IsInvertible() ? "SI" : "NO") << "\n";
    std::cout << "IsInvertible(Zero)? " << (Matrix4D::Zero().IsInvertible() ? "SI" : "NO") << "\n\n";

    // Invert() in-place
    Matrix4D R_copia = R;
    R_copia.Invert();
    std::cout << "Invert() in-place: R * R^-1 == I? " << ((R * R_copia).IsIdentity() ? "SI" : "NO") << "\n\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 8: Acceso a columnas, filas y submatrices
// ─────────────────────────────────────────────

void example8_column_row_and_submatrix() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 8: Columnas, Filas y Submatrices\n";
    std::cout << "========================================\n\n";

    Matrix3D rot3 = Matrix3D::RotationY(PHYS_PI / 3.0f);
    Vector3D traslacion(7.0f, 8.0f, 9.0f);
    Matrix4D m(rot3, traslacion);
    printMatrix("Matriz M (rot Y 60° + traslacion 7,8,9)", m);

    // Columnas
    std::cout << "Columnas:\n";
    for (int col = 0; col < 4; ++col) {
        printVector4("  Columna " + std::to_string(col), m.GetColumn(col));
    }
    std::cout << "\n";

    // Filas
    std::cout << "Filas:\n";
    for (int row = 0; row < 4; ++row) {
        printVector4("  Fila " + std::to_string(row), m.GetRow(row));
    }
    std::cout << "\n";

    // Submatriz 3×3 (bloque de rotacion/escala)
    Matrix3D rot_extraida = m.GetUpper3x3();
    std::cout << "Upper3x3 == rot3 original? "
              << (rot_extraida == rot3 ? "SI" : "NO") << "\n";

    // Traslacion
    Vector3D t_extraida = m.GetTranslation();
    printVector3("GetTranslation()", t_extraida);
    std::cout << "\n";

    // Modificar columna
    Matrix4D m2 = m;
    m2.SetColumn(3, Vector4D(100.0f, 200.0f, 300.0f, 1.0f));
    printVector3("SetColumn(3, ...) -> nueva traslacion", m2.GetTranslation());

    // Modificar traslacion directamente
    m2.SetTranslation(Vector3D(-1.0f, -2.0f, -3.0f));
    printVector3("SetTranslation(-1,-2,-3)", m2.GetTranslation());
    std::cout << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 9: Proyecciones (Perspectiva y Ortografica)
// ─────────────────────────────────────────────

void example9_projection_matrices() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 9: Matrices de Proyeccion\n";
    std::cout << "========================================\n\n";

    // Proyeccion perspectiva (convencion OpenGL, mano derecha, NDC [-1,1])
    float fovY   = PHYS_PI / 2.0f;    // 90° de campo visual vertical
    float aspect = 16.0f / 9.0f; // Relacion 16:9
    float near   = 0.1f;
    float far    = 100.0f;

    Matrix4D P = Matrix4D::Perspective(fovY, aspect, near, far);
    printMatrix("Perspective(fovY=90°, aspect=16/9, near=0.1, far=100)", P);

    // En la perspectiva OpenGL: m(3,2) = -1 (convierte w_clip = -z_eye)
    std::cout << "m(3,2) == -1? " << (std::abs(P(3,2) + 1.0f) < 1e-5f ? "SI" : "NO")
              << "  (valor: " << P(3,2) << ")\n\n";

    // Proyeccion ortografica (2D, interfaz, etc.)
    Matrix4D O = Matrix4D::Orthographic(-8.0f, 8.0f, -4.5f, 4.5f, 0.1f, 100.0f);
    printMatrix("Orthographic(-8,8, -4.5,4.5, near=0.1, far=100)", O);

    // En ortografica: m(3,3) = 1 y no hay perspectiva (w se conserva)
    std::cout << "m(3,3) == 1? " << (std::abs(O(3,3) - 1.0f) < 1e-5f ? "SI" : "NO")
              << "  (valor: " << O(3,3) << ")\n\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 10: LookAt (camara)
// ─────────────────────────────────────────────

void example10_lookat() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 10: LookAt (Matriz de Vista)\n";
    std::cout << "========================================\n\n";

    Vector3D eye(0.0f, 3.0f, 10.0f);    // posicion de la camara
    Vector3D target(0.0f, 0.0f, 0.0f);  // punto al que mira
    Vector3D up(0.0f, 1.0f, 0.0f);      // vector "arriba"

    Matrix4D V = Matrix4D::LookAt(eye, target, up);
    printMatrix("LookAt(eye=(0,3,10), target=(0,0,0), up=(0,1,0))", V);

    // La vista transforma el ojo al origen del espacio de camara
    Vector4D eye_h(eye.x(), eye.y(), eye.z(), 1.0f);
    Vector4D eye_en_camara = V * eye_h;
    printVector4("Ojo transformado (debe ser ~(0,0,0,1))", eye_en_camara);
    std::cout << "\n";

    // Pipeline MVP completo: Modelo -> Vista -> Proyeccion
    Matrix4D M = Matrix4D::TRS(
        Vector3D(2.0f, 0.0f, 0.0f),
        Matrix3D::RotationY(PHYS_PI / 4.0f),
        Vector3D(1.0f, 1.0f, 1.0f)
    );
    Matrix4D P   = Matrix4D::Perspective(PHYS_PI / 3.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    Matrix4D MVP = P * V * M;

    printMatrix("M (modelo)", M);
    printMatrix("MVP = P * V * M", MVP);

    Vector4D vertice(0.0f, 0.0f, 0.0f, 1.0f);  // origen del modelo
    Vector4D clip = MVP * vertice;
    printVector4("Vertice (0,0,0,1) en clip space", clip);
    std::cout << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 11: Traza, propiedades e interpolacion
// ─────────────────────────────────────────────

void example11_trace_properties_lerp() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 11: Traza, Propiedades e Interpolacion\n";
    std::cout << "========================================\n\n";

    // Traza
    std::cout << "Trace(I)         = " << Matrix4D::Identity().Trace()     << " (esperado 4)\n";
    std::cout << "Trace(Scale(2,3,4)) = " << Matrix4D::Scale(2,3,4).Trace() << " (esperado 10)\n\n";

    // Propiedades de matrices especiales
    Matrix4D I  = Matrix4D::Identity();
    Matrix4D Rz = Matrix4D::RotationZ(PHYS_PI / 4.0f);
    Matrix4D T  = Matrix4D::Translation(1.0f, 2.0f, 3.0f);
    Matrix4D S  = Matrix4D::Scale(2.0f, 2.0f, 2.0f);

    std::cout << "Identidad:\n";
    std::cout << "  IsIdentity? " << (I.IsIdentity() ? "SI" : "NO") << "\n";
    std::cout << "  IsOrthogonal? " << (I.IsOrthogonal() ? "SI" : "NO") << "\n\n";

    std::cout << "Rotacion Z 45°:\n";
    std::cout << "  IsIdentity? " << (Rz.IsIdentity() ? "SI" : "NO") << "\n";
    std::cout << "  IsOrthogonal? " << (Rz.IsOrthogonal() ? "SI" : "NO") << "\n";
    std::cout << "  det = " << std::abs(Rz.Determinant()) << " (esperado 1)\n\n";

    std::cout << "Traslacion (1,2,3):\n";
    std::cout << "  IsOrthogonal? " << (T.IsOrthogonal() ? "SI" : "NO")
              << "  (columna de traslacion no es unitaria en 4D)\n\n";

    // Interpolacion lineal entre matrices
    Matrix4D inicio = Matrix4D::Identity();
    Matrix4D fin    = Matrix4D::Scale(3.0f, 3.0f, 3.0f);

    printMatrix("Inicio (identidad)", inicio);
    printMatrix("Fin (escala 3x)", fin);

    std::cout << "Interpolacion (diagonal principal):\n";
    for (float t = 0.0f; t <= 1.01f; t += 0.25f) {
        Matrix4D mid = Matrix4D::Lerp(inicio, fin, t);
        std::cout << std::fixed << std::setprecision(2)
                  << "  t = " << t << ": diag = ("
                  << mid(0,0) << ", " << mid(1,1) << ", "
                  << mid(2,2) << ", " << mid(3,3) << ")\n";
    }

    // Clamp fuera de [0, 1]
    std::cout << "\nClamp:\n";
    std::cout << "  t = -0.5 -> "
              << Matrix4D::Lerp(inicio, fin, -0.5f)(0,0) << " (esperado 1)\n";
    std::cout << "  t =  2.0 -> "
              << Matrix4D::Lerp(inicio, fin,  2.0f)(0,0) << " (esperado 3)\n\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 12: Aplicaciones practicas en fisicas
// ─────────────────────────────────────────────

void example12_practical_physics() {
    std::cout << "========================================\n";
    std::cout << "EJEMPLO 12: Aplicaciones Practicas en Fisicas\n";
    std::cout << "========================================\n\n";

    // ── 1. Transformar un cuerpo rigido ──────
    std::cout << "1. Transformacion de cuerpo rigido\n";
    Vector3D posicion(5.0f, 0.0f, 0.0f);
    Matrix3D orientacion = Matrix3D::RotationY(PHYS_PI / 6.0f);  // 30° en Y
    Matrix4D world = Matrix4D::TRS(posicion, orientacion, Vector3D(1, 1, 1));

    // Punto en espacio local del cuerpo
    Vector4D punto_local(0.5f, 0.0f, 0.0f, 1.0f);
    Vector4D punto_mundo = world * punto_local;
    printVector4("  Punto local (0.5, 0, 0, 1)", punto_local);
    printVector4("  Punto mundo (despues de world transform)", punto_mundo);
    std::cout << "\n";

    // ── 2. Transformar al espacio local del objeto ──
    std::cout << "2. Transformar al espacio local (inversa)\n";
    Matrix4D world_inv = world.Inverted();
    Vector4D punto_mundo2(5.5f, 0.0f, 0.0f, 1.0f);
    Vector4D de_vuelta = world_inv * punto_mundo2;
    printVector4("  Punto mundo (5.5, 0, 0, 1)", punto_mundo2);
    printVector4("  Punto local recuperado", de_vuelta);
    std::cout << "\n";

    // ── 3. Cambio de base: local -> mundo -> camara ──
    std::cout << "3. Pipeline de transformaciones: Local -> Mundo -> Camara\n";
    Matrix4D M = world;
    Matrix4D V = Matrix4D::LookAt(
        Vector3D(10.0f, 5.0f, 10.0f),  // ojo
        Vector3D(5.0f, 0.0f, 0.0f),    // mira al cuerpo
        Vector3D(0.0f, 1.0f, 0.0f)     // arriba
    );
    Matrix4D P = Matrix4D::Perspective(PHYS_PI / 3.0f, 16.0f / 9.0f, 0.1f, 200.0f);
    Matrix4D MVP = P * V * M;

    Vector4D vertice_local(0.0f, 1.0f, 0.0f, 1.0f);
    Vector4D clip_space = MVP * vertice_local;
    printVector4("  Vertice local  (0, 1, 0, 1)", vertice_local);
    printVector4("  Clip space     (MVP * v)", clip_space);
    // Division de perspectiva (NDC)
    if (std::abs(clip_space.w()) > 1e-5f) {
        Vector3D ndc(clip_space.x() / clip_space.w(),
                     clip_space.y() / clip_space.w(),
                     clip_space.z() / clip_space.w());
        printVector3("  NDC            (dividido por w)", ndc);
    }
    std::cout << "\n";

    // ── 4. Composicion de transformaciones de articulaciones ──
    std::cout << "4. Articulacion: hombro -> codo -> muneca\n";
    Matrix4D hombro = Matrix4D::TRS(
        Vector3D(0.0f, 2.0f, 0.0f),
        Matrix3D::RotationZ(PHYS_PI / 6.0f),   // 30° en Z (abduccion)
        Vector3D(1, 1, 1)
    );
    Matrix4D codo = Matrix4D::TRS(
        Vector3D(0.0f, -1.5f, 0.0f),      // desplazamiento local desde el hombro
        Matrix3D::RotationZ(-PHYS_PI / 4.0f),  // -45° (flexion de codo)
        Vector3D(1, 1, 1)
    );
    Matrix4D muneca = Matrix4D::TRS(
        Vector3D(0.0f, -1.0f, 0.0f),      // desplazamiento local desde el codo
        Matrix3D::RotationZ(PHYS_PI / 8.0f),   // 22.5° (leve rotacion)
        Vector3D(1, 1, 1)
    );

    // Posicion de la muñeca en espacio mundo = hombro * codo * muneca
    Matrix4D muneca_mundo = hombro * codo * muneca;
    Vector3D origen_muneca = muneca_mundo.GetTranslation();
    printVector3("  Posicion de la muneca en espacio mundo", origen_muneca);
    std::cout << "\n";
}

// ─────────────────────────────────────────────
// main
// ─────────────────────────────────────────────

int main() {
    std::cout << "EJEMPLOS DE MATRIX4D\n";
    std::cout << "=====================\n\n";
    std::cout << "Demuestra el uso de Matrix4D para transformaciones 3D,\n";
    std::cout << "proyecciones, pipeline de renderizado y aplicaciones de fisicas.\n\n";

    example1_constructors_and_special_matrices();
    example2_basic_transforms();
    example3_trs();
    example4_arithmetic();
    example5_matrix_multiply_and_transform();
    example6_transpose();
    example7_determinant_and_inverse();
    example8_column_row_and_submatrix();
    example9_projection_matrices();
    example10_lookat();
    example11_trace_properties_lerp();
    example12_practical_physics();

    std::cout << "========================================\n";
    std::cout << "TODOS LOS EJEMPLOS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}