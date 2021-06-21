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
layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_color;

in vec3 vert_pos;

in vec3 normal;
in vec3 normal_raw;
in vec3 normal_camera;

in vec2 uv;

uniform vec4 MANTA_albedoColor;

void main() {
   out_position = vec4(vert_pos, 1);
   out_normal = vec4(normal, 1);
   out_color = vec4(1, 1, 1, 1);
   //out_color = MANTA_albedoColor;
}

#endif
