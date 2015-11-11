#ifndef LU_SPARSE_H
#define LU_SPARSE_H
#include "Matrix.h"

namespace volcano 
{
	void solve_up_matrix(float *val, int *rowptr, int *colind, int N, float *b);
	void solve_down_matrix(float *val, int *rowptr, int *colind, int N, float *b);
	int DenseLU(DenseMatrix *A, DenseMatrix *L, DenseMatrix *U);
	int NodeOpt(CSRMatrix *A, int *&perm, int *&iperm);
	int NodeOpt(CSRMatrix *&A);
	int MetisOpt(CSRMatrix *&A);
	int SparseLU(CSRMatrix *A, CSRMatrix *&L, CSRMatrix *&U);
}
#endif