#ifdef VERTEX
layout(location = 0) in vec3 _vertex;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _uv;

out vec3 normal;
out vec2 uv;

uniform mat4 MANTA_mV;
uniform mat4 MANTA_mP;

void main() {
  mat4 MANTA_mM = mat4(1.0);
  mat4 mMVP = MANTA_mP * MANTA_mV * MANTA_mM;

  gl_Position = mMVP * vec4(_vertex, 1.0);

  mat3 mx3norm = mat3(MANTA_mM);
  mx3norm = inverse(mx3norm);
  mx3norm = transpose(mx3norm);

  normal = (mx3norm * _normal);
  //normal_raw = _normal;
  //normal_camera = (MANTA_mV * MANTA_mM * vec4(_normal, 0)).xyz;

  uv = _uv;
}

#endif

#ifdef FRAGMENT

in vec3 normal;
in vec2 uv;

out vec3 color;

void main() {
  color = vec3(1.0, 1.0, 1.0) * dot(normal, vec3(1.0, 1.0, 0.0));
}

#endif
