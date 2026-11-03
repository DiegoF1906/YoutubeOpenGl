#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    // COLOR DEL OBJETO
    vec3 objectColor = texture(tex0, TexCoord).rgb;

    // NORMAL
    vec3 norm = normalize(Normal);

    // DIRECCIÓN DE LA LUZ
    vec3 lightDir = normalize(lightPos - FragPos);

    // 1. ILUMINACIÓN AMBIENTAL
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // 2. ILUMINACIÓN DIFUSA
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 3. ILUMINACIÓN ESPECULAR
    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(
        max(dot(viewDir, reflectDir), 0.0),
        32.0
    );

    vec3 specular = specularStrength * spec * lightColor;

    // RESULTADO FINAL
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}