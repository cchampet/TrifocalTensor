#include <iostream>

#include "draw.hpp"
#include "DisplayManager.hpp"

using namespace std;

DisplayManager::DisplayManager() {
	// some colors
	m_color1 = 0xffff0000; // red
	m_color2 = 0xff0000ff; // blue
	m_color3 = 0xffffff00; // yellow
}

int DisplayManager::initSDLImage() {
	// init SDL image
	if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == -1){
		cerr << "error IMG_Init" << endl;
		return EXIT_FAILURE;
	}
	return 0;
}

int DisplayManager::initScreenSurface() {
	// init screen surface
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		cerr << "error SDL_Init" << endl;
		return EXIT_FAILURE;
	}
	return 0;
}

void DisplayManager::createScreenSurface(UserInputs& inputs) {
	m_screen = SDL_SetVideoMode(inputs.m_image1->w + inputs.m_image2->w + inputs.m_image3->w, inputs.m_image1->h, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Trifocal Tensor", NULL);
}

void DisplayManager::drawImages(UserInputs& inputs) {
	m_imageOffset.x = 0;
	m_imageOffset.y = 0;
	SDL_BlitSurface(inputs.m_image1, NULL, m_screen, &m_imageOffset);
	m_imageOffset.x = inputs.m_image1->w;
	SDL_BlitSurface(inputs.m_image2, NULL, m_screen, &m_imageOffset);
	m_imageOffset.x = inputs.m_image1->w + inputs.m_image2->w;
	SDL_BlitSurface(inputs.m_image3, NULL, m_screen, &m_imageOffset);
  // update display (flip buffer)
  SDL_Flip(m_screen);
}

void DisplayManager::drawPointsFromLists(UserInputs& inputs) {
	// draw points on image1
	for(int i=0; i<inputs.m_list1.rows(); ++i)
		fill_circle(m_screen, inputs.m_list1(i,0), inputs.m_list1(i,1), 3, m_color1);

	// draw points on image2
	for(int i=0; i<inputs.m_list2.rows(); ++i)
		fill_circle(m_screen, inputs.m_list2(i,0) + inputs.m_image1->w, inputs.m_list2(i,1), 3, m_color2);

	// draw points on image3
	for(int i=0; i<inputs.m_list3.rows(); ++i)
		fill_circle(m_screen, inputs.m_list3(i,0) + inputs.m_image1->w + inputs.m_image2->w, inputs.m_list3(i,1), 3, m_color3);
  // update display (flip buffer)
	SDL_Flip(m_screen);
}

void DisplayManager::drawCorrespondence(Correspondence& c, UserInputs& inputs) {
  // if p1 is created by a user click
	if (c.m_p1[0] != -1.0 && c.m_p1[1] != -1.0 && c.m_p1[2] != -1.0) {
      fill_circle(m_screen, c.m_p1[0], c.m_p1[1], 3, m_color1);
  }
  // if p2 is created by a user click
  if (c.m_p2[0] != -1.0 && c.m_p2[1] != -1.0 && c.m_p2[2] != -1.0) {
    fill_circle(m_screen, c.m_p2[0] + inputs.m_image1->w, c.m_p2[1], 3, m_color2);
  }
  // if p3 is created by a user click
  if (c.m_p3[0] != -1.0 && c.m_p3[1] != -1.0 && c.m_p3[2] != -1.0) {
    fill_circle(m_screen, c.m_p3[0] + inputs.m_image1->w + inputs.m_image2->w, c.m_p3[1], 3, m_color3);
  }
  // update display (flip buffer)
  SDL_Flip(m_screen);
}

void DisplayManager::createLists(UserInputs& inputs) {
  int nbCorrespondences = 0;

  bool onP1, onP2, onP3;
  onP1 = true;
  onP2 = false;
  onP3 = false;
  bool clicImg1, clicImg2, clicImg3;
  clicImg1 = false;
  clicImg2 = false;
  clicImg3 = false;
  bool setP1_P2_P3 = false;
  bool quit = false;
  bool validationCorrespondence = false;

  Correspondence c;
 
  SDL_Event e;
  while(validationCorrespondence == false && quit == false) {

    drawCorrespondence(c, inputs);

    while(SDL_PollEvent(&e)) {
      switch(e.type) {

        // leave app
        case SDL_QUIT :
            quit = true;
          break;

        case SDL_KEYDOWN :
          if(nbCorrespondences >=7){
            validationCorrespondence = true;
          }
          break;

        // clic mouse
        case SDL_MOUSEBUTTONDOWN :
          // doesn't matter if it's a right or a left clic
          if (onP1) {
            // case where the first point clicked is on the first img
            if (e.button.x >= 0 && e.button.x < inputs.m_image1->w){
              c.m_p1[0] = e.button.x;
              c.m_p1[1] = e.button.y;
              c.m_p1[2] = 1.0;
              clicImg1 = true;

              onP1 = false;
              onP2 = true;
            }
            // case where the first point clicked is on the second img
            else if (e.button.x > inputs.m_image1->w && e.button.x < inputs.m_image1->w + inputs.m_image2->w){
              c.m_p2[0] = e.button.x - inputs.m_image1->w;
              c.m_p2[1] = e.button.y;
              c.m_p2[2] = 1.0;
              clicImg2 = true;

              onP1 = false;
              onP2 = true;
            }
            // case where the first point clicked is on the third img
            else if(e.button.x > inputs.m_image1->w + inputs.m_image2->w && e.button.x < inputs.m_image1->w + inputs.m_image2->w + inputs.m_image3->w){
              c.m_p3[0] = e.button.x - inputs.m_image1->w - inputs.m_image2->w;
              c.m_p3[1] = e.button.y;
              c.m_p3[2] = 1.0;
              clicImg3 = true;

              onP1 = false;
              onP2 = true;
            }
          }
          else if (onP2) {
            // case where the first point clicked is on the first img
            if (e.button.x >= 0 && e.button.x < inputs.m_image1->w && !clicImg1){
              c.m_p1[0] = e.button.x;
              c.m_p1[1] = e.button.y;
              c.m_p1[2] = 1.0;
              clicImg1 = true;

              onP2 = false;
              onP3 = true;
            }
            // case where the first point clicked is on the second img
            else if (e.button.x > inputs.m_image1->w && e.button.x < inputs.m_image1->w + inputs.m_image2->w && !clicImg2){
              c.m_p2[0] = e.button.x - inputs.m_image1->w;
              c.m_p2[1] = e.button.y;
              c.m_p2[2] = 1.0;
              clicImg2 = true;

              onP2 = false;
              onP3 = true;
            }
            // case where the first point clicked is on the third img
            else if(e.button.x > inputs.m_image1->w + inputs.m_image2->w && e.button.x < inputs.m_image1->w + inputs.m_image2->w + inputs.m_image3->w && !clicImg3){
              c.m_p3[0] = e.button.x - inputs.m_image1->w - inputs.m_image2->w;
              c.m_p3[1] = e.button.y;
              c.m_p3[2] = 1.0;
              clicImg3 = true;

              onP2 = false;
              onP3 = true;
            }
          }
          else if (onP3) {
            // case where the first point clicked is on the first img
            if (e.button.x >= 0 && e.button.x < inputs.m_image1->w && !clicImg1){
              c.m_p1[0] = e.button.x;
              c.m_p1[1] = e.button.y;
              c.m_p1[2] = 1.0;
              clicImg1 = true;

              onP3 = false;
              onP1 = true;
              setP1_P2_P3 = true;
            }
            // case where the first point clicked is on the second img
            else if (e.button.x > inputs.m_image1->w && e.button.x < inputs.m_image1->w + inputs.m_image2->w && !clicImg2){
              c.m_p2[0] = e.button.x - inputs.m_image1->w;
              c.m_p2[1] = e.button.y;
              c.m_p2[2] = 1.0;
              clicImg2 = true;

              onP3 = false;
              onP1 = true;
              setP1_P2_P3 = true;
            }
            // case where the first point clicked is on the third img
            else if(e.button.x > inputs.m_image1->w + inputs.m_image2->w && e.button.x < inputs.m_image1->w + inputs.m_image2->w + inputs.m_image3->w && !clicImg3){
              c.m_p3[0] = e.button.x - inputs.m_image1->w - inputs.m_image2->w;
              c.m_p3[1] = e.button.y;
              c.m_p3[2] = 1.0;
              clicImg3 = true;

              onP3 = false;
              onP1 = true;
              setP1_P2_P3 = true;
            }
          }
          break;

        default :
          break;
      }
      if (setP1_P2_P3 && clicImg1 && clicImg2 && clicImg3) {
        c.writeInFile(nbCorrespondences);
        clicImg1 = false;
        clicImg2 = false;
        clicImg3 = false;
        setP1_P2_P3 = false;
        ++nbCorrespondences;
      }
    }
  }
  inputs.loadLists("list1.list", "list2.list", "list3.list");
}