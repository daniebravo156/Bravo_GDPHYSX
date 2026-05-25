#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <vector>
#include <chrono> //Time library

//Physics Particle Class 
class Particle {
public:
    float x, y, z;       // Position
    float vx, vy, vz;    // Velocity
    float ax, ay, az;    // Acceleration

    // constructor
    Particle(float startX, float startY, float startZ) 
        : x(startX), y(startY), z(startZ), vx(0), vy(0), vz(0), ax(0), ay(0), az(0) {}

    // update velocity
    void UpdateVelocity(float time) {
        vx += ax * time;
        vy += ay * time;
        vz += az * time;
    }

    // update position
    void UpdatePosition(float time) {
        x += vx * time;
        y += vy * time;
        z += vz * time;
    }

    // update both
    void Update(float time) {
        UpdateVelocity(time);
        UpdatePosition(time);
    }
};

// 3d model loading helper function
bool LoadMy3DModel(std::string filePath, tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes) {
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    // make sure the path is correct and the file exists
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str());
    if (!ret) {
        std::cerr << "Failed to load asset " << filePath << ": " << err << std::endl;
    }
    return ret;
}

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

    // assets file 
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    
    // Load the 3D model using our simplified helper function
    if (!LoadMy3DModel("assets/sphere.obj", attrib, shapes)) {
        return -1;
    }

    // Orthographic camera setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //(left, right, bottom, top, near, far)
    glOrtho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0); 
    glMatrixMode(GL_MODELVIEW);

    // create a new particle in main
    Particle myParticle(0.0f, 0.0f, 0.0f); 
    myParticle.vx = 2.0f; // Give it initial horizontal velocity

    // Initialize the clock variables
    using namespace std::chrono;
    auto prev_time = high_resolution_clock::now();

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        
        // Get the time in between frames inside the game loop
        auto curr_time = high_resolution_clock::now();
        duration<float> time_span = duration_cast<duration<float>>(curr_time - prev_time);
        float deltaTime = time_span.count();
        prev_time = curr_time;
        
        // Call particles update
        myParticle.Update(deltaTime);

        // Bouncing within the window bounds horizontally
        if (myParticle.x >= 1.5f) 
        {
            myParticle.x = 1.5f;       
            myParticle.vx *= -1.0f;    
        } else if (myParticle.x <= -1.5f) 
        {
            myParticle.x = -1.5f;      
            myParticle.vx *= -1.0f;    
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Update the position of 3D model aka the particle
        glTranslatef(myParticle.x, myParticle.y, myParticle.z);

        // Scale the sphere 
        glScalef(0.6f, 0.6f, 0.6f);

        // Render sphere (coloring it red) 
        glColor3f(0.4f, 0.0f, 0.0f); // coloring it red 
        
        glBegin(GL_TRIANGLES);
        for (size_t s = 0; s < shapes.size(); s++) {
            for (size_t f = 0; f < shapes[s].mesh.indices.size(); f++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[f];
                // Grab the vertex coordinates from the loaded OBJ
                glVertex3f(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                );
            }
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}