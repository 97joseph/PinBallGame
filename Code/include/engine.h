#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "window.h"
#include "graphics.h"
#include "event.h"
#include "physObject.h" //This is for the callback that occurs

class Engine
{
  public:

    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize();
    void Run();
    void Keyboard(eventType);
    unsigned int getDT();
    long long GetCurrentTimeMillis();

   

  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    event m_event;

 		//physics engine
    btDefaultCollisionConfiguration *collConfig;
    btCollisionDispatcher *physDispatcher;
    btBroadphaseInterface *overlapPairCache;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *physWorld;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
};

#endif // ENGINE_H
