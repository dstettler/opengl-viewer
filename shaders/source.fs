#version 330 core
out vec4 FragColor;
in vec3 vertColor;

uniform float near = 0.1;
uniform float far = 1.0;

// 0 = uniform, 1 = phong, 2 = gouraud
uniform int lightingMode;
uniform int zBufferEnabled;

float LinearizeDepth(float depth) 
{
    float firstAdd = (far + near) / (far - near);
    float secondAdd = ((-2 * far * near) / (far - near)) / depth;
    return firstAdd +  secondAdd;
}

void main()
{
    if (zBufferEnabled == 1) {
        float depth = LinearizeDepth(gl_FragCoord.z); // / far;
        FragColor = vec4(vec3(depth), 1.0);
    } else {
        FragColor = vec4(vertColor, 1.0);
    }
}