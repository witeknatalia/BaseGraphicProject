#version 410
uniform sampler2D diffuse_map;

layout(location=0) out vec3 Texture;

in vec2 vertex_texture;


void main() {
    Texture = vertex_texture;
    vFragColor = texture(color, texture_coords);
}
