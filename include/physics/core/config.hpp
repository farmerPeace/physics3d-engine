// Archivo de configuración para todo el proyecto

# pragma once
// Tipo escalar global
// using real = float; para juegos, simulaciones en tiempo real, etc.
// using real = double; para mayor precisión, simulaciones científicas, etc.

using real = float;

// Tolerancia general
// Para comparaciones de igualdad, evitar problemas de precisión numérica, etc.

constexpr real PHYS_EPSILON = static_cast<real>(1e-6);

// Tolerancia geometrica
// Para distancias y deteccion de colisiones

constexpr real PHYS_EPSILON_GEOM = static_cast<real>(1e-4);

// Constantes matematicas
// Reemplazan definiciones locales

constexpr real PHYS_PI = static_cast<real>(3.14159265358979323846);
constexpr real PHYS_TWO_PI = static_cast<real>(2.0) * PHYS_PI;
constexpr real PHYS_HALF_PI = static_cast<real>(0.5) * PHYS_PI;
constexpr real PHYS_INV_PI = static_cast<real>(1.0) / PHYS_PI;

// Conversion de angulos

constexpr real PHYS_DEG_TO_RAD = PHYS_PI / static_cast<real>(180.0);
constexpr real PHYS_RAD_TO_DEG = static_cast<real>(180.0) / PHYS_PI;

// Constantes fisicas
// Valores usados para la simulacion en el mundo

constexpr real PHYS_GRAVITY_EARTH = static_cast<real>(-9.80665);
constexpr real PHYS_GRAVITY_MOON = static_cast<real>(-1.625);
constexpr real PHYS_GRAVITY_MARS = static_cast<real>(-3.72076);

// Parametros de simulación
// Valores por defecto para el paso de tiempo, iteraciones, etc.

constexpr real PHYS_DEFAULT_TIMESTEP = static_cast<real>(1.0 / 60.0); // Paso del tiempo por defecto
constexpr real PHYS_DEFAULT_RESTITUTION = static_cast<real>(0.5); // Coeficiente de rebote, 0 = sin rebote, 1 = rebote perfecto
constexpr real PHYS_DEFAULT_FRICTION = static_cast<real>(0.5); // Coeficiente de friccion, 0 = sin friccion, 1 = friccion completa

// Velocidades angular y lineal de un cuerpo dormido
constexpr real PHYS_SLEEP_LINEAR_THRESHOLD = static_cast<real>(0.01); // Velocidad lineal por debajo de la cual un cuerpo se considera dormido
constexpr real PHYS_SLEEP_ANGULAR_THRESHOLD = static_cast<real>(0.01); // Velocidad angular por debajo de la cual un cuerpo se considera dormido

// Limites del motor
constexpr int PHYS_MAX_BODIES = 4096; // Numero maximo de cuerpos en la simulacion
constexpr int PHYS_MAX_CONTACTS = 8192; // Numero maximo de contactos simultaneos
constexpr int PHYS_MAX_JOINTS = 1024; // Numero maximo de articulaciones
constexpr int PHYS_MAX_ITERATIONS = 10; // Numero maximo de iteraciones para resolver el sistema de ecuaciones

// Flags de compilación
// Se activan o desactivan características específicas del proyecto
// Se edita en CMakeLists.txt, ejemplo:
//          target_compile_definitions(physics PUBLIC PHYS_DEBUG PHYS_ENABLE_PROFILER)
// Si no esta definido, se define

#ifndef PHYS_DEBUG
#define PHYS_DEBUG 0
#endif

#ifndef PHYS_ENABLE_PROFILER
#define PHYS_ENABLE_PROFILER 0
#endif