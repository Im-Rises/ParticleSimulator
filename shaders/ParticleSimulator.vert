#version 430 core

// Can't use directly vec3 in SSBO (indexed by 4 bytes not 3): https://computergraphics.stackexchange.com/questions/5810/shader-storage-buffer-indexing-by-4-bytes-instead-of-3
struct Particle {
    vec3 position;
    float offset1;
    vec3 velocity;
    float offset2;
};

layout (std430, binding = 0) buffer ParticlesSsbo {
    Particle particles[];
} particlesSsboData;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_deltaTime;
uniform vec3 u_pointOfGravity;
uniform bool u_isPaused;

out vec3 v_color;

void main()
{
    Particle particle = particlesSsboData.particles[gl_VertexID-1];// Get the particle data

    if (u_isPaused) {
        // Set the output
        gl_Position = u_projection * u_view * vec4(particle.position, 1.0);
        v_color = vec3(0.0, 0.0, 0.0);
        return;
    }

    // Newton's law of gravity F = G * m1 * m2 / r^2 (F = force, G = gravitational constant, m1 = mass of the particle, m2 = mass of the point of gravity, r = distance between the particle and the point of gravity)
    const float G = 1000.0f;// Gravitational constant
    const float m1 = 1000.0f;// Mass of the particle
    const float m2 = 1.0f;// Mass of the point of gravity
    const float distanceOffset = 1000.0f;

    vec3 r = u_pointOfGravity - particle.position;
    float rSquared = dot(r, r) + distanceOffset;// (dot(toMass, toMass)) gives the square of the magnitude (length) of the vector
    vec3 force = G * m1 * m2 * normalize(r) / rSquared;// normalize(r) gives the direction of the vector

    // F = ma
    vec3 acceleration = force / m1;// a = F / m

    // v = v0 + at
    particle.velocity += acceleration * u_deltaTime;

    // p = p0 + v * t
    particle.position += particle.velocity * u_deltaTime;

    // Set the new particle data
    particlesSsboData.particles[gl_VertexID-1] = particle;

    // Set the output
    gl_Position = u_projection * u_view * vec4(particle.position, 1.0);

    // Set the color
    v_color = vec3(0.0, 1.0, 1.0);
}
