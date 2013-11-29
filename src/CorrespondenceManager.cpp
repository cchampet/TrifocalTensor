#include "CorrespondenceManager.hpp"

using namespace std;

CorrespondenceManager::CorrespondenceManager() {
	m_onP1 = true;
	m_onP2 = false;
	m_setP1andP2 = false;
	m_clicImg1 = false;
	m_clicImg2 = false;
	m_clicImg3 = false;
}

void CorrespondenceManager::manageCorrespondences(DisplayManager& displayManager, UserInputs& inputs, TrifocalTensor& tensor) {
	Correspondence c;

	bool done = false;
	while(!done) {

		displayManager.drawCorrespondence(c, inputs);

		while(SDL_PollEvent(&m_event)) {

		  switch(m_event.type) {

		    // leave app
		    case SDL_QUIT :
		        done = true;
		        break;

		    // clic mouse
		    case SDL_MOUSEBUTTONDOWN :
		      // doesn't matter if it's a right or a left clic
		      if (m_onP1) {
		      	// case where the first point clicked is on the first img
	            if (m_event.button.x >= 0 && m_event.button.x < inputs.m_image1->w){
	              c.m_p1[0] = m_event.button.x;
	              c.m_p1[1] = m_event.button.y;
	              c.m_p1[2] = 1.0;
	              m_clicImg1 = true;

		          m_onP1 = false;
		          m_onP2 = true;
	            }
	            // case where the first point clicked is on the second img
	            else if (m_event.button.x > inputs.m_image1->w && m_event.button.x < inputs.m_image1->w + inputs.m_image2->w){
	              c.m_p2[0] = m_event.button.x - inputs.m_image1->w;
	              c.m_p2[1] = m_event.button.y;
	              c.m_p2[2] = 1.0;
	              m_clicImg2 = true;

		          m_onP1 = false;
		          m_onP2 = true;
	            }
	            // case where the first point clicked is on the third img
	            else if(m_event.button.x > inputs.m_image1->w + inputs.m_image2->w && m_event.button.x < inputs.m_image1->w + inputs.m_image2->w + inputs.m_image3->w){
	              c.m_p3[0] = m_event.button.x - inputs.m_image1->w - inputs.m_image2->w;
	              c.m_p3[1] = m_event.button.y;
	              c.m_p3[2] = 1.0;
	              m_clicImg3 = true;

		          m_onP1 = false;
		          m_onP2 = true;
	            }
		      }
		      else if (m_onP2) {
				// case where the second point clicked is on the first img
	            if (m_event.button.x >= 0 && m_event.button.x < inputs.m_image1->w && !m_clicImg1){
	              c.m_p1[0] = m_event.button.x;
	              c.m_p1[1] = m_event.button.y;
	              c.m_p1[2] = 1.0;
	              m_clicImg1 = true;

	              m_onP2 = false;
		          m_onP1 = true;
		          m_setP1andP2 = true;
	            }
	            // case where the second point clicked is on the second img
	            else if (m_event.button.x > inputs.m_image1->w && m_event.button.x < inputs.m_image1->w + inputs.m_image2->w && !m_clicImg2){
	              c.m_p2[0] = m_event.button.x - inputs.m_image1->w;
	              c.m_p2[1] = m_event.button.y;
	              c.m_p2[2] = 1.0;
	              m_clicImg2 = true;

	              m_onP2 = false;
		          m_onP1 = true;
		          m_setP1andP2 = true;
	            }
	            //case where the second point clicked is on the third img
	            else if(m_event.button.x > inputs.m_image1->w + inputs.m_image2->w && m_event.button.x < inputs.m_image1->w + inputs.m_image2->w + inputs.m_image3->w && !m_clicImg3){
	              c.m_p3[0] = m_event.button.x - inputs.m_image1->w - inputs.m_image2->w;
	              c.m_p3[1] = m_event.button.y;
	              c.m_p3[2] = 1.0;
	              m_clicImg3 = true;
	              
	              m_onP2 = false;
		          m_onP1 = true;
		          m_setP1andP2 = true;
	            }
		      }
		      break;

		    default :
		      break;
		  }
		}
		if (m_setP1andP2) {
			// According to the images clicked, tensor and transfert changed
	        if(m_clicImg1 && m_clicImg2){
	          tensor.computeTensor(inputs.m_list1, inputs.m_list2, inputs.m_list3);
	          tensor.transfert(c.m_p1, c.m_p2, c.m_p3);
	        }
	        else if(m_clicImg1 && m_clicImg3){
	          tensor.computeTensor(inputs.m_list1, inputs.m_list3, inputs.m_list2);
	          tensor.transfert(c.m_p1, c.m_p3, c.m_p2);
	        }
	        else if (m_clicImg2 && m_clicImg3){
	          tensor.computeTensor(inputs.m_list2, inputs.m_list3, inputs.m_list1);
	          tensor.transfert(c.m_p2, c.m_p3, c.m_p1);
	        }
	        // reinitialisation of the flags to allow to clic many points
	        m_clicImg1 = false;
	        m_clicImg2 = false;
	        m_clicImg3 = false;
			m_setP1andP2 = false;
		}
	}
}
