#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "Model.h"
#include "particle.h"

float window_width = 800, window_height = 600;

My_Camera mc;
bool enable_camera = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
}

void processInput(GLFWwindow *window) {
    static bool first_press_enter = true;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && first_press_enter) {
        enable_camera = !enable_camera;
        first_press_enter = false;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
        first_press_enter = true;
    }
    if (enable_camera) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            mc.moveForward();
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            mc.moveBack();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            mc.moveLeft();
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            mc.moveRight();
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static double last_x = xpos, last_y = ypos;
    bool set = false;
    float delta_x = xpos - last_x, delta_y = ypos - last_y;
    if (delta_x != 0 || delta_y != 0) {

        mc.rotate(delta_x, delta_y);
        if (xpos < window_width * 0.1) {
            xpos = window_width * 0.8;
            set = true;
        }
        if (xpos > window_width * 0.9) {
            xpos = window_width * 0.2;
            set = true;
        }
        if (ypos < window_height * 0.1) {
            ypos = window_height * 0.8;
            set = true;
        }
        if (ypos > window_height * 0.9) {
            ypos = window_height * 0.2;
            set = true;
        }
        last_x = xpos;
        last_y = ypos;

        //if(xpos < 0) 
        if (set)glfwSetCursorPos(window, xpos, ypos);
    }
}

GLuint particleFBO, colorBuffers, pingpongFBO[2], pingpongBuffer[2];;

void init_particle_buffer() {
    glGenFramebuffers(1, &particleFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, particleFBO);
    glGenTextures(1, &colorBuffers);
    glBindTexture(GL_TEXTURE_2D, colorBuffers);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB16F, window_width, window_height, 0, GL_RGB, GL_FLOAT, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffers, 0
    );


    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (GLuint i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, window_width, window_height, 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
        );
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void use_shader(Shader shader, glm::mat4 view, glm::mat4 projection) {
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Homework", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, window_width, window_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);






    Shader PhongShader("assets/shader/final_phong.vs", "assets/shader/final_phong.fs");
    Shader ParticleShader("assets/shader/final_particle_gaussian.vs", "assets/shader/final_particle.fs");
    Shader ParticleHDRShader("assets/shader/final_particle.vs", "assets/shader/final_particle_hdr.fs");
    Shader shaderBlur("assets/shader/final_particle_gaussian.vs", "assets/shader/final_particle_gaussian.fs");

    //在这个地方改下路径就完事了
    string path("assets/model/nanosuit/nanosuit.obj");

    Model ourModel(path, false);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfwGL3_Init(window, true);




    int selection = 0;
    glm::vec3 camera_direction = glm::vec3(0.0f, 0.0f, 0.0f), camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float ambient_strengh = 0.1f, diffuse_strength = 1.0f, specular_strength = 0.5f, n_shiny = 32.0f;
    glm::vec3 view_pos = glm::vec3(2.0f, 2.0f, 2.0f), light_pos = glm::vec3(10.0f, 10.0f, 10.0f), light_color = glm::vec3(1.0f, 1.0f, 1.0f), object_color = glm::vec3(1.0f, 1.0f, 1.0f);
    double now_time = glfwGetTime();


    float z = 2.0f;

    /*
    round_particle_action::init();
    round_particle_action rpa_1(32, 8, 20.0f, 12.0f, 0.01f, 0.31f, 0.45, -50);
    round_particle_action rpa_2(32, 4, 20.0f, 12.0f, 0.08f, 0.31f, 0.45, -20);
    round_particle_action rpa_3(64, 4, 100.0f, 3.0f, 0.05f, 0.31f, 0.01, 0);
    */

    init_particle_buffer();


    while (!glfwWindowShouldClose(window)) {
        static bool draw = false;
        ImGui_ImplGlfwGL3_NewFrame();

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);


        glm::mat4 model, view, projection;

        PhongShader.use();

        if (enable_camera) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetCursorPosCallback(window, mouse_callback);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, 0);
        }
        ImGui::Checkbox("press enter to enable/disable control", &enable_camera);


        {
            double new_time = glfwGetTime();
            now_time = new_time;
            view = mc.getLookAt();
            projection = mc.getProjection();
        }



        PhongShader.setFloat("ambient_strength", ambient_strengh);
        PhongShader.setFloat("diffuse_strength", diffuse_strength);
        PhongShader.setFloat("specular_strength", specular_strength);
        PhongShader.setFloat("n_shiny", n_shiny);


        PhongShader.setVec3("light_pos", light_pos);
        PhongShader.setVec3("light_color", light_color);
        PhongShader.setVec3("object_color", object_color);


        PhongShader.setMat4("view", view);
        PhongShader.setMat4("projection", projection);

        PhongShader.setVec3("view_pos", mc.getPosition());
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
        PhongShader.setMat4("model", model);


        ourModel.Draw(PhongShader);

        /*
        //取消这块注释就是画粒子隧道
            use_shader(ParticleShader, view, projection);
            rpa_1.Draw(ParticleShader);
            rpa_2.Draw(ParticleShader);
            rpa_3.Draw(ParticleShader);

        */







        /*粒子光效
        {
            glBindFramebuffer(GL_FRAMEBUFFER, particleFBO);
            use_shader(ParticleHDRShader, view, projection);
            rpa_1.Draw(ParticleHDRShader);
            rpa_2.Draw(ParticleHDRShader);
            rpa_3.Draw(ParticleHDRShader);

            GLboolean horizontal = true, first_iteration = true;
            GLuint amount = 10;
            use_shader(shaderBlur, view, projection);
            for ( GLuint i = 0; i < amount; i++ ) {
                glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                shaderBlur.setBool("horizontal", horizontal);
                glBindTexture(
                    GL_TEXTURE_2D, first_iteration ? colorBuffers : pingpongBuffer[!horizontal]
                );
                rpa_1.Draw(shaderBlur);
                rpa_2.Draw(shaderBlur);
                rpa_3.Draw(shaderBlur);
                horizontal = !horizontal;
                if ( first_iteration )
                    first_iteration = false;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);

            use_shader(ParticleShader, view, projection);
            rpa_1.Draw(ParticleShader);
            rpa_2.Draw(ParticleShader);
            rpa_3.Draw(ParticleShader);
        }
        */
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();

    }




    glfwTerminate();
    return 0;
}

