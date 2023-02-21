#version 430 core

in vec3 v_color;

out vec4 o_fragColor;

void main() {
    o_fragColor = vec4(v_color, 1.0f);
}
