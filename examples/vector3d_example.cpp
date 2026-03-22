#include "physics/math/vector3d.hpp"
#include <iostream>

int main() {
    // Ejemplo: Movimiento simple
    Vector3D posicion(0, 0, 0);
    Vector3D velocidad(10, 20, 0);
    Vector3D gravedad(0, -9.8f, 0);
    
    float delta_time = 0.1f;
    
    // Actualizar posición
    posicion += velocidad * delta_time;
    velocidad += gravedad * delta_time;
    
    std::cout << "Posición: (" 
              << posicion.x() << ", " 
              << posicion.y() << ", " 
              << posicion.z() << ")\n";
    
    std::cout << "Velocidad: (" 
              << velocidad.x() << ", " 
              << velocidad.y() << ", " 
              << velocidad.z() << ")\n";
    
    // Calcular distancia al origen
    float distancia = Vector3D::Distance(Vector3D(0, 0, 0), posicion);
    std::cout << "Distancia al origen: " << distancia << "\n";
    
    return 0;
}