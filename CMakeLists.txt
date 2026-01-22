cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine VERSION 0.1.0 LANGUAGES CXX)

# Estandar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opciones
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# Biblioteca principal
add_library(physics 
    src/math/Vector3D.cpp 
    src/math/Matrix3.cpp)

# Headers
target_include_directories(physics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Tests
if (BUILD_TESTS)
    enable_testing()
    add_executable(test_vector3d tests/math/test_Vector3D.cpp)
    target_link_libraries(test_vector3d physics)
    add_test(NAME vector3d_test COMMAND test_vector3d)

    add_executable(test_matrix3 tests/math/test_Matrix3.cpp)
    target_link_libraries(test_matrix3 physics)
    add_test(NAME matrix3_test COMMAND test_matrix3)
endif()

# Ejemplos
if (BUILD_EXAMPLES)
    add_executable(example_vector3d examples/vector_example.cpp)
    target_link_libraries(example_vector3d physics)
endif()