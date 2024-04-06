#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertColor;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 proj;

void main()
{
    gl_Position = proj * viewTransform * modelTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertColor = aColor;
}