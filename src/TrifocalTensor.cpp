#include <cstdlib>
#include <iostream>

#include "MathIO.hpp"
#include "TrifocalTensor.hpp"

using namespace std;

//constructor
TrifocalTensor::TrifocalTensor() {
	for(size_t k = 0; k<3; ++k) {
		for(size_t i = 0; i<3; ++i) {
			for(size_t j = 0; j<3; ++j) {
				tensorMatrix[i][j][k] = 0;
			}
		}
	}
}

// overloading () for a easy acces to the elements of the tensor
float TrifocalTensor::operator()(size_t row, size_t col, size_t z) {
    return tensorMatrix[row][col][z];
}

// display the tensor, just for debug
void TrifocalTensor::print() const {
	cout << "Display the trifocal tensor : " << endl;
	for(size_t k = 0; k<3; ++k) {
		cout << "For z = " << k << endl;
		for(size_t i = 0; i<3; ++i) {
			for(size_t j = 0; j<3; ++j) {
				cout << tensorMatrix[i][j][k] << " ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}
}

// compute elements of the tensor
void TrifocalTensor::computeTensor(const MatrixXd list1, const MatrixXd list2, const MatrixXd list3) {
	MatrixXd A;
	computeMatrixA(A, list1, list2, list3);
	// resolve equation : A*t = 0 where t is unknown
	VectorXd tensorVector = resolveEquationWithSVD(A, 26);
	fillTensor(tensorVector);
}

// compute elements of the matrix A in the equation A*tensor = 0
void TrifocalTensor::computeMatrixA(MatrixXd& A, const MatrixXd list1, const MatrixXd list2, const MatrixXd list3) {
	size_t nbCorrespondence = list1.rows();
	// fill A by zeros
	A.setZero(4 * nbCorrespondence, 27);
	// for each correspondence (= 3 pixel, 1 per image)
	for(size_t p = 0; p<nbCorrespondence; ++p) {
		// we have 4 equations per correspondence
		for(size_t i = 0; i<2; ++i) {
			for(size_t l = 0; l<2; ++l) {
				// we change 12 (4 * 3) elements per row
				for(size_t k = 0; k<3; ++k) {
					A(4*p + 2*i + l, 9*k + 3*2 + l) = list1(p, k) * list2(p, i) * list3(p, 2);
					A(4*p + 2*i + l, 9*k + 3*i + l) = -list1(p, k) * list2(p, 2) * list3(p, 2);
					A(4*p + 2*i + l, 9*k + 3*2 + 2) = -list1(p, k) * list2(p, i) * list3(p, l);
					A(4*p + 2*i + l, 9*k + 3*i + 2) = list1(p, k) * list2(p, 2) * list3(p, l);
				}
			}
		}
	}
	kn::saveMatrix(A,"debug/A.mat");
}

// resolve equation by finding tensor in A*tensor = 0 excluding the solution t = 0 with SVD decomposition
VectorXd TrifocalTensor::resolveEquationWithSVD(MatrixXd& A, size_t indexOfLastColumnOfV) {
	JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);

	const MatrixXd U = svd.matrixU();
	const VectorXd D_diag = svd.singularValues();
	MatrixXd V = svd.matrixV();
	kn::saveMatrix(U,"debug/U.mat");
	kn::saveMatrix(D_diag,"debug/D_diag.mat");
	kn::saveMatrix(V,"debug/V.mat");

	VectorXd tensorVector;
	tensorVector.Zero(27);
	tensorVector = V.col(indexOfLastColumnOfV);
	kn::saveMatrix(tensorVector,"debug/tensorVector.mat");

	return tensorVector;
}

// fill elements of the tensor with the solution found
void TrifocalTensor::fillTensor(const VectorXd& tensorVector) {
	for(size_t k = 0; k<3; ++k) {
		for(size_t i = 0; i<3; ++i) {
			for(size_t j = 0; j<3; ++j) {
				tensorMatrix[i][j][k] = tensorVector(j + i*3 + k*9);
			}
		}
	}
}


/*////////////////////////////////////////TRANSFERT//////////////////////////////////////////////*/
// compute B matrix and b vector in the equation Bx = b from the coordinates of two pixels
void TrifocalTensor::computeMatrixB(MatrixXd& B, VectorXd& b, const float* p1, const float* p2){
	B.setZero(4, 2);
	b.setZero(4, 1);
	for(size_t i = 0; i<2; ++i) {
		for(size_t l = 0; l<2; ++l) {
			for(size_t k = 0; k<3; ++k) { 
				//fill the 1st and 2nd column of B
				B(2*i+l, l) += p1[k] * (p2[2] * tensorMatrix[i][2][k] - p2[i] * tensorMatrix[2][2][k]);//compute coefficients for x''1 and x''2
				//fill b
				b(2*i+l) += - p1[k] * (p2[i] * tensorMatrix[2][l][k] - p2[2]* tensorMatrix[i][l][k]);

			}
		}
	}
	kn::saveMatrix(B,"debug/B.mat");
}

// compute the coordinates of the point p3 on third img corresponding to p1 and p2 of others images
void TrifocalTensor::transfert(const float* p1 , const float* p2, float* p3) {
	MatrixXd B;
	VectorXd b;
	computeMatrixB(B, b, p1, p2);

	JacobiSVD<MatrixXd> svd(B, ComputeThinU | ComputeThinV);
	VectorXd searchedPoint;
	searchedPoint.Zero(2);
	// returns the solution x in Bx=b where x has two coordinates
	searchedPoint = svd.solve(b); 
	
	p3[0] = searchedPoint[0];
	p3[1] = searchedPoint[1];
	p3[2] = 1;
}

