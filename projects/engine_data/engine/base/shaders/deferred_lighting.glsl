#ifdef VERTEX

layout(location = 0) in vec3 _vertex;
layout(location = 2) in vec2 _uv;

out vec2 uv;

void main() {
  gl_Position = vec4(_vertex, 1.0);
  uv = _uv;
}

#endif

#ifdef FRAGMENT

out vec3 color;

uniform sampler2D MANTA_GBUFFER_POS; //GBuffer Pos
uniform sampler2D MANTA_GBUFFER_NORMAL; //GBuffer Norm
uniform sampler2D MANTA_GBUFFER_ALBEDO; //GBuffer Albedo
uniform sampler2D MANTA_GBUFFER_MRS; //GBuffer combined metallic, roughness, and specular maps
uniform sampler2D MANTA_GBUFFER_EMISSION; //GBuffer Emission

uniform samplerCube MANTA_CUBEMAP_ENVIRONMENT;

uniform sampler2D MANTA_TEX_BRDF_LUT; // BRDF LUT

uniform int MANTA_LIGHT_COUNT;
uniform vec3 MANTA_LIGHT_POSITIONS[32];
uniform vec3 MANTA_LIGHT_DIRECTIONS[32];
uniform vec3 MANTA_LIGHT_COLORS[32];
uniform float MANTA_LIGHT_RANGES[32];
uniform float MANTA_LIGHT_INTENSITIES[32];
uniform float MANTA_LIGHT_PARAMS1[32];
uniform float MANTA_LIGHT_PARAMS2[32];
uniform int MANTA_LIGHT_TYPES[32];

#define DEG_TO_RAD 0.01745329251

in vec2 uv;

uniform vec3 MANTA_CAMERA_POSITION;
uniform vec3 MANTA_AMBIENT_COLOR;

float sphereAtten(vec3 vert_pos, vec3 position, float range, float intensity) {
   vec3 direction = position - vert_pos;
   float dist = length(direction);

   return max(0.0, 1 - dist / range) * intensity;
}

float spotAtten(vec3 vert_pos, vec3 position, vec3 direction, float range, float intensity, float innerAngle, float outerAngle) {
   vec3 lightDir = position - vert_pos;
   float dist = length(lightDir);
   float spot = max(0.0, dot(normalize(direction), normalize(-lightDir)));

   // Fades the edges of the light
   float innerEpsilon = cos(innerAngle * DEG_TO_RAD);
   float outerEpsilon = cos(outerAngle * DEG_TO_RAD);
   float epsilon = innerEpsilon - outerEpsilon;
   if (spot > outerEpsilon)
      spot = clamp((spot - outerEpsilon) / epsilon, 0.0, 1.0);
   else
      spot = 0;

   return (spot * (range / dist)) * intensity;
}

float blinnPhongSpecular(vec3 vert_pos, vec3 normal, vec3 direction, float power) {
   vec3 halfway = normalize(normalize(direction) + normalize(MANTA_CAMERA_POSITION - vert_pos));
   return clamp(pow(max(dot(normalize(normal), halfway), 0.0), power), 0.0, 1.0);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


float fresnel(vec3 vert_pos, vec3 normal) {
   vec3 camera_direction = vert_pos - MANTA_CAMERA_POSITION;
   return max(0.0, dot(normalize(camera_direction), normalize(normal)));
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main() {
   vec3 fragPos = texture2D(MANTA_GBUFFER_POS, uv).rgb;
   vec3 fragNorm = texture2D(MANTA_GBUFFER_NORMAL, uv).rgb;

   vec3 fragColor = texture2D(MANTA_GBUFFER_ALBEDO, uv).rgb;
   vec3 fragEmiss = texture2D(MANTA_GBUFFER_EMISSION, uv).rgb;

   if (fragPos == vec3(0) && fragNorm == vec3(0)) { // There must be nothing here that requires shading, just draw the emissive (skyboxes and other effects)
      color = fragEmiss;
      return;
   }

   vec3 normal = normalize(fragNorm);
   vec3 view = normalize(MANTA_CAMERA_POSITION - fragPos);
   vec3 Lo = vec3(0.0);

   vec3 mrs = texture2D(MANTA_GBUFFER_MRS, uv).rgb;
   float metallic = mrs.r;
   float roughness = mrs.g;

   vec3 albedo = fragColor;

   vec3 F0 = vec3(0.04);
   F0 = mix(F0, albedo.xyz, metallic);

   for (int l = 0; l < MANTA_LIGHT_COUNT; l++) {
      float atten = 1;
      if (MANTA_LIGHT_TYPES[l] == 1) // Point lights
          atten = sphereAtten(fragPos, MANTA_LIGHT_POSITIONS[l], MANTA_LIGHT_RANGES[l], MANTA_LIGHT_INTENSITIES[l]);

      if (MANTA_LIGHT_TYPES[l] == 2) // Spot lights
          atten = spotAtten(fragPos, MANTA_LIGHT_POSITIONS[l], MANTA_LIGHT_DIRECTIONS[l], MANTA_LIGHT_RANGES[l], MANTA_LIGHT_INTENSITIES[l], MANTA_LIGHT_PARAMS1[l], MANTA_LIGHT_PARAMS2[l]);

      vec3 light = normalize(MANTA_LIGHT_POSITIONS[l] - fragPos);

      if (MANTA_LIGHT_TYPES[l] == 0)
          light = normalize(MANTA_LIGHT_DIRECTIONS[l]);

      vec3 halfway = normalize(view + light);

      vec3 F = FresnelSchlick(max(dot(halfway, view), 0.0), F0);

      float NDF = DistributionGGX(normal, halfway, roughness);
      float G = GeometrySmith(normal, view, light, roughness);

      vec3 numerator = NDF * G * F;
      float denominator = 4.0 * max(dot(normal, view), 0.0) * max(dot(normal, light), 0.0);
      vec3 specular = numerator / max(denominator, 0.001);

      vec3 radiance = MANTA_LIGHT_COLORS[l] * atten;

      vec3 kS = F;
      vec3 kD = vec3(1.0) - kS;

      kD *= 1.0 - metallic;

      float NdotL = max(dot(normal, light), 0.0);
      Lo += ((kD * albedo.xyz / PI + specular) * radiance * NdotL);
   }

   vec3 F = FresnelSchlickRoughness(max(dot(normal, view), 0.0), F0, roughness);
    
   vec3 kS = F;
   vec3 kD = 1.0 - kS;
   kD *= 1.0 - metallic;	

   vec3 diffuse = MANTA_AMBIENT_COLOR * albedo;

   vec3 R = reflect(-view, normal);
   const float MAX_ROUGH = 4;

   vec3 prefilter = textureLod(MANTA_CUBEMAP_ENVIRONMENT, R, roughness * MAX_ROUGH).xyz;
   vec2 brdf = texture(MANTA_TEX_BRDF_LUT, vec2(max(dot(normal, view), 0.0), roughness)).rg;
   vec3 specular = prefilter * (F * brdf.x + brdf.y);

   vec3 ambient = (kD * diffuse + specular);
   color = (ambient + Lo) + fragEmiss;

   //color = texture(MANTA_CUBEMAP_ENVIRONMENT, fragNorm).xyz;
}

#endif
