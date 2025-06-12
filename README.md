# CC4102 - Tarea 2: Algoritmo de Kruskal

## Cómo Ejecutar

### Experimento Principal (Requerido para T2)

```bash
make run
```

**Genera**: `resultados/df.csv` y `resultados/logs.txt` con todos los datos experimentales.

**Nota**: El experimento usa 5 hilos paralelos para acelerar la ejecución. Si experimentas problemas de rendimiento, puedes modificar `MAX_N_THREADS` en `main.cpp`.

### Tests de Verificación
```bash
# Test rápido (verifica que todo funciona)
make test-quick

# Test comprehensivo (prueba exhaustiva)
make test-comprehensive
```

### Compilación Manual
```bash
# Solo compilar
make compilate

# Solo ejecutar (después de compilar)
make execute

# Limpiar archivos generados
make clean
```

## Descripción
Implementación del algoritmo de Kruskal para encontrar el árbol cobertor mínimo (MST) con las 4 variantes requeridas en T2.pdf:

1. **ArraySort + UnionFind básico** - Array ordenado sin path compression
2. **ArraySort + UnionFind optimizado** - Array ordenado con path compression  
3. **HeapMin + UnionFind básico** - Min-heap sin path compression
4. **HeapMin + UnionFind optimizado** - Min-heap con path compression

## Mapeo de Requerimientos → Código

| Requerimiento del PDF | Ubicación en el Código | Descripción |
|----------------------|------------------------|-------------|
| **Union-Find con/sin optimización** | `src/union_find.hpp` | Clase `UnionFind` con parámetro `path_compression` |
| **Algoritmo de Kruskal** | `src/kruskal.cpp` | Función `kruskal()` que implementa las 4 variantes |
| **Estructura heap clásico** | `include/edge_extractor.h` | Clase `HeapMin` usando `std::priority_queue` |
| **Estructura array ordenado** | `include/edge_extractor.h` | Clase `ArraySort` usando `std::sort` |
| **Sistema de punteros** | `include/utils.h` | Estructuras `node` y `edge` según especificación |
| **Experimentos N∈{2⁵...2¹²}** | `main.cpp` | Generación de 5 secuencias por tamaño |
| **Medición de tiempos** | `main.cpp`, `test_comprehensive.cpp` | Tiempos de inserción y Kruskal |

## Estructura del Proyecto

```
cc4102-kruskal/
├── include/                    # Headers con interfaces
│   ├── edge_extractor.h       # Clases HeapMin y ArraySort
│   ├── kruskal.h             # Interfaz del algoritmo de Kruskal
│   └── utils.h               # Estructuras node, edge, datapoint
├── src/                      # Implementaciones
│   ├── kruskal.cpp           # Algoritmo de Kruskal (4 variantes)
│   └── union_find.hpp        # Union-Find con/sin path compression
├── resultados/               # Archivos de salida
│   ├── df.csv               # Datos experimentales principales
│   ├── logs.txt             # Log detallado de ejecución
│   └── kruskal_results.csv  # Resultados del test comprehensivo
├── test/                     # Tests y validaciones
│   ├── test_quick.cpp       # Test rápido de funcionamiento
│   ├── test_comprehensive.cpp  # Test completo de las 4 variantes
│   └── edge_extractor_validation.cpp  # Validación de componentes
├── main.cpp                  # Experimento principal (REQUERIDO)
└── Makefile                 # Sistema de compilación
```


## Resultados Generados

### `resultados/df.csv`
Datos principales para generar gráficos (formato requerido):
```
N, EdgeExtractorName, TimeInsertion, KruskalOptiPath, TimeKruskal;
32, ArraySort, 0.0001234, 1, 0.0005678;
32, ArraySort, 0.0001234, 0, 0.0007890;
...
```

### `resultados/logs.txt`
Log detallado de la ejecución con información de progreso.

### `resultados/kruskal_results.csv`
Resultados del test comprehensivo con estadísticas detalladas.

## Configuración de Experimentos

En `main.cpp` se pueden modificar los parámetros:
- `LOG2_N_INIT = 5` y `LOG2_N_END = 12`: Rango de tamaños (2⁵ a 2¹²)
- `LIMIT_SUBITER = 5`: Número de secuencias por tamaño
- `MAX_N_THREADS = 5`: Número máximo de hilos paralelos
- `SEED = 1234`: Semilla para reproducibilidad

## Requisitos del Sistema

- **Compilador**: g++ con soporte C++20 o superior
- **Memoria RAM**: Mínimo 4GB (recomendado 8GB para N=4096)
- **Tiempo estimado**: 30-60 minutos para experimento completo

## Verificación de Correctitud

El sistema incluye múltiples verificaciones:
- Las 4 variantes producen el mismo peso de MST para cada grafo
- El MST tiene exactamente n-1 aristas
- Comparación de resultados entre diferentes implementaciones

## Notas Importantes

- **No se incluye informe ni gráficos** (responsabilidad de otra persona según indicaciones)
- Los archivos de resultados se guardan automáticamente en `resultados/`
- El sistema usa paralelización para acelerar los experimentos
- Todos los experimentos son reproducibles con la misma semilla
