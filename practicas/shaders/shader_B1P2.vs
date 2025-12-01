#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aGroup;

out vec3 ourColor;
out float groupID;

uniform vec2 positionOffset;

void main()
{
    gl_Position = vec4(aPos.x + positionOffset.x, aPos.y + positionOffset.y, aPos.z, 1.0);
    ourColor = aColor;
    groupID = aGroup;
}
