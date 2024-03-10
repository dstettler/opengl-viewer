// template based on material from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Util.h"
#include "VAOContainer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, VAOContainer *container);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const std::string MODEL_FILENAME = "./data/head.obj";
const std::string VERTEX_SHADER_FILE = "./shaders/source.vs";
const std::string FRAGMENT_SHADER_FILE = "./shaders/source.fs";

// Latches for movement keys
bool zoomIn = false, 
    lastZoomIn = false, 
    zoomOut = false, 
    lastZoomOut = false, 
    wireframe, 
    lastWireframe = false;

double currentTime = 0, prevTime = 0, fps = 0;

unsigned int tick = 0;

bool rotateCpu = true;

void fpsTick()
{
    currentTime = glfwGetTime();
    double d = currentTime - prevTime;
    tick++;
    if (d >= 1)
    {
        fps = (double)tick / d;
        prevTime = currentTime;
        tick = 0;
    }    
}

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

    // imgui init
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

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
    vaos.load(MODEL_FILENAME, &VAO, &VBO, &EBO, &vertexShader, &fragmentShader, &shaderProgram);

    // Disable vsync and uncap frames
    glfwSwapInterval(0);

    glDisable(GL_CULL_FACE);    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fpsTick();

        // input
        // -----
        processInput(window, &vaos);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Scene"))
        {
            ImGui::BeginChild("VAO");

            if (ImGui::CollapsingHeader("Select Model"))
            {
                const char* models[] = { "head", "pawn", "cube_tri", "eagle_tri" };
                static int selectedModel = 1;
                ImGui::ListBox("Model Select", &selectedModel, models, IM_ARRAYSIZE(models), 4);
                
                static int mode = 0;
                ImGui::RadioButton("Indexed Tris", &mode, 0); ImGui::SameLine();
                ImGui::RadioButton("Separate Tris", &mode, 1);

                if (ImGui::Button("Load Selected Model"))
                {
                    std::string filepath = "./data/";
                    filepath += models[selectedModel];
                    filepath += ".obj";
                    vaos.load(filepath, &VAO, &VBO, &EBO, &vertexShader, &fragmentShader, &shaderProgram,
                        (mode == 0) ? VAOContainer::TriMode::IndexedTris : VAOContainer::TriMode::SeparateTris
                    );
                }
            }

            ImGui::Spacing();

            if (ImGui::Button("Swap CPU/GPU"))
            {
                rotateCpu = !rotateCpu;
            }
            ImGui::Text((rotateCpu) ? "Using CPU" : "Using GPU");
            ImGui::Text("FPS: %f", fps);

            ImGui::EndChild();
        }
        ImGui::End();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use program for shaders
        glUseProgram(shaderProgram);

        vaos.drawGlMesh();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
        if (zoomIn && !lastZoomIn && rotateCpu)
            container->scaleMesh(1.5);
        else if (zoomIn && !lastZoomIn && !rotateCpu)
            container->scaleMeshGpu(1.5);
    }
    else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE)
    {
        zoomIn = false;
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        zoomOut = true;
        if (zoomOut && !lastZoomOut && rotateCpu)
            container->scaleMesh(0.75);
        else if (zoomOut && !lastZoomOut && !rotateCpu)
            container->scaleMeshGpu(0.75);
    }
    else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE)
    {
        zoomOut = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        wireframe = true;
        if (wireframe && !lastWireframe)
            container->setWireframe(!container->getWireframe());
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
    {
        wireframe = false;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (rotateCpu)
        {
            container->rotateMesh(-0.1, VAOContainer::MeshRotation::YAxis);
        }
        else
        {
            container->rotateMeshGpu(-0.1, VAOContainer::MeshRotation::YAxis);
        }
    }
        
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (rotateCpu)
        {
            container->rotateMesh(0.1, VAOContainer::MeshRotation::YAxis);
        }
        else
        {
            container->rotateMeshGpu(0.1, VAOContainer::MeshRotation::YAxis);
        }
    }    
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (rotateCpu)
        {
            container->rotateMesh(-0.1, VAOContainer::MeshRotation::XAxis);
        }
        else
        {
            container->rotateMeshGpu(-0.1, VAOContainer::MeshRotation::XAxis);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (rotateCpu)
        {
            container->rotateMesh(0.1, VAOContainer::MeshRotation::XAxis);
        }
        else
        {
            container->rotateMeshGpu(0.1, VAOContainer::MeshRotation::XAxis);
        }
    }

    lastZoomIn = zoomIn;
    lastZoomOut = zoomOut;
    lastWireframe = wireframe;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}