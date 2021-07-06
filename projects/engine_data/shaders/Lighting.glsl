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

uniform sampler2D MANTA_tex0; //GBuffer Pos
uniform sampler2D MANTA_tex1; //GBuffer Norm
uniform sampler2D MANTA_tex2; //GBuffer Albedo
uniform sampler2D MANTA_tex3; //GBuffer Emission

uniform int MANTA_lightCount;
uniform vec3 MANTA_lightPositions[32];
uniform vec3 MANTA_lightDirections[32];
uniform vec3 MANTA_lightColors[32];
uniform float MANTA_lightRanges[32];
uniform float MANTA_lightIntensities[32];
uniform float MANTA_lightParams1[32];
uniform float MANTA_lightParams2[32];
uniform int MANTA_lightTypes[32];

#define DEG_TO_RAD 0.01745329251

in vec2 uv;

uniform vec3 MANTA_pCamera;
uniform vec3 MANTA_ambientColor;

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
   vec3 halfway = normalize(normalize(direction) + normalize(MANTA_pCamera - vert_pos));
   return clamp(pow(max(dot(normalize(normal), halfway), 0.0), power), 0.0, 1.0);
}

float fresnel(vec3 vert_pos, vec3 normal) {
   vec3 camera_direction = vert_pos - MANTA_pCamera;
   return max(0.0, dot(normalize(camera_direction), normalize(normal)));
}

void main() {
   vec3 fragPos = texture2D(MANTA_tex0, uv).xyz;
   vec3 fragNorm = texture2D(MANTA_tex1, uv).xyz;

   if (fragPos == vec3(0) && fragNorm == vec3(0)) { // There must be nothing here
      color = vec3(0);
      return;
   }

   vec3 fragColor = texture2D(MANTA_tex2, uv).xyz;  
   vec3 fragEmiss = texture2D(MANTA_tex3, uv).xyz;

   vec3 lighting = MANTA_ambientColor;

   for (int l = 0; l < MANTA_lightCount; l++) {
      float lDot = 0.0;

      if (MANTA_lightTypes[l] != 0)
	 lDot = dot(normalize(fragNorm), normalize(MANTA_lightPositions[l] - fragPos));
      else
	 lDot = dot(normalize(fragNorm), normalize(MANTA_lightDirections[l]));

      if (lDot <= 0.0)
	 continue;

      float atten = 1;
      if (MANTA_lightTypes[l] == 1) // Point lights
	 atten = sphereAtten(fragPos, MANTA_lightPositions[l], MANTA_lightRanges[l], MANTA_lightIntensities[l]);

      if (MANTA_lightTypes[l] == 2) // Spot lights
	 atten = spotAtten(fragPos, MANTA_lightPositions[l], MANTA_lightDirections[l], MANTA_lightRanges[l], MANTA_lightIntensities[l], MANTA_lightParams1[l], MANTA_lightParams2[l]);

      vec3 specDir = MANTA_lightPositions[l] - fragPos;

      if (MANTA_lightTypes[l] == 0)
	 specDir = MANTA_lightDirections[l] + fragPos;

      lighting += MANTA_lightColors[l] * lDot * atten;
      lighting += MANTA_lightColors[l] * blinnPhongSpecular(fragPos, fragNorm, specDir, 256) * atten;
   }

   float gamma = 1.0;
   color = pow((fragColor * lighting) + fragEmiss, vec3(1.0 / gamma));
}

#endif
