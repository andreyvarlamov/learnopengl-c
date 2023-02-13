#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Define vertex shader source code
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Define fragment shader source code
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = ourColor;"
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
    // ----------------- INITIALIZE WINDOW -----------------------------------
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

    // GLAD: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Error when loading GLAD.\n");
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ----------------- COMPILE SHADER PROGRAM ------------------------------
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

    // Compile the fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check if the compilation was successful
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        printf(infoLog, '\n');
    }

    // Create a shader program, attach shaders, and link
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if linking was successful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINKING_FAILED\n");
        printf(infoLog, '\n');
    }

    // Activate the shader program
    // Every shader and rendering call after this will use this program object,
    // and thus the shaders
    glUseProgram(shaderProgram);

    // Don't need the actual shaders once they have been linked into the shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ----------------- DRAW TRIANGLE ---------------------------------------
    // Specify triangle vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.0f,  0.5f, 0.0f
    };

    // Vertex Buffer Object (VBO) - send vertices to the GPU's memory in
    // batches.
    // Generate an OpenGL buffer object.
    // unsigned int VBO;
    // glGenBuffers(1, &VBO);

    // Using Element Buffer Object (EBO)
    // index drawing
    // float vertices [] = {
    //     0.5f,  0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //    -0.5f, -0.5f, 0.0f
    //    -0.5f,  0.5f, 0.0f
    // };
    // unsigned int indices[] = {
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // unsigned int EBO;
    // glGenBuffers(1, &EBO);

    // Vertex Array Object (VAO) - any subsequent vertex attribute calls will
    // be stored inside the VAO.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. Bind VAO

    glBindVertexArray(VAO);

    // 2. Copy our vertices array in a buffer for OpenGL to use

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ^^^ As of this step, we stored the vertex data within the GPU memory
    // as managed by a vertex buffer object named VBO

    // (Using EBO)
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. Set our vertex attributes pointers

    // Tell OpenGL how to interpret vertex buffer data
    // The first argument - we set location of the vertex attribute to 0 in
    // vertex shader (layout (location = 0))
    // The fifth argument - "stride" - space between consecutive vertex attributes.
    // Each vertex attribute takes its data from memory mamange by a VBO;
    // Which VBO it takes its data from is determined by the VBO currently bound
    // to GL_ARRAY_BUFFER; the VBO object that we created is still bound to it
    // since the line: glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ............... WITHOUT VAOs ..............
    // Actually drawing an object would look like this
    // This process has to be repeated every time we want to draw an object
    // 0. copy our vertices in a buffer for OpenGL to use
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 1. then set the vertex attributes pointers
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // 2. use our shader program when we want to render an object
    // glUseProgram(shaderProgram);
    // 3. now draw the object
    // someOpenGLFunctionThatDrawsOurTriangle();
    // ...........................................

    // ............... WITH VAOs .................
    // Drawing code (in render loop)
    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // someOpenGLFunctionThatDrawsOurTriangle();
    // ...........................................

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // 4. Draw the triangle

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // Draw primitives using currently active shader, the previously defined
        // vertex attribute configuration and the VBO's vertex data (indirectly
        // bound via the VAO)
        // Draw in triangles mode, read 3 vertices started from 0
        // (If using VBO)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // (If using EBO)
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
