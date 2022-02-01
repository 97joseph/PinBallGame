#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "Camera.h"
#include "shader.h"
#include "object.h"
#include "world.h"
#include "gui.h"
#include "event.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, SDL_Window *window);
    void Update(unsigned int dt);
    void Render();

  private:
    std::string ErrorString(GLenum error);
    void RenderList(vector<Object*>);
    void TreeRender(Object*);
    bool InitShader(Shader*&, string, string);
    void addRenderTarget(Shader *shader, GLuint texTarget);
    void generateFrameBuffer(GLuint &fbo, GLuint &fbTarget, int width, int height);
    void updateFPS(unsigned int dt);

    GUI ui;

    camera *m_camera;
    Shader *m_shader;
    Shader *m_vshader;
    Shader *selected;
    Shader *m_screenShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLuint screen;

    GLuint fbo;
    GLuint fbTex;

    World *world;
    Object *renderTarget;
    event listener;

    int width;
    int height;
    int frameCount;
    float fps;
    unsigned int delay;

    float vertices[24] = {
    	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
    	     1.0f,  1.0f, 1.0f, 1.0f, // Top-right
    	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right

    	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
    	    -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
    	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
        };
};

#endif /* GRAPHICS_H */
