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
uniform vec3 u_attractorPosition;
uniform float u_damping;
uniform float u_attractorMass;
uniform float u_particleMass;
uniform float u_gravity;
uniform float u_distanceOffset;
uniform float u_isAttracting;
uniform float u_isRunning;

out vec3 v_vel;

void main()
{
    Particle particle = particlesSsboData.particles[gl_VertexID];

    vec3 r = u_attractorPosition - particle.position;
    float rSquared = dot(r, r) + u_distanceOffset;
    vec3 force = (u_gravity * u_attractorMass * u_particleMass * normalize(r) / rSquared) * u_isAttracting * u_isRunning;

    vec3 acceleration = force / u_particleMass;
    vec3 position = a_pos + (a_vel * u_deltaTime + 0.5f * acceleration * u_deltaTime * u_deltaTime) * u_isRunning;
    vec3 velocity = a_vel + acceleration * u_deltaTime;

    particle.position = position;

//    particle.velocity = velocity * u_damping;
//    particle.velocity = velocity * (u_isRunning == 1.0 ? 1.0 : u_damping);
    particle.velocity = mix(velocity, velocity * u_damping, u_isRunning);

    particlesSsboData.particles[gl_VertexID] = particle;

    gl_Position = u_mvp * vec4(particle.position, 1.0);

    v_vel = particle.velocity;
}
