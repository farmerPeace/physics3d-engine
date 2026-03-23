cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine VERSION 0.1.0 LANGUAGES CXX)

# Estandar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Opciones
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# Biblioteca principal
add_library(physics 
    src/math/vector3d.cpp 
    src/math/matrix3d.cpp
    src/math/vector4d.cpp
    src/math/matrix4d.cpp
    src/math/quaternion.cpp
    )

# Headers
target_include_directories(physics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Tests
if (BUILD_TESTS)
    enable_testing()
    add_executable(test_vector3d tests/math/test_vector3d.cpp)
    target_link_libraries(test_vector3d physics)
    add_test(NAME vector3d_test COMMAND test_vector3d)

    add_executable(test_matrix3d tests/math/test_matrix3d.cpp)
    target_link_libraries(test_matrix3d physics)
    add_test(NAME matrix3d_test COMMAND test_matrix3d)

    add_executable(test_vector4d tests/math/test_vector4d.cpp)
    target_link_libraries(test_vector4d physics)
    add_test(NAME vector4d_test COMMAND test_vector4d)

    add_executable(test_matrix4d tests/math/test_matrix4d.cpp)
    target_link_libraries(test_matrix4d physics)
    add_test(NAME matrix4d_test COMMAND test_matrix4d)

    add_executable(test_quaternion tests/math/test_quaternion.cpp)
    target_link_libraries(test_quaternion physics)
    add_test(NAME quaternion_test COMMAND test_quaternion)
endif()

# Ejemplos
if (BUILD_EXAMPLES)
    add_executable(example_vector3d examples/vector3d_example.cpp)
    target_link_libraries(example_vector3d physics)

    add_executable(example_vector4d examples/vector4d_example.cpp)
    target_link_libraries(example_vector4d physics)

    add_executable(example_matrix3d examples/matrix3d_example.cpp)
    target_link_libraries(example_matrix3d physics)

    add_executable(example_matrix4d examples/matrix4d_example.cpp)
    target_link_libraries(example_matrix4d physics)
endif()