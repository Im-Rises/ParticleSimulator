#version 430 core

in vec3 v_velocity;

out vec4 o_fragColor;

void main() {
    // Set the color based on the velocity
    vec3 v_color = vec3(min(v_velocity.y, 0.8f), max(v_velocity.x, 0.5f), min(v_velocity.z, 0.5f));
    o_fragColor = vec4(v_color, 1.0f);

    // Old code
    //    vec3 v_color = vec3(max(v_velocity.x, 0.3f), min(v_velocity.y, 0.8f), min(v_velocity.z, 0.2f));
    //    o_fragColor = vec4(v_color, 1.0f);

    //    //    // Old code
    //    //    v_color = particle.velocity * 0.1f;

    //    // Set fixed color
    //    o_fragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
}
