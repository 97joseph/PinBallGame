
#ifndef GUI_H_
#define GUI_H_


#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <iostream>

class GUI {
public:
	GUI(SDL_Window *window);
	GUI();
	~GUI();
	void Render();
	void Update(unsigned int dt);
	void initGUI(SDL_Window *window);

	int GetScore ();
	int GetLives ();

	void ChangeScore (int delta);
	void ChangeLives (int delta);

	static GUI* getInstance (); //Singletons FTW
private:
	SDL_Window *gWindow;
	static GUI* m_instance;


	int lives, score;
	int newLifeCountdown;
};


#endif /* GUI_H_ */
