#ifndef _CORRESPONDENCE_
#define _CORRESPONDENCE_

// represent a correspondence : 3 points corresponding to the same in 3d on 3 differents images 2d
struct Correspondence {
	float m_p1[3], m_p2[3], m_p3[3];

	Correspondence();

	void writeInFile(int nbCorrespondencesAlreadySaved);

	// to debug
	void print() const;
};


#endif