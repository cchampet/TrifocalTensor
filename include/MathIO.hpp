/*
 * Anti-doublon
 */
#ifndef __OPENKN_MATH__MATH_IO_HPP__
#define __OPENKN_MATH__MATH_IO_HPP__

/*
 * External Includes
 */
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include <Eigen/Dense>

using namespace Eigen;

/*
 * Namespace
 */
namespace kn {

  /**
   * \cond
   * \brief ignore the comments and read the header on a matrix file if specified; the user should not use this function.
   * \param matrixFile the input matrix ifstream
   * \return true if the matrix format is spefifyed, false otherwise
   * \throw MathException invalid format
   */
  bool readMatrixHeader(std::ifstream &matrixFile, unsigned int &row, unsigned int &column);
  /// \endcond


  /**
   * \cond
   * \brief ignore the comments and read the header on a matrix file if specified; the user should not use this function.
   * \param matrixFile the input matrix ifstream
   * \return true if the matrix format is spefifyed, false otherwise
   * \throw MathException invalid format
   */
  bool readMatrixHeader(std::ifstream &matrixFile, unsigned int &mat);
  /// \endcond

  /**
   * \brief Load a Matrix from a file. The format is : some optional comments begining by '#', a optional header writing "row " and the number of rows, in the next line, "column " and the number of columns, and then, line by line, the matrix content.
   * \param M the Matrix to be loaded
   * \param fileName the name of the file to open
   * \throw MathException matrix size is incorrect / error opening file / invalid format
   */
  template <class EigenMatrix>
  void loadMatrix(EigenMatrix &M, const std::string &fileName){
    // open the file
    std::ifstream matrixFile(fileName.c_str());
    if(!matrixFile.is_open()){
      std::cerr << "error opening file : " << fileName << std::endl;
      exit(0);				
    }

    // read header
    unsigned int row    = 0;
    unsigned int column = 0;
    bool header = readMatrixHeader(matrixFile,row,column);

    // read the data
    if(header) readMatrixFromHeader(M,matrixFile,row,column);
    else readMatrix(M,matrixFile);

    // close
    matrixFile.close();
  }

  /**
   * \cond
   * \brief read the content of a Matrix from the header information (row,col); the user should not use this function.
   * \param M the Matrix to be filled
   * \param matrixFile the input matrix ifstream
   * \param row the expected matrix row number
   * \param column the expected matrix column number
   * \throw MathException incompatible matrix size
   */
  template <class EigenMatrix>
  void readMatrixFromHeader(EigenMatrix &M,
                            std::ifstream &matrixFile,
                            const unsigned int &row,
                            const unsigned int &column) {

    // if the matrix is already build
    if((unsigned int)M.rows() != row || (unsigned int)M.cols() != column)
      M.resize(row,column);
	
    // read the matrix
    for(unsigned int i=0; i<row; ++i)
      for(unsigned int j=0; j<column; ++j)
        matrixFile >> M(i,j);
  }
  /// \endcond


  /**
   * \cond
   * \brief read the content of a Matrix without any size information (row,col); the user should not use this function.
   * \param M the Matrix to be filled
   * \param matrixFile the input matrix ifstream
   * \throw MathException incompatible matrix size / invalid matrix file
   */
  template <class EigenMatrix>
  void readMatrix(EigenMatrix &M, std::ifstream &matrixFile){

    unsigned int row = 0;
    int column = -1; 	// unknown while you didn't read a line

    std::vector<double> content;
    std::string stringContent;

    // first read the matrix in a std::vector to check the consistency of the data
    do{ // for every line
      std::getline(matrixFile,stringContent);
      std::istringstream readContent(stringContent, std::istringstream::in);
      unsigned int index = 0;
      while(!readContent.eof() && stringContent != ""){ // for every element of a line
        double value;
        readContent >> value;
        content.push_back(value);
        index++;
      }

      // remove the eof
      //content.erase(content.end()-1);
      //index--;

      if(column == -1 && index != 0){   // 1st line : find 'column'
        column = index;
        row++;
      }else{
        if(index != 0){ // check empty line or invalid column
          if(column != (int)index  ){
            std::cerr << "readMatrix : invalid matrix file" << std::endl;
            exit(0);
          }else  row++;
        }else{
          //Matrix reading complete (empty line found)
          break;
        }
      }
    }

    while(!matrixFile.eof());

    if(row==0 && column==-1) return;

    // if the matrix is already build
    if((unsigned int)M.rows() != row || (unsigned int)M.cols() != column)
      M.resize(row,column);

    // copy the data
    for(unsigned int i=0; i<row; ++i)
      for(int j=0; j<column; ++j)
        M(i,j) = content[i*column+j];
        //std::copy(content.begin(), content.end(), M.data());
  }
  /// \endcond


  /**
   * \brief Export a Matrix in a file. The format is : some optional comments begining by '#', a optional header writing "row " and the number of rows, in the next line, "column " and the number of columns, and then, line by line, the matrix content.
   * \param M the Matrix to be exported
   * \param fileName the name of the target file
   * \param headerMode if true, write the number of row and colums before the data, else write directly the data
   * \param comments add some comments at the begining of the file, the caracter "#" is automatically added
   * \throw MathException error opening file
   */
  template <class EigenMatrix>
  void saveMatrix(const EigenMatrix &M,
                  const std::string &fileName,
                  const bool headerMode =  false,
                  const std::string &comments = "") {

    // open the file
    std::ofstream matrixFile(fileName.c_str());
    if(!matrixFile.is_open()){
      std::cerr << "error opening file : " << fileName << std::endl;
      exit(0);				
    }

    // write comments
    if(comments != "")
      matrixFile << "# " << comments << std::endl;

    // write header
    if(headerMode)
      matrixFile << "row " << M.rows() << std::endl
                 << "col " << M.cols() << std::endl;

    // additional line 
    if(comments != "" || headerMode)
      matrixFile << std::endl;

    // write matrix content
    for (int i=0; i<M.rows(); ++i){
      for (int j=0; j<M.cols(); ++j)
        matrixFile << M(i,j) << " ";

      matrixFile << std::endl;
    }

    // close file
    matrixFile.close();
  }



  /*
   * End of Namespace
   */
}


/*
 * End of Anti-doublon
 */
#endif



