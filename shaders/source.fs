#version 330 core
out vec4 FragColor;
in vec3 vertColor;

uniform float near = 0.1;
uniform float far = 3.0;

uniform int zBufferEnabled;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    if (zBufferEnabled == 1) {
        float depth = LinearizeDepth(gl_FragCoord.z) / far;
        FragColor = vec4(vec3(depth), 1.0);
    } else {
        FragColor = vec4(vertColor, 1.0);
    }
}