#pragma once
//  Macros de utilidad del motor: detección de plataforma, control de
//  inlining, sistema de aserciones y marcadores semánticos.

#include "physics/core/types.hpp"

#include <cstdio>    // std::fprintf, stderr
#include <cstdlib>   // std::abort


// ─── Detección de plataforma ──────────────────────────────────────────────────
#if defined(_WIN32) || defined(_WIN64)
    #define PHYS_PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #define PHYS_PLATFORM_APPLE
#elif defined(__linux__)
    #define PHYS_PLATFORM_LINUX
#else
    #define PHYS_PLATFORM_UNKNOWN
#endif


// ─── Control de inlining ──────────────────────────────────────────────────────
//
// PHYS_FORCE_INLINE: orden al compilador de insertar el cuerpo de la
// función en el punto de llamada. Se aplica a funciones llamadas millones
// de veces por frame: Dot3D, operator+, Magnitude, etc.
//
// PHYS_NO_INLINE: lo contrario. Se aplica a funciones de error y
// diagnóstico para no duplicar su código en cada punto de uso y no
// inflar el binario.

#if defined(_MSC_VER)
    #define PHYS_FORCE_INLINE  __forceinline
    #define PHYS_NO_INLINE     __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
    #define PHYS_FORCE_INLINE  __attribute__((always_inline)) inline
    #define PHYS_NO_INLINE     __attribute__((noinline))
#else
    #define PHYS_FORCE_INLINE  inline
    #define PHYS_NO_INLINE
#endif

// ─── Manejador interno de aserciones ─────────────────────────────────────────
// PHYS_NO_INLINE garantiza que el compilador no la fusione con el código
// circundante. 

PHYS_NO_INLINE inline void phys_assert_fail(
    const char* expr,
    const char* file,
    int         line,
    const char* func,
    const char* msg = nullptr)
{
    std::fprintf(stderr, "\n[PHYS ASSERT FAILED]\n");
    std::fprintf(stderr, "  Expresion : %s\n", expr);
    std::fprintf(stderr, "  Archivo   : %s:%d\n", file, line);
    std::fprintf(stderr, "  Funcion   : %s\n", func);
    if (msg)
    {
        std::fprintf(stderr, "  Mensaje   : %s\n", msg);
    }
    std::fprintf(stderr, "\n");
    std::abort();
}

// ─── Macros de aserción en runtime ───────────────────────────────────────────
//
// Solo activas cuando PHYS_DEBUG está definido (modo debug).
// En release ambas se convierten en ((void)(expr)) — costo exactamente cero.
//
// El patrón do { ... } while(0) permite usar la macro como una sentencia
// ordinaria en cualquier contexto, incluyendo un if sin llaves:
//
//   if (mass <= 0)          <- sin do-while, el `else` siguiente
//       PHYS_ASSERT(false); <- se asociaría al if interno de la macro,
//   else                    <- no al if externo del usuario.
//       simulate();
//
// El operador # convierte la expresión en un string literal en compilación:
//   PHYS_ASSERT(dot > 0.0f)  →  mensaje: "dot > 0.0f"
//
// Uso:
//   PHYS_ASSERT(ptr != nullptr);
//   PHYS_ASSERT_MSG(mass > 0, "La masa debe ser estrictamente positiva");

#ifdef PHYS_DEBUG

    #define PHYS_ASSERT(expr)                                   \
        do {                                                    \
            if (!(expr)) {                                      \
                phys_assert_fail(                               \
                    #expr,                                      \
                    __FILE__,                                   \
                    __LINE__,                                   \
                    __func__                                    \
                );                                              \
            }                                                   \
        } while(0)

    #define PHYS_ASSERT_MSG(expr, msg)                          \
        do {                                                    \
            if (!(expr)) {                                      \
                phys_assert_fail(                               \
                    #expr,                                      \
                    __FILE__,                                   \
                    __LINE__,                                   \
                    __func__,                                   \
                    msg                                         \
                );                                              \
            }                                                   \
        } while(0)

#else

    // En release: evalúa la expresión (por posibles efectos secundarios)
    // pero descarta el resultado. El compilador elimina la evaluación
    // si la expresión no tiene efectos secundarios.
    #define PHYS_ASSERT(expr)          ((void)(expr))
    #define PHYS_ASSERT_MSG(expr, msg) ((void)(expr))

#endif


// ─── Aserción en tiempo de compilación ───────────────────────────────────────
//
// Se evalúa durante la compilación — si la condición es falsa el programa
// directamente no compila. No necesita #ifdef PHYS_DEBUG porque no genera
// código en el binario: si la condición es verdadera desaparece por completo.
//
// Uso típico en el motor:
//   PHYS_STATIC_ASSERT(sizeof(Vector3D) == 12);
//   PHYS_STATIC_ASSERT_MSG(sizeof(real) == 4, "Esperado float de 32 bits");

#define PHYS_STATIC_ASSERT(expr) \
    static_assert(expr, #expr)

#define PHYS_STATIC_ASSERT_MSG(expr, msg) \
    static_assert(expr, msg)


// ─── Operaciones de bits ──────────────────────────────────────────────────────
//
// Para máscaras de flags: capas de colisión, propiedades de cuerpos, etc.
// Un entero donde cada bit representa un estado o categoría independiente.
//
// Se usa `1u` (unsigned) en lugar de `1` (int con signo) porque desplazar
// un bit con signo hasta la posición 31 (el bit de signo) es comportamiento
// indefinido en C++. Con `1u` el resultado es siempre bien definido.
//
// Uso:
//   u32 flags = 0;
//   PHYS_SET_BIT(flags, 3);          // activa la capa 3
//   if (PHYS_TEST_BIT(flags, 3)) ... // comprueba la capa 3
//   PHYS_CLEAR_BIT(flags, 3);        // desactiva la capa 3

#define PHYS_BIT(n)           (1u << (n))
#define PHYS_SET_BIT(x, n)    ((x) |=  PHYS_BIT(n))
#define PHYS_CLEAR_BIT(x, n)  ((x) &= ~PHYS_BIT(n))
#define PHYS_TEST_BIT(x, n)   (((x) >> (n)) & 1u)


// ─── Marcadores semánticos ────────────────────────────────────────────────────
//
// No generan código — comunican intención al compilador y al programador.

// Silencia el warning de parámetro no usado. Necesario al implementar
// interfaces donde un parámetro es obligatorio por la firma pero no se
// usa en esa versión concreta.
//   void OnCollision(BodyID a, BodyID b, PHYS_UNUSED(ContactInfo info)) { ... }
#define PHYS_UNUSED(x) ((void)(x))

// Obliga al caller a usar el valor de retorno. El compilador emite un
// warning si el resultado se descarta. Aplicar a funciones como
// Normalized(), Inverted() o AddBody() — que devuelven una copia o un
// resultado que silenciosamente se pierde si no se asigna.
//
//   PHYS_NODISCARD Vector4D Normalized() const;
//   PHYS_NODISCARD PhysResult AddBody(const BodyDesc& desc);
#define PHYS_NODISCARD [[nodiscard]]

// Marca una API como obsoleta. El compilador emite un warning en cada
// punto de uso, guiando al programador hacia la alternativa.
//
//   PHYS_DEPRECATED void OldIntegrate(float dt);
#define PHYS_DEPRECATED [[deprecated]]

// Hints de predicción de ramas. Hoy son no-ops — el compilador moderno
// hace buen trabajo sin ellos en la mayoría de casos. El placeholder
// existe para sustituirlos por __builtin_expect cuando el profiler
// identifique un hot path con una rama muy sesgada.
//
//   if (PHYS_LIKELY(body->IsActive())) { ... }
#define PHYS_LIKELY(x)   (x)
#define PHYS_UNLIKELY(x) (x)