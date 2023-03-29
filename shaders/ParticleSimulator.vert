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

uniform mat4 u_mvp;
uniform float u_deltaTime;
uniform vec3 u_pointOfGravity;
// Use float instead of bool as boolean condition in shader can produce unexpected results, the float is 0.0f or 1.0f
// and used to multiply the force to apply or not
uniform float u_isTargeting;
uniform float u_isRunning;

out vec3 v_velocity;

const float G = 1000.0f;// Gravitational constant
const float m1 = 1000.0f;// Mass of the particle
const float m2 = 1.0f;// Mass of the point of gravity
const float distanceOffset = 100.0f;
//const float friction = 0.99f;

void main()
{
    Particle particle = particlesSsboData.particles[gl_VertexID];// Get the particle data

    // Newton's law of gravity F = G * m1 * m2 / r^2 (F = force, G = gravitational constant,
    // m1 = mass of the particle, m2 = mass of the point of gravity, r = distance between the
    // particle and the point of gravity)
    vec3 r = u_pointOfGravity - particle.position;
    float rSquared = dot(r, r) + distanceOffset;// (dot(toMass, toMass)) gives the square of the magnitude (length) of the vector
    vec3 force = (G * m1 * m2 * normalize(r) / rSquared) * u_isTargeting * u_isRunning;// normalize(r) gives the direction of the vector

    // F = ma
    vec3 acceleration = force / m1;// a = F / m

    // p = p0 + v * t + 1/2 * a * t^2
    particle.position += (particle.velocity * u_deltaTime + 0.5f * acceleration * u_deltaTime * u_deltaTime) * u_isRunning;

    // v = v0 + at
    particle.velocity += acceleration * u_deltaTime;

    // Friction
    //        particle.velocity *= friction;

    // Set the new particle data
    particlesSsboData.particles[gl_VertexID] = particle;

    // Set the output
    gl_Position = u_mvp * vec4(particle.position, 1.0);

    v_velocity = particle.velocity;
}
