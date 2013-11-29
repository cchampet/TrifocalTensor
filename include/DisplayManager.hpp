#ifndef _DISPLAY_MANAGER_
#define _DISPLAY_MANAGER_

#include <SDL/SDL.h>

#include "UserInputs.hpp"
#include "Correspondence.hpp"

// manage the display
struct DisplayManager {
	SDL_Surface* m_screen;
	SDL_Rect m_imageOffset;

	// color of the points drawing on the 3 images
	Uint32 m_color1;
	Uint32 m_color2;
	Uint32 m_color3;

	DisplayManager();

	int initSDLImage();
	int initScreenSurface();

	void createScreenSurface(UserInputs& inputs);

	void drawImages(UserInputs& inputs);
	void drawPointsFromLists(UserInputs& inputs);
	void drawCorrespondence(Correspondence& c, UserInputs& inputs);

	// create the lists and write it in files
	void createLists(UserInputs& inputs);
};


#endif