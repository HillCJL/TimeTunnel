#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_move_callback(GLFWwindow* window, double x, double y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void Bezier(int vao, int vbo);
 //������ɫ��
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Ƭ����ɫ��
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.7f, 0.5f, 1.0f);\n"
"}\n\0";

float xPos; //��ǰ����x����
float yPos;//��ǰ����y����
int count = 0;
static float vertices[4][3] = {
	0.0,  0.0,  0.0,
	0.0,  0.0,  0.0,
	0.0,  0.0,  0.0,
	0.0,  0.0,  0.0
};


int main()
{
	glfwInit();//��ʼ��GLFW��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//GFLW������汾Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//GFLW��Ĵΰ汾Ϊ3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ָ�����õ�OPENGLΪ����ģʽ

	GLFWwindow* window = glfwCreateWindow(800, 600, "PlotGraph", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "��������ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "��ʼ��GLADʧ��" << std::endl;
		return -1;
	}

	//����imGUI��
	ImGui_ImplGlfwGL3_Init(window, true);

	//��װstyle
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//��ʼ��VAO��VBO

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		ImGui_ImplGlfwGL3_NewFrame();

		{
			if (ImGui::Button("Write Points Position"))
			{
				Bezier(VAO, VBO);
				count = 4;
			}
			if (ImGui::Button("Delete All Position"))
			{
				count = 0;
			}

			ImGui::InputFloat3("First Point", vertices[0]);
			ImGui::InputFloat3("Second Point", vertices[1]);
			ImGui::InputFloat3("Third Point", vertices[2]);
			ImGui::InputFloat3("Fourth Point", vertices[3]);

		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		//��Ⱦ

		if (count == 4)
		{
			Bezier(VAO, VBO);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawArrays(GL_POINTS, 4, 404);
			glDrawArrays(GL_LINES, 0, 2);
			glDrawArrays(GL_LINES, 1, 2);
			glDrawArrays(GL_LINES, 2, 2);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//ɾ��VAO��VBO

	glfwTerminate();//�ͷ�GLFW��Դ

	return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//�����ڴ�С�����仯ʱ�����µĴ��ڴ�С������Ⱦ���ڵĳߴ��С
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_move_callback(GLFWwindow* window, double x, double y)
{
	xPos = x;
	yPos = y;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (count < 4 && count >=0)
		{
			vertices[count][0] = float(xPos - 400) / 400;
			vertices[count][1] = float(300 - yPos) / 300;
			vertices[count][2] = 0.0;
			std::cout << vertices[count][0]  << "  " << vertices[count][1] << std::endl;
			count++;
		}
	}
}

void Bezier(int vao, int vbo)
{
	//�������е��ĸ����ƶ������Bezier��������Ҫ�Ķ�����Ŀ��ĿǰԤ��400��������400������ϳ�һ������
	static float vertices_line[404][3];
	for (int i = 5; i <= 404; i++)
	{
		float t = float(i) / 400;
		float b0 = pow(1 - t, 3.0);
		float b1 = 3.0 * t * pow(1 - t, 2.0);
		float b2 = 3.0 * t * t * (1 - t);
		float b3 = t * t * t;
		//std::cout << b0 << b1 << b2 << b3 << std::endl;
		float x = vertices[0][0] * b0 + vertices[1][0] * b1 + vertices[2][0] * b2 + vertices[3][0] * b3;
		float y = vertices[0][1] * b0 + vertices[1][1] * b1 + vertices[2][1] * b2 + vertices[3][1] * b3;
		float z = 0.0;

		vertices_line[i - 1][0] = x;
		vertices_line[i - 1][1] = y;
		vertices_line[i - 1][2] = z;
	}

	for (int i = 0; i < 4; i++)
	{
		vertices_line[i][0] = vertices[i][0];
		vertices_line[i][1] = vertices[i][1];
		vertices_line[i][2] = vertices[i][2];
	}
	

	glBindVertexArray(vao);//��VAO��ʹVAO��VBO���а�
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_line), vertices_line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/*
	��VBO��GL_ARRAY_BUFFER
	�����ڴ�
	���벢����VBO������
	*/

	glEnableVertexAttribArray(0);//���ö������ԣ�ʹ��ɫ�����Է���


	glBindBuffer(GL_ARRAY_BUFFER, 0);//glBindBuffer(GL_ARRAY_BUFFER,0) �������ǰ������״̬����VBO��GL_ARRAY_BUFFER�İ�
	glBindVertexArray(0);//���VAO�İ󶨣���GL_CONTEXT��

}