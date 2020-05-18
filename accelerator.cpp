#include "accelerator.hpp"

#include <cmath>

void errorCallback(int errorCode, const char* description)
{
    printToFile("\nGLFW ERROR: " + std::to_string(errorCode) + "\n\n");
    printToFile(std::string(description) + "\n");
    printToFile("\nGLFW ERROR: " + std::to_string(errorCode) + "\n\n");
}

void windowCloseCallback(GLFWwindow* window)
{
    Accelerator* accelerator = (Accelerator*) glfwGetWindowUserPointer(window);

    accelerator->onWindowClose();
}

void windowFocusCallback(GLFWwindow* window, int focused)
{
    Accelerator* accelerator = (Accelerator*) glfwGetWindowUserPointer(window);

    accelerator->onWindowFocus(focused);
}

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    Accelerator* accelerator = (Accelerator*) glfwGetWindowUserPointer(window);

    accelerator->onCursorPosition(xPos, yPos);
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Accelerator* accelerator = (Accelerator*) glfwGetWindowUserPointer(window);

    accelerator->onKey(key, scanCode, action, mods);
}

glm::vec2 screenToOpenGL(glm::vec2 pos, glm::vec2 dim)
{
    return glm::vec2((pos.x / (dim.x * 0.5)) - 1.0, (pos.y / (dim.y * 0.5)) - 1.0);
}

Accelerator::Accelerator()
{
    glfwSetErrorCallback(errorCallback);

    START_TIMING();

    if(glfwInit() != GLFW_TRUE)
    {
        m_running = false;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    m_window = glfwCreateWindow(mode->width, mode->height, "Akcelerátor", glfwGetPrimaryMonitor(), NULL);

    glfwGetWindowSize(m_window, &m_width, &m_height);

    if(m_window == nullptr)
    {
        m_running = false;
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowCloseCallback(m_window, windowCloseCallback);
    glfwSetWindowFocusCallback(m_window, windowFocusCallback);
    glfwSetCursorPosCallback(m_window, cursorPositionCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    
    glfwMakeContextCurrent(m_window);

    glewExperimental = true;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        printToFile("\nGLEW ERROR: " + std::string((char*) glewGetErrorString(err)) + "\n\n");
    }

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // default
    glFrontFace(GL_CCW); // default


    m_player = new Player;

    m_projection = glm::perspective(glm::radians(45.0), (double) m_width / (double) m_height, 0.01, 100.0);

    if(!m_shader.loadFromFiles("basic.vert", "basic.frag"))
    {
        printToFile("ERROR reading files, aborting...\n");
        m_running = false;
    }

    RECORD_TIMING("glfw init");
}

Accelerator::~Accelerator()
{
    printToFile("Shutting down...\n");
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Accelerator::run()
{
    m_start = std::chrono::high_resolution_clock::now();
    auto last = std::chrono::high_resolution_clock::now();
    auto lastPrint = std::chrono::high_resolution_clock::now();
    int lastFrames = 0;

    while(m_running)
    {
        double elapsed = (std::chrono::high_resolution_clock::now() - last).count() / std::pow(10.0, 9.0);
        last = std::chrono::high_resolution_clock::now();

        glfwPollEvents();

        bool isFocused = glfwGetWindowAttrib(m_window, GLFW_FOCUSED);

        if(isFocused)
        {
            m_elapsed += elapsed;

            auto printCooldown = (last - lastPrint).count() / std::pow(10.0, 9.0);
            if(printCooldown > 1.0)
            {
                printToFile("frames: " + std::to_string(m_frames) +
                        ", fps: " + std::to_string((double) (m_frames - lastFrames) / printCooldown) +
                        ", μs: " + std::to_string(elapsed * 1000000.0) +
                        ", ms: " + std::to_string(elapsed * 1000.0) +
                        ", s: " + std::to_string(elapsed) +
                        ", elapsed: " + std::to_string(m_elapsed) +
                        ", start: " + std::to_string((std::chrono::high_resolution_clock::now() - m_start).count() / std::pow(10.0, 9.0)) + 
                        "\n"
                );
            
                printToFile("yaw: " + std::to_string(m_player->m_yaw) +
                            ", pitch: " + std::to_string(m_player->m_pitch) +
                            ", pos: {" + std::to_string(m_player->m_pos.x) + ", " + std::to_string(m_player->m_pos.y) + ", " + std::to_string(m_player->m_pos.z) + "}" +
                            "\n"
                );

                lastPrint = last;
                lastFrames = m_frames;
            }

            if(!m_paused)
            {
                update(elapsed);
            }

            glClearColor(1.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render();

            ++m_frames;
        }

        glfwSwapBuffers(m_window);
    }
}

void Accelerator::update(double elapsed)
{
    m_player->update(elapsed);
}

void Accelerator::render()
{
    glUseProgram(m_shader.m_programID);

    GLint modelL = glGetUniformLocation(m_shader.m_programID, "model");
    GLint viewL = glGetUniformLocation(m_shader.m_programID, "view");
    GLint projectionL = glGetUniformLocation(m_shader.m_programID, "projection");

    glm::mat4 model(1.0);

    glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewL, 1, GL_FALSE, glm::value_ptr(m_player->m_view));
    glUniformMatrix4fv(projectionL, 1, GL_FALSE, glm::value_ptr(m_projection));

    renderCrosshair();

    glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-1.0, -1.0, -5.0);
        glVertex3f(1.0, -1.0, -5.0);
        glVertex3f(0.0, 1.0, -5.0);
    glEnd();

    m_player->render(modelL);
}

void Accelerator::renderCrosshair()
{
    glPointSize(1.0);

    glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
        
        for(int i = -5; i <= 5; i++)
        {
            for(int j = -5; j <= 5; j++)
            {
                if(i == -5 || i == 5 || j == -5 || j == 5) 
                {
                    glm::vec2 vtx = screenToOpenGL(glm::vec2(i + m_width / 2.0, j + m_height / 2.0), glm::vec2(m_width, m_height));
                    glVertex2f(vtx.x, vtx.y);
                }
            }
        }

    glEnd();
}

void Accelerator::onWindowClose()
{
    printToFile("onWindowClose()\n");
    m_running = false;
}

void Accelerator::onWindowFocus(bool focused)
{
    printToFile("onWindowFocus(" + std::string(focused ? "true" : "false") + ")\n");
}

void Accelerator::onCursorPosition(double xPos, double yPos)
{
    //printToFile("onCursorPosition(" + std::to_string(xPos) + ", " + std::to_string(yPos) + ")\n");

    static bool firstMouse = true;
    static double lastX = 0.0;
    static double lastY = 0.0;

    if(firstMouse)
    {
        lastX = xPos;
        lastY = yPos;

        firstMouse = false;
    }

    m_player->updateCamera(xPos - lastX, lastY - yPos); // reverted since GLFW coords go top to bottom

    lastX = xPos;
    lastY = yPos;
}

void Accelerator::onKey(int key, int scanCode ,int action, int mods)
{
    if(action == GLFW_RELEASE)
    {
        if(key == GLFW_KEY_ESCAPE)
        {
            m_running = false;
            printToFile("User pressed ESCAPE, closing...\n");
        }
        else if(key == GLFW_KEY_W)
        {
            m_player->gameOver();
        }
    }
}