# cc4102-kruskal

## Descripción
Este proyecto implementa una prueba de rendimiento que utiliza el algoritmo de Kruskal para analizar diferentes métodos de extracción de aristas en un grafo. La implementación aprovecha la concurrencia para mejorar el rendimiento, utilizando hilos y colas de trabajo.

### Características Principales:
- Pruebas concurrentes con múltiples iteraciones
- Uso de colas de trabajo para manejar los experimentos
- Implementación usando paralelismo
- Registro detallado en archivos CSV y texto
- Optimización por caminos críticos
- Manejo eficiente de memoria

## Cómo Ejecutar el Código

### Requisitos
- Compilador C++23 (se recomienda g++)
- Bibliotecas estándar de C++
- Herramientas de desarrollo básicas

### Pasos para la Ejecución

1. **Ejecutar las Pruebas**
   ```bash
   make run
   ```

2. **Ver los Resultados**
   - Archivo CSV con resultados: `df.csv`
   - Registro detallado en tiempo real: `logs.txt`

## Descripción de los Archivos

### Makefile
Comandos:
- `make compilate`: Compila el ejecutable
- `make execute`: Ejecuta el programa
- `make clean`: Limpia archivos generados
- `make run`: Crea, ejecuta y limpia en un solo paso
- `make check`: Verifica la integridad del build

### main.cpp
Archivo principal que contiene:
- Configuración global del experimento
- Implementación de las colas de trabajo
- Lógica principal del bucle multithread
- Manejadores de sincronización y mutex

### Header Files (en la carpeta include)
- `utils.h`: Funciones auxiliares y estructuras de datos definidas
- `edge_extractor.h`: Clases base para la extracción de aristas
- `kruskal.h`: Implementación del algoritmo de Kruskal