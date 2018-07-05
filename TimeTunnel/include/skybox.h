
#ifndef SKY_BOX
#define SKY_BOX
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

class SkyBox {
    unsigned int VAO, VBO;
public:
    SkyBox();
    void Draw(Shader);
};

#endif
