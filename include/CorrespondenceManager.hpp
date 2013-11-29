#ifndef _CORRESPONDENCE_MANAGER_
#define _CORRESPONDENCE_MANAGER_

#include <SDL/SDL.h>

#include "Correspondence.hpp"
#include "TrifocalTensor.hpp"
#include "DisplayManager.hpp"

using namespace std;

// manage the correspondences
struct CorrespondenceManager {
	 // flags to understand what point we are going to create when a clic happened 
	bool m_onP1, m_onP2;
	// flag to launch the transfert 
	bool m_setP1andP2; 
	// flags to know on which img the user clicks 
    bool m_clicImg1, m_clicImg2, m_clicImg3;

	SDL_Event m_event;

	CorrespondenceManager();

	void createCorrespondenceWithTransfert(Correspondence& c, TrifocalTensor& tensor);
	void manageCorrespondences(DisplayManager& displayManager, UserInputs& inputs, TrifocalTensor& tensor);
};


#endif