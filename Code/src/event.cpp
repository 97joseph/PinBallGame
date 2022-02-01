//

#include "event.h"

vector<eventType> event::eventQueue;
btDiscreteDynamicsWorld *event::physWorld = NULL;

event::event()
{

}

event::~event()
{

}

void event::pushEvent(Uint32 type, Uint8 mButton, SDL_Keycode key, Sint32 mousex, Sint32 mousey)
{
	eventType temp;
	temp.eventVer = type;
	temp.mButton = mButton;
	temp.key = key;
	temp.x = mousex;
	temp.y = mousey;

	//add to queue
	eventQueue.push_back(temp);
}

//Only the engine is allowed to update the event broadcaster
//Other objects calling update may cause unknown behavior

void event::update()
{
	eventQueue.clear();
	while (SDL_PollEvent(&m_event) != 0)
	{
		if(m_event.type == SDL_QUIT)
		{
			pushEvent(m_event.type, (Uint8) 0, SDLK_0, 0,0);
		}
		else if (m_event.type == SDL_KEYDOWN)
		{
			pushEvent(m_event.type, (Uint8) 0, m_event.key.keysym.sym, 0,0);
		}
		else if (m_event.type == SDL_KEYUP)
		{
			pushEvent(m_event.type, (Uint8) 0, m_event.key.keysym.sym, 0,0);
		}
		else if (m_event.type == SDL_MOUSEBUTTONDOWN)
		{
			pushEvent(m_event.type, m_event.button.button, SDLK_0, 0,0);
		}
		else if (m_event.type == SDL_MOUSEMOTION)
		{
			pushEvent(m_event.type, 0, 0, m_event.motion.xrel, m_event.motion.yrel);
		}
		else if (m_event.type == SDL_MOUSEWHEEL)
		{
			pushEvent(m_event.type, 0, 0, m_event.wheel.x, m_event.wheel.y);
		}
	}
}

eventType event::getEvent(int index)
{
	return eventQueue[index];
}

int event::getSize()
{
	return eventQueue.size();
}

void event::init(btDiscreteDynamicsWorld* world)
{
	physWorld = world;
}

btDiscreteDynamicsWorld* event::getWorld()
{
	return physWorld;
}
