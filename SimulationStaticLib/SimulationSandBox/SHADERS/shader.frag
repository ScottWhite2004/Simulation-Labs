#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec2 fragTexCoord;

layout(set = 0, binding = 1) uniform sampler2D texSampler;

// Match C++ LightType enum
const uint LIGHT_TYPE_POINT = 0u;
const uint LIGHT_TYPE_DIRECTIONAL = 1u;
const uint LIGHT_TYPE_SPOT = 2u;

struct GPULight
{
    vec3 position;  float _padPos;
    vec3 direction; float _padDir;
    vec3 color;     float _padColor;

    uint type;
    float ambient;
    float specular;
    float pad0;

    float innerCos;
    float outerCos;
    float range;
    float pad4;

    float attConst;
    float attLinear;
    float attQuadratic;
    float pad5;
};

layout(set = 0, binding = 2) uniform LightingUBO
{
    GPULight lights[8];
    vec3 viewPosWorld;
    float shininess;
    int lightCount;
    uint padA;
    uint padB;
    uint padC;
} lighting;

layout(location = 0) out vec4 outColor;

vec3 proceduralChecker(vec2 uv)
{
    const float checkerScale = 10.0;
    const vec3 colorA = vec3(0.08, 0.08, 0.08);
    const vec3 colorB = vec3(0.92, 0.92, 0.92);

    vec2 cell = floor(uv * checkerScale);
    float parity = mod(cell.x + cell.y, 2.0);

    return mix(colorA, colorB, parity);
}

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(lighting.viewPosWorld - fragPos);

    vec3 lit = vec3(0.0);

    int count = clamp(lighting.lightCount, 0, 8);
    for (int i = 0; i < count; ++i)
    {
        GPULight Ld = lighting.lights[i];

        vec3 L;
        float attenuation = 1.0;

        if (Ld.type == LIGHT_TYPE_DIRECTIONAL)
        {
            L = normalize(-Ld.direction);
        }
        else
        {
            vec3 toLight = Ld.position - fragPos;
            float dist = length(toLight);
            L = (dist > 0.0001) ? (toLight / dist) : vec3(0.0, 0.0, 1.0);

            attenuation = 1.0 / (Ld.attConst + Ld.attLinear * dist + Ld.attQuadratic * dist * dist);

            if (Ld.type == LIGHT_TYPE_SPOT)
            {
                float theta = dot(normalize(-Ld.direction), L);
                float eps = max(Ld.innerCos - Ld.outerCos, 0.0001);
                float intensity = clamp((theta - Ld.outerCos) / eps, 0.0, 1.0);
                attenuation *= intensity;
            }
        }

        float ndotl = max(dot(N, L), 0.0);
        vec3 ambient = Ld.ambient * Ld.color;
        vec3 diffuse = ndotl * Ld.color;

        vec3 R = reflect(-L, N);
        float spec = pow(max(dot(V, R), 0.0), max(lighting.shininess, 1.0));
        vec3 specular = Ld.specular * spec * Ld.color;

        lit += (ambient + diffuse + specular) * attenuation;
    }

    if (count == 0)
    {
        lit = vec3(1.0);
    }

    vec3 checker = proceduralChecker(fragTexCoord);
    vec3 texTint = texture(texSampler, fragTexCoord).rgb;

    vec3 baseColor = checker * texTint * fragColor;
    outColor = vec4(baseColor * lit, 1.0);
}