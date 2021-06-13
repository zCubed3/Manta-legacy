// libmain.cpp files aren't really main files, instead they're meant to declare commonly used exports and make it easier to debug exporting :)

#include <manta_macros.hpp>
#include <gl3_renderer.hpp>

MANTA_EXPORT Renderer* get_Renderer() { return new GL3Renderer(); }
