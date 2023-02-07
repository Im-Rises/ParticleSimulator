# ParticleSimulator

Techniques for particle emission in OpenGL.
- Use GL_POINTS to render particles
- Use a single shader program for all particles in which we pass the position and velocity of each particle
- Use a SSBO to store particle data and have access to it in the shader
- Calculate speed, acceleration, and velocity of particles on the GPU using a Buffer Object
- No lifetime management of particles
- Use Newton's gravitational law to calculate the force of attraction to the mouse
- Use Newton's second law to calculate the acceleration of each particle $$ \sum F = ma $$ where m is the mass and F is the force of attraction
- Use Euler's method to calculate the velocity of each particle $$ v = v_0 + at $$ where v is the velocity, v_0 is the initial velocity, a is the acceleration, and t is the time
- Change particle color based on velocity or age or the distance to the mouse

Features:
- Modify size of particles with glPointSize or gl_PointSize in the vertex shader (it needs glEnable(GL_PROGRAM_POINT_SIZE) to work)

Deleted:  
[//]: # (- ~~Use a single VAO and VBO for all particles~~ &#40;no need to do this because there is no need to change the vertex data&#41;)
- ~~Use Instanced Arrays to render multiple particles at once~~ (no need to do this because there is no need to change the vertex data)
- ~~Attract particles to the mouse using the laws of physics (Newton's second law, F = ma) where m = 1 and F is the force of attraction~~
