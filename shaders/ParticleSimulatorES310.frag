#version 310 es

precision highp float;

in vec3 v_velocity;

out vec4 o_fragColor;

void main() {
    vec3 v_color = vec3(min(v_velocity.y, 0.8f), max(v_velocity.x, 0.5f), min(v_velocity.z, 0.5f));
    o_fragColor = vec4(v_color, 1.0f);
}
