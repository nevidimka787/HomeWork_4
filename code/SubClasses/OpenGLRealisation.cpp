#include "OpenGLRealisation.h"
#include <iostream>

OpenGL::OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window) :
    cursore_press_position(Vec2D(void)),
    cursore_last_position(Vec2F(void)),
    cursore_current_position(Vec2F(void)),
    flags_clk(OPEN_GL_REALISATION_KEY_NOTHIHG),
    update_frame(OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT)
{
    
    InitOpenGL(void);
    *window = CreateWindows(width, height, title, monitor, share, Function);
    window_height = height;
    window_width = width;
    InitGlad(void);
    InitBuffers(void);
    InitShaders(void);
    InitTextures(void);
    
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
        glfwTerminate(void);
        exit(-1);
    }

    window_scale = (float)width / (float)height;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Function);

    return window;
}

#define VERTEX_ARRAY_LENGTH 6

void OpenGL::InitBuffers(void)
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

void OpenGL::InitGlad(void)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void OpenGL::InitOpenGL(void)
{
    glfwInit(void);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGL::InitShaders(void)
{
    point_shader.Initialisate("Shaders/Objects/Vertex/Node.glsl", "Shaders/Objects/Fragment/Node.glsl");
    segment_shader.Initialisate("Shaders/Objects/Vertex/Segment.glsl", "Shaders/Objects/Fragment/Segment.glsl");
}

void OpenGL::InitTextures(void)
{
    symbols_texture.Initialisate("Textures/Symbols.bmp");
}

//Draw functions

void OpenGL::DrawFrame(void)
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
#define TIMES_PARAMETER  0.2f
    
    //position.Set(0.0f, 0.0f);
    
    GraphTypes::point_t points_count = graph.GetPointsCount(void);
    GraphTypes::point_t connections_count = graph.GetConnectionsCount(void);
    if(points_count == 0 || connections_count == 0)
    {
        return;
    }
    Node* points = new Node[points_count];
    
    Connection* c_arr = graph.GetConnectionsArray(void);
    GraphTypes::point_t* p_arr = graph.GetPointsArray(void);
    
    GraphTypes::point_t last_point = c_arr[0].GetPoint1(void);
    unsigned ux = 0;
    unsigned uy = 0;
    bool* sets = new bool[points_count];
    for(GraphTypes::point_t p = 1; p < points_count; p++)
    {
        sets[p] = false;
    }
    sets[0] = true;
    
    Connection last_con = c_arr[0];
    unsigned max_times = 0;
    
    for(unsigned connection = 1, times = 0; connection < connections_count; connection++)
    {
        if(c_arr[connection] == last_con)
        {
            times++;
            if(max_times < times)
            {
                max_times = times;
            }
        }
    }
    
    
    Vec2F radius_vector = Vec2F((float)points_count * POINT_RADIUS * 2.0f * (1.0f + (float)max_times * TIMES_PARAMETER), 0.0f);

    points[0] = Node(
        p_arr[0],                                   //point id
        radius_vector + position * CELL_SIZE,       //point position
        POINT_RADIUS);                              //point radius
    
    for(GraphTypes::point_t point = 0; point < points_count; point++)
    {
        points[point] = Node(
            p_arr[point],
            radius_vector.Rotate((float)point / (float)points_count * M_PI * 2.0f),
            POINT_RADIUS);
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
            if(c_arr[c].GetPoint1(void) == p_arr[p])
            {
                p1_id = p;
            }
            if(c_arr[c].GetPoint2(void) == p_arr[p])
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
            shift++;
        }
        
        dist = points[p1_id].position.GetDistance(points[p2_id].position);
        connection = PhysicConnection(
            &points[p1_id],    //first point
            &points[p2_id],    //second point
            (p1_id != p2_id) ? 
                 dist / 2.0f: 
                CEL_SIZE_2 + shift * CELL_SIZE,      //shift_x
            (p1_id != p2_id) ? 
                shift * POINT_RADIUS * TIMES_PARAMETER : 
                CEL_SIZE_2 + sqrt(shift * CELL_SIZE));   //shift_y
        
        DrawObject(&connection, c == 0);
    }
  
    point_buffer.Use(void);
    point_shader.Use(void);
    symbols_texture.Use(void);
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

void OpenGL::DrawObject(Node* point, bool update_shader)
{
    if(update_shader)
    {
        point_buffer.Use(void);
        point_shader.Use(void);
        symbols_texture.Use(void);
        point_shader.SetUniform("scale", window_scale);
        point_shader.SetUniform("camera_position", camera.position);
        point_shader.SetUniform("camera_size", camera.size);
    }
    point_shader.SetUniform("position", point->position);
    point_shader.SetUniform("radius", point->radius);
    point_shader.SetUniform("color_id", node->color_id);
    int* str = point->GetNumberAsTextI(void);
    int text_length = (int)point->GetNumberTextLength(void);
    point_shader.SetUniform("text", str, text_length);
    delete[] str;
    point_shader.SetUniform("text_length", text_length);
    point_shader.SetUniform("text_size", point->text_size);
    point_buffer.Draw(void);
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
        segment_buffer.Use(void);
        segment_shader.Use(void);
        segment_shader.SetUniform("scale", window_scale);
        segment_shader.SetUniform("camera_position", camera.position);
        segment_shader.SetUniform("camera_size", camera.size);
    }
    segment_shader.SetUniform("segment", segment);
    segment_buffer.Draw(void);
}

void OpenGL::DrawObject(Segment* segment, bool update_shader)
{
    if(update_shader)
    {
        segment_buffer.Use(void);
        segment_shader.Use(void);
        segment_shader.SetUniform("scale", window_scale);
        segment_shader.SetUniform("camera_position", camera.position);
        segment_shader.SetUniform("camera_size", camera.size);
    }
    segment_shader.SetUniform("segment", segment);
    segment_buffer.Draw(void);
}

void OpenGL::DrawObject(Graph graph, bool update_shader)
{
    Graph div_g;
    graph.Sort(void);
    Vec2F position = Vec2F(0.0f, 0.0f);
    if(graph.Divide(&div_g))
    {
        DrawConnectedGraph(div_g, position);
        position.x += div_g.GetWidth(void) + div_g.GetHight(void);
    }
    else
    {
        DrawConnectedGraph(graph, position);
        return;
    }
    while(graph.Divide(&div_g))
    {
        DrawConnectedGraph(div_g, position);
        position.x += div_g.GetWidth(void) + div_g.GetHight(void);
    }
    DrawConnectedGraph(graph, position);
}

//Multydraw functions

//Get data functions

bool OpenGL::CanDrawFrame(GLFWwindow* window)
{
    return glfwWindowShouldClose(window) == false;
}

float OpenGL::GetScale(void)
{
    return window_scale;
}

//Get data functions

OpenGL::~OpenGL(void)
{
    //glfwTerminate(void);
}






