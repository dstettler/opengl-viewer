// template based on material from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <magic_enum/include/magic_enum/magic_enum.hpp>

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "Util.h"
#include "VAOContainer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, VAOContainer *container);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const std::string MODEL_FILENAME = "D:/Documents/GitHub/opengl-viewer/data/cube_tri.obj";
const std::string VERTEX_SHADER_FILE = "D:/Documents/GitHub/opengl-viewer/shaders/source.vs";
const std::string FRAGMENT_SHADER_FILE = "D:/Documents/GitHub/opengl-viewer/shaders/source.fs";

bool zoomIn = false, lastZoomIn = false, zoomOut = false, lastZoomOut = false;

int main()
{
    VAOContainer vaos;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Viewer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexSource = readFileToString(VERTEX_SHADER_FILE);
    const char* vertexSourceConst = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSourceConst, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentSource = readFileToString(FRAGMENT_SHADER_FILE);
    const char* fragmentSourceConst = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSourceConst, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_DEPTH_TEST);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // float vertices[] = {
    //     -0.5f, -.25f, 0.0f, 1.0f, 0.0f, 0.0f,  // left  
    //      0.5f, -.75f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
    //      0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top   

    //     -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // left  
    //      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // right 
    //      0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f // bottom
    // };
    // unsigned int numVertices = sizeof(vertices)/3;

    unsigned int VBO, VAO, EBO;

    // Read model
    vaos.load(MODEL_FILENAME, &VAO, &VBO, &EBO);
    auto vertsPtr =  vaos.getVertsArray();
    float* vertices = vertsPtr.get();
    unsigned int numVertices = vaos.getNumVerts();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // input
        // -----
        processInput(window, &vaos);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);

        vaos.drawGlMesh();

        // float currentTime = glfwGetTime();
        // float greenVal = std::sin(currentTime) / 2.0f + 0.5f;
        // int colorLocation = glGetUniformLocation(shaderProgram, "setColor");
        // glUniform4f(colorLocation, 0.0f, greenVal, 0.0f, 0.1f);

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, numVertices);
        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, VAOContainer *container)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    {
        zoomIn = true;
        if (zoomIn && !lastZoomIn)
            container->scaleMesh(1.5);
    }
    else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE)
    {
        zoomIn = false;
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        zoomOut = true;
        if (zoomOut && !lastZoomOut)
            container->scaleMesh(0.75);
    }
    else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE)
    {
        zoomOut = false;
    }

    lastZoomIn = zoomIn;
    lastZoomOut = zoomOut; 
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}