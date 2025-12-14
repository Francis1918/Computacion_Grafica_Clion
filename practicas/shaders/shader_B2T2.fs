#version 330 core
in vec2 TexCoord;
in float GroupID;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform bool texSwap;

void main()
{
    // Optimización: lógica de intercambio en GPU
    bool useTexture1 = (GroupID == 0.0 && !texSwap) || (GroupID != 0.0 && texSwap);

    if (useTexture1)
        FragColor = texture(texture1, TexCoord);
    else
        FragColor = texture(texture2, TexCoord);
}