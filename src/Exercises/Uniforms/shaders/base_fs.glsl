#version 410

layout(location=0) out vec4 vFragColor;

in vec3 vertex_color;

void main() {
    vFragColor.a = vertex_color.a;
    vFragColor.rgb = strength*light*vertex_color.rgb;
}

layout(std140) uniform Modifiers {
   float light_intensity;
  vec3 light_color;
};
