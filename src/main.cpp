#include "DisplayManager.hpp"
#include "UserInputs.hpp"
#include "TrifocalTensor.hpp"
#include "CorrespondenceManager.hpp"

int main(int argc, char *argv[])
{
  DisplayManager displayManager;

  // init SDL and SDL_Image
  displayManager.initSDLImage();
  displayManager.initScreenSurface();

  // manage user inputs
  UserInputs inputs;
  if (EXIT_FAILURE == inputs.manageInputs(argc, argv)) // if problems during construction of user inputs
    return EXIT_FAILURE;

  // create screen surface
  displayManager.createScreenSurface(inputs);
  
  // draw inputs
  displayManager.drawImages(inputs);
  if (argc == 4) // if the user notices 3 images
    displayManager.createLists(inputs);
  displayManager.drawPointsFromLists(inputs); 

  // trifocal tensor
  TrifocalTensor tensor;

  // manage correspondences
  CorrespondenceManager correspondenceManager;
  correspondenceManager.manageCorrespondences(displayManager, inputs, tensor);

  // quit SDL
  inputs.freeImages(); 
  IMG_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
