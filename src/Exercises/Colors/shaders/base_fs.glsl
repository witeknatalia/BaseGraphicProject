#version 410

layout(location=0) out vec4 vFragColor;

in vec4 vertex_color;

void main() {
    vFragColor.a = 1.0;
    vFragColor.rgb = vertex_color;
}
