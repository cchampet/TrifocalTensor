/*
 * Internal Includes
 */
#include "MathIO.hpp"


/*
 * External Includes
 */
#include <cstdlib>

/*
 * Namespace
 */
namespace kn {

     /*
      * \brief read successive empty lines (ie. a line that contains only spaces and tabs) from a std::ifstream.
      * \param matrixFile the input matrix ifstream
      * \return true if an empty line has been read
      */
      bool skipEmptyLines(std::ifstream &dataFile){

	  std::streampos currentPos;
	  std::string data;
	  bool stillEmpty;
	  bool emptyLineRead = false; // true if an empty line has been read

	  do{ // read a line
	    currentPos = dataFile.tellg();
	    std::getline(dataFile,data);

	    // read space or tabs
	    stillEmpty = true;
	    unsigned int i = 0;
	    while(i<data.size()){
	      if(data[i]!=' ' && data[i]!='\t'){
		stillEmpty = false;
		i = data.size();
	      }
	      ++i;
	    }

	  if(stillEmpty) emptyLineRead = true; // at least a line has bee read
	  } while (stillEmpty);

	  // rewind to the begining of the last line read
	  dataFile.seekg(currentPos); // rewind

	  return emptyLineRead;
      }


     /*
      * \brief read successive comment lines (ie. a line that starts with a '#') from a std::ifstream.
      * \param matrixFile the input matrix ifstream
      * \return true if a comment has been read
      */
      bool skipComments(std::ifstream &dataFile){

	  std::streampos currentPos;
	  std::string comments;
	  bool stillComment;
	  bool commentsRead = false; // true if a comment has been read
	  do{
	    // read a comment
	    currentPos = dataFile.tellg();
	    std::getline(dataFile,comments);

	    // read space or tabs
	    unsigned int i = 0;
	    while( (comments[i]==' ' || comments[i]=='\t') &&  i<comments.size()) // man isblank
	      ++i;

	    // detect comment line
	    stillComment = false;
	    if(comments[i]=='#'){
	      stillComment = true;
	      commentsRead = true; // at least a comment has bee read
	    }
	  } while (stillComment);

	  // rewind to the first line after the comments
	  dataFile.seekg(currentPos); // rewind

	  return commentsRead;
      }


     /*
      * \brief read a keyword (ie 'row' or 'column') and the corresponding value (ie 'row 15')
      * \param dataFile : the input data ifstream
      * \param keyword : the keyword to read, before its corresponding value
      * \param value : the value to be read
      * \return true if the keyword and value have been successfully read, else false
      */
      bool readSize(std::ifstream &dataFile, const std::string &keyword, double &value){

	  // remember the curent position
	  std::streampos currentPos = dataFile.tellg();

	  // read a line
 	  std::string data;
	  std::getline(dataFile,data);

	  // read the keyword
	  if(data.compare(0,keyword.size(),keyword) != 0){
	    dataFile.seekg(currentPos);
	    return false;
	  }

	  // read the value
	  data = data.substr(keyword.size()+1,data.size()-(keyword.size()+1));
	  const char *nptr = data.c_str();
	  char *endptr = 0;
	  value = strtod(nptr,&endptr);
	  if(endptr == nptr || endptr == 0){
	    dataFile.seekg(currentPos);
	    return false;
	  }

	  // success
	  return true;
      }


     /*
      * \brief ignore the comments and read the header on a matrix file if specified; the user should not use this function.
      * \param matrixFile the input matrix ifstream
      * \return true if the matrix format is spefifyed, false otherwise
      * \throw MathException invalid format
      */
      bool readMatrixHeader(std::ifstream &matrixFile, unsigned int &row, unsigned int &column){

	  // read comments
	  while(skipComments(matrixFile) || skipEmptyLines(matrixFile));

	  // read row
	  double value = 0.0;
	  if(!readSize(matrixFile,"row",value))
	     return false;
	  row = (unsigned int) value;

	  // read comments
	  while(skipComments(matrixFile) || skipEmptyLines(matrixFile));

	  // read column
		  if(!readSize(matrixFile,"col",value)){
			  std::cerr << "readMatrixHeader : invalid format : 'col' expected" << std::endl;
			  exit(0);
		  }

		  column = (unsigned int) value;

	  // read comments
	  while(skipComments(matrixFile) || skipEmptyLines(matrixFile));

	  return true;
      }

      /*
      * \brief ignore the comments and read the header on a matrix file if specified; the user should not use this function.
      * \param matrixFile the input matrix ifstream
      * \param mat the number of matrix in the file
      * \return true if the matrix format is spefifyed, false otherwise
      * \throw MathException invalid format
      */
      bool readMatrixHeader(std::ifstream &matrixFile, unsigned int &mat){

	  // read comments
	  while(skipComments(matrixFile) || skipEmptyLines(matrixFile));

	  // read mat
	  double value = 0.0;
	  if(!readSize(matrixFile,"mat",value))
	     return false;
	  mat = (unsigned int) value;

	  // read comments
	  while(skipComments(matrixFile) || skipEmptyLines(matrixFile));

	  return true;
      }



  /*
   * End of Namespace
   */
}

