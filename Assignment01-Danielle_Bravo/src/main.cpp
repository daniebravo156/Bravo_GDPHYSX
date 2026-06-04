#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/Particle.h"
#include "headers/ModelLoader.h"
#include "headers/PhysicsWorld.h"
#include "headers/ForceGenerator.h"

#include <iostream>
#include <vector>
#include <chrono> 
#include <cmath>
#include <iomanip>
#include <cstdlib> // system cls clear console

int main() {

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to load GLFW" << std::endl;
        return -1;
    }

    // Window 
    GLFWwindow* window = glfwCreateWindow(800, 800, "Danielle R. Bravo", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load GLAD" << std::endl;
        return -1;
    }

    // Load the 3D model 
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    if (!LoadMy3DModel("src/assets/sphere.obj", attrib, shapes)) {
        if (!LoadMy3DModel("assets/sphere.obj", attrib, shapes)) {
            return -1;
        }
    }

    // Orthographic camera setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Scale window so we can see the full arc from left to right
    glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0); 
    glMatrixMode(GL_MODELVIEW);

    // PHYSICS WORLD SETUP
    PhysicsWorld world;
    
    // Create Force Generators
    // Gravity pulls straight down
    GravityForceGenerator downwardGravity(0.0f, -9.8f, 0.0f);   //magnitude of gravity is 9.8 m/s^2 downwards
    DragForceGenerator drag(0.5f, 0.0f); 

    // Red Particle (Gravity Only) 
    Particle redParticle(-15.0f, 0.0f, -1.0f, "Red", 0.8f, 0.0f, 0.0f);
    redParticle.mass = 2.0f;
   
    redParticle.vx = 15.0f; //throw right
    redParticle.vy = 5.0f;  //throw up

    // Blue Particle (Gravity + Drag) 
    Particle blueParticle(-15.0f, 0.0f, 1.0f, "Blue", 0.0f, 0.0f, 0.8f);
    blueParticle.mass = 2.0f;
    // Given the exact same initial throw as the red particle
    blueParticle.vx = 15.0f; 
    blueParticle.vy = 5.0f; 

    std::vector<Particle*> activeParticles = {&blueParticle, &redParticle};

    // Register Red Particle (Only Gravity)
    world.AddParticle(&redParticle);
    world.forceRegistry.add(&redParticle, &downwardGravity);

    // Register Blue Particle (Gravity & Drag)
    world.AddParticle(&blueParticle);
    world.forceRegistry.add(&blueParticle, &downwardGravity);
    world.forceRegistry.add(&blueParticle, &drag);

    // Initialize the clock variables
    // for delta time calculation 
    using namespace std::chrono;
    auto prev_time = high_resolution_clock::now();

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        
        auto curr_time = high_resolution_clock::now();
        duration<float> time_span = duration_cast<duration<float>>(curr_time - prev_time);
        float deltaTime = time_span.count();
        prev_time = curr_time;
        
        // Limit delta time to prevent bugging
        if (deltaTime > 0.05f) deltaTime = 0.05f; 
        
        // CENTRALIZED PHYSICS UPDATE
        world.Update(deltaTime);

        // Looper: If they hit bottom edge of screen, reset to the start of the throw
        for (Particle* p : activeParticles) {
            if (p->y <= -15.0f || p->x >= 15.0f) {
                p->x = -15.0f;          // Reset to left side
                p->y = 0.0f;            // Reset to middle ish height
                p->vx = 15.0f;          // Reset initial horizontal throw
                p->vy = 5.0f;           // Reset initial vertical toss
                p->vz = 0.0f;
            }
        }

        // CONSOLE FORCE TRACKER MONITOR
        // Clear the console and print the current state of each particle
        system("CLS");
        std::cout << "--- FORCE TRACKER MONITOR ---\n\n";

        for (Particle* p : activeParticles) {
            float forceMag = std::sqrt(p->fx * p->fx + p->fy * p->fy + p->fz * p->fz);
            
            std::cout << p->name << " Particle | "
                      << "Pos: (" << std::fixed << std::setprecision(2) << p->x << ", " << p->y << ", " << p->z << ") | "
                      << "Vel: (" << std::fixed << std::setprecision(2) << p->vx << ", " << p->vy << ", " << p->vz << ") | "
                      << "Force: (" << std::fixed << std::setprecision(2) << p->fx << ", " << p->fy << ", " << p->fz << ") | "
                      << "Magnitude of Net Force: " << std::fixed << std::setprecision(2) << forceMag << "\n\n";
        }

        // RENDER PASS
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST); 

        for (Particle* p : activeParticles) {
            glLoadIdentity();

            // translate model to particle physics coords
            glTranslatef(p->x, p->y, p->z);
            glScalef(1.5f, 1.5f, 1.5f); 
            glColor3f(p->r, p->g, p->b);
            
            glBegin(GL_TRIANGLES);
            for (size_t s = 0; s < shapes.size(); s++) {
                for (size_t f = 0; f < shapes[s].mesh.indices.size(); f++) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[f];
                    glVertex3f(
                        attrib.vertices[3 * idx.vertex_index + 0],
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2]
                    );
                }
            }
            glEnd();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}