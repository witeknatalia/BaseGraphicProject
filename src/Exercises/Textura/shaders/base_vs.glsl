#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec3 a_vertex_texture;

out vec2 vertex_texture;

void main() {
    vertex_color=a_vertex_texture;
    gl_Position = a_vertex_position;

}
