#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "loader.h"
#include "obj.h"
#include "event.h"

#define SPEED_STEP 		0.02f

//support for instanced textures and models
struct ModelInstance {
	GLuint VB;
	GLuint IB;
	unsigned int size;
	string name;
	ModelInstance(GLuint v, GLuint i, unsigned int s, string n): VB(v), IB(i), size(s), name(n){}
	ModelInstance(): VB(0), IB(0), size(0), name("NULL"){}
};

struct TexInstance {
	GLuint texture;
	string name;
	TexInstance(GLuint m, string n): texture(m), name(n){}
	TexInstance(): texture(0), name("NULL"){};
};

class Object
{
  public:
    Object();
    virtual ~Object();

    static void init();
    static void end();

    bool loadModel(string filename);
    bool loadTexture(string filename);
    void loadTexture(string filename, int index);
    void loadNormal(string filename);

    virtual void Update(unsigned int dt);
    virtual void Begin();
    virtual void Render();
    void setVisual(string, string, string);
    void setMultiplier(float);
    void setTex(Texture);
    void addChild(Object*);
    void translate(glm::vec3);
    void rotate(float angle, glm::vec3 axis);
    void scale(float amount);

    float getSize();
    glm::mat4 GetModel();
    std::vector<Object*> getChildren();
    GLuint bindTex(GLuint&, GLenum);

  protected:
    static string rootDir;
    glm::mat4 model;
    glm::mat4 mtranslate;
    glm::mat4 mscale;
    glm::mat4 mrotate;
    std::vector<Object*> children;
    event listener;

    float multiplier;
    float size;

    virtual void keyboard(eventType);

  private:
    static vector<ModelInstance> modelBank;
    static vector<TexInstance> textureBank;

    TexInstance pushTexture(string, GLenum);
    ModelInstance pushModel(string);

    std::vector<unsigned int> texIndex;
    std::vector<GLuint> texPointer;

    ModelInstance modelData;
    TexInstance textureData;
    TexInstance normal;
};

#endif /* OBJECT_H */
