# Flujo de trabajo para desarrollar
1. Crear una rama `git checkout -b feature/modulo`
2. Escribir declaraciones en `modulo.hpp`
3. Escribir implementaciones en `modulo.cpp`
4. Escribir tests en `test_modulo.cpp`
5. Escribir ejemplo en `modulo_examples.cpp`
6. Añadir las declaraciones, implementaciones, test y ejemplos en `CMakeLists.txt`
7. Compilar para verificar las funciones
8. Añadir cambios a git con `git add files`
9. Subir cambios a git con `git commit -m ""`
10. Cambiar a la rama principal `git switch main`
11. Juntar los cambios del modulo con la rama principal `git merge feature/modulo`
12. Subir los cambios a Github `git push -u origin main`
13. Documentar todo el proceso
# Flujo del proyecto
La siguiente sección del `README.md` se centra en el progreso del proyecto, las partes implementadas, las partes en proceso de implementación y en progreso de implementación; así mismo, una sección que incluye la dirección del proyecto por etapas en las que se implementarán los diferentes módulos correspondientes.
## ✅ Completado
- **Core**
	- [x] Macros `core/macros`
	- [x] Configuraciones `core/config`
	- [x] Tipos `core/types`
- **Matemáticas** `/math`
	- [x] Vector3D `vector3d`
	- [x] Vector4D `vector4d`
	- [x] Matrix3D `matrix3d`
	- [x] Matrix4D `matrix4d`
	- [x] Quaternion `quaternion`
## 🔄 En progreso
## ❌ Sin implementar
- **Matemáticas** `/math`
	- [ ] Transformaciones `transform`
-  **Geometría** `/geometry`
	- [ ] AABB `aabb`
	- [ ] Ray o`ray`
	- [ ] Plano `plane`
	- [ ] Esfera `sphere` 
- **Dinámica** `/dynamics` 
	🎯 **Objetivo:** una esfera cayendo por gravedad
	- [ ] Integrador `integration/integrator`
	- [ ] Integrador de Euler `integration/euler`
	- [ ] Cuerpo rígido `body`
	- [ ] Material `material`
	- [ ] Mundo `world`
- **Colisiones** `/collisions`
	🎯 **Objetivo:** dos esferas que se detectan y reportan un contacto
	- [ ] Esférica `shapes/sphere_shape`
	- [ ] Base `shapes/shape`
	- [ ] Caja `shapes/box_shape`
	- [ ] Capsula `shapes/capsule_shape`
	- [ ] Malla `shapes/mesh_shape`
	- [ ] Fuerza bruta `broadphase/brute_force`
	- [ ] Árbol dinámico `broadphase/dynamic_aabb_tree`
	- [ ] `broadphase/sweep_and_prune`
	- [ ] Algoritmo de colisión `collision_algorithm`
	- [ ] Esfera contra esfera `narrowphase/sphere_sphere`
	- [ ] Esfera contra caja `narrowphase/sphere_box`
	- [ ] Detección de intersección entre convexos `gjk_epa/gjk`
	- [ ] Profundidad de penetración y normal `gjk_epa/epa`
- **Resoluciones de contacto**`/solver`
	🎯 **Objetivo:** dos esferas que rebotan y no se atraviesan
	- [ ] Restricciones de contacto `constaint/contact_constraint`
	- [ ] Solver `solver`
	- [ ] Impulso secuencial `sequential_impulse`
	- [ ] Solver de contactor `contact_solver`
- **Restricciones de articulaciones** `/constraints/Joints`
	- [ ] Restricciones `constraint`
	- [ ] Conjunto base `joint`
	- [ ] Conjunto arreglado `fixed_joint`
	- [ ] Conjunto bisagra `hinge_joint`
- **Optimizaciones**
	- [ ] Integrador de Verlet `integration/verlet_integrator`
	- [ ] Runge-Kutta `integration/runge_kutta4`
	- [ ] Asignador de memoria `utils/pool_allocator`
	- [ ] Grupo de objetos `utils/objects_pool`
	- [ ] Perfilador `utils/profiler`
	- [ ] Depuración `utils/debug_draw`
	- [ ] Visualizador OpenGL `tools/visualizer/opengl_backend
- Benchmarks `/benchmarks`
	- [ ] Matemáticas `benchmark_math`
	- [ ] Fase amplia `benchmark_broadphase`
	- [ ] Colisiones `benchmark_collision`
	- [ ] Solver `benchmark_solver`
	- [ ] Simulador `benchmark_world`
---
Cada punto de este proceso será debidamente documentado en su respectivo apartado con explicaciones de como funciona cada modulo y como contribuye al proyecto en general.
