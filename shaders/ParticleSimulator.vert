#version 430 core

struct Particle {
    vec3 position;
    vec3 velocity;
};

layout (std430, binding = 0) buffer Particles {
    Particle particles[];
} particlesSsboData;

layout(location = 0) in float a_deltaTime;
layout(location = 1) in vec3 a_pointOfGravity;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_color;

void main()
{
    Particle particle = particlesSsboData.particles[gl_VertexID];// Get the particle data

    // Newton's law of gravity F = G * m1 * m2 / r^2 (F = force, G = gravitational constant, m1 = mass of the particle, m2 = mass of the point of gravity, r = distance between the particle and the point of gravity)
    const float G = 1.0f;// Gravitational constant
    const float m1 = 1.0f;// Mass of the particle
    const float m2 = 1.0f;// Mass of the point of gravity
    vec3 r = a_pointOfGravity - particle.position;
    float rSquared = dot(r, r);// (dot(toMass, toMass)) gives the square of the magnitude (length) of the vector

    // F = G * m1 * m2 / r^2 or F = G * m1 * m2 * r / r^2 (to get the direction of the force)
    vec3 force = G * m1 * m2 * r / rSquared;

    // F = ma
    vec3 acceleration = force / m1;// a = F / m

    // v = v0 + at
    particle.velocity += acceleration * a_deltaTime;

    // p = p0 + v * t
    particle.position += particle.velocity * a_deltaTime;

    // Set the new particle data
    particlesSsboData.particles[gl_VertexID] = particle;

    // Set the output
    gl_Position = u_projection * u_view * vec4(particle.position, 1.0);
    v_color = vec3(1.0, 0.0, 0.0);
}
