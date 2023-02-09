#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Define shader source code
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Error when creating a GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Error when loading GLAD.\n");
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if the compilation was successful
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        printf(infoLog, '\n');
    }

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Specify triangle vertices
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.0f,  0.5f, 0.0f
        };

        // Vertex Buffer Object(VBO) - send vertices to the GPU's memory in
        // batches.
        // Generate an OpenGL buffer object.
        unsigned int VBO;
        glGenBuffers(1, &VBO);

        // OpenGL has many types of buffer objects.
        // Buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
        // Bind the newly created buffer to the GL_ARRAY_BUFFER target.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // With the currently bound buffer - VBO, copy previously defined vertex
        // data into the buffer's memory
        // Fourth parameter - how we want the GPU to manage the given data.
        // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        // GL_STATIC_DRAW: the data is set only once and used many times.
        // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
        // E.g. for the latter, the GPU will place the data in memory that allows
        // for faster writes
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertices),
                     vertices,
                     GL_STATIC_DRAW);

        // ^^^ As of this step, we stored the vertex data within the GPU memory
        // as managed by a vertex buffer object named VBO

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
