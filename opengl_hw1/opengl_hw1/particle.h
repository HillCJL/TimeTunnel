#ifndef PARTICLE
#define PARTICLE

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
using namespace std;



struct Particle{

    int life_time;
    glm::vec3 positon;
    float size;
    glm::vec4 color;
};

class round_particle_action {
    
    vector<Particle> partical_list;
    int init_size, init_size_per_circle;
    float total_length, R, speed_forward, speed_rotate, speed_scale;
    float z_pos;
    void Update();
public:
    static unsigned int VAO;
    static void init();
    round_particle_action(int size, int per_circle, float total, float r, float f, float rot, float s, float z);
    void Draw(Shader);
};


#endif

