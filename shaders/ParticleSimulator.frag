#version 430 core

in vec3 v_vel;

out vec4 o_fragColor;

void main()
{
    vec3 v_color = vec3(min(v_vel.y, 0.8f), max(v_vel.x, 0.5f), min(v_vel.z, 0.5f));
    o_fragColor = vec4(v_color, 1.0f);
}
