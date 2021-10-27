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
  normal = _normal;

  uv = _uv;
}

#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_color;
layout(location = 3) out vec4 out_mrs;
layout(location = 4) out vec4 out_emission;

in vec3 vert_position;
in vec3 normal;
in vec2 uv;

uniform vec4 MANTA_ALBEDO;

void main() {
   out_position = vec4(vert_position, 1);
   out_normal = vec4(normal, 1);
   out_color = vec4(1, 1, 1, 1);
   out_mrs = vec4(0, 0.05, 0, 1);
   //out_emission = out_color;
}

#endif
