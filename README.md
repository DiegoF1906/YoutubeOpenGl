# YoutubeOpenGL

Proyecto de gráficos por computadora desarrollado en C++ y OpenGL en Visual Studio, con carga de texturas, cámara,iluminacióny de curvas Bézier.


Nombre: Diego Fabián Hernández Rincón
Código: 6014381

## Estructura del software

```text
YoutubeOpenGL/
├── Archivos de encabezado/       # Definiciones e interfaces de OpenGL
│   ├── EBO.h                     # Element Buffer Object (índices de vértices)
│   ├── VAO.h                     # Vertex Array Object (estados de atributos)
│   ├── VBO.h                     # Vertex Buffer Object (datos en memoria GPU)
│   └── shaderClass.h             # Gestión y compilación de programas Shader
│
├── Archivos de origen/           # Implementación principal y punto de entrada
│   ├── Main.cpp                  # Bucle de renderizado, cámara y texturas
│   ├── EBO.cpp                   # Implementación de EBO
│   ├── VAO.cpp                   # Implementación de VAO
│   ├── VBO.cpp                   # Implementación de VBO
│   ├── shaderClass.cpp           # Lectura y enlace de shaders GLSL
│   └── glad.c                    # Cargador de punteros a funciones OpenGL
│
├── Bezier/                       # Módulo de modelado paramétrico
│   ├── BezierCurve.h             # Definición de puntos de control y curva
│   └── BezierCurve.cpp           # Cálculo matemático de la curva Bézier
│
├── Model/                        # Módulo de carga de geometría 3D
│   ├── Mesh.h / Mesh.cpp         # Representación de mallas poligonales
│   └── Model.h / Model.cpp       # Carga y dibujo de modelos completos
│
└── Archivos de recursos/         # Recursos gráficos y programas GLSL
    └── Shaders/
        ├── default.vert / default.frag         # Shaders principales de escena
        ├── background.vert / background.frag   # Shaders para el fondo texturizado
        └── bezier.vert / bezier.frag           # Shaders dedicados a curvas Bézier