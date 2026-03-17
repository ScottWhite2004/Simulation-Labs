#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

// Inputs from the vertex buffer
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal; // Added: Normal from the vertex data

// Outputs down the pipeline to the fragment shader
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;   // Added: World position of the fragment
layout(location = 2) out vec3 fragNormal; // Added: World space normal

void main() {
    // Standard screen-space projection
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    
    // Pass color to fragment shader
    fragColor = inColor;
    
    // Calculate world position for lighting calculations
    fragPos = vec3(ubo.model * vec4(inPosition, 1.0));
    
    // Transform the normal vector to world space
    // Using transpose(inverse()) ensures normals remain correct even if the object is scaled non-uniformly
    fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal;
}