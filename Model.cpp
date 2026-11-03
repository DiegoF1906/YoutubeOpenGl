// IMPLEMENTACIÓN DE TINYGLTF Y STB
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

#include "Model.h"

#include <iostream>
#include <cstring>
#include <cmath>

// ==========================================
// LEER ATRIBUTOS DEL MODELO
// ==========================================

bool ReadAttribute(
    const tinygltf::Model& model,
    const tinygltf::Accessor& accessor,
    int components,
    std::vector<float>& output
)
{
    if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
        return false;

    if (accessor.bufferView < 0)
        return false;

    const tinygltf::BufferView& view =
        model.bufferViews[accessor.bufferView];

    const tinygltf::Buffer& buffer =
        model.buffers[view.buffer];

    int stride = accessor.ByteStride(view);

    if (stride < 0)
        return false;

    const unsigned char* data =
        buffer.data.data() +
        view.byteOffset +
        accessor.byteOffset;

    output.resize(accessor.count * components);

    for (size_t i = 0; i < accessor.count; i++)
    {
        for (int j = 0; j < components; j++)
        {
            std::memcpy(
                &output[i * components + j],
                data + i * stride + j * sizeof(float),
                sizeof(float)
            );
        }
    }

    return true;
}

// ==========================================
// LEER ÍNDICES
// ==========================================

bool ReadIndices(
    const tinygltf::Model& model,
    const tinygltf::Accessor& accessor,
    std::vector<GLuint>& indices
)
{
    const tinygltf::BufferView& view =
        model.bufferViews[accessor.bufferView];

    const tinygltf::Buffer& buffer =
        model.buffers[view.buffer];

    const unsigned char* data =
        buffer.data.data() +
        view.byteOffset +
        accessor.byteOffset;

    indices.resize(accessor.count);

    for (size_t i = 0; i < accessor.count; i++)
    {
        if (accessor.componentType ==
            TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
        {
            indices[i] = data[i];
        }
        else if (accessor.componentType ==
            TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
            unsigned short value;

            std::memcpy(
                &value,
                data + i * sizeof(unsigned short),
                sizeof(unsigned short)
            );

            indices[i] = value;
        }
        else if (accessor.componentType ==
            TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            unsigned int value;

            std::memcpy(
                &value,
                data + i * sizeof(unsigned int),
                sizeof(unsigned int)
            );

            indices[i] = value;
        }
        else
        {
            return false;
        }
    }

    return true;
}

// ==========================================
// GENERAR NORMALES SI NO EXISTEN
// ==========================================

void GenerateNormals(
    const std::vector<float>& positions,
    const std::vector<GLuint>& indices,
    std::vector<float>& normals
)
{
    size_t vertexCount = positions.size() / 3;

    normals.assign(vertexCount * 3, 0.0f);

    for (size_t i = 0; i + 2 < indices.size(); i += 3)
    {
        GLuint a = indices[i];
        GLuint b = indices[i + 1];
        GLuint c = indices[i + 2];

        float ax = positions[a * 3];
        float ay = positions[a * 3 + 1];
        float az = positions[a * 3 + 2];

        float bx = positions[b * 3];
        float by = positions[b * 3 + 1];
        float bz = positions[b * 3 + 2];

        float cx = positions[c * 3];
        float cy = positions[c * 3 + 1];
        float cz = positions[c * 3 + 2];

        float ux = bx - ax;
        float uy = by - ay;
        float uz = bz - az;

        float vx = cx - ax;
        float vy = cy - ay;
        float vz = cz - az;

        float nx = uy * vz - uz * vy;
        float ny = uz * vx - ux * vz;
        float nz = ux * vy - uy * vx;

        for (GLuint index : { a, b, c })
        {
            normals[index * 3] += nx;
            normals[index * 3 + 1] += ny;
            normals[index * 3 + 2] += nz;
        }
    }

    for (size_t i = 0; i < vertexCount; i++)
    {
        float x = normals[i * 3];
        float y = normals[i * 3 + 1];
        float z = normals[i * 3 + 2];

        float length = std::sqrt(x * x + y * y + z * z);

        if (length > 0.0f)
        {
            normals[i * 3] /= length;
            normals[i * 3 + 1] /= length;
            normals[i * 3 + 2] /= length;
        }
        else
        {
            normals[i * 3 + 1] = 1.0f;
        }
    }
}

// ==========================================
// CONSTRUCTOR DEL MODELO
// ==========================================

Model::Model(const std::string& path)
{
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF loader;

    std::string error;
    std::string warning;

    bool loaded = false;

    // Detectar si es GLB o GLTF
    if (path.size() >= 4 &&
        path.substr(path.size() - 4) == ".glb")
    {
        loaded = loader.LoadBinaryFromFile(
            &gltfModel,
            &error,
            &warning,
            path
        );
    }
    else
    {
        loaded = loader.LoadASCIIFromFile(
            &gltfModel,
            &error,
            &warning,
            path
        );
    }

    if (!warning.empty())
        std::cout << "GLTF Warning: " << warning << std::endl;

    if (!error.empty())
        std::cout << "GLTF Error: " << error << std::endl;

    if (!loaded)
    {
        std::cout << "No se pudo cargar el modelo: "
            << path << std::endl;
        return;
    }

    std::cout << "Modelo cargado correctamente" << std::endl;

    // ======================================
    // RECORRER LAS MALLAS
    // ======================================

    for (const auto& mesh : gltfModel.meshes)
    {
        for (const auto& primitive : mesh.primitives)
        {
            // Esta primera versión dibuja triángulos
            if (primitive.mode != -1 &&
                primitive.mode != TINYGLTF_MODE_TRIANGLES)
            {
                continue;
            }

            // Buscar posiciones
            auto positionIt =
                primitive.attributes.find("POSITION");

            if (positionIt == primitive.attributes.end())
                continue;

            const tinygltf::Accessor& positionAccessor =
                gltfModel.accessors[positionIt->second];

            std::vector<float> positions;

            if (!ReadAttribute(
                gltfModel,
                positionAccessor,
                3,
                positions
            ))
            {
                std::cout << "Error leyendo posiciones" << std::endl;
                continue;
            }

            size_t vertexCount = positions.size() / 3;

            // ==================================
            // NORMALES
            // ==================================

            std::vector<float> normals;

            auto normalIt =
                primitive.attributes.find("NORMAL");

            if (normalIt != primitive.attributes.end())
            {
                const tinygltf::Accessor& normalAccessor =
                    gltfModel.accessors[normalIt->second];

                if (!ReadAttribute(
                    gltfModel,
                    normalAccessor,
                    3,
                    normals
                ))
                {
                    normals.clear();
                }
            }

            // ==================================
            // ÍNDICES
            // ==================================

            std::vector<GLuint> indices;

            if (primitive.indices >= 0)
            {
                const tinygltf::Accessor& indexAccessor =
                    gltfModel.accessors[primitive.indices];

                if (!ReadIndices(
                    gltfModel,
                    indexAccessor,
                    indices
                ))
                {
                    std::cout << "Error leyendo indices" << std::endl;
                    continue;
                }
            }
            else
            {
                for (GLuint i = 0; i < vertexCount; i++)
                    indices.push_back(i);
            }

            // Si no hay normales, calcularlas
            if (normals.empty())
            {
                GenerateNormals(
                    positions,
                    indices,
                    normals
                );
            }

            // ==================================
            // COORDENADAS UV
            // ==================================

            std::vector<float> texCoords(vertexCount * 2, 0.0f);

            auto uvIt =
                primitive.attributes.find("TEXCOORD_0");

            if (uvIt != primitive.attributes.end())
            {
                const tinygltf::Accessor& uvAccessor =
                    gltfModel.accessors[uvIt->second];

                std::vector<float> uvData;

                if (ReadAttribute(
                    gltfModel,
                    uvAccessor,
                    2,
                    uvData
                ))
                {
                    texCoords = uvData;
                }
            }

            // ==================================
            // UNIR LOS DATOS DE LOS VÉRTICES
            // ==================================

            std::vector<float> vertices;

            for (size_t i = 0; i < vertexCount; i++)
            {
                // Posición
                vertices.push_back(positions[i * 3]);
                vertices.push_back(positions[i * 3 + 1]);
                vertices.push_back(positions[i * 3 + 2]);

                // Normal
                vertices.push_back(normals[i * 3]);
                vertices.push_back(normals[i * 3 + 1]);
                vertices.push_back(normals[i * 3 + 2]);

                // UV
                vertices.push_back(texCoords[i * 2]);
                vertices.push_back(texCoords[i * 2 + 1]);
            }

            // Crear la malla
            meshes.push_back(
                new Mesh(vertices, indices)
            );
        }
    }

    std::cout << "Cantidad de mallas: "
        << meshes.size() << std::endl;
}

// ==========================================
// DIBUJAR MODELO
// ==========================================

void Model::Draw()
{
    for (Mesh* mesh : meshes)
    {
        mesh->Draw();
    }
}

// ==========================================
// DESTRUCTOR
// ==========================================

Model::~Model()
{
    for (Mesh* mesh : meshes)
    {
        delete mesh;
    }

    meshes.clear();
}