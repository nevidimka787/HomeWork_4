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
    point_shader.Initialisate("Shaders/Objects/Vertex/Point.glsl", "Shaders/Objects/Fragment/Point.glsl");
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

void OpenGL::DrawConnectedGraph(Graph graph, Vec2F position)
{
#define POINT_RADIUS    0.2f
#define CONNECT_RADIUS  (POINT_RADIUS * 1.5f)
#define CELL_SIZE       1.0f
#define CEL_SIZE_2      (CELL_SIZE / 2.0f)
    
    //position.Set(0.0f, 0.0f);
    
    GraphTypes::point_t points_count = graph.GetPointsCount();
    GraphTypes::point_t connections_count = graph.GetConnectionsCount();
    if(points_count == 0 || connections_count == 0)
    {
        return;
    }
    Point* points = new Point[points_count];
    
    Connection* c_arr = graph.GetConnectionsArray();
    GraphTypes::point_t* p_arr = graph.GetPointsArray();
    
    GraphTypes::point_t last_point = c_arr[0].GetPoint1();
    unsigned ux = 0;
    unsigned uy = 0;
    bool* sets = new bool[points_count];
    for(GraphTypes::point_t p = 1; p < points_count; p++)
    {
        sets[p] = false;
    }
    sets[0] = true;

    points[0] = Point(
        p_arr[0],                                   //point id
        Vec2F(0.0f, 0.0f) + position * CELL_SIZE,   //point position
        POINT_RADIUS);                              //point radius
    
    Connection last_con = c_arr[0];
    
    for(GraphTypes::point_t c = 0; c < connections_count; c++)
    {
        if(last_point != c_arr[c].GetPoint1())
        {
            last_point = c_arr[c].GetPoint1();
            uy++;
            ux = 0;
        }
        if(last_con != c_arr[c])
        {
            last_con = c_arr[c];
            for(GraphTypes::point_t p = 0; p < points_count; p++)
            {
                if(p_arr[p] == c_arr[c].GetPoint1())
                {
                    points[p] = Point(
                        p_arr[p],
                        Vec2F((float)ux * CELL_SIZE, -(float)uy * CELL_SIZE) + position * CELL_SIZE,
                        POINT_RADIUS);
                    //ux++;
                    sets[p] = true;
                }
            }
        }
    }
    uy++;
    ux = 0;
    GraphTypes::point_t cp;
    for(GraphTypes::point_t p = points_count; p > 0; p--)
    {
        cp = p - 1;
        if(!sets[cp])
        {
            points[cp] = Point(
                p_arr[cp],
                Vec2F((-(float)ux - 1.0f) * CELL_SIZE, -(float)uy * CELL_SIZE) + position * CELL_SIZE,
                POINT_RADIUS);
            uy++;
        }
    }
    
    PhysicConnection connection = PhysicConnection(&points[0], &points[0], 0.0f, 0.0f);
    unsigned shift = 0;
    GraphTypes::point_t p1_id;
    GraphTypes::point_t p2_id;
    last_con = c_arr[0];
    float dist;
        
    for(GraphTypes::point_t c = 0; c < connections_count; c++)
    {
        p1_id = points_count;
        p2_id = points_count;
        for(GraphTypes::point_t p = 0; p < points_count; p++)
        {
            if(c_arr[c].GetPoint1() == p_arr[p])
            {
                p1_id = p;
            }
            if(c_arr[c].GetPoint2() == p_arr[p])
            {
                p2_id = p;
            }
            if(p1_id != points_count && p2_id != points_count)
            {
                break;
            }
        }
        
        if(last_con != c_arr[c])
        {
            last_con = c_arr[c];
            shift = 0;
        }
        else if(c > 0)
        {
            if(shift == 0)
            {
                shift = 2;
            }
            else
            {
                shift++;
            }
        }
        
        dist = points[p1_id].position.GetDistance(points[p2_id].position);
        connection = PhysicConnection(
            &points[p1_id],    //first point
            &points[p2_id],    //second point
            (p1_id != p2_id) ? 
                fminf(dist  / 2.0f, 5.0f / 7.0f) / (float)(shift + 1) * CELL_SIZE : 
                CEL_SIZE_2 + shift * CELL_SIZE,      //shift_x
            (p1_id != p2_id) ? 
                -((dist - CELL_SIZE - (float)shift) * POINT_RADIUS * 19.0f / 5.0f) * CELL_SIZE : 
                CEL_SIZE_2 + sqrt(shift * CELL_SIZE));   //shift_y
        
        DrawObject(&connection, c == 0);
    }
  
    point_buffer.Use();
    point_shader.Use();
    symbols_texture.Use();
    point_shader.SetUniform("scale", window_scale);
    point_shader.SetUniform("camera_position", camera.position);
    point_shader.SetUniform("camera_size", camera.size);
    for(GraphTypes::point_t p = 0; p < points_count; p++)
    {
        DrawObject(&points[p]);
    }
    
    delete[] points;
    delete[] p_arr;
    delete[] c_arr;
}

void OpenGL::DrawObject(Point* point, bool update_shader)
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
    point_shader.SetUniform("position", point->position);
    point_shader.SetUniform("radius", point->radius);
    int* str = point->GetNumberAsTextI();
    int text_length = (int)point->GetNumberTextLength();
    point_shader.SetUniform("text", str, text_length);
    delete[] str;
    point_shader.SetUniform("text_length", text_length);
    point_shader.SetUniform("text_size", point->text_size);
    point_buffer.Draw();
}

void OpenGL::DrawObject(PhysicConnection* connection, bool update_shader)
{
    DrawObject(connection->GetSegment(CONNECTION_SEGMENT_ID_START), update_shader);
    DrawObject(connection->GetSegment(CONNECTION_SEGMENT_ID_CENTER));
    DrawObject(connection->GetSegment(CONNECTION_SEGMENT_ID_END));
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

void OpenGL::DrawObject(Segment* segment, bool update_shader)
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

void OpenGL::DrawObject(Graph graph, bool update_shader)
{
    Graph div_g;
    graph.Sort();
    Vec2F position = Vec2F(0.0f, 0.0f);
    if(graph.Divide(&div_g))
    {
        DrawConnectedGraph(div_g, position);
        position.x += div_g.GetWidth() + div_g.GetHight();
    }
    else
    {
        DrawConnectedGraph(graph, position);
        return;
    }
    while(graph.Divide(&div_g))
    {
        DrawConnectedGraph(div_g, position);
        position.x += div_g.GetWidth() + div_g.GetHight();
    }
    DrawConnectedGraph(graph, position);
}

//Multydraw functions

//Get data functions

bool OpenGL::CanDrawFrame(GLFWwindow* window)
{
    return glfwWindowShouldClose(window) == false;
}

float OpenGL::GetScale()
{
    return window_scale;
}

//Get data functions

OpenGL::~OpenGL()
{
    //glfwTerminate();
}






