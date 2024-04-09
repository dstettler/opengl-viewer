#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 vertColor;
flat out vec3 vertColorFlat;
out vec3 fragPos;
out vec3 normal;

// 0 = uniform, 1 = phong, 2 = gouraud, 3 = flat
uniform int lightingMode;

uniform vec3 lightPos;
uniform float ambientLight;
uniform float specularStrength;
uniform float materialShine;

uniform mat4 modelTransform;
uniform vec3 viewPos;
uniform mat4 viewTransform;
uniform mat4 proj;

void main()
{
    gl_Position = proj * viewTransform * modelTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);

    fragPos = vec3(modelTransform * vec4(aPos, 1.0));
    normal = aNormal;

    // For gouraud lighting
    if (lightingMode == 2) {
        // diffuse
        vec3 normalized = normalize(aNormal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diffuseDiff = max(dot(normalized, lightDir), 0.0);
        vec3 diffuse = diffuseDiff * vec3(1.0, 1.0, 1.0);
        
        // specular
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectionDir = reflect(-lightDir, normalized);
        float specularScaling = max(pow(max(dot(viewDir, reflectionDir), 0.0), materialShine), 0.0);
        vec3 specular = vec3(specularStrength * specularScaling);

        vec3 luminance = ambientLight + diffuse + specular;
        
        vertColor = luminance * aColor;
    } else if (lightingMode == 3) {
        // diffuse
        vec3 normalized = normalize(aNormal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diffuseDiff = max(dot(normalized, lightDir), 0.0);
        vec3 diffuse = diffuseDiff * vec3(1.0, 1.0, 1.0);
        
        // specular
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectionDir = reflect(-lightDir, normalized);
        float specularScaling = max(pow(max(dot(viewDir, reflectionDir), 0.0), materialShine), 0.0);
        vec3 specular = vec3(specularStrength * specularScaling);

        vec3 luminance = ambientLight + diffuse + specular;
        
        vertColorFlat = luminance * aColor;
        vertColor = aColor;
    }
    else {
        vertColor = aColor;
    }
}