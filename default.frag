#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform vec4 baseColorFactor;

void main()
{
    // COLOR Y TEXTURA DEL MATERIAL
    vec4 texColor = texture(tex0, TexCoord);

    vec3 objectColor =
        texColor.rgb * baseColorFactor.rgb;

    // NORMAL
    vec3 norm = normalize(Normal);

    // DIRECCIÓN DE LA LUZ
    vec3 lightDir = normalize(lightPos - FragPos);

    // LUZ AMBIENTAL
    float ambientStrength = 0.4;

    vec3 ambient = ambientStrength * lightColor;

    // LUZ DIFUSA
    float diff = max(dot(norm, lightDir), 2.0);

    vec3 diffuse = diff * lightColor;

    // LUZ ESPECULAR
    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(
        max(dot(viewDir, reflectDir), 0.0),
        32.0
    );

    vec3 specular =
        specularStrength * spec * lightColor;

    // RESULTADO
    vec3 result =
        (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(
        result,
        texColor.a * baseColorFactor.a
    );
}