#version 310 es

/*
* This is the Vertex Shader of the Particle Simulator. It is an implementation of the one from the ParticleSimulator project.
* Check the original Shader in the following link: https://github.com/Im-Rises/ParticleSimulator/shaders/ParticleSimulator.vert
* to have a better view of the code and the comments.
*/

precision highp float;

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

uniform float u_isTargeting;
uniform float u_isRunning;

out vec3 v_velocity;

const float G = 1000.0;
const float m1 = 1000.0;
const float m2 = 1.0;
const float distanceOffset = 1000.0;

void main()
{
    Particle particle = particlesSsboData.particles[gl_VertexID];

    vec3 r = u_pointOfGravity - particle.position;
    float rSquared = dot(r, r) + distanceOffset;
    vec3 force = (G * m1 * m2 * normalize(r) / rSquared) * u_isTargeting * u_isRunning;

    vec3 acceleration = force / m1;

    particle.position += (particle.velocity * u_deltaTime + 0.5 * acceleration * u_deltaTime * u_deltaTime) * u_isRunning;

    particle.velocity += acceleration * u_deltaTime;

    particlesSsboData.particles[gl_VertexID] = particle;

    gl_Position = u_mvp * vec4(particle.position, 1.0);

    v_velocity = particle.velocity;
}
