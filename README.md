# YoutubeOpenGL

Proyecto de gráficos por computadora desarrollado en C++ y OpenGL en Visual Studio, con carga de texturas, cámara,iluminacióny de curvas Bézier.


Nombre: Diego Fabián Hernández Rincón
Código: 6014381

## Estructura del software

YoutubeOpenGL/
├── Archivos de encabezado/
│   ├── EBO.h
│   ├── shaderClass.h
│   ├── VAO.h
│   └── VBO.h
├── Archivos de origen/
│   ├── EBO.cpp
│   ├── glad.c
│   ├── Main.cpp
│   ├── shaderClass.cpp
│   ├── VAO.cpp
│   └── VBO.cpp
├── Archivos de recursos/
│   └── Shaders/
│       ├── bezier.frag
│       ├── default.frag
│       ├── default.vert
│       ├── bezier.vert
│       ├── background.vert
│       └── background.frag
├── Model/
│   ├── Mesh.cpp
│   ├── Mesh.h
│   ├── Model.cpp
│   └── Model.h
└── Bezier/
    ├── BezierCurve.h
    └── BezierCurve.cpp