#ifndef _USER_INPUT_
#define _USER_INPUT_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <Eigen/Dense>

using namespace Eigen;

// gather user inputs for the application (3 images, and 3 lists)
struct UserInputs {
	SDL_Surface *m_image1, *m_image2, *m_image3;
	MatrixXd m_list1, m_list2, m_list3;

	void loadImages(const char* path1, const char* path2, const char* path3); // load images from files
	void loadLists(const char* path1, const char* path2, const char* path3); // load lists from files
	
	int manageInputs(int argc, char** argv); // read the lign of commands

	void freeImages(); // free images in heap
};


#endif

