#include "physics/math/vector4d.hpp"
#include "physics/math/vector3d.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

// ─────────────────────────────────────────────
// Utilidades de impresion
// ─────────────────────────────────────────────

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

void printSeparator(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << title << "\n";
    std::cout << "========================================\n\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 1: Constructores
// ─────────────────────────────────────────────
// Vector4D tiene cinco formas de construccion:
//   - Por defecto              -> (0, 0, 0, 0)
//   - Cuatro componentes       -> (x, y, z, w)
//   - Array de cuatro floats
//   - Desde Vector3D           -> w = 1  (punto homogeneo)
//   - Desde Vector3D + w       -> w explicito

void example1_constructors() {
    printSeparator("EJEMPLO 1: Constructores");

    // Constructor por defecto: vector nulo
    Vector4D v_default;
    printVector4("Constructor por defecto", v_default);

    // Constructor con cuatro componentes
    Vector4D v_comp(1.0f, 2.0f, 3.0f, 4.0f);
    printVector4("Constructor (1, 2, 3, 4)", v_comp);

    // Constructor desde array
    float arr[4] = { 5.0f, 6.0f, 7.0f, 8.0f };
    Vector4D v_arr(arr);
    printVector4("Constructor desde array [5, 6, 7, 8]", v_arr);

    // Constructor desde Vector3D: w se establece en 1 automaticamente,
    // lo que convierte el vector en un punto en coordenadas homogeneas.
    Vector3D v3d(10.0f, 20.0f, 30.0f);
    Vector4D v_from_v3d(v3d);
    printVector4("Desde Vector3D (10, 20, 30) -> w=1 (punto)", v_from_v3d);

    // Constructor desde Vector3D con w explicito.
    // w=0 representa una direccion; cualquier otro valor distinto de 0 o 1
    // es un punto no homogenizado.
    Vector4D v_dir(v3d, 0.0f);
    printVector4("Desde Vector3D + w=0 (direccion)", v_dir);

    Vector4D v_w2(v3d, 2.0f);
    printVector4("Desde Vector3D + w=2 (punto no normalizado)", v_w2);
}

// ─────────────────────────────────────────────
// EJEMPLO 2: Getters, setters y acceso por indice
// ─────────────────────────────────────────────
// x()->[0], y()->[1], z()->[2], w()->[3]

void example2_accessors() {
    printSeparator("EJEMPLO 2: Getters, Setters y Acceso por indice");

    Vector4D v(1.0f, 2.0f, 3.0f, 4.0f);

    // Getters
    std::cout << "Getters  ->  x=" << v.x() << "  y=" << v.y()
              << "  z=" << v.z() << "  w=" << v.w() << "\n";

    // Setters
    v.set_x(10.0f);
    v.set_y(20.0f);
    v.set_z(30.0f);
    v.set_w(40.0f);
    printVector4("Despues de setters (10, 20, 30, 40)", v);

    // Acceso por indice (lectura)
    std::cout << "Por indice -> v[0]=" << v[0] << "  v[1]=" << v[1]
              << "  v[2]=" << v[2] << "  v[3]=" << v[3] << "\n";

    // Acceso por indice (escritura)
    v[0] = 100.0f;
    v[3] = 400.0f;
    std::cout << "Tras v[0]=100, v[3]=400 -> x=" << v.x() << "  w=" << v.w() << "\n";

    // indice fuera de rango lanza std::out_of_range
    try {
        float val = v[4];
        (void)val;
    } catch (const std::out_of_range& e) {
        std::cout << "indice 4 -> excepcion capturada: " << e.what() << "\n";
    }
}

// ─────────────────────────────────────────────
// EJEMPLO 3: Operadores aritmeticos
// ─────────────────────────────────────────────
// Suma, resta, negacion y sus versiones in-place (+=, -=).

void example3_arithmetic() {
    printSeparator("EJEMPLO 3: Operadores Aritmeticos");

    Vector4D a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4D b(5.0f, 6.0f, 7.0f, 8.0f);

    printVector4("a", a);
    printVector4("b", b);

    // Suma
    Vector4D sum = a + b;
    printVector4("a + b", sum);

    // Resta
    Vector4D diff = b - a;
    printVector4("b - a", diff);

    // Negacion
    Vector4D neg = -a;
    printVector4("-a", neg);

    // In-place +=
    Vector4D c = a;
    c += b;
    printVector4("c = a; c += b", c);
    std::cout << "¿c == a + b? " << (c == sum ? "SI" : "NO") << "\n";

    // In-place -=
    Vector4D d = b;
    d -= a;
    printVector4("d = b; d -= a", d);
    std::cout << "¿d == b - a? " << (d == diff ? "SI" : "NO") << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 4: Multiplicacion y division por escalar
// ─────────────────────────────────────────────
// Los operadores con escalar son plantillas (*=, /=, *, /)
// y aceptan cualquier tipo aritmetico (int, float, double).

void example4_scalar_operations() {
    printSeparator("EJEMPLO 4: Operaciones con Escalar");

    Vector4D v(2.0f, 4.0f, 6.0f, 8.0f);
    printVector4("v", v);

    // Multiplicacion
    Vector4D v_x3 = v * 3.0f;
    printVector4("v * 3.0f", v_x3);

    // Division
    Vector4D v_half = v / 2.0f;
    printVector4("v / 2.0f", v_half);

    // In-place
    Vector4D v2 = v;
    v2 *= 0.5f;
    printVector4("v * 0.5f (in-place)", v2);

    v2 /= 0.5f;
    printVector4("Despues /= 0.5f (restaurado)", v2);
    std::cout << "¿Restaurado == v? " << (v2 == v ? "SI" : "NO") << "\n";

    // Escalar entero
    Vector4D v_int = v * 2;
    printVector4("v * 2 (escalar int)", v_int);

    // Division por (casi) cero lanza std::domain_error
    try {
        Vector4D bad = v / 0.0f;
        (void)bad;
    } catch (const std::domain_error& e) {
        std::cout << "Division por cero -> excepcion: " << e.what() << "\n";
    }
}

// ─────────────────────────────────────────────
// EJEMPLO 5: Comparacion
// ─────────────────────────────────────────────
// La igualdad utiliza un EPSILON = 1e-6 para tolerancia numerica,
// por lo que pequeñas diferencias de punto flotante se consideran iguales.

void example5_comparisons() {
    printSeparator("EJEMPLO 5: Comparacion");

    Vector4D v1(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4D v2(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4D v3(1.0000005f, 2.0000005f, 3.0000005f, 4.0000005f);
    Vector4D v4(9.0f, 8.0f, 7.0f, 6.0f);

    std::cout << "v1 == v2 (identicos)               : " << (v1 == v2 ? "SI" : "NO") << "\n";
    std::cout << "v1 == v3 (diferencia < epsilon)     : " << (v1 == v3 ? "SI" : "NO") << "\n";
    std::cout << "v1 == v4 (distintos)               : " << (v1 == v4 ? "SI" : "NO") << "\n";
    std::cout << "v1 != v4                            : " << (v1 != v4 ? "SI" : "NO") << "\n";
    std::cout << "v1 != v2                            : " << (v1 != v2 ? "SI" : "NO") << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 6: Producto punto y multiplicacion componente a componente
// ─────────────────────────────────────────────
// Dot4D opera sobre los cuatro componentes (x, y, z, w).
// ComponentWiseMultiply es útil para modulacion de color, mascaras, etc.

void example6_dot_and_component_multiply() {
    printSeparator("EJEMPLO 6: Producto Punto y Multiplicacion Componente a Componente");

    Vector4D a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4D b(5.0f, 6.0f, 7.0f, 8.0f);

    printVector4("a", a);
    printVector4("b", b);

    // Producto punto 4D: a·b = 1*5 + 2*6 + 3*7 + 4*8
    float dot = a.Dot4D(b);
    std::cout << "\nProducto punto (Dot4D):\n";
    std::cout << "a * b = " << dot << "  (esperado: " << (1*5 + 2*6 + 3*7 + 4*8) << ")\n";

    // Un vector unitario tiene producto punto consigo mismo igual a 1
    Vector4D dir(1.0f, 0.0f, 0.0f, 0.0f);
    std::cout << "dir unitario * dir unitario = " << dir.Dot4D(dir) << "\n";

    // Multiplicacion componente a componente
    Vector4D cw = a.ComponentWiseMultiply(b);
    std::cout << "\nMultiplicacion componente a componente:\n";
    printVector4("a * b (componente)", cw);

    // Caso de uso tipico: modulacion de color RGBA
    // Cada canal se mezcla por separado (ej. tinte o alpha blend)
    Vector4D color(0.8f, 0.5f, 0.2f, 1.0f);   // RGBA base
    Vector4D tint(1.0f, 0.7f, 1.0f, 0.5f);    // factor de tinte
    Vector4D result = color.ComponentWiseMultiply(tint);
    std::cout << "\nCaso de uso - modulacion de color RGBA:\n";
    printVector4("  color base", color);
    printVector4("  tinte", tint);
    printVector4("  color resultante", result);
}

// ─────────────────────────────────────────────
// EJEMPLO 7: Magnitud y normalizacion
// ─────────────────────────────────────────────
// Solo los vectores direccion (w=0) tienen magnitud definida.
// Normalize preserva la semantica de direccion forzando w=0 al final.

void example7_magnitude_and_normalization() {
    printSeparator("EJEMPLO 7: Magnitud y Normalizacion");

    Vector4D dir(3.0f, 0.0f, 4.0f, 0.0f);   // direccion en el plano XZ
    printVector4("dir (direccion, w=0)", dir);

    // Magnitud y magnitud al cuadrado
    float mag    = dir.Magnitude();
    float sq_mag = dir.SquareMagnitude();
    std::cout << "Magnitud        = " << mag    << "  (esperado 5)\n";
    std::cout << "Magnitud²       = " << sq_mag << "  (esperado 25)\n";

    // Normalized() devuelve copia; Normalize() modifica en sitio
    Vector4D n = dir.Normalized();
    printVector4("dir.Normalized()", n);
    std::cout << "Magnitud del normalizado = " << n.Magnitude() << "  (esperado 1)\n";
    std::cout << "w del normalizado        = " << n.w()         << "  (forzado a 0)\n";

    // Normalize() in-place
    Vector4D dir2(6.0f, 0.0f, 8.0f, 0.0f);
    dir2.Normalize();
    printVector4("dir2(6,0,8,0) tras Normalize()", dir2);

    // Los puntos (w != 0) no tienen magnitud definida
    Vector4D point(1.0f, 2.0f, 3.0f, 1.0f);
    std::cout << "\nIntentar Magnitude() sobre un punto (w=1):\n";
    try {
        float m = point.Magnitude();
        (void)m;
    } catch (const std::domain_error& e) {
        std::cout << "  Excepcion capturada: " << e.what() << "\n";
    }

    std::cout << "\nIntentar Normalize() sobre un punto (w=1):\n";
    try {
        point.Normalize();
    } catch (const std::domain_error& e) {
        std::cout << "  Excepcion capturada: " << e.what() << "\n";
    }
}

// ─────────────────────────────────────────────
// EJEMPLO 8: Coordenadas homogeneas - puntos y direcciones
// ─────────────────────────────────────────────
// En coordenadas homogeneas:
//   w != 0  ->  punto    (puede ser trasladado)
//   w == 0  ->  direccion (no se traslada)
// Homogenize() divide x, y, z entre w para llevar w -> 1.

void example8_homogeneous_coordinates() {
    printSeparator("EJEMPLO 8: Coordenadas Homogeneas - Puntos y Direcciones");

    // ── IsPoint / IsDirection ──────────────────
    Vector4D point(3.0f, 6.0f, 9.0f, 1.0f);
    Vector4D dir(3.0f, 6.0f, 9.0f, 0.0f);
    Vector4D non_unit(2.0f, 4.0f, 6.0f, 2.0f);   // punto con w=2 (sin normalizar)

    std::cout << "IsPoint  (w=1): " << (point.IsPoint()    ? "SI" : "NO") << "\n";
    std::cout << "IsPoint  (w=0): " << (dir.IsPoint()      ? "SI" : "NO") << "\n";
    std::cout << "IsDirection(w=0): " << (dir.IsDirection() ? "SI" : "NO") << "\n";
    std::cout << "IsPoint  (w=2): " << (non_unit.IsPoint() ? "SI" : "NO") << "\n\n";

    // ── Homogenize ────────────────────────────
    // Divide (x, y, z) entre w y establece w=1.
    printVector4("Punto no normalizado (2, 4, 6, 2)", non_unit);
    Vector4D h = non_unit.Homogenized();
    printVector4("Homogenized() -> (1, 2, 3, 1)", h);

    // Homogenize() in-place
    Vector4D p2(6.0f, 9.0f, 12.0f, 3.0f);
    p2.Homogenize();
    printVector4("(6,9,12,3) tras Homogenize() in-place", p2);

    // w=0 (direccion): Homogenize() no modifica, evita division por cero
    Vector4D d2(1.0f, 2.0f, 3.0f, 0.0f);
    d2.Homogenize();
    printVector4("Direccion (w=0) tras Homogenize(): sin cambios", d2);

    // ── MakePoint / MakeDirection ─────────────
    std::cout << "\nMakeDirection fuerza w=0 sobre cualquier vector:\n";
    Vector4D v(5.0f, 3.0f, 1.0f, 7.0f);
    v.MakeDirection();
    printVector4("  v tras MakeDirection()", v);

    // ── Conversion a Vector3D ─────────────────
    std::cout << "\nConversiones a Vector3D:\n";
    Vector4D pt(4.0f, 8.0f, 12.0f, 4.0f);
    Vector3D pt3d = pt.ToVector3D();        // divide entre w
    printVector4("  Punto (4, 8, 12, 4)", pt);
    printVector3("  ToVector3D() -> (1, 2, 3)", pt3d);

    Vector4D direction(7.0f, 0.0f, 0.0f, 0.0f);
    Vector3D dir3d = direction.ToDirection3D();   // copia directa de xyz
    printVector4("  Direccion (7, 0, 0, 0)", direction);
    printVector3("  ToDirection3D() -> (7, 0, 0)", dir3d);

    // ToVector3D sobre una direccion (w=0) lanza std::domain_error
    std::cout << "\nToVector3D() sobre una direccion (w=0):\n";
    try {
        direction.ToVector3D();
    } catch (const std::domain_error& e) {
        std::cout << "  Excepcion capturada: " << e.what() << "\n";
    }
}

// ─────────────────────────────────────────────
// EJEMPLO 9: Distancia entre puntos homogeneos
// ─────────────────────────────────────────────
// Distance() y SquareDistance() homogenizan automaticamente los puntos
// antes de calcular la distancia euclidiana 3D.

void example9_distance() {
    printSeparator("EJEMPLO 9: Distancia entre Puntos Homogeneos");

    // Puntos ya normalizados (w=1)
    Vector4D p1(1.0f, 0.0f, 0.0f, 1.0f);
    Vector4D p2(4.0f, 4.0f, 0.0f, 1.0f);

    float dist   = Vector4D::Distance(p1, p2);
    float sq_dist = Vector4D::SquareDistance(p1, p2);

    printVector4("p1", p1);
    printVector4("p2", p2);
    std::cout << "Distance(p1, p2)       = " << dist    << "  (esperado 5)\n";
    std::cout << "SquareDistance(p1, p2) = " << sq_dist << "  (esperado 25)\n";

    // Puntos no normalizados: Distance() los homogeniza antes de operar.
    // (2,4,6,2) se homogeniza a (1,2,3) y (6,8,10,2) a (3,4,5).
    Vector4D q1(2.0f, 4.0f, 6.0f, 2.0f);
    Vector4D q2(6.0f, 8.0f, 10.0f, 2.0f);
    float dist2 = Vector4D::Distance(q1, q2);
    float expected = std::sqrt(4.0f + 4.0f + 4.0f);   // sqrt(12)

    std::cout << "\nPuntos no normalizados (w=2):\n";
    printVector4("q1 = (2,4,6,2) -> homogeniza a (1,2,3)", q1);
    printVector4("q2 = (6,8,10,2) -> homogeniza a (3,4,5)", q2);
    std::cout << "Distance(q1, q2) = " << dist2    << "  (esperado " << expected << ")\n";

    // Las direcciones (w=0) no soportan Distance
    Vector4D bad_dir(1.0f, 0.0f, 0.0f, 0.0f);
    std::cout << "\nDistance entre una direccion y un punto:\n";
    try {
        Vector4D::Distance(bad_dir, p1);
    } catch (const std::domain_error& e) {
        std::cout << "  Excepcion capturada: " << e.what() << "\n";
    }
}

// ─────────────────────────────────────────────
// EJEMPLO 10: Interoperabilidad con Vector3D y acceso a datos raw
// ─────────────────────────────────────────────

void example10_interop_and_data() {
    printSeparator("EJEMPLO 10: Interoperabilidad con Vector3D y Datos Raw");

    // ── Conversiones Vector3D ↔ Vector4D ──────
    Vector3D pos3(1.0f, 2.0f, 3.0f);
    Vector3D vel3(0.0f, 0.0f, -10.0f);

    Vector4D pos4(pos3);           // w=1 -> punto
    Vector4D vel4(vel3, 0.0f);     // w=0 -> direccion

    printVector3("Posicion 3D", pos3);
    printVector4("Posicion 4D (w=1)", pos4);
    printVector3("Velocidad 3D", vel3);
    printVector4("Velocidad 4D (w=0)", vel4);

    // Recuperar la posicion de vuelta a 3D
    Vector3D pos_back = pos4.ToVector3D();
    printVector3("ToVector3D() -> posicion recuperada", pos_back);

    // ── Acceso a datos raw (Data()) ───────────
    // Devuelve un puntero al primer float (x_), contiguo en memoria.
    // Necesario para enviarlo a la GPU (OpenGL/Vulkan) o a buffers de fisica.
    Vector4D v(1.0f, 2.0f, 3.0f, 4.0f);
    const float* data = v.Data();

    std::cout << "\nAcceso raw (Data()):\n";
    std::cout << "  data[0]=" << data[0] << "  data[1]=" << data[1]
              << "  data[2]=" << data[2] << "  data[3]=" << data[3] << "\n";

    // Escritura a traves del puntero mutable
    float* data_mutable = v.Data();
    data_mutable[0] = 99.0f;
    std::cout << "  Tras data[0]=99: v.x() = " << v.x() << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 11: Caso de uso completo - simulacion de particula
// ─────────────────────────────────────────────
// Demuestra como Vector4D se integra en un ciclo de simulacion basico:
//   - La posicion se almacena como punto  (w=1)
//   - La velocidad se almacena como direccion (w=0)
// La semantica homogenea garantiza que sumar una direccion a un punto
// produce otro punto, y que la traslacion no afecta a las velocidades.

void example11_particle_simulation() {
    printSeparator("EJEMPLO 11: Caso de Uso - Simulacion de Particula");

    // Estado inicial de la particula
    Vector4D position(0.0f, 10.0f, 0.0f, 1.0f);   // punto (w=1)
    Vector4D velocity(5.0f,  0.0f, 0.0f, 0.0f);   // direccion (w=0)
    Vector4D gravity (0.0f, -9.8f, 0.0f, 0.0f);   // direccion (w=0)

    printVector4("Posicion inicial", position);
    printVector4("Velocidad inicial", velocity);
    printVector4("Gravedad", gravity);
    std::cout << "\n";

    const float dt      = 0.1f;
    const int   steps   = 10;
    float       t       = 0.0f;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "  t(s)   posicion x      posicion y      velocidad y\n";
    std::cout << "  -----  ------------    ------------    ------------\n";

    for (int i = 0; i <= steps; ++i) {
        std::cout << "  " << std::setw(5) << t
                  << "  " << std::setw(13) << position.x()
                  << "  " << std::setw(13) << position.y()
                  << "  " << std::setw(13) << velocity.y() << "\n";

        // Integracion de Euler:
        //   vel += gravedad * dt
        //   pos += vel * dt
        // La semantica w se conserva correctamente:
        //   direccion * escalar  -> direccion
        //   punto + direccion    -> punto (w sigue siendo 1)
        velocity += gravity  * dt;
        position += velocity * dt;

        t += dt;
    }

    std::cout << "\nPosicion final: ";
    printVector4("", position);
    std::cout << "¿Sigue siendo un punto (w=1)? "
              << (position.IsPoint() ? "SI" : "NO") << "\n";
    std::cout << "¿Velocidad sigue siendo direccion (w=0)? "
              << (velocity.IsDirection() ? "SI" : "NO") << "\n";
}

// ─────────────────────────────────────────────
// EJEMPLO 12: Caso de uso completo - interpolacion de camara
// ─────────────────────────────────────────────
// Muestra como interpolar la posicion de una camara entre dos puntos
// usando una mezcla manual basada en ComponentWiseMultiply y suma.
// Tambien ilustra normalizacion de direccion de vista.

void example12_camera_interpolation() {
    printSeparator("EJEMPLO 12: Caso de Uso - Interpolacion de Camara");

    Vector4D cam_start(0.0f,  5.0f,  10.0f, 1.0f);  // posicion inicial (punto)
    Vector4D cam_end  (10.0f, 5.0f, -10.0f, 1.0f);  // posicion final   (punto)
    Vector4D target   (5.0f,  0.0f,   0.0f, 1.0f);  // punto de interes

    printVector4("Inicio camara", cam_start);
    printVector4("Fin camara", cam_end);
    printVector4("Objetivo", target);
    std::cout << "\n";

    std::cout << "   t      posicion camara                       distancia al objetivo\n";
    std::cout << "  ------  -----------------------------------   -----------------------\n";

    std::cout << std::fixed << std::setprecision(3);

    for (int i = 0; i <= 5; ++i) {
        float t = i / 5.0f;

        // Interpolacion lineal: lerp(a, b, t) = a*(1-t) + b*t
        Vector4D one_minus_t(1.0f - t, 1.0f - t, 1.0f - t, 1.0f - t);
        Vector4D t_vec(t, t, t, t);

        Vector4D cam_pos = cam_start.ComponentWiseMultiply(one_minus_t)
                         + cam_end.ComponentWiseMultiply(t_vec);
        cam_pos.set_w(1.0f);    // mantener semantica de punto

        // Direccion de la camara hacia el objetivo
        Vector4D to_target = target - cam_pos;
        to_target.MakeDirection();    // asegurar w=0

        // Homogenizar para obtener la posicion 3D y calcular distancia
        cam_pos.Homogenize();
        float dist = Vector4D::Distance(cam_pos, target);

        std::cout << "  " << std::setw(5) << t;
        std::cout << "  (" << std::setw(7) << cam_pos.x()
                  << ", " << std::setw(7) << cam_pos.y()
                  << ", " << std::setw(7) << cam_pos.z() << ")";
        std::cout << "   " << std::setw(8) << dist << "\n";
    }
}

// ─────────────────────────────────────────────
// main
// ─────────────────────────────────────────────

int main() {
    std::cout << "EJEMPLOS DE VECTOR4D\n";
    std::cout << "====================\n\n";
    std::cout << "Demuestra el uso de Vector4D para transformaciones 3D\n";
    std::cout << "en coordenadas homogeneas, operaciones matematicas y\n";
    std::cout << "aplicaciones en fisicas y graficos.\n";

    example1_constructors();
    example2_accessors();
    example3_arithmetic();
    example4_scalar_operations();
    example5_comparisons();
    example6_dot_and_component_multiply();
    example7_magnitude_and_normalization();
    example8_homogeneous_coordinates();
    example9_distance();
    example10_interop_and_data();
    example11_particle_simulation();
    example12_camera_interpolation();

    std::cout << "\n========================================\n";
    std::cout << "TODOS LOS EJEMPLOS COMPLETADOS\n";
    std::cout << "========================================\n";

    return 0;
}