#pragma once

#include "GraphRealisation.h"
#include "../Classes/Engine.h"

class OpenGL;

class OpenGL
{
protected:
	GLint window_height;
	GLint window_width;
	//width / height
	float window_scale;
	Vec2D cursore_press_position;
	Vec2F cursore_current_position;
	Vec2F cursore_last_position;
    uint8_t flags_clk;
    
public:

    unsigned update_frame;
    
	StaticBuffer point_buffer;
	StaticBuffer segment_buffer;

	//buffers

	//The shader use for draw gravity generators.
	Shader point_shader;
	//The shader use for draw mega lasers.
	Shader segment_shader;

	//shaders
    
    Camera camera;

	//textures

	//The texture stores symbols and signs that will be drawn on the buttons.
	Texture2D symbols_texture;

	//textures

	OpenGL(
        int width,
        int height,
        const char* title,
        GLFWmonitor* monitor,
        GLFWwindow* share,
        GLFWframebuffersizefun Function,
        GLFWwindow** window);

	//Callback functions

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void ProcessInput(GLFWwindow* window);
    
    
    void DrawFrame();

	//Multydraw functions

	//Get data functions

	bool CanDrawFrame(GLFWwindow* window) const;
	float GetScale() const;

	//Get data functions

	//Initialisations functions

	GLFWwindow* CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function);

	void InitBuffers();
	void InitGlad();
	void InitOpenGL();
	void InitShaders();
	void InitTextures();
    
    void DrawObject(const Node* point, bool update_shader = false);
    void DrawObject(const PhysicConnection* connection, bool update_shader = false);
    void DrawObject(const PhysicGraph* ph_graph);
    void DrawObject(Segment segment, bool update_shader = false);
    void DrawObject(const Segment* segment, bool update_shader = false);
    
    ~OpenGL();
    
};




























