# YoutubeOpenGL — Motor Gráfico 3D y Curvas Bézier

Aplicación gráfica desarrollada en **C++** y **OpenGL** dentro de Visual Studio. El proyecto implementa renderizado en tiempo real con gestión de buffers de memoria en GPU, carga de texturas espaciales de fondo, modelos 3D mallados y evaluación de curvas paramétricas de Bézier.

---

## Información del Estudiante

* **Autor:** Diego Fabián Hernández Rincón
* **Código:** `[6014381]`


---

## Características Principales

* **Gestión de Buffers de OpenGL:** Abstracción orientada a objetos para `VAO`, `VBO` y `EBO`.
* **Pipeline de Shaders Personalizados:** Compilación y enlace dinámico de shaders de vértices y fragmentos (`.vert` / `.frag`) para geometría base, fondo y curvas.
* **Renderizado de Curvas Bézier:** Módulo dedicado (`BezierCurve`) con su propio programa de shaders (`bezier.vert` y `bezier.frag`) para el cálculo y trazado de curvas suaves.
* **Carga de Modelos:** Soporte estructural para importar y renderizar modelos 3D mediante las clases `Model` y `Mesh`.
* **Texturizado y Cámara:** Carga de imágenes mediante `stb_image` (ej. `fondoespacial.png`) con configuración de filtrado lineal (`GL_LINEAR`) y ajuste de bordes (`GL_CLAMP_TO_EDGE`), además de control de parámetros de cámara (`cameraAngle`, `cameraHeight`, `cameraDistance`).

---

## Arquitectura y Estructura del Software

El proyecto está organizado modularmente separando la lógica de núcleo de OpenGL, la geometría avanzada y los recursos gráficos:

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