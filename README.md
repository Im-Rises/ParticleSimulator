# ParticleEmission

Techniques for particle emission in OpenGL.
[//]: # (- ~~Use a single VAO and VBO for all particles~~ &#40;no need to do this because there is no need to change the vertex data&#41;)
- ~~Use Instanced Arrays to render multiple particles at once~~ (no need to do this because there is no need to change the vertex data)
- Use GL_POINTS to render particles
- Use a single shader program for all particles in which we pass the position and color of each particle
- Use a SSBO to store particle data and have access to it in the shader and in the CPU

Physics:  
- Attract particles to the mouse using the laws of physics (Newton's second law, F = ma) where m = 1 and F is the force of attraction
- Change particle color based on velocity or age or the distance to the mouse
- Calculate speed, acceleration, and velocity of particles on the GPU using a Buffer Object
- No lifetime management of particles
- Make all particles the same size and begin at a random position

Features:
- Modify size of particles with glPointSize or gl_PointSize in the vertex shader (it nees glEnable(GL_PROGRAM_POINT_SIZE) to work)
