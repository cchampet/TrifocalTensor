#include <iostream>
#include <iomanip>

#include "MathIO.hpp"
#include "UserInputs.hpp"
#include "Correspondence.hpp"

using namespace std;

void UserInputs::loadImages(const char* path1, const char* path2, const char* path3) {
	m_image1 = IMG_Load(path1);
  m_image2 = IMG_Load(path2);
  m_image3 = IMG_Load(path3);

  if(m_image1 == 0 || m_image2 == 0 || m_image3 == 0)
	 cerr << "error loading images" << endl;
}

void UserInputs::loadLists(const char* path1, const char* path2, const char* path3) {
  kn::loadMatrix(m_list1, path1);
  kn::loadMatrix(m_list2, path2);
  kn::loadMatrix(m_list3, path3);
}

int UserInputs::manageInputs(int argc, char** argv) {
  // just launch application
  if (argc == 1) {
    loadImages("input/image1.jpg", "input/image2.jpg", "input/image3.jpg");
    loadLists("input/list1.list", "input/list2.list", "input/list3.list");
  }
  // user wants to see the help
  else if (argc == 2) {
    const char* tab = "\t";

    cout << "Man of the application." << endl << endl;
    printf ("\033[37;01mNAME\033[00m\n");
    cout << tab << "./bin/trifocal     Application, example of using trifocal tensor." << endl << endl;
    printf ("\033[37;01mSYNOPSIS\033[00m\n");
    cout << tab << "./bin/trifocal [-h] [image1 image2 image3] [list1 list2 list3]" << endl << endl;
    printf ("\033[37;01mDESCRIPTION\033[00m\n");
    cout << tab << "This application is a demo using a trifocal tensor, an important notion of projective geometry." << endl << endl;
    cout << tab; printf ("\033[37;01m-h\033[00m\n");
    cout << tab << tab << "Launch application with no additional features, and display help menu on terminal." << endl << endl;
    cout << tab; printf ("\033[37;01mimage1.jpg image2.jpg image3.jpg\033[00m\n");
    cout << tab << tab << "Launch application with your 3 images. You need to select correspondences by clicked on the images, and after that, type any key on your keyboard to start to use the trifocal tensor." << endl << endl;
    cout << tab; printf ("\033[37;01mimage1.jpg image2.jpg image3.jpg list1.list list2.list list3.list\033[00m\n");
    cout << tab << tab << "Launch application with your 3 images and your 3 lists. You just need to clic 2 points on 2 images, and the trifocal tensor do the job !" << endl << endl;
    cout << endl;
    
    loadImages("input/image1.jpg", "input/image2.jpg", "input/image3.jpg");
    loadLists("input/list1.list", "input/list2.list", "input/list3.list");
  }
  // user notices 3 images
  else if (argc == 4) {
    loadImages(argv[1], argv[2], argv[3]);
  }
  // user notices 3 images and 3 lists of points
  else if (argc == 7) {
    loadImages(argv[1], argv[2], argv[3]);
    loadLists(argv[4], argv[5], argv[6]);
  }
  else {
    cerr << "Bad way to launch application ! See the help by typing -h when you launch the application." << endl;
    return EXIT_FAILURE;
  }
  return 0;
}

void UserInputs::freeImages() {
  SDL_FreeSurface(m_image1); 
  SDL_FreeSurface(m_image2); 
  SDL_FreeSurface(m_image3);
}