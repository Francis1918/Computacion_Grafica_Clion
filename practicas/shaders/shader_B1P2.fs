#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in float groupID;

uniform vec3 colorModifier;
uniform bool isAnimating;
uniform float timeValue;

void main()
{
    vec3 finalColorModifier = colorModifier;
    if (isAnimating)
    {
        float hue = timeValue * 0.5f + groupID * 0.8976f; // 2*PI / 7 ≈ 0.8976 para distribuir fases
        finalColorModifier.r = (sin(hue) + 1.0) / 2.0;
        finalColorModifier.g = (sin(hue + 2.0944) + 1.0) / 2.0; // 120 grados
        finalColorModifier.b = (sin(hue + 4.1888) + 1.0) / 2.0; // 240 grados
    }
    FragColor = vec4(ourColor * finalColorModifier, 1.0f);
}
