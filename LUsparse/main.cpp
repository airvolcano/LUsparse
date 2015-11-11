#include <iostream>
#include <time.h>
#include "Sort.h"
#include "LUsparse.h"
#include "Bintree.h"
#include "Matrix.h"
#include "InjectNum.h"
#include "metis.h"
using namespace std;
using namespace volcano;

int testBtree()
{
	BinTree<int, float> *b = new BinTree<int, float>();
	b->insert(4, (float)2.6);
	b->insert(5, (float)6.3);
	b->insert(4, (float)2.78);
	b->print();
	return 0;
}

int testLU()
{
	long t0 = clock();
	CSRMatrix *c1 = randomSymmetricCSRMatrix(250, 5);

	int inject = 0;
	int *injects = injectNum(c1);
	for (int i = 0; i < c1->m; i++)
	{
		inject += injects[i];
	}
	cout << "\tInjects=" << inject << endl;
	long t1 = clock();
	cout << t1 - t0 << "ms" << endl;
	MetisOpt(c1);


	CSRMatrix *L, *U;
	long t2 = clock();
	cout << "LU:" << t2 - t1 << "ms" << endl;
	SparseLU(c1, L, U);
	long t3 = clock();
	cout << "LU:" << t3 - t2 << "ms" << endl;

	cout << "U->nnz:" << U->nnz;



	return 0;
}

int test1()
{
	srand(clock());
	float *val; int *colind; int *rowptr; float *b;
	int n = 3, nnz = 4;
	b = (float *)malloc(sizeof(float)*n);
	val = (float *)malloc(sizeof(float)*nnz);
	colind = (int *)malloc(sizeof(int)*nnz);
	rowptr = (int *)malloc(sizeof(int)*(n + 1));
	val[0] = 1; val[1] = 2; val[2] = 3; val[3] = 4;// val[4] = 7;
	colind[0] = 0; colind[1] = 1; colind[2] = 0; colind[3] = 2;// colind[4] = 5;
	rowptr[0] = 0; rowptr[1] = 1; rowptr[2] = 2; rowptr[3] = 4; //rowptr[4] = 5;
	b[0] = 1; b[1] = 1; b[2] = 1;





	CSRMatrix *A, *B;
	A = new CSRMatrix(4, 5, 7);
	float *valA = (float *)malloc(sizeof(float) * 7);
	int *colindA = (int *)malloc(sizeof(int) * 7);
	int *rowptrA = (int *)malloc(sizeof(int) * 6);
	valA[0] = 1; valA[1] = 2; valA[2] = 3; valA[3] = 4; valA[4] = 5; valA[5] = 6; valA[6] = 7;
	colindA[0] = 0; colindA[1] = 2; colindA[2] = 1; colindA[3] = 0; colindA[4] = 2; colindA[5] = 3; colindA[6] = 1;
	rowptrA[0] = 0; rowptrA[1] = 2; rowptrA[2] = 3; rowptrA[3] = 5; rowptrA[4] = 6; rowptrA[5] = 7;
	A->colind = colindA; A->rowptr = rowptrA; A->val = valA;

	B = new CSRMatrix(5, 4, 7);
	float *valB = (float *)malloc(sizeof(float) * 7);
	int *colindB = (int *)malloc(sizeof(int) * 7);
	int *rowptrB = (int *)malloc(sizeof(int) * 5);
	valB[0] = 1; valB[1] = 2; valB[2] = 3; valB[3] = 4; valB[4] = 7; valB[5] = 5; valB[6] = 6;
	colindB[0] = 0; colindB[1] = 3; colindB[2] = 1; colindB[3] = 2; colindB[4] = 4; colindB[5] = 1; colindB[6] = 3;
	rowptrB[0] = 0; rowptrB[1] = 2; rowptrB[2] = 3; rowptrB[3] = 5; rowptrB[4] = 7;
	B->colind = colindB; B->rowptr = rowptrB; B->val = valB;
	//CSRMul(A, B)->showDense();


	//CSRMatrix *C = randomCSRMatrix(10, 10, 5);
	//C->showDense();
	//cout << endl;
	//CSRMatrix *D = randomSymmetricCSRMatrix(10, 5);
	//D->showDense();
	//cout << endl;




	DenseMatrix *dA, *dL, *dU;
	dA = new DenseMatrix(4, 4);
	dL = new DenseMatrix();
	dU = new DenseMatrix();
	float valdA[4][4] = { { 4,0,0,2 },{ 0,7,2,0 },{ 0,4,3,6 },{ 7,0,4,9 } };
	float valdB[4][4] = { { 4,2,1,5 },{ 8,7,2,10 },{ 4,8,3,6 },{ 6,8,4,9 } };
	//dA->val = valdA;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dA->val[i][j] = valdA[i][j];
		}
	}
	DenseLU(dA, dL, dU);
	//cout << endl;
	//dA->show();
	//cout << endl;
	//dL->show();
	//cout << endl;
	//dU->show();


	/**************SPARSE_LU_TEST1*************************/
	//float csrValA[10] = { 4,2,7,2,4,3,6,7,4,9 };
	//int csrColindA[10] = { 0,3,1,2,1,2,3,0,2,3 };
	//int csrRowptrA[5] = { 0,2,4,7,10 };
	//CSRMatrix *csrA = new CSRMatrix();
	//csrA->m = 4;
	//csrA->n = 4;
	//csrA->colind = csrColindA;
	//csrA->nnz = 10;
	//csrA->rowptr = csrRowptrA;
	//csrA->val = csrValA;
	//CSRMatrix *csrL, *csrU;

	//SparseLU(csrA, csrL, csrU);
	//csrA->showDense();
	//cout << endl;
	//cout << endl;
	//injectNum(csrA);
	//cout << endl;
	/***************************************************/

	///**************SPARSE_LU_TEST2*************************/
	//float csrValA[16] = { 4,2,1,5,8,7,2,10,4,8,3,6,6,8,4,9 };
	//int csrColindA[16] = { 0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3 };
	//int csrRowptrA[5] = { 0,4,8,12,16 };
	//CSRMatrix *csrA = new CSRMatrix();
	//csrA->m = 4;
	//csrA->n = 4;
	//csrA->colind = csrColindA;
	//csrA->nnz = 16;
	//csrA->rowptr = csrRowptrA;
	//csrA->val = csrValA;
	//CSRMatrix *csrL, *csrU;

	//SparseLU(csrA, csrL, csrU);
	////csrA->showDense();
	////cout << endl;
	////cout << endl;
	////injectNum(csrA);
	////cout << endl;
	///***************************************************/


	/*********************Mul_TEST************************/
	//long t0 = clock();
	//CSRMatrix *c1 = randomSymmetricCSRMatrix(1000, 5);
	//CSRMatrix *c2 = randomSymmetricCSRMatrix(1000, 5);
	//long t1 = clock();
	//cout << t1 - t0 << "ms" << endl;
	//CSRMul(c1, c2);
	//long t2 = clock();

	//cout << t2 - t1 << "ms" << endl;
	/*********************************************************/




	//CSRMatrix *c3 = randomSymmetricCSRMatrix(10, 4);
	//DenseMatrix *c4 = c3->toDenseMatrix();
	//DenseMatrix *c4_1=new DenseMatrix(), *c4_2=new DenseMatrix();
	////c3->showDense();
	//c4->show();
	//cout << endl<<endl;
	//DenseLU(c4,c4_1, c4_2);
	//c4_2->show();
	//
	//cout << endl<<endl;
	//injectNum(c3);

	//system("pause");
	//testBtree();
	//system("pause");
	system("pause");
	return 0;
}

int main()
{
	CSRMatrix *A, *B;
	A = new CSRMatrix(4, 5, 7);
	float *valA = (float *)malloc(sizeof(float) * 7);
	int *colindA = (int *)malloc(sizeof(int) * 7);
	int *rowptrA = (int *)malloc(sizeof(int) * 6);
	valA[0] = 1; valA[1] = 2; valA[2] = 3; valA[3] = 4; valA[4] = 5; valA[5] = 6; valA[6] = 7;
	colindA[0] = 0; colindA[1] = 2; colindA[2] = 1; colindA[3] = 0; colindA[4] = 2; colindA[5] = 3; colindA[6] = 1;
	rowptrA[0] = 0; rowptrA[1] = 2; rowptrA[2] = 3; rowptrA[3] = 5; rowptrA[4] = 6; rowptrA[5] = 7;
	A->colind = colindA; A->rowptr = rowptrA; A->val = valA;

	B = new CSRMatrix(5, 4, 7);
	float *valB = (float *)malloc(sizeof(float) * 7);
	int *colindB = (int *)malloc(sizeof(int) * 7);
	int *rowptrB = (int *)malloc(sizeof(int) * 5);
	valB[0] = 1; valB[1] = 2; valB[2] = 3; valB[3] = 4; valB[4] = 7; valB[5] = 5; valB[6] = 6;
	colindB[0] = 0; colindB[1] = 3; colindB[2] = 1; colindB[3] = 2; colindB[4] = 4; colindB[5] = 1; colindB[6] = 3;
	rowptrB[0] = 0; rowptrB[1] = 2; rowptrB[2] = 3; rowptrB[3] = 5; rowptrB[4] = 7;
	B->colind = colindB; B->rowptr = rowptrB; B->val = valB;
	CSRMul(A, B)->showDense();
	system("pause");
}