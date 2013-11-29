#ifndef TENSOR_BASIC__
#define TENSOR_BASIC__

#include <Eigen/Dense>

using namespace Eigen;

// represent the tensor, and enable to compute it
struct TrifocalTensor {
	float tensorMatrix[3][3][3];

	TrifocalTensor();

	float operator()(size_t row, size_t col, size_t z);

	// fonctions to compute the tensor
	void computeTensor(const MatrixXd list1, const MatrixXd list2, const MatrixXd list3);
	void computeMatrixA(MatrixXd& A, const MatrixXd list1, const MatrixXd list2, const MatrixXd list3);
	VectorXd resolveEquationWithSVD(MatrixXd& A, size_t indexOfLastColumnOfV);
	void fillTensor(const VectorXd& t);

	// the transfert
	void computeMatrixB(MatrixXd& B, VectorXd& b, const float* p1, const float* p2);
	void transfert(const float* p1 , const float* p2, float* p3);

	// to debug
	void print() const;
};


#endif

