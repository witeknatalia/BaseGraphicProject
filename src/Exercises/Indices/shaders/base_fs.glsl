#version 410

layout(location=0) out vec3 vFragColor;

in vec3 vertex_color;

void main() {
    vFragColor = vertex_color;
}
