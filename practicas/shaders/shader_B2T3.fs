#version 330 core
// Salida del fragment shader (color del pixel)
out vec4 FragColor;

// Entrada desde el vertex shader
in vec2 TexCoord;

// Sampler de textura
uniform sampler2D texture1;

void main()
{
    // Obtener color de la textura en las coordenadas UV
    FragColor = texture(texture1, TexCoord);
}