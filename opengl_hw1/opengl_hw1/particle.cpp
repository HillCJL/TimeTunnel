#include "particle.h"
#include <time.h>
#include <stdlib.h>
unsigned int round_particle_action::VAO = 0;
void round_particle_action::init() {

    unsigned int VBO, EBO;

    float little_cube[] = {
        -0.1f, -0.1f, -0.1f,
        -0.1f, -0.1f, 0.1f,
        0.1f, -0.1f, 0.1f,
        0.1f, -0.1f, -0.1f,
        -0.1f, 0.1f, -0.1f,
        -0.1f, 0.1f, 0.1f,
        0.1f, 0.1f, 0.1f,
        0.1f, 0.1f, -0.1f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        0, 1, 4,
        4, 5, 1,
        1, 2, 5,
        5, 6, 2,
        2, 3, 6,
        6, 7, 3,
        4, 5, 6,
        6, 7, 4
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(little_cube), little_cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(4 * 3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

round_particle_action::round_particle_action(int size, int per_circle, float total, float r, float f, float rot, float s,float z) {
    init_size = size;
    init_size_per_circle = per_circle;
    total_length = total;
    R = r;
    speed_forward = f;
    speed_rotate = rot;
    z_pos = z;

    //srand(time(NULL));
    rand();
    int circle = init_size / init_size_per_circle;
    for ( int i = 0; i < init_size; i++ ) {
        int radius = i % init_size_per_circle, h = i / init_size_per_circle;
        //float randfloat = (float(rand() % 1000) / 1000 - 0.5)/4;
        Particle particle;
        particle.positon = glm::vec3(float(cos(float(radius + float(h)/circle) / init_size_per_circle * 360)) * R, float(sin(float(radius + float(h) / circle) / init_size_per_circle * 360)) * R, z_pos -total_length * ((h + float(radius)/init_size_per_circle - 0.5)/circle - 0.5f));

        //particle.positon = glm::vec3(float(cos((radius + randfloat) / init_size_per_circle * 360)) * R, float(sin((radius + randfloat) / init_size_per_circle * 360)) * R, -total_length * ((h + randfloat)/circle - 0.5f));
        particle.life_time = int(total_length / speed_forward) * (circle - h + 1)/10;
        particle.size = rand() % 100 / 50 + 1;
        particle.color = glm::vec4(0.0f, 0.7f, 1.0f, 0.6f);
        partical_list.push_back(particle);
    }
}

void round_particle_action::Draw(Shader shader) {
    glBindVertexArray(VAO);
    shader.use();
    for ( int i = 0; i < partical_list.size(); i++ ) {
        shader.setMat4("model", glm::translate(glm::mat4(), partical_list[i].positon));
        shader.setVec4("Color", partical_list[i].color);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
    Update();
}

void round_particle_action::Update() {
    for ( int i = partical_list.size() - 1; i >=0; i-- ) {
        partical_list[i].life_time--;
        if ( partical_list[i].life_time < 0 || partical_list[i].positon.z < -total_length/2 + z_pos) {
            partical_list.erase(partical_list.begin() + i);
            continue;
        }
        glm::mat4 model;
        model = glm::rotate(model, speed_rotate, glm::vec3(0, 0, 1));
        model = glm::translate(model, glm::vec3(0, 0, -speed_forward));
        model = glm::scale(model, glm::vec3(1 - speed_scale, 1 - speed_scale, 1));
        partical_list[i].positon = glm::vec3(model * glm::vec4(partical_list[i].positon, 1.0));
    }
    if ( partical_list.size() < init_size - init_size_per_circle + 1 ) {
        int circle = init_size / init_size_per_circle;
        for ( int i = 0; i < init_size_per_circle; i++ ) {
            //float randfloat = (float(rand() % 1000) / 1000 - 0.5) / 4;
            Particle particle;
            //particle.positon = glm::vec3(float(cos((i + randfloat) / init_size_per_circle * 360)) * R, float(sin((i + randfloat) / init_size_per_circle * 360)) * R, 0.5f +  float(rand() % 20-10)/20 * 0.5f/circle * total_length);
            particle.positon = glm::vec3(float(cos(float(i) / init_size_per_circle * 360)) * R, float(sin(float(i) / init_size_per_circle * 360)) * R, 0.5f + (float(i)/init_size_per_circle - 0.5) * 0.5f / circle * total_length + z_pos);
            particle.life_time = int(total_length / speed_forward) * circle / 10;
            particle.size = rand() % 100 / 50 + 1;
            particle.color = glm::vec4(0.0f, 0.7f, 1.0f, 0.6f);
            partical_list.push_back(particle);
        }
    }
}