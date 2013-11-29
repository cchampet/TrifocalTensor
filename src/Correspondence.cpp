#include <iostream>
#include <fstream>

#include "Correspondence.hpp"

using namespace std;

Correspondence::Correspondence() {
  m_p1[0] = -1.0;
  m_p1[1] = -1.0;
  m_p1[2] = -1.0;

  m_p2[0] = -1.0;
  m_p2[1] = -1.0;
  m_p2[2] = -1.0;

  m_p3[0] = -1.0;
  m_p3[1] = -1.0;
  m_p3[2] = -1.0;
}

void Correspondence::writeInFile(int nbCorrespondences) {
  // first correspondence to save : erase all data in possible list files.
  if (nbCorrespondences == 0) {
    ofstream list1("list1.list", ios::out | ios::trunc);
    list1.close();
    ofstream list2("list2.list", ios::out | ios::trunc);
    list2.close();
    ofstream list3("list3.list", ios::out | ios::trunc);
    list3.close();
  }

  ofstream list1("list1.list", ios::out | ios::app);
  if(list1) {
    list1 << m_p1[0] << " " << m_p1[1] << " " << m_p1[2] << " " << endl;
    list1.close();
  }
  else
    cerr << "Error during writing in list1.list" << endl;

  ofstream list2("list2.list", ios::out | ios::app);
  if(list2) {
    list2 << m_p2[0] << " " << m_p2[1] << " " << m_p2[2] << " " << endl;
    list2.close();
  }
  else
    cerr << "Error during writing in list2.list" << endl;

  ofstream list3("list3.list", ios::out | ios::app);
  if(list3) {
    list3 << m_p3[0] << " " << m_p3[1] << " " << m_p3[2] << " " << endl;
    list3.close();
  }
  else
    cerr << "Error during writing in list3.list" << endl;
}

void Correspondence::print() const{
	cout << "[ " << m_p1[0] << ", " << m_p1[1] << ", " << m_p1[2] << " ]" << endl;
	cout << "[ " << m_p2[0] << ", " << m_p2[1] << ", " << m_p2[2] << " ]" << endl;
	cout << "[ " << m_p3[0] << ", " << m_p3[1] << ", " << m_p3[2] << " ]" << endl;
}