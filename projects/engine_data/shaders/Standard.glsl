#ifdef VERTEX
layout(location = 0) in vec3 _vertex;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _uv;

out vec3 vert_pos;

out vec3 normal;
out vec3 normal_raw;
out vec3 normal_camera;

out vec2 uv;

uniform mat4 MANTA_mM;
uniform mat4 MANTA_mV;
uniform mat4 MANTA_mP;

void main() {
  mat4 mMVP = MANTA_mP * MANTA_mV * MANTA_mM;

  gl_Position = mMVP * vec4(_vertex, 1.0);
  vert_pos = vec3(MANTA_mM * vec4(_vertex, 1.0)).xyz;

  mat3 mx3norm = mat3(MANTA_mM);
  mx3norm = inverse(mx3norm);
  mx3norm = transpose(mx3norm);

  normal = (mx3norm * _normal);
  normal_raw = _normal;
  normal_camera = (MANTA_mV * MANTA_mM * vec4(_normal, 0)).xyz;

  uv = _uv;
}

#endif

#ifdef FRAGMENT

in vec3 vert_pos;

in vec3 normal;
in vec3 normal_raw;
in vec3 normal_camera;

in vec2 uv;

out vec3 color;

uniform float MANTA_fTime;

uniform vec3 MANTA_pCamera;

uniform vec3 MANTA_ambientColor;

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

float sphereAtten(vec3 position, float range, float intensity) {
   vec3 direction = position - vert_pos;
   float dist = length(direction);

   float falloff = 1 - dist / range;
   return max(dot(normalize(normal), normalize(direction)) * falloff, 0.0) * intensity;
}

float spotAtten(vec3 position, vec3 direction, float range, float intensity, float innerAngle, float outerAngle) {
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

float blinnPhongSpecular(vec3 position, float power) {
   vec3 halfway = normalize(normalize(position - vert_pos) + normalize(MANTA_pCamera - vert_pos));
   return clamp(pow(max(dot(normalize(normal), halfway), 0.0), power), 0.0, 1.0);
}

float fresnel() {
   vec3 camera_direction = vert_pos - MANTA_pCamera;
   return max(0.0, dot(normalize(camera_direction), normalize(normal)));
}

void main() {
   vec3 lighting = MANTA_ambientColor;

   for (int l = 0; l < MANTA_lightCount; l++) {
      float lDot = 0.0;

      if (MANTA_lightTypes[l] != 0)
	 lDot = dot(normalize(normal), normalize(MANTA_lightPositions[l] - vert_pos));
      else
	 lDot = dot(normalize(normal), normalize(MANTA_lightDirections[l]));

      if (lDot <= 0.0)
	 continue;

      float atten = 1;
      if (MANTA_lightTypes[l] == 1) // Point lights
	 atten = sphereAtten(MANTA_lightPositions[l], MANTA_lightRanges[l], MANTA_lightIntensities[l]);

      if (MANTA_lightTypes[l] == 2) // Spot lights
	 atten = spotAtten(MANTA_lightPositions[l], MANTA_lightDirections[l], MANTA_lightRanges[l], MANTA_lightIntensities[l], MANTA_lightParams1[l], MANTA_lightParams2[l]);
      
      lighting += MANTA_lightColors[l] * lDot * atten;
      lighting += MANTA_lightColors[l] * blinnPhongSpecular(MANTA_lightPositions[l], 256) * atten;
   }

   //lighting = clamp(lighting, 0, 1);

   color = vec3(1, 1, 1) * lighting;
   //color = normal;
}

#endif
