#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aGroupID;

out vec2 TexCoord;
out float GroupID;

uniform vec2 positionOffset;

void main()
{
    gl_Position = vec4(aPos.x + positionOffset.x, aPos.y + positionOffset.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
    GroupID = aGroupID;
}