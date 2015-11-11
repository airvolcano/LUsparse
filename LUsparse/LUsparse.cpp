#include <iostream>
#include <time.h>
#include "Sort.h"
#include "LUsparse.h"
#include "Bintree.h"
#include "InjectNum.h"
#include "metis.h"
using namespace std;
using namespace volcano;




namespace volcano
{

	void solve_up_matrix(float *val, int *rowptr, int *colind, int N, float *b)
	{
		for (int i = N - 1; i >= 0; i--)
		{
			for (int j = rowptr[i + 1] - 1; j > rowptr[i]; j--)
			{
				b[i] -= val[j] * b[colind[j]];
			}
			b[i] /= val[rowptr[i]];
		}
	}
	void solve_down_matrix(float *val, int *rowptr, int *colind, int N, float *b)//可能需要改成无对角
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = rowptr[i]; j < rowptr[i + 1] - 1; j++)
			{
				b[i] -= val[j] * b[colind[j]];
			}
			b[i] /= val[rowptr[i + 1] - 1];
		}
	}


	int DenseLU(DenseMatrix *A, DenseMatrix *L, DenseMatrix *U)
	{
		if (A->m != A->n)
		{
			cout << "fail" << endl;
			return 1;
		}
		int m = A->m;
		L->m = m;
		L->n = m;
		U->m = m;
		U->n = m;
		L->val = (float **)malloc(sizeof(float *)*m);
		U->val = (float **)malloc(sizeof(float *)*m);
		for (int i = 0; i < m; i++)
		{
			L->val[i] = (float *)malloc(sizeof(float)*m);
			U->val[i] = (float *)malloc(sizeof(float)*m);
		}
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < m; j++)
			{
				L->val[i][j] = 0;
				U->val[i][j] = 0;
			}
		}
		for (int i = 0; i < m; i++)
		{
			L->val[i][i] = 1;
		}
		for (int i = 0; i < m; i++)
		{
			for (int j = i; j < m; j++)
			{
				U->val[i][j] = A->val[i][j];
				for (int k = 0; k < i; k++)
				{
					U->val[i][j] -= L->val[i][k] * U->val[k][j];
				}
			}
			for (int j = i + 1; j < m; j++)
			{
				L->val[j][i] = A->val[j][i];
				for (int k = 0; k < i; k++)
				{
					L->val[j][i] -= L->val[j][k] * U->val[k][i];
				}
				L->val[j][i] /= U->val[i][i];
			}
		}

		return 0;
	}

	int NodeOpt(CSRMatrix *A, int *&perm, int *&iperm)
	{
		perm = (int *)malloc(sizeof(int)*A->m);
		iperm = (int *)malloc(sizeof(int)*A->m);
		int m = A->m;

		int *axis = (int *)malloc(sizeof(int)*m);
		for (int i = 0; i < A->m; i++)
		{
			for (int j = A->rowptr[i]; j < A->rowptr[i + 1]; j++)
			{
				if (A->colind[j] == i)
				{
					axis[i] = j;
				}
			}
			//cout << axis[i] << "\t";
		}


		long t3 = clock();
		BinTree<int, float> *bintreeU = new BinTree<int, float>[m];

		for (int i = 0; i < m; i++)
		{
			for (int j = axis[i] + 1; j < A->rowptr[i + 1]; j++)
			{
				bintreeU[i].insert(A->colind[j], 0);
			}
			for (int j = A->rowptr[i]; j < axis[i]; j < j++)
			{
				bintreeU[i].insert(A->colind[j], 0);
			}
		}

		for (int i = 0; i < m; i++)
		{
			int min_index = 0;

			for (int j = 0; j < m; j++)
			{
				if (bintreeU[j].num > 0)
				{
					min_index = j;
					break;
				}
			}
			for (int j = min_index; j < m; j++)
			{
				if ((bintreeU[j].num < bintreeU[min_index].num) && (bintreeU[j].num>0))
				{
					min_index = j;
				}
			}

			//cout << "\nmin_index:\t" << min_index;
			//cout << endl;
			//for (int j = 0; j < m; j++)
			//{
			//	cout << bintreeU[j].num << "\t";
			//}
			//cout << endl;
			//system("pause");

			int *data0 = bintreeU[min_index].data0toArray();
			for (int j = 0; j < bintreeU[min_index].num; j++)
			{
				for (int k = j + 1; k < bintreeU[min_index].num; k++)
				{
					bintreeU[data0[j]].insert(data0[k], 0);
					bintreeU[data0[k]].insert(data0[j], 0);
				}
			}
			for (int j = 0; j < bintreeU[min_index].num; j++)
			{
				bintreeU[data0[j]].remove(min_index);
			}
			//bintreeU[min_index].root = NULL;
			//bintreeU[min_index].num = 0;
			bintreeU[min_index].del();
			perm[i] = min_index;
			//cout << min_index << "\t";
			free(data0);
		}
		cout << endl;
		for (int i = 0; i < m; i++)
		{
			iperm[perm[i]] = i;
		}
		return 0;
	}
	int NodeOpt(CSRMatrix *&A)
	{
		int *perm, *iperm;
		NodeOpt(A, perm, iperm);
		CSRMatrix *B = new CSRMatrix(A->m, A->n, A->nnz);
		int *nnz_line = (int *)malloc(sizeof(int)*A->m);
		for (int i = 0; i < A->m; i++)
		{
			cout << iperm[i] << "\t";
		}
		system("pause");
		for (int i = 0; i < A->m; i++)
		{
			nnz_line[iperm[i]] = A->rowptr[i + 1] - A->rowptr[i];
		}
		B->rowptr[0] = 0;
		for (int i = 0; i < B->m; i++)
		{
			B->rowptr[i + 1] = B->rowptr[i] + nnz_line[i];
		}
		for (int i = 0; i < A->m; i++)
		{
			for (int j = A->rowptr[i], k = B->rowptr[iperm[i]]; j < A->rowptr[i + 1]; j++, k++)
			{
				B->colind[k] = iperm[A->colind[j]];
				B->val[k] = A->colind[j];
			}
			Qsort(B->colind, B->val, B->rowptr[iperm[i]], B->rowptr[iperm[i] + 1] - 1);
		}
		A = B;
		return 0;
	}

	int MetisOpt(CSRMatrix *&A)
	{
		idx_t *xadj, *adjncy;
		xadj = A->rowptr;
		adjncy = A->colind;
		idx_t *perm;
		idx_t *iperm;
		//idx_t *vwgt;
		perm = (idx_t  *)malloc(sizeof(idx_t)*A->m);
		iperm = (idx_t  *)malloc(sizeof(idx_t)*A->m);

		idx_t options[METIS_NOPTIONS];
		//options[METIS_OPTION_DBGLVL]=METIS_DBG_TIME;
		METIS_SetDefaultOptions(options);
		//options[METIS_OPTION_PTYPE]=METIS_PTYPE_RB;
		//options[METIS_OPTION_PTYPE]=METIS_PTYPE_KWAY;
		options[METIS_OPTION_COMPRESS] = 0;
		options[METIS_OPTION_DBGLVL] = 2;

		METIS_NodeND((idx_t *)(&A->m), xadj, adjncy, NULL, options, perm, iperm);

		/*CSRMatrix *B = new CSRMatrix(A->m, A->n, A->nnz);
		int *nnz_line = (int *)malloc(sizeof(int)*A->m);
		for (int i = 0; i < A->m; i++)
		{
			nnz_line[iperm[i]] = A->rowptr[i + 1] - A->rowptr[i];
		}
		B->rowptr[0] = 0;
		for (int i = 0; i < B->m; i++)
		{
			B->rowptr[i + 1] = B->rowptr[i] + nnz_line[i];
		}
		for (int i = 0; i < A->m; i++)
		{
			for (int j = A->rowptr[i], k = B->rowptr[iperm[i]]; j < A->rowptr[i + 1]; j++, k++)
			{
				B->colind[k] = iperm[A->colind[j]];
				B->val[k] = A->colind[j];
			}
			Qsort(B->colind, B->val, B->rowptr[iperm[i]], B->rowptr[iperm[i] + 1] - 1);
		}
		A = B;*/


		return 0;
	}

	int SparseLU(CSRMatrix *A, CSRMatrix *&L, CSRMatrix *&U)//设A为对称矩阵
	{
		//int *randomn = randomN(A->n, 0, A->n - 1);
		//int *rradomn = (int *)malloc(sizeof(int)*A->n);
		//for (int i = 0; i < A->n; i++)
		//{
		//	rradomn[randomn[i]] = i;
		//}

		//A->showDense(); cout << endl << endl;
		long t1 = clock();//
		if (A->m != A->n)
		{
			cout << "非对称矩阵" << endl;
			return 1;
		}
		int m = A->m;


		int nnzL;

		CSRMatrix *AT = CSRTran(A);
		for (int i = 0; i < A->nnz; i++)
		{
			if (A->colind[i] != AT->colind[i])
			{
				cout << "矩阵不对称" << endl;
				return 3;
			}
		}
		for (int i = 0; i < m + 1; i++)
		{
			if (A->rowptr[i] != AT->rowptr[i])
			{
				cout << "矩阵不对称" << endl;
				return 3;
			};
		}
		long t2 = clock();//

		int *axis = (int *)malloc(sizeof(int)*m);
		for (int i = 0; i < A->m; i++)
		{
			for (int j = A->rowptr[i]; j < A->rowptr[i + 1]; j++)
			{
				if (A->colind[j] == i)
				{
					axis[i] = j;
				}
			}
			//cout << axis[i] << "\t";
		}


		long t3 = clock();
		BinTree<int, float> *bintreeU = new BinTree<int, float>[m];
		BinTree<int, float> *bintreeL = new BinTree<int, float>[m];

		for (int i = 0; i < m; i++)
		{
			for (int j = axis[i]; j < A->rowptr[i + 1]; j++)
			{
				bintreeU[i].insert(A->colind[j], A->val[j]);
			}
			//bintreeU[i].print();
		}
		//cout << endl;

		long t4 = clock();//
		cout << "检查矩阵时间" << t2 - t1 << endl << "插入数据时间" << t4 - t3 << endl;//

		for (int i = 0; i < m; i++)
		{

			int *s0 = bintreeU[i].data0toArray();
			float *s1 = bintreeU[i].data1toArray();
			//for (int k = 0; k < bintreeU[i].num; k++)
			//{
			//	cout << s1[k] << "\t";
			//}
			for (int j = axis[i] + 1; j < AT->rowptr[i + 1]; j++)
			{
				bintreeL[i].insert(AT->colind[j], AT->val[j]);
			}
			bintreeL[i].divide(s1[0]);
			bintreeL[i].insert(i, 1);
			int *y0 = bintreeL[i].data0toArray();
			float *y1 = bintreeL[i].data1toArray();


			/*bintreeU[i].print();*/
			for (int j = 1; j < bintreeU[i].num; j++)
			{
				for (int k = j; k < bintreeU[i].num; k++)
				{

					bintreeU[s0[j]].insert(s0[k], 0 - y1[j] * s1[k]);
				}


			}
			for (int j = 1; j < bintreeU[i].num; j++)
			{
				for (int k = j + 1; k < bintreeU[i].num; k++)
				{
					bintreeL[s0[j]].insert(s0[k], 0 - y1[k] * s1[j]);
				}
			}
			// cout<<"\t\t";
			//bintreeU[i].print();
			//cout << endl;



		}
		long t5 = clock();
		cout << "计算时间" << t5 - t4 << endl;
		U = new CSRMatrix();

		U->nnz = 0;
		U->m = m;
		U->n = m;
		for (int i = 0; i < m; i++)
		{
			U->nnz += bintreeU[i].num;
		}
		U->colind = (int *)malloc(sizeof(int)*U->nnz);
		U->rowptr = (int *)malloc(sizeof(int)*(m + 1));
		U->val = (float *)malloc(sizeof(int)*U->nnz);

		int U_colind_i = 0;
		U->rowptr[0] = 0;
		for (int i = 0; i < m; i++)
		{
			int *linem = bintreeU[i].data0toArray();
			float *valU = bintreeU[i].data1toArray();
			//bintreeU[i].print();
			//cout << endl;
			U->rowptr[i + 1] = U->rowptr[i] + bintreeU[i].num;
			for (int j = 0; j < bintreeU[i].num; j++)
			{
				U->colind[U_colind_i] = linem[j];
				U->val[U_colind_i++] = valU[j];
				//cout << valU[j] << "\t";
			}

		}





		int *L_colind_i = (int *)malloc(sizeof(int)*m);
		for (int i = 0; i < m; i++)
		{
			L_colind_i[i] = 0;
		}
		L = CSRTran_noVal(U);
		for (int i = 0; i < m; i++)
		{
			int *linem = bintreeL[i].data0toArray();
			float *valL = bintreeL[i].data1toArray();
			for (int j = 0; j < bintreeL[i].num; j++)
			{
				//L->colind[L->rowptr[linem[j]] + L_colind_i[linem[j]]] = i;
				L->val[L->rowptr[linem[j]] + L_colind_i[linem[j]]++] = valL[j];
			}

		}


		return 0;
	}






}