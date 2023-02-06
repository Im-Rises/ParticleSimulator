#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;

layout (std140, binding = 0) buffer Particles {
    vec4 particles[];
} particles_data;

uniform mat4 view;
uniform mat4 projection;
uniform float deltaTime;

out vec4 particleColor;

void main()
{
    vec4 particle = particles_data.particles[gl_InstanceID];
    vec3 pos = particle.xyz + velocity * deltaTime;
    gl_Position = projection * view * vec4(pos, 1.0);

    //    particleColor = particle.w;
}
