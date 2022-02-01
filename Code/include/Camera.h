#ifndef CAMERALOC_H
#define CAMERALOC_H

#include "graphics_headers.h"
#include "event.h"
#include "object.h"

class camera
{
  public:
    camera();
    ~camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void SetParent(Object*);
    void SetWorld(Object*);
    void Update(unsigned int);

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 lookAt;
    glm::vec3 position;
    event listener;
    Object *parent;
    Object *world;
    float orbit;
    float xPos;
    float yPos;
    float distance;
    float height;
    float fov;
    int index;
    int w;
    int h;
};

#endif /* CAMERALOC_H */
