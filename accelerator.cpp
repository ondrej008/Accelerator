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

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    Accelerator* accelerator = (Accelerator*) glfwGetWindowUserPointer(window);

    accelerator->onWindowSize(width, height);
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
    glfwSetWindowSizeCallback(m_window, windowSizeCallback);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_player = new Player;
    m_tunnel = new Tunnel;

    m_projection = glm::perspective((double) glm::radians(m_fov), (double) m_width / (double) m_height, 0.01, 100.0);

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

    printToFile("update: " + std::to_string(m_update) + ", render: " + std::to_string(m_render) + ", swap: " + std::to_string(m_swap) + ", total: " + std::to_string(m_total) + ", ran for: " + std::to_string((std::chrono::high_resolution_clock::now() - m_start).count() / std::pow(10.0, 9.0)));

    delete m_player;
    delete m_tunnel;

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
                            ", velocity: " + std::to_string(m_player->m_velocity) + 
                            "\n"
                );

                std::string windowStr = "Akcelerátor - " + std::to_string((double) (m_frames - lastFrames) / printCooldown) + ", " + std::to_string(m_player->m_velocity) + ", " + std::to_string(m_player->m_pos.z) + ", " + std::to_string(FLT_EPSILON * m_player->m_pos.z);

                glfwSetWindowTitle(m_window, windowStr.c_str());

                lastPrint = last;
                lastFrames = m_frames;
            }

            if(!m_paused)
            {
                auto startU = std::chrono::high_resolution_clock::now();
                update(elapsed);
                m_update += ((std::chrono::high_resolution_clock::now() - startU).count() / std::pow(10.0, 9.0));
            }

            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto startR = std::chrono::high_resolution_clock::now();

            render();

            m_render += ((std::chrono::high_resolution_clock::now() - startR).count() / std::pow(10.0, 9.0));

            ++m_frames;
        }

        auto startS = std::chrono::high_resolution_clock::now();

        glfwSwapBuffers(m_window);

        m_swap += ((std::chrono::high_resolution_clock::now() - startS).count() / std::pow(10.0, 9.0));
        m_total += ((std::chrono::high_resolution_clock::now() - last).count() / std::pow(10.0, 9.0));
    }
}

void Accelerator::update(double elapsed)
{
    m_player->update(elapsed);
    m_tunnel->update(elapsed, m_player->m_pos);

    if(m_tunnel->playerCollide(m_player->m_pos, m_player->m_radius))
    {
        m_player->gameOver();
    }
}

void Accelerator::render()
{
    glUseProgram(m_shader.m_programID);

    GLint colorL = glGetUniformLocation(m_shader.m_programID, "color");
    GLint modelL = glGetUniformLocation(m_shader.m_programID, "model");
    GLint viewL = glGetUniformLocation(m_shader.m_programID, "view");
    GLint projectionL = glGetUniformLocation(m_shader.m_programID, "projection");

    glm::mat4 model(1.0);

    glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewL, 1, GL_FALSE, glm::value_ptr(m_player->m_view));
    glUniformMatrix4fv(projectionL, 1, GL_FALSE, glm::value_ptr(m_projection));

    glm::vec4 color(0.0, 0.0, 0.0, 1.0);

    m_tunnel->render(modelL, colorL);
    m_player->render(modelL, colorL);
    
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

void Accelerator::onWindowSize(int width, int height)
{
    printToFile("onWindowSize(" + std::to_string(width) + ", " + std::to_string(height) + ")\n");

    glViewport(0, 0, width, height);

    m_width = width;
    m_height = height;
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
        else if(key == GLFW_KEY_SPACE)
        {
            delete m_tunnel;
            m_tunnel = new Tunnel;

            delete m_player;
            m_player = new Player;
        }
        else if(key == GLFW_KEY_ENTER)
        {
            if(mods & GLFW_MOD_SHIFT)
            {
                const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

                if(glfwGetWindowMonitor(m_window) != NULL)
                {
                    glfwSetWindowMonitor(m_window, NULL, 64, 128, mode->width - 128, mode->height - 256, mode->refreshRate);
                }
                else
                {
                    glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
                }
            }
        }
    }
}