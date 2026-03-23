#pragma once
// Todos los tipos usados en el proyecto se definen aquí, para mantener consistencia y facilitar cambios futuros

#include "config.hpp"
#include <cstdint>
#include <cstddef>

// Enteros de tamaño fijo
using u8 = uint8_t; // Flags de bits
using u16 = uint16_t; // Contadores medianos
using u32 = uint32_t; // IDs, indices o contadores
using u64 = uint64_t; // Manejos de recursos

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t; // Indices negativos
using i64 = int64_t;

// Alias explicitos de escalares
using f32 = float;
using f64 = double;

using usize = size_t;
using isize = ptrdiff_t;

// Tipos de identidad
using BodyID = u32;
using ShapeID = u32;
using MaterialID = u32;
using ConstraintID = u32;
using WorldID = u32;

// Tipos de cuerpo rigido
enum class BodyType : u8 {
    Static, // No se mueve, no responde a fuerzas, colisiona con otros cuerpos
    Dynamic, // Se mueve, responde a fuerzas, colisiona con otros cuerpos
    Kinematic, // Se mueve, no responde a fuerzas, colisiona con otros cuerpos
};

// Tipos de formas
enum class ShapeType : u8 {
    Sphere,
    Box,
    Capsule,
    Mesh,
};

// Tipo de integrador
enum class IntegratorType : u8 {
    Euler,
    Verlet,
    RungeKutta4,
};

// Estado de cuerpo
enum class BodyState : u8 {
    Active, // El cuerpo esta activo, se simula normalmente
    Sleeping, // El cuerpo esta dormido, no se simula hasta que se despierte por una colision o fuerza
    Disabled, // El cuerpo esta congelado, no se simula ni se despierta, solo se mueve por teletransporte
};

// Colisiones
enum class CollisionType : u8 {
    Broadphase, // Pares candidatos por AABB
    Narrowphase, // Contacto real con normal y penetracion
    Solver, // Resolcion de impulsos
};

// Resultados de operaciones
enum class PhysResult : u8 {
    Ok,
    ErrorWorldFull, // Maximo numero de cuerpos, contactos o articulaciones alcanzado
    ErrorInvalidArgument, // Parametro nulo o fuera de rango
    ErrorNotFound, // ID no existe en el mundo
    ErrorNotImplemented, // Funcionalidad no implementada o no soportada
};

