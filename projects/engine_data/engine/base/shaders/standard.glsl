#version 330 core

#ifdef VERTEX
layout(location = 0) in vec3 _vertex;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _uv;

out vec3 vert_position;
out vec3 normal;
out vec2 uv;

uniform mat4 MANTA_MATRIX_M;
uniform mat4 MANTA_MATRIX_V;
uniform mat4 MANTA_MATRIX_P;
uniform mat4 MANTA_MATRIX_M_IT;

void main() {
  mat4 mMVP = MANTA_MATRIX_P * MANTA_MATRIX_V * MANTA_MATRIX_M;
  gl_Position = mMVP * vec4(_vertex, 1.0);  

  vert_position = (MANTA_MATRIX_M * vec4(_vertex, 1.0)).xyz;  
  normal = (MANTA_MATRIX_M_IT * vec4(_normal, 1.0)).xyz;

  uv = _uv;
}

#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_color;
layout(location = 3) out vec4 out_mra;
layout(location = 4) out vec4 out_emission;

in vec3 vert_position;
in vec3 normal;
in vec2 uv;

uniform vec4 MANTA_COLOR_ALBEDO;

uniform float MANTA_SCALAR_METALLIC;
uniform float MANTA_SCALAR_ROUGHNESS;

uniform sampler2D MANTA_MAP_ALBEDO;

void main() {
   out_position = vec4(vert_position, 1);
   out_normal = vec4(normalize(normal), 1);
   out_color = texture(MANTA_MAP_ALBEDO, uv) * MANTA_COLOR_ALBEDO;
   out_mra = vec4(MANTA_SCALAR_METALLIC, MANTA_SCALAR_ROUGHNESS, 0, 1);
   out_emission = vec4(0, 0, 0, 1);
}

#endif
