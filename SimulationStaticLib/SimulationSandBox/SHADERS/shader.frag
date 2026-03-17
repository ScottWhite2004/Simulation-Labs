#version 450

// Inputs from the vertex shader
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;

// Uniform buffer containing lighting and camera data
layout(binding = 0) uniform LightingUBO {
    vec3 lightPos;
    vec3 viewPos;
    vec3 lightColor;
} ubo;

layout(location = 0) out vec4 outColor;

void main() {
    // 1. Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * ubo.lightColor;
    
    // Normalize the normal vector (interpolated normals might not be unit length)
    vec3 norm = normalize(fragNormal);
    
    // 2. Diffuse lighting
    vec3 lightDir = normalize(ubo.lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * ubo.lightColor;
    
    // 3. Specular lighting
    float specularStrength = 0.5;
    float shininess = 32.0;
    vec3 viewDir = normalize(ubo.viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * ubo.lightColor;  
    
    // Combine results
    vec3 result = (ambient + diffuse + specular) * fragColor;
    outColor = vec4(result, 1.0);
}