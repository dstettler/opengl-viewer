#version 330 core
out vec4 FragColor;

in vec3 vertColor;
flat in vec3 vertColorFlat;
in vec3 fragPos;
in vec3 normal;

uniform float near = 0.1;
uniform float far = 1.0;

// 0 = uniform, 1 = phong, 2 = gouraud, 3 = flat
uniform int lightingMode;
uniform int zBufferEnabled;

uniform vec3 lightPos;
uniform float ambientLight;
uniform float specularStrength;
uniform float materialShine;

uniform vec3 viewPos;

float GetDepth(float depth) 
{
    float firstAdd = (far + near) / (far - near);
    float secondAdd = ((-2 * far * near) / (far - near)) / depth;
    return firstAdd +  secondAdd;
}

void main()
{
    if (zBufferEnabled == 1) {
        float depth = GetDepth(gl_FragCoord.z);
        FragColor = vec4(vec3(depth), 1.0);
    } else {
        // uniform and gouraud will both be the same, so only phong needs to be specified
        if (lightingMode == 1) {
            // diffuse
            vec3 normalized = normalize(normal);
            vec3 lightDir = normalize(lightPos - fragPos);
            float diffuseDiff = max(dot(normalized, lightDir), 0.0);
            vec3 diffuse = diffuseDiff * vec3(1.0, 1.0, 1.0);
            
            // specular
            vec3 viewDir = normalize(viewPos - fragPos);
            vec3 reflectionDir = reflect(-lightDir, normalized);
            float specularScaling = max(pow(max(dot(viewDir, reflectionDir), 0.0), materialShine), 0.0);
            vec3 specular = vec3(specularStrength * specularScaling);

            vec3 luminance = ambientLight + diffuse + specular;
            
            FragColor = vec4(luminance * vertColor, 1.0);
        } else if (lightingMode == 3) {
            FragColor = vec4(vertColorFlat, 1.0);
        } else {
            FragColor = vec4(vertColor, 1.0);
        }
    }
}