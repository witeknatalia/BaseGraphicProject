#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec2 a_texture_coords;
out vec2 texture_coords;

layout(std140) uniform Transformations {
    mat4 P;
    mat4 V;
};

void main() {
    gl_Position = P*V*a_vertex_position;
    texture_coords = a_texture_coords;
}
