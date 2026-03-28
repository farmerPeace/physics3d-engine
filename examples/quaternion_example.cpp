#include "physics/math/quaternion.hpp"
#include "physics/math/vector3d.hpp"
#include "physics/math/matrix3d.hpp"
#include "physics/math/matrix4d.hpp"
#include "physics/core/config.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

/*
El archivo cubre 14 ejemplos progresivos:

1. Constructores y métodos estáticos (`Identity`, `FromAxisAngle`, `FromEulerAngles`, `FromRotationMatrix`)
2. Acceso por índice y `Data()` raw
3. Suma, resta y negación (con nota sobre `q` y `-q`)
4. Multiplicación/división por escalar e in-place
5. Multiplicación de cuaterniones y composición de rotaciones
6. Producto punto, conjugado e inverso
7. Magnitud, normalización y deriva numérica
8. Rotación de vectores con propiedades demostradas
9. `ToAxisAngle` y `ToEulerAngles` con ida y vuelta
10. Conversión a `Matrix3D` y `Matrix4D`
11. `Slerp` y `Nlerp` con tabla comparativa
12. Derivada temporal e integración de orientación
13. Exponencial, logaritmo y Squad simplificado
14. Simulación de cuerpo rígido con cámara orbital
*/

// ---------------------------------------------
// Utilidades de impresion
// ---------------------------------------------

void printQuat(const std::string& name, const Quaternion& q) {
    std::cout << name << " = (w=" << std::fixed << std::setprecision(4)
              << q[0] << ", x=" << q[1]
              << ", y=" << q[2] << ", z=" << q[3] << ")\n";
}

void printVector3(const std::string& name, const Vector3D& v) {
    std::cout << name << " = ("
              << std::fixed << std::setprecision(4)
              << v.x() << ", " << v.y() << ", " << v.z() << ")\n";
}

void printSeparator(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << title << "\n";
    std::cout << "========================================\n\n";
}

// ---------------------------------------------
// EJEMPLO 1: Constructores y metodos estaticos de creacion
// ---------------------------------------------
// Un cuaternion de rotacion tiene la forma:
//   q = cos(θ/2) + sin(θ/2) * (xi + yj + zk)
// donde (x, y, z) es el eje de rotacion normalizado y θ el angulo.

void example1_constructors() {
    printSeparator("EJEMPLO 1: Constructores y Metodos Estaticos de Creacion");

    // Constructor por defecto: cuaternion identidad (sin rotacion)
    Quaternion q_default;
    printQuat("Constructor por defecto (identidad)", q_default);
    std::cout << "IsUnit? " << (q_default.IsUnit() ? "SI" : "NO") << "\n\n";

    // Constructor con cuatro componentes (w, x, y, z)
    Quaternion q_comp(0.707f, 0.0f, 0.707f, 0.0f);
    printQuat("Constructor (w=0.707, x=0, y=0.707, z=0)", q_comp);

    // Constructor desde escalar y Vector3D (w, parte vectorial)
    Vector3D parte_vectorial(0.0f, 1.0f, 0.0f);
    Quaternion q_vec(0.5f, parte_vectorial);
    printQuat("Constructor (0.5, Vector3D(0,1,0))", q_vec);

    // -- Metodos estaticos ---------------------

    // Identity: ninguna rotacion
    Quaternion id = Quaternion::Identity();
    printQuat("\nIdentity()", id);

    // FromAxisAngle: rotacion de 90° alrededor del eje Y
    Quaternion q_aa = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI / 2.0f);
    printQuat("FromAxisAngle(Y, 90°)", q_aa);
    std::cout << "IsUnit? " << (q_aa.IsUnit() ? "SI" : "NO") << "\n";

    // FromEulerAngles(PHYS_PItch, yaw, roll): equivale a Rz(roll)*Ry(yaw)*Rx(PHYS_PItch)
    // PHYS_PItch = rotacion en X, Yaw = rotacion en Y, Roll = rotacion en Z
    Quaternion q_euler = Quaternion::FromEulerAngles(PHYS_PI / 6.0f, PHYS_PI / 4.0f, PHYS_PI / 3.0f);
    printQuat("\nFromEulerAngles(PHYS_PItch=30°, yaw=45°, roll=60°)", q_euler);

    // FromRotationMatrix: construye el cuaternion desde una matriz de rotacion
    Matrix3D rot_z = Matrix3D::RotationZ(PHYS_PI / 2.0f);
    Quaternion q_from_mat = Quaternion::FromRotationMatrix(rot_z);
    printQuat("\nFromRotationMatrix(RotZ 90°)", q_from_mat);
}

// ---------------------------------------------
// EJEMPLO 2: Acceso por indice y datos raw
// ---------------------------------------------
// El indice sigue el orden: [0]=w, [1]=x, [2]=y, [3]=z
// Data() devuelve un puntero contiguo al primer elemento (w_).

void example2_accessors() {
    printSeparator("EJEMPLO 2: Acceso por indice y Datos Raw");

    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    printQuat("q", q);

    // Acceso por indice (lectura)
    std::cout << "q[0]=w=" << q[0] << "  q[1]=x=" << q[1]
              << "  q[2]=y=" << q[2] << "  q[3]=z=" << q[3] << "\n";

    // Acceso por indice (escritura)
    q[0] = 10.0f;
    q[3] = 40.0f;
    std::cout << "Tras q[0]=10, q[3]=40: w=" << q[0] << "  z=" << q[3] << "\n";

    // indice fuera de rango lanza std::out_of_range
    try {
        float val = q[4];
        (void)val;
    } catch (const std::out_of_range& e) {
        std::cout << "indice 4 -> excepcion: " << e.what() << "\n";
    }

    // Datos raw - util para enviar a la GPU o serializar
    Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
    const float* data = q2.Data();
    std::cout << "\nData() raw: [" << data[0] << ", " << data[1]
              << ", " << data[2] << ", " << data[3] << "]\n";

    // Escritura a traves del puntero mutable
    q2.Data()[0] = 99.0f;
    std::cout << "Tras Data()[0]=99: w=" << q2[0] << "\n";
}

// ---------------------------------------------
// EJEMPLO 3: Operadores aritmeticos
// ---------------------------------------------
// Suma y resta operan componente a componente y se usan
// principalmente en interpolaciones (Nlerp, derivadas).
// La negacion invierte todos los componentes.

void example3_arithmetic() {
    printSeparator("EJEMPLO 3: Operadores Aritmeticos");

    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(5.0f, 6.0f, 7.0f, 8.0f);

    printQuat("a", a);
    printQuat("b", b);

    Quaternion sum = a + b;
    printQuat("\na + b", sum);

    Quaternion diff = b - a;
    printQuat("b - a", diff);

    Quaternion neg = -a;
    printQuat("-a", neg);

    // In-place
    Quaternion c = a;
    c += b;
    std::cout << "\nc = a; c += b -> igual a a+b? " << (c == sum ? "SI" : "NO") << "\n";

    Quaternion d = b;
    d -= a;
    std::cout << "d = b; d -= a -> igual a b-a? " << (d == diff ? "SI" : "NO") << "\n";

    // Nota: -q representa la misma rotacion que q.
    // q y -q codifican identica orientacion porque ambos mapean el
    // mismo arco en la hiperesfera S³.
    Quaternion q_rot = Quaternion::FromAxisAngle(Vector3D(0, 0, 1), PHYS_PI / 3.0f);
    Quaternion q_neg = -q_rot;
    Vector3D v_prueba(1.0f, 0.0f, 0.0f);
    Vector3D r1 = q_rot.Rotate(v_prueba);
    Vector3D r2 = q_neg.Rotate(v_prueba);
    std::cout << "\nq y -q producen la misma rotacion:\n";
    printVector3("  q.Rotate(1,0,0)", r1);
    printVector3("  (-q).Rotate(1,0,0)", r2);
}

// ---------------------------------------------
// EJEMPLO 4: Operaciones con escalar
// ---------------------------------------------
// La multiplicacion y division por escalar escalan los cuatro componentes.
// Se usa principalmente al ponderar cuaterniones en interpolaciones.

void example4_scalar_operations() {
    printSeparator("EJEMPLO 4: Operaciones con Escalar");

    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    printQuat("q", q);

    Quaternion q_x2 = q * 2.0f;
    printQuat("q * 2.0", q_x2);

    Quaternion q_half = q / 2.0f;
    printQuat("q / 2.0", q_half);

    // Conmutativa: 2.0 * q == q * 2.0
    Quaternion q_comm = 2.0f * q;
    std::cout << "2.0 * q == q * 2.0? " << (q_comm == q_x2 ? "SI" : "NO") << "\n";

    // In-place
    Quaternion q2 = q;
    q2 *= 3.0f;
    printQuat("\nq *= 3.0", q2);
    q2 /= 3.0f;
    printQuat("Despues /= 3.0 (restaurado)", q2);
    std::cout << "Restaurado == q? " << (q2 == q ? "SI" : "NO") << "\n";

    // Division por (casi) cero lanza std::domain_error
    try {
        Quaternion bad = q / 0.0f;
        (void)bad;
    } catch (const std::domain_error& e) {
        std::cout << "\nDivision por cero -> excepcion: " << e.what() << "\n";
    }
}

// ---------------------------------------------
// EJEMPLO 5: Multiplicacion de cuaterniones (composicion de rotaciones)
// ---------------------------------------------
// La multiplicacion q1 * q2 compone la rotacion de q2 primero y q1 despues.
// NO es conmutativa: q1*q2 ≠ q2*q1 en general.
// Reglas de los imaginarios: i²=j²=k²=-1, ij=k, ji=-k, jk=i, kj=-i, ki=j, ik=-j

void example5_quaternion_multiplication() {
    printSeparator("EJEMPLO 5: Multiplicacion de Cuaterniones - Composicion de Rotaciones");

    // Reglas basicas de los imaginarios
    Quaternion qi(0.0f, 1.0f, 0.0f, 0.0f);   // i
    Quaternion qj(0.0f, 0.0f, 1.0f, 0.0f);   // j
    Quaternion qk(0.0f, 0.0f, 0.0f, 1.0f);   // k

    printQuat("i", qi);
    printQuat("j", qj);

    Quaternion ij = qi * qj;
    printQuat("i * j (esperado k)", ij);

    Quaternion ji = qj * qi;
    printQuat("j * i (esperado -k)", ji);

    std::cout << "\nNo es conmutativa: i*j != j*i -> "
              << (ij != ji ? "CORRECTO" : "ERROR") << "\n";

    // -- Composicion de rotaciones --------------
    // Rotar 90° en X y luego 90° en Y
    Quaternion qX = Quaternion::FromAxisAngle(Vector3D(1, 0, 0), PHYS_PI / 2.0f);
    Quaternion qY = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI / 2.0f);

    // qY * qX = "primero X, luego Y"
    Quaternion qYX = qY * qX;

    Vector3D v(1.0f, 0.0f, 0.0f);
    Vector3D r_x    = qX.Rotate(v);
    Vector3D r_yx   = qYX.Rotate(v);

    std::cout << "\nRotar (1, 0, 0):\n";
    printVector3("  Solo Rx(90°)", r_x);
    printVector3("  Ry(90°) * Rx(90°)", r_yx);

    // q * Identity = q
    std::cout << "\nq * Identity == q? "
              << ((qX * Quaternion::Identity()) == qX ? "SI" : "NO") << "\n";

    // In-place *=
    Quaternion q2 = qY;
    q2 *= qX;
    std::cout << "qY *= qX == qY * qX? " << (q2 == qYX ? "SI" : "NO") << "\n";
}

// ---------------------------------------------
// EJEMPLO 6: Producto punto, conjugado e inverso
// ---------------------------------------------
// Producto punto: mide la similitud entre dos cuaterniones.
//   dot(q, q) == |q|²   para cualquier q.
//   dot(q1, q2) > 0  ->  rotaciones en el mismo hemisferio de S³.
// Conjugado: invierte la parte vectorial -> invierte la direccion de rotacion.
// Inverso: para unitarios, Inverse() == Conjugate().

void example6_dot_conjugate_inverse() {
    printSeparator("EJEMPLO 6: Producto Punto, Conjugado e Inverso");

    // -- Producto punto -------------------------
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(5.0f, 6.0f, 7.0f, 8.0f);

    float dot = a.Dot(b);
    float expected_dot = 1*5 + 2*6 + 3*7 + 4*8;
    std::cout << "Dot(a, b) = " << dot << "  (esperado " << expected_dot << ")\n";

    Quaternion u = a.Normalized();
    std::cout << "Dot(u, u) = " << u.Dot(u) << "  (esperado 1 para unitario)\n";

    // El signo del producto punto determina el camino mas corto en Slerp.
    // Si dot < 0, negamos uno de los cuaterniones antes de interpolar.
    Quaternion q1 = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI / 4.0f);
    Quaternion q2 = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), 3.0f * PHYS_PI / 2.0f);
    std::cout << "\nDot(q1, q2) = " << q1.Dot(q2) << "  "
              << (q1.Dot(q2) < 0.0f ? "(caminos opuestos -> se negara q2 en Slerp)" : "(mismo hemisferio)") << "\n";

    // -- Conjugado -----------------------------
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion conj = q.Conjugate();
    printQuat("\nq", q);
    printQuat("q* (conjugado)", conj);
    std::cout << "La parte escalar se conserva: w=" << conj[0] << "\n";
    std::cout << "La parte vectorial se invierte: x=" << conj[1]
              << " y=" << conj[2] << " z=" << conj[3] << "\n";

    // q * q* = |q|² (escalar puro)
    Quaternion q_x_conj = q * conj;
    printQuat("q * q*  (parte vectorial debe ser ~0)", q_x_conj);

    // -- Inverso -------------------------------
    Quaternion qu = q.Normalized();
    Quaternion inv = qu.Inverse();
    printQuat("\nq unitario", qu);
    printQuat("Inverse()", inv);
    printQuat("Conjugate()", qu.Conjugate());
    std::cout << "Para unitarios: Inverse() == Conjugate()? "
              << (inv == qu.Conjugate() ? "SI" : "NO") << "\n";

    // q * q⁻¹ = Identity
    Quaternion prod = qu * inv;
    printQuat("q * q⁻¹ (debe ser ~identidad)", prod);

    // Inverso de cuaternion nulo lanza excepcion
    try {
        PHYS_UNUSED(Quaternion(0, 0, 0, 0).Inverse());
    } catch (const std::domain_error& e) {
        std::cout << "\nInverse(0,0,0,0) -> excepcion: " << e.what() << "\n";
    }
}

// ---------------------------------------------
// EJEMPLO 7: Magnitud y normalizacion
// ---------------------------------------------
// Los cuaterniones de rotacion deben ser unitarios (|q| = 1).
// Acumular muchas multiplicaciones puede introducir error numerico;
// renormalizar periodicamente garantiza la estabilidad.

void example7_magnitude_normalization() {
    printSeparator("EJEMPLO 7: Magnitud y Normalizacion");

    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    printQuat("q", q);

    float mag    = q.Magnitude();
    float sq_mag = q.SquareMagnitude();
    std::cout << "Magnitude()       = " << mag    << "  (esperado sqrt(30) ≈ 5.4772)\n";
    std::cout << "SquareMagnitude() = " << sq_mag << "  (esperado 30)\n";

    // Normalized() devuelve PHYS_PIa
    Quaternion n = q.Normalized();
    printQuat("\nNormalized()", n);
    std::cout << "Magnitude del normalizado = " << n.Magnitude() << "  (esperado 1)\n";
    std::cout << "IsUnit()? " << (n.IsUnit() ? "SI" : "NO") << "\n";

    // Normalize() in-place
    Quaternion q2 = q;
    q2.Normalize();
    std::cout << "\nNormalize() in-place == Normalized()? "
              << (q2 == n ? "SI" : "NO") << "\n";

    // Ejemplo de deriva numerica y renormalizacion
    std::cout << "\nSimulacion de deriva numerica:\n";
    Quaternion q_rot = Quaternion::FromAxisAngle(Vector3D(1, 1, 0).Normalized(), 0.01f);
    Quaternion acum  = Quaternion::Identity();

    for (int i = 0; i < 100; ++i) acum *= q_rot;

    std::cout << "  Magnitud tras 100 multiplicaciones: " << acum.Magnitude() << "\n";
    acum.Normalize();
    std::cout << "  Magnitud tras renormalizar:         " << acum.Magnitude() << "\n";

    // Normalizar el cuaternion nulo lanza excepcion
    try {
        Quaternion(0, 0, 0, 0).Normalize();
    } catch (const std::domain_error& e) {
        std::cout << "\nNormalize(0,0,0,0) -> excepcion: " << e.what() << "\n";
    }
}

// ---------------------------------------------
// EJEMPLO 8: Rotacion de vectores
// ---------------------------------------------
// La formula  v' = q * v_puro * q⁻¹  rota el vector v.
// Internamente se usa la optimizacion de Rodrigues para evitar
// construir el cuaternion puro intermedio.
// PHYS_PIedades garantizadas: preserva longitud, preserva angulo entre vectores.

void example8_rotate_vector() {
    printSeparator("EJEMPLO 8: Rotacion de Vectores");

    // -- Rotaciones elementales -----------------
    Vector3D ex(1.0f, 0.0f, 0.0f);
    Vector3D ey(0.0f, 1.0f, 0.0f);
    Vector3D ez(0.0f, 0.0f, 1.0f);

    Quaternion qX90 = Quaternion::FromAxisAngle(ex, PHYS_PI / 2.0f);
    Quaternion qY90 = Quaternion::FromAxisAngle(ey, PHYS_PI / 2.0f);
    Quaternion qZ90 = Quaternion::FromAxisAngle(ez, PHYS_PI / 2.0f);

    // Rx(90°): Y -> Z, Z -> -Y
    printVector3("Rx(90°) * Y  (esperado  Z)", qX90.Rotate(ey));
    printVector3("Rx(90°) * Z  (esperado -Y)", qX90.Rotate(ez));

    // Ry(90°): Z -> X, X -> -Z
    printVector3("\nRy(90°) * Z  (esperado  X)", qY90.Rotate(ez));
    printVector3("Ry(90°) * X  (esperado -Z)", qY90.Rotate(ex));

    // Rz(90°): X -> Y, Y -> -X
    printVector3("\nRz(90°) * X  (esperado  Y)", qZ90.Rotate(ex));
    printVector3("Rz(90°) * Y  (esperado -X)", qZ90.Rotate(ey));

    // -- PHYS_PIedades ----------------------------
    Vector3D v(3.0f, 1.0f, 4.0f);
    Quaternion q_arb = Quaternion::FromAxisAngle(Vector3D(1, 2, 3).Normalized(), PHYS_PI / 5.0f);

    Vector3D v_rot = q_arb.Rotate(v);
    std::cout << "\nRotacion arbitraria de v=(3,1,4):\n";
    printVector3("  v rotado", v_rot);
    std::cout << "  Magnitud preservada: |v|=" << v.Magnitude()
              << "  |v'|=" << v_rot.Magnitude() << "\n";

    // La identidad no rota
    Vector3D v_id = Quaternion::Identity().Rotate(v);
    std::cout << "\nIdentidad.Rotate(v) == v? ";
    bool same = (std::abs(v_id.x()-v.x()) < 1e-5f &&
                 std::abs(v_id.y()-v.y()) < 1e-5f &&
                 std::abs(v_id.z()-v.z()) < 1e-5f);
    std::cout << (same ? "SI" : "NO") << "\n";

    // -- Rotacion 180° -------------------------
    Quaternion q180 = Quaternion::FromAxisAngle(ez, PHYS_PI);
    printVector3("\nRz(180°) * X  (esperado -X)", q180.Rotate(ex));
    printVector3("Rz(180°) * Y  (esperado -Y)", q180.Rotate(ey));
    printVector3("Rz(180°) * Z  (esperado  Z)", q180.Rotate(ez));
}

// ---------------------------------------------
// EJEMPLO 9: Extraccion de eje y angulo, angulos de Euler
// ---------------------------------------------
// ToAxisAngle descompone el cuaternion en su eje y angulo de rotacion.
// ToEulerAngles retorna Vector3D(PHYS_PItch, yaw, roll):
//   PHYS_PItch = rotacion en X, yaw = rotacion en Y, roll = rotacion en Z
// Composicion interna: Rz(roll) * Ry(yaw) * Rx(PHYS_PItch)

void example9_axis_angle_and_euler() {
    printSeparator("EJEMPLO 9: Extraccion de Eje/angulo y angulos de Euler");

    // -- Eje y angulo ---------------------------
    float angulo_entrada = PHYS_PI / 3.0f;   // 60°
    Vector3D eje_entrada(0.0f, 1.0f, 0.0f);
    Quaternion q = Quaternion::FromAxisAngle(eje_entrada, angulo_entrada);

    Vector3D eje_out;
    float    angulo_out;
    q.ToAxisAngle(eje_out, angulo_out);

    std::cout << "Entrada -> eje=" << eje_entrada.x() << "," << eje_entrada.y() << ","
              << eje_entrada.z() << "  angulo=" << angulo_entrada * 180.0f / PHYS_PI << "°\n";
    printVector3("Eje extraido (debe ser Y)", eje_out);
    std::cout << "angulo extraido = " << angulo_out * 180.0f / PHYS_PI << "°\n";

    // La identidad tiene angulo ≈ 0 y eje arbitrario (por convenio X)
    Quaternion::Identity().ToAxisAngle(eje_out, angulo_out);
    std::cout << "\nIdentidad: angulo=" << angulo_out * 180.0f / PHYS_PI
              << "°  eje=(" << eje_out.x() << "," << eje_out.y() << "," << eje_out.z() << ")\n";

    // -- angulos de Euler -----------------------
    std::cout << "\n-- angulos de Euler (PHYS_PItch, yaw, roll) --\n";

    // Solo PHYS_PItch (eje X)
    Quaternion PHYS_PItch = Quaternion::FromAxisAngle(Vector3D(1, 0, 0), PHYS_PI / 4.0f);
    Vector3D euler_p = PHYS_PItch.ToEulerAngles();
    std::cout << "Solo Rx(45°) -> PHYS_PItch=" << euler_p.x()*180/PHYS_PI
              << "° yaw=" << euler_p.y()*180/PHYS_PI
              << "° roll=" << euler_p.z()*180/PHYS_PI << "°\n";

    // Solo yaw (eje Y)
    Quaternion q_yaw = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI / 6.0f);
    Vector3D euler_y = q_yaw.ToEulerAngles();
    std::cout << "Solo Ry(30°) -> PHYS_PItch=" << euler_y.x()*180/PHYS_PI
              << "° yaw=" << euler_y.y()*180/PHYS_PI
              << "° roll=" << euler_y.z()*180/PHYS_PI << "°\n";

    // Solo roll (eje Z)
    Quaternion q_roll = Quaternion::FromAxisAngle(Vector3D(0, 0, 1), PHYS_PI / 3.0f);
    Vector3D euler_r = q_roll.ToEulerAngles();
    std::cout << "Solo Rz(60°) -> PHYS_PItch=" << euler_r.x()*180/PHYS_PI
              << "° yaw=" << euler_r.y()*180/PHYS_PI
              << "° roll=" << euler_r.z()*180/PHYS_PI << "°\n";

    // Ida y vuelta: FromEulerAngles -> ToEulerAngles
    float p_in = PHYS_PI / 5.0f, y_in = PHYS_PI / 7.0f, r_in = PHYS_PI / 9.0f;
    Quaternion q_idt = Quaternion::FromEulerAngles(p_in, y_in, r_in);
    Vector3D euler_idt = q_idt.ToEulerAngles();
    Vector3D v_test(2.0f, 1.0f, 3.0f);
    Vector3D r_orig = q_idt.Rotate(v_test);
    Quaternion q_reconstruido = Quaternion::FromEulerAngles(
        euler_idt.x(), euler_idt.y(), euler_idt.z());
    Vector3D r_reconstruido = q_reconstruido.Rotate(v_test);

    std::cout << "\nIda y vuelta (ida -> euler -> reconstruir):\n";
    printVector3("  Rotacion original", r_orig);
    printVector3("  Rotacion reconstruida", r_reconstruido);
}

// ---------------------------------------------
// EJEMPLO 10: Conversion a matrices
// ---------------------------------------------
// ToRotationMatrix() -> Matrix3D (solo rotacion).
// ToTransformMatrix() -> Matrix4D (rotacion sin traslacion, lista para MVP).

void example10_to_matrix() {
    printSeparator("EJEMPLO 10: Conversion a Matrices (Matrix3D y Matrix4D)");

    // -- ToRotationMatrix() --------------------
    // Equivalencia con la matriz construida directamente
    float angle = PHYS_PI / 2.0f;
    Quaternion qZ = Quaternion::FromAxisAngle(Vector3D(0, 0, 1), angle);
    Matrix3D m3  = qZ.ToRotationMatrix();
    Matrix3D m3_direct = Matrix3D::RotationZ(angle);

    std::cout << "ToRotationMatrix() == Matrix3D::RotationZ(90°)? "
              << (m3 == m3_direct ? "SI" : "NO") << "\n";
    std::cout << "IsOrthogonal? " << (m3.IsOrthogonal() ? "SI" : "NO") << "\n";
    std::cout << "|det| = " << std::abs(m3.Determinant()) << "  (esperado 1)\n";

    // Identidad -> Identidad
    Matrix3D m_id = Quaternion::Identity().ToRotationMatrix();
    std::cout << "Identity -> Identity3D? " << (m_id == Matrix3D::Identity() ? "SI" : "NO") << "\n";

    // Ida y vuelta: Matrix3D -> Quaternion -> Matrix3D
    Matrix3D rot_orig = Matrix3D::RotationX(PHYS_PI / 5.0f);
    Quaternion q_trip = Quaternion::FromRotationMatrix(rot_orig);
    Matrix3D rot_back = q_trip.ToRotationMatrix();
    std::cout << "\nIda y vuelta Matrix3D -> Q -> Matrix3D: igual? "
              << (rot_orig == rot_back ? "SI" : "NO") << "\n";

    // -- ToTransformMatrix() -------------------
    std::cout << "\n-- ToTransformMatrix() --\n";
    Quaternion q_arb = Quaternion::FromAxisAngle(Vector3D(1, 0, 0), PHYS_PI / 4.0f);
    Matrix4D m4 = q_arb.ToTransformMatrix();

    std::cout << "Identity -> Identity4D? "
              << (Quaternion::Identity().ToTransformMatrix() == Matrix4D::Identity() ? "SI" : "NO") << "\n";

    // El bloque 3×3 superior coincide con ToRotationMatrix()
    Matrix3D upper = m4.GetUpper3x3();
    std::cout << "Upper3x3 == ToRotationMatrix()? "
              << (upper == q_arb.ToRotationMatrix() ? "SI" : "NO") << "\n";

    // La cuarta fila/columna es la del vector base homogeneo
    std::cout << "m4(3,0)=0? " << (std::abs(m4(3,0)) < 1e-5f ? "SI" : "NO")
              << "  m4(3,3)=1? " << (std::abs(m4(3,3) - 1.0f) < 1e-5f ? "SI" : "NO") << "\n";

    // Uso PHYS_PIco en PHYS_PIpeline de renderizado: rotar un punto con la Matrix4D
    Vector4D punto(1.0f, 0.0f, 0.0f, 1.0f);
    Vector4D resultado = m4 * punto;
    Vector3D por_quat  = q_arb.Rotate(Vector3D(1, 0, 0));
    std::cout << "\nRotar (1,0,0) con Matrix4D:\n";
    std::cout << "  Resultado:  (" << resultado.x() << ", " << resultado.y()
              << ", " << resultado.z() << ")\n";
    printVector3("  Via Rotate()", por_quat);
}

// ---------------------------------------------
// EJEMPLO 11: Interpolaciones - Slerp y Nlerp
// ---------------------------------------------
// Slerp: interpolacion esferica lineal. Trayectoria de velocidad constante
//        en la esfera S³. Preferido para animaciones precisas.
// Nlerp: normalizacion de interpolacion lineal. Mas barato de calcular,
//        velocidad angular no constante, pero produce resultados suaves.
// Ambos respetan t ∈ [0, 1] (clamped).

void example11_slerp_nlerp() {
    printSeparator("EJEMPLO 11: Interpolaciones - Slerp y Nlerp");

    Quaternion q_start = Quaternion::Identity();
    Quaternion q_end   = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI / 2.0f);

    std::cout << "Interpolando de Identity a Ry(90°):\n\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "   t     Slerp-eje-y                  Nlerp-eje-y\n";
    std::cout << "  -----  -------------------------    ------------------------\n";

    for (int i = 0; i <= 5; ++i) {
        float t = i / 5.0f;
        Quaternion qs = Quaternion::Slerp(q_start, q_end, t);
        Quaternion qn = Quaternion::Nlerp(q_start, q_end, t);
        Vector3D rs = qs.Rotate(Vector3D(1, 0, 0));
        Vector3D rn = qn.Rotate(Vector3D(1, 0, 0));
        std::cout << "  " << std::setw(5) << t
                  << "  (" << std::setw(7) << rs.x() << ","
                  <<          std::setw(7) << rs.y() << ","
                  <<          std::setw(7) << rs.z() << ")   "
                  << "  (" << std::setw(7) << rn.x() << ","
                  <<          std::setw(7) << rn.y() << ","
                  <<          std::setw(7) << rn.z() << ")\n";
    }

    // Extremos: t=0 y t=1
    std::cout << "\nSlerp en t=0 == q_start? "
              << (Quaternion::Slerp(q_start, q_end, 0.0f) == q_start ? "SI" : "NO") << "\n";
    std::cout << "Slerp en t=1 == q_end?   "
              << (Quaternion::Slerp(q_start, q_end, 1.0f) == q_end   ? "SI" : "NO") << "\n";

    // PHYS_PIng: valores fuera de [0,1] se fijan al extremo
    std::cout << "Slerp(t=-0.5) == q_start? "
              << (Quaternion::Slerp(q_start, q_end, -0.5f) == q_start ? "SI" : "NO") << "\n";
    std::cout << "Slerp(t= 2.0) == q_end?   "
              << (Quaternion::Slerp(q_start, q_end,  2.0f) == q_end   ? "SI" : "NO") << "\n";

    // Todos los resultados deben ser unitarios
    std::cout << "\nTodos los Slerp son unitarios:\n";
    bool todos_unitarios = true;
    for (int i = 0; i <= 10; ++i) {
        float t = i / 10.0f;
        if (!Quaternion::Slerp(q_start, q_end, t).IsUnit()) {
            todos_unitarios = false; break;
        }
    }
    std::cout << "  " << (todos_unitarios ? "SI" : "NO") << "\n";

    // Slerp elige el camino mas corto (mismo resultado con q_end negado)
    std::cout << "\nCamino mas corto (q_end negado produce el mismo resultado):\n";
    Quaternion q_end_neg(-q_end[0], -q_end[1], -q_end[2], -q_end[3]);
    Quaternion slerp1 = Quaternion::Slerp(q_start, q_end,     0.5f);
    Quaternion slerp2 = Quaternion::Slerp(q_start, q_end_neg, 0.5f);
    Vector3D r1 = slerp1.Rotate(Vector3D(1, 0, 0));
    Vector3D r2 = slerp2.Rotate(Vector3D(1, 0, 0));
    bool same = std::abs(r1.x()-r2.x()) < 1e-4f &&
                std::abs(r1.y()-r2.y()) < 1e-4f &&
                std::abs(r1.z()-r2.z()) < 1e-4f;
    std::cout << "  Slerp(q) == Slerp(-q) en t=0.5? " << (same ? "SI" : "NO") << "\n";
}

// ---------------------------------------------
// EJEMPLO 12: Derivada temporal
// ---------------------------------------------
// La derivada temporal de un cuaternion de orientacion es:
//   dq/dt = 0.5 * ω_q * q
// donde ω_q = (0, ωx, ωy, ωz) es la velocidad angular como cuaternion puro.
// Se usa para integrar la orientacion en simulaciones de cuerpos rigidos.

void example12_derivative() {
    printSeparator("EJEMPLO 12: Derivada Temporal - Integracion de Orientacion");

    // Cuerpo girando a 1 rad/s alrededor del eje Z
    Vector3D omega(0.0f, 0.0f, 1.0f);   // velocidad angular en rad/s
    Quaternion q = Quaternion::Identity();

    std::cout << "Integrando orientacion (ω = 1 rad/s en Z):\n";
    std::cout << "  Tiempo  angulo obtenido  angulo esperado\n";
    std::cout << "  ------  ---------------  ---------------\n";

    const float dt = 0.05f;
    float t = 0.0f;

    for (int i = 0; i <= 10; ++i) {
        Vector3D eje; float angulo;
        q.Normalized().ToAxisAngle(eje, angulo);

        // Eje puede ser X arbitrario cuando angulo≈0; ignorar el signo
        std::cout << "  " << std::fixed << std::setprecision(2) << std::setw(6) << t
                  << "  " << std::setw(15) << angulo * 180.0f / PHYS_PI
                  << "  " << std::setw(15) << t * 180.0f / PHYS_PI << "\n";

        // Paso de integracion de Euler:  q += dq/dt * dt
        Quaternion dq = q.Derivative(omega);
        q += dq * dt;
        q.Normalize();   // renormalizar para evitar deriva
        t += dt;
    }

    // La derivada de la identidad respecto a omega=(0,1,0) da (0, 0, 0.5, 0)
    Quaternion dq_id = Quaternion::Identity().Derivative(Vector3D(0, 1, 0));
    printQuat("\nDerivative(Identity, omega=(0,1,0))", dq_id);
    std::cout << "(esperado: w=0, x=0, y=0.5, z=0)\n";
}

// ---------------------------------------------
// EJEMPLO 13: Exponencial y logaritmo
// ---------------------------------------------
// exp(q) y log(q) son la base de Squad (Slerp cubico) y otras
// interpolaciones avanzadas. Para cuaterniones unitarios:
//   q = exp(log(q))

void example13_exp_log() {
    printSeparator("EJEMPLO 13: Exponencial y Logaritmo de Cuaterniones");

    // Logaritmo de la identidad -> cuaternion nulo
    Quaternion log_id = Quaternion::Identity().Logarithm();
    printQuat("log(Identity)", log_id);
    std::cout << "(todos los componentes deben ser ~0)\n";

    // exp(log(q)) == q para cuaternion unitario
    Quaternion q = Quaternion::FromAxisAngle(Vector3D(1, 0, 0), PHYS_PI / 4.0f);
    Quaternion log_q   = q.Logarithm();
    Quaternion exp_log = log_q.Exponential();

    printQuat("\nq (Rx 45°)", q);
    printQuat("log(q)", log_q);
    printQuat("exp(log(q))", exp_log);

    bool roundtrip_ok =
        std::abs(exp_log[0] - q[0]) < 1e-4f &&
        std::abs(exp_log[1] - q[1]) < 1e-4f &&
        std::abs(exp_log[2] - q[2]) < 1e-4f &&
        std::abs(exp_log[3] - q[3]) < 1e-4f;
    std::cout << "exp(log(q)) == q? " << (roundtrip_ok ? "SI" : "NO") << "\n";

    // El logaritmo del cuaternion nulo lanza excepcion
    try {
        PHYS_UNUSED(Quaternion(0, 0, 0, 0).Logarithm());
    } catch (const std::domain_error& e) {
        std::cout << "\nlog(0,0,0,0) -> excepcion: " << e.what() << "\n";
    }

    // Aplicacion: Squad (interpolacion esferica cubica) usando log/exp
    // Squad(q0, q1, s0, s1, t) puede expresarse como:
    //   Slerp(Slerp(q0,q1,t), Slerp(s0,s1,t), 2t(1-t))
    // La implementacion simplificada usa Slerp dos veces, lo cual
    // se basa internamente en la misma geometria esferica que log/exp.
    std::cout << "\nSquad simplificado (Slerp doble):\n";
    Quaternion q0 = Quaternion::Identity();
    Quaternion q1 = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI / 2.0f);
    Quaternion q2 = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI);

    float t = 0.5f;
    Quaternion squad_approx = Quaternion::Slerp(
        Quaternion::Slerp(q0, q1, t),
        Quaternion::Slerp(q1, q2, t),
        2.0f * t * (1.0f - t)
    );
    printQuat("Squad(t=0.5)", squad_approx);
    std::cout << "IsUnit? " << (squad_approx.IsUnit() ? "SI" : "NO") << "\n";
}

// ---------------------------------------------
// EJEMPLO 14: Caso de uso completo - cuerpo rigido
// ---------------------------------------------
// Demuestra como un cuaternion representa la orientacion de un cuerpo
// rigido a lo largo de una simulacion: integracion, transformacion de
// vectores de cuerpo a mundo y camara orbital.

void example14_rigid_body_simulation() {
    printSeparator("EJEMPLO 14: Caso de Uso - Cuerpo Rigido en Simulacion");

    // Estado inicial del cuerpo
    Quaternion orientacion = Quaternion::Identity();
    Vector3D omega(0.5f, 1.0f, 0.2f);   // velocidad angular en rad/s (espacio mundo)

    // Vectores fijos en el cuerpo (espacio local)
    Vector3D delante_local(0.0f, 0.0f, -1.0f);  // frente de la nave
    Vector3D arriba_local (0.0f, 1.0f,  0.0f);  // techo de la nave

    const float dt    = 0.1f;
    const int   pasos = 6;

    std::cout << "  Paso  Frente (espacio mundo)              Arriba (espacio mundo)\n";
    std::cout << "  ----  --------------------------------    ----------------------------------\n";

    std::cout << std::fixed << std::setprecision(3);

    for (int i = 0; i <= pasos; ++i) {
        Vector3D delante_mundo = orientacion.Rotate(delante_local);
        Vector3D arriba_mundo  = orientacion.Rotate(arriba_local);

        std::cout << "  " << std::setw(4) << i
                  << "  (" << std::setw(7) << delante_mundo.x()
                  << ", "  << std::setw(7) << delante_mundo.y()
                  << ", "  << std::setw(7) << delante_mundo.z() << ")   "
                  << "  (" << std::setw(7) << arriba_mundo.x()
                  << ", "  << std::setw(7) << arriba_mundo.y()
                  << ", "  << std::setw(7) << arriba_mundo.z() << ")\n";

        // Integracion: dq = Derivative(omega) * dt
        orientacion += orientacion.Derivative(omega) * dt;
        orientacion.Normalize();
    }

    // Transformar al espacio de camara
    std::cout << "\nOrientacion del cuerpo como Matrix4D para la GPU:\n";
    Matrix4D m_mundo = orientacion.ToTransformMatrix();
    std::cout << "  Upper3x3 IsOrthogonal? "
              << (m_mundo.GetUpper3x3().IsOrthogonal() ? "SI" : "NO") << "\n";
    std::cout << "  Traslacion (debe ser 0,0,0): ";
    Vector3D t = m_mundo.GetTranslation();
    std::cout << "(" << t.x() << "," << t.y() << "," << t.z() << ")\n";

    // Interpolacion de camara orbital usando Slerp
    std::cout << "\nInterpolacion de camara orbital (Slerp):\n";
    Quaternion cam_inicio = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), 0.0f);
    Quaternion cam_fin    = Quaternion::FromAxisAngle(Vector3D(0, 1, 0), PHYS_PI / 2.0f);

    for (int i = 0; i <= 4; ++i) {
        float t_cam = i / 4.0f;
        Quaternion cam = Quaternion::Slerp(cam_inicio, cam_fin, t_cam);
        Vector3D posicion_cam = cam.Rotate(Vector3D(0.0f, 0.0f, 5.0f));
        std::cout << "  t=" << t_cam
                  << "  cam=(" << std::setw(6) << posicion_cam.x()
                  << ", "      << std::setw(6) << posicion_cam.y()
                  << ", "      << std::setw(6) << posicion_cam.z() << ")\n";
    }
}

// ---------------------------------------------
// main
// ---------------------------------------------

int main() {
    std::cout << "EJEMPLOS DE QUATERNION\n";
    std::cout << "======================\n\n";
    std::cout << "Demuestra el uso de Quaternion para representar y componer\n";
    std::cout << "rotaciones en 3D, interpolaciones y simulacion de cuerpos rigidos.\n";

    example1_constructors();
    example2_accessors();
    example3_arithmetic();
    example4_scalar_operations();
    example5_quaternion_multiplication();
    example6_dot_conjugate_inverse();
    example7_magnitude_normalization();
    example8_rotate_vector();
    example9_axis_angle_and_euler();
    example10_to_matrix();
    example11_slerp_nlerp();
    example12_derivative();
    example13_exp_log();
    example14_rigid_body_simulation();

    std::cout << "\n========================================\n";
    std::cout << "TODOS LOS EJEMPLOS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}