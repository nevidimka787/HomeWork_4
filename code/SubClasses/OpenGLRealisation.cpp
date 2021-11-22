#include "OpenGLRealisation.h"
#include <iostream>

OpenGL::OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window) :
    cursore_press_position(Vec2D()),
    cursore_last_position(Vec2F()),
    cursore_current_position(Vec2F()),
    flags_clk(OPEN_GL_REALISATION_KEY_NOTHIHG),
    update_frame(OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT)
{
    
    InitOpenGL();
    *window = CreateWindows(width, height, title, monitor, share, Function);
    window_height = height;
    window_width = width;
    InitGlad();
    InitBuffers();
    InitShaders();
    InitTextures();
    
    camera.size = 5.0f;
    
    //glEnable(GL_MULTISAMPLE);       //not work
    //glfwWindowHint(GLFW_SAMPLES, 4);
}

//Callback functions

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    update_frame = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    
    window_height = height;
    window_width = width;
    window_scale = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

void OpenGL::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Debug pause." << std::endl;
        int i = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        update_frame = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(flags_clk & OPEN_GL_REALISATION_KEY_MOUSE_LEFT)
        {
            glfwGetCursorPos(window, &cursore_press_position.x, &cursore_press_position.y);

            cursore_current_position = Vec2F(
                ((float)cursore_press_position.x / window_width - 0.5f) * 2.0f,
                ((float)cursore_press_position.y / -window_height) * 2.0f / window_scale + 1.0f);
            
            camera.Move((cursore_last_position - cursore_current_position) * camera.size);
            
            cursore_last_position = cursore_current_position;
        }
        else
        {
            flags_clk |= OPEN_GL_REALISATION_KEY_MOUSE_LEFT;

            glfwGetCursorPos(window, &cursore_press_position.x, &cursore_press_position.y);

            cursore_last_position = Vec2F(
                ((float)cursore_press_position.x / window_width - 0.5f) * 2.0f,
                ((float)cursore_press_position.y / -window_height) * 2.0f / window_scale + 1.0f);
        }
        
        update_frame = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    }
    else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE &&
        flags_clk & OPEN_GL_REALISATION_KEY_MOUSE_LEFT)
    {
        flags_clk &= OPEN_GL_REALISATION_KEY_FULL - OPEN_GL_REALISATION_KEY_MOUSE_LEFT;
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS &&
        !(flags_clk & OPEN_GL_REALISATION_KEY_UP))
    {
        flags_clk |= OPEN_GL_REALISATION_KEY_UP;
        if(camera.size < OPEN_GL_REALISATION_CAMERA_MAX_EDGE_SIZE)
        {
            camera.size *= OPEN_GL_REALISATION_CAMERA_RESIZE_PARAMETER;
        }
        if(camera.size < CAMERA_DEFAULT_SIZE + 0.05f && camera.size > CAMERA_DEFAULT_SIZE - 0.05f)
        {
            camera.size = CAMERA_DEFAULT_SIZE;
        }
    update_frame = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    }
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE &&
        flags_clk & OPEN_GL_REALISATION_KEY_UP)
    {
        flags_clk &= OPEN_GL_REALISATION_KEY_FULL - OPEN_GL_REALISATION_KEY_UP;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) &&
        !(flags_clk & OPEN_GL_REALISATION_KEY_DOWN))
    {
        flags_clk |= OPEN_GL_REALISATION_KEY_DOWN;
        if(camera.size > OPEN_GL_REALISATION_CAMERA_MIN_EDGE_SIZE)
        {
            camera.size /= OPEN_GL_REALISATION_CAMERA_RESIZE_PARAMETER;
        }
        if(camera.size < CAMERA_DEFAULT_SIZE * 
                (1.0f + (OPEN_GL_REALISATION_CAMERA_RESIZE_PARAMETER - 1.0f) / 2.0f) &&
            camera.size > CAMERA_DEFAULT_SIZE * 
                (1.0f - (OPEN_GL_REALISATION_CAMERA_RESIZE_PARAMETER - 1.0f) / 2.0f))
        {
            camera.size = CAMERA_DEFAULT_SIZE;
        }
        update_frame = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE &&
        flags_clk & OPEN_GL_REALISATION_KEY_DOWN)
    {
        flags_clk &= OPEN_GL_REALISATION_KEY_FULL - OPEN_GL_REALISATION_KEY_DOWN;
    }
}



//Callback functions

//Initialisation functions

GLFWwindow* OpenGL::CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    window_scale = (float)width / (float)height;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Function);

    return window;
}

#define VERTEX_ARRAY_LENGTH 6

void OpenGL::InitBuffers()
{
    Vec2F points[VERTEX_ARRAY_LENGTH];

    points[0].Set(1.0f, 1.0f);
    points[1].Set(-1.0f, 1.0f);
    points[2].Set(1.0f, -1.0f);
    points[3].Set(-1.0f, -1.0f);
    points[4].Set(-1.0f, 1.0f);
    points[5].Set(1.0f, -1.0f);
    
    point_buffer.Initialisate(points, 6);
    
    points[0].Set(1.0f, 1.0f);
    points[1].Set(0.0f, 1.0f);
    points[2].Set(1.0f, -1.0f);
    points[3].Set(0.0f, -1.0f);
    points[4].Set(0.0f, 1.0f);
    points[5].Set(1.0f, -1.0f);
    segment_buffer.Initialisate(points, 6);
}

void OpenGL::InitGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void OpenGL::InitOpenGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGL::InitShaders()
{
    point_shader.Initialisate("Shaders/Objects/Vertex/Node.glsl", "Shaders/Objects/Fragment/Node.glsl");
    segment_shader.Initialisate("Shaders/Objects/Vertex/Segment.glsl", "Shaders/Objects/Fragment/Segment.glsl");
}

void OpenGL::InitTextures()
{
    symbols_texture.Initialisate("Textures/Symbols.bmp");
}

//Draw functions

void OpenGL::DrawFrame()
{
    if(update_frame)
    {
        update_frame--;
    }
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL::DrawObject(const Node* node, bool update_shader)
{
    if(update_shader)
    {
        point_buffer.Use();
        point_shader.Use();
        symbols_texture.Use();
        point_shader.SetUniform("scale", window_scale);
        point_shader.SetUniform("camera_position", camera.position);
        point_shader.SetUniform("camera_size", camera.size);
    }
    point_shader.SetUniform("position", node->position);
    point_shader.SetUniform("radius", node->radius);
    point_shader.SetUniform("color_id", (int)node->color_id);
    int* str = node->GetNumberAsTextI();
    int text_length = (int)node->GetNumberTextLength();
    point_shader.SetUniform("text", str, text_length);
    delete[] str;
    point_shader.SetUniform("text_length", text_length);
    point_shader.SetUniform("text_size", node->text_size);
    point_buffer.Draw();
}

void OpenGL::DrawObject(const PhysicConnection* connection, bool update_shader)
{
    DrawObject(connection->GetSegment(CONNECTION_SEGMENT_ID_START), update_shader);
    DrawObject(connection->GetSegment(CONNECTION_SEGMENT_ID_CENTER));
    DrawObject(connection->GetSegment(CONNECTION_SEGMENT_ID_END));
}

void OpenGL::DrawObject(const PhysicGraph* ph_graph)
{
    const PhysicConnection* connections = ph_graph->GetConnectionsArrayPointer();
    const Node* nodes = ph_graph->GetNodesArrayPointer();
    point_t connections_count = ph_graph->GetConnectionsCount();
    point_t nodes_count = ph_graph->GetNodesCount();
    
    if(connections_count > 0)
    {
        DrawObject(&connections[0], true);
    }
    for(point_t connection = 1; connection < connections_count; connection++)
    {
        DrawObject(&connections[connection]);
    }
    
    if(nodes_count > 0)
    {
        DrawObject(&nodes[0], true);
    }
    for(point_t node = 0; node < nodes_count; node++)
    {
        DrawObject(&nodes[node]);
    }
}

void OpenGL::DrawObject(Segment segment, bool update_shader)
{
    if(update_shader)
    {
        segment_buffer.Use();
        segment_shader.Use();
        segment_shader.SetUniform("scale", window_scale);
        segment_shader.SetUniform("camera_position", camera.position);
        segment_shader.SetUniform("camera_size", camera.size);
    }
    segment_shader.SetUniform("segment", segment);
    segment_buffer.Draw();
}

void OpenGL::DrawObject(const Segment* segment, bool update_shader)
{
    if(update_shader)
    {
        segment_buffer.Use();
        segment_shader.Use();
        segment_shader.SetUniform("scale", window_scale);
        segment_shader.SetUniform("camera_position", camera.position);
        segment_shader.SetUniform("camera_size", camera.size);
    }
    segment_shader.SetUniform("segment", *segment);
    segment_buffer.Draw();
}

//Multydraw functions

//Get data functions

bool OpenGL::CanDrawFrame(GLFWwindow* window) const
{
    return glfwWindowShouldClose(window) == false;
}

float OpenGL::GetScale() const
{
    return window_scale;
}

//Get data functions

OpenGL::~OpenGL()
{
    //glfwTerminate();
}






