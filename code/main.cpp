
#define ERROR   -1
#define SUCSSES 0
#define UNSIGNED_ERROR_CODE UINT_MAX


#include "NotMy/glad.h"
#include <GLFW/glfw3.h> //need install OpenGL libraries

#include <iostream>
#include <unistd.h> 

#include "Types/AllTypes.h"
#include "Constants/AllConstants.h"
#include "Classes/Engine.h"

#include "SubClasses/GraphRealisation.h"
#include "SubClasses/OpenGLRealisation.h"

#define CONNENTIONS_COUNT 5

#define SCR_WIDTH   800
#define SCR_HEIGHT  800

void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height);

OpenGL* main_draw;

void ReadGraph(Graph* graph);
void Error(const char* type, const char* name, const char* text);
void Warning(const char* type, const char* name, const char* text);
unsigned ConvertToUnsigned(char* str, bool invert = false);
unsigned GetLength(char* str, unsigned limit = 10);

int main(void)
{
    //Init graph
    Graph graph;
    
    std::cout << "Read graph as <node> - <node>" << std::endl;
    
    //Write data to graph.
    ReadGraph(&graph);
    
    PhysicGraph ph_graph = PhysicGraph(&graph, Vec2F());
    
    GLFWwindow* window = nullptr;
    
    //Init OpenGL.
    main_draw = new OpenGL(
        SCR_WIDTH,                      //width
        SCR_HEIGHT,                     //height
        "Graph visualisation",          //window name
        nullptr,                        //monitor
        nullptr,                        //share
        LocalFramebufferSizeCallback,   //resize callback function
        &window                         //pointer to window
    );
    
    //Draw graph.
    while(!glfwWindowShouldClose(window))
    {
        main_draw->ProcessInput(window);
        
        if(main_draw->update_frame)
        {
            main_draw->DrawFrame(void);
            main_draw->DrawObject(graph);
            glfwSwapBuffers(window);
        }
        else
        {
            usleep(100);
        }
        
        glfwPollEvents(void);
        glfwSwapInterval(1);//vertical synchronisation
    }
}

void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    main_draw->FramebufferSizeCallback(window, width, height);
}

void ReadGraph(Graph* graph)
{
#define NO_PHASE        0x00 //0000 0000
#define FIRST_PHASE     0x01 //0000 0001
#define SECOND_PHASE    0x02 //0000 0010
#define THIRD_PHASE     0x04 //0000 0100
#define FOURTH_PHASE    0x08 //0000 1000
    
    char current = '\0';
    char last = '\0';
    uint8_t flags = NO_PHASE;
    Connection* connections_array = (Connection*)malloc(sizeof(Connection));
    if(connections_array == nullptr)
    {
        Error("Function", "ReadGraph", "Realloc return null pointer.");
        exit(ERROR);
    }
    unsigned connections_count = 0u;
    GraphTypes::point_t point1_value = 0u;
    GraphTypes::point_t point2_value = 0u;
    
    char* str = (char*)malloc(sizeof(char));
    if(str == nullptr)
    {
        Error("Function", "ReadGraph", "Realloc return null pointer.");
        exit(ERROR);
    }
    unsigned str_length = 0u;
    
    while(true)
    {
        current = getchar(void);
        switch(current)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if(flags == NO_PHASE)
            {
                flags = FIRST_PHASE;
            }
            else if(flags & SECOND_PHASE)
            {
                flags = THIRD_PHASE;
            }
            str_length++;
            str = (char*)realloc(str, sizeof(char) * str_length);
            if(str == nullptr)
            {
                Error("Function", "ReadGraph", "Realloc return null pointer.");
            }
            str[str_length - 1] = current;
            break;
        case '-':
        case '>':
            if(flags & FIRST_PHASE)
            {
                str = (char*)realloc(str, sizeof(char) * (str_length + 1));
                if(str == nullptr)
                {
                    Error("Function", "ReadGraph", "Realloc return null pointer.");
                }
                str[str_length] = '\0';
                point1_value = ConvertToUnsigned(str, true);
                str_length = 0;
                
                flags = SECOND_PHASE;
            }
            else if(!(flags & SECOND_PHASE))
            {
                Error("Function", "ReadGraph", "Incorrect input. Phase 2");
            }
            break;
        case ' ':
            break;
        case '\n':
            if(last == '\n')
            {
                break;
            }
            if(flags & THIRD_PHASE)
            {
                str = (char*)realloc(str, sizeof(char) * (str_length + 1));
                if(str == nullptr)
                {
                    Error("Function", "ReadGraph", "Realloc return null pointer.");
                }
                str[str_length] = '\0';
                point2_value = ConvertToUnsigned(str, true);
                str_length = 0;
                connections_count++;
                connections_array = (Connection*)realloc(connections_array, sizeof(Connection) * connections_count);
                connections_array[connections_count - 1] = Connection(point1_value, point2_value);
                point1_value = 0u;
                point2_value = 0u;
                flags = NO_PHASE;
            }
            else
            {
                Error("Function", "ReadGraph", "Incorrect input. Phase 3");
            }
            break;
            
            break;
        default:
            Error("Function", "ReadGraph", "Incorrect input.");
        }
        
        if(current == '\n' && last == '\n')
        {
            break;
        }
        last = current;
    }
    
    *graph = Graph(
        connections_array,  //array of connections
        connections_count); //count of elements in the array
    
    free(str);
    free(connections_array);
}

unsigned GetLength(char* str, unsigned limit)
{
    unsigned l = 0;
    for(; str[l] != '\0'; l++)
    {
        if(l > limit)
        {
            Warning("Function", "GetLength", "Length of string is higher then limit. The function return 0.");
            return 0;
        }
    }
    return l;
}

unsigned ConvertToUnsigned(char* str, bool invert)
{
#define EXP_0   1u
#define EXP_1   10u
    
    if(str == nullptr)
    {
        Warning("Function", "ConvertToUnsigned", "Input pointer is null.");
        return UNSIGNED_ERROR_CODE;
    }
    unsigned l = GetLength(str);
    if(l == 0)
    {
        Warning("Function", "ConvertToUnsigned", "Input string have zero length.");
        return UNSIGNED_ERROR_CODE;
    }
    unsigned value = 0;
    unsigned exp = EXP_0;
    if(invert)
    {
        for(; l > 0; l--)
        {
            if(str[l - 1] < '0' || str[l - 1] > '9')
            {
                Warning("Function", "ConvertToUnsigned", "Input string is not number.");
                return UNSIGNED_ERROR_CODE;
            }
            value += (unsigned)(str[l - 1] - '0') * exp;
            
            if((exp % EXP_1) && exp != EXP_0 || value / exp != (unsigned)(str[l - 1] - '0'))
            {
                Warning("Function", "ConvertToUnsigned", "Output value is overflow.");
                return UNSIGNED_ERROR_CODE;
            }
            exp *= EXP_1;
        }
    }
    else
    {
        for(unsigned i = 0; i < l; i++)
        {
            if(str[i] < '0' || str[i] > '9')
            {
                Warning("Function", "ConvertToUnsigned", "Input string is not number.");
                return UNSIGNED_ERROR_CODE;
            }
            value += (unsigned)(str[i] - '0') * exp;
            if((exp % EXP_1) && exp != EXP_0 || value / exp != (unsigned)(str[l - 1] - '0'))
            {
                Warning("Function", "ConvertToUnsigned", "Output value is overflow.");
                return UNSIGNED_ERROR_CODE;
            }
            exp *= EXP_1;
        }
    }
    if(value == UNSIGNED_ERROR_CODE)
    {
        Warning("Function", "ConvertToUnsigned", "Output value is error code.");
        return UNSIGNED_ERROR_CODE;
    }
    return value;
}


void Warning(const char* type, const char* name, const char* text)
{
    std::cout << "WARNING::" << type << "::" << name << "::" << text << std::endl;
}

void Error(const char* type, const char* name, const char* text)
{
    std::cout << "ERROR::" << type << "::" << name << "::" << text << std::endl;
    exit(ERROR);
}

















