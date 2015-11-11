#include <vector>
#include <algorithm>
#include <iostream>
#include "Matrix.h"
#include "Sort.h"
using namespace volcano;

namespace volcano
{
	std::vector<int> get_square_grid(int n)
	{
		// allocate square grid with -1 
		std::vector<int> grid(n*n, -1);

		// fill interior index map
		int idx = 0;
		for (int i = 1; i < n - 1; i++)
			for (int j = 1; j < n - 1; j++)
				grid[i*n + j] = idx++;

		return grid;
	}
	int build_lapacian_float(int n, std::vector<float>& values, std::vector<int>& rowInd, std::vector<int>& colInd)
	{
		// get square grid
		std::vector<int> grid = get_square_grid(n);

		// build index set list
		std::vector<int> index_set;
		for (size_t i = 0; i < grid.size(); i++)
			if (grid[i] != -1)
				index_set.push_back(int(i));

		// fill sparse vectors
		for (size_t i = 0; i < index_set.size(); i++)
		{
			// add self
			int self_index = grid[index_set[i]];
			values.push_back(4.0);
			rowInd.push_back(self_index);
			colInd.push_back(self_index);

			// add adjacent sides
			int degree = 1;
			int adj_index[4];
			adj_index[0] = grid[index_set[i] - 1];
			adj_index[1] = grid[index_set[i] + 1];
			adj_index[2] = grid[index_set[i] - n];
			adj_index[3] = grid[index_set[i] + n];
			for (int j = 0; j < 4; j++)
			{
				if (adj_index[j] != -1)
				{
					// values.push_back(-1.0);
					values.push_back(-1.0);
					rowInd.push_back(self_index);
					colInd.push_back(adj_index[j]);
					degree++;
				}
			}

		}

		return int(index_set.size());
	}

	CSRMatrix* CSRTran(CSRMatrix *A)
	{
		int *nnz_line;
		nnz_line = (int *)malloc(sizeof(int)*A->m);
		for (int i = 0; i < A->m; i++)
		{
			nnz_line[i] = 0;
		}
		for (int i = 0; i < A->nnz; i++)
		{
			nnz_line[A->colind[i]]++;
		}
		CSRMatrix *AT;
		AT = new CSRMatrix(A->n, A->m, A->nnz);
		AT->rowptr[0] = 0;
		for (int i = 0; i < AT->n; i++)
		{
			AT->rowptr[i + 1] = AT->rowptr[i] + nnz_line[i];
		}
		free(nnz_line);
		int *AT_line_index;
		AT_line_index = (int *)malloc(sizeof(int)*AT->n);
		for (int i = 0; i < AT->n; i++)
		{
			AT_line_index[i] = 0;
		}
		for (int i = 0; i < A->n; i++)
		{
			for (int j = A->rowptr[i]; j < A->rowptr[i + 1]; j++)
			{
				AT->val[AT->rowptr[A->colind[j]] + AT_line_index[A->colind[j]]] = A->val[j];
				AT->colind[AT->rowptr[A->colind[j]] + AT_line_index[A->colind[j]]] = i;
				AT_line_index[A->colind[j]]++;
			}
		}
		free(AT_line_index);
		return AT;
	}
	CSRMatrix* CSRTran_noVal(CSRMatrix *A)
	{
		int *nnz_line;
		nnz_line = (int *)malloc(sizeof(int)*A->m);
		for (int i = 0; i < A->m; i++)
		{
			nnz_line[i] = 0;
		}
		for (int i = 0; i < A->nnz; i++)
		{
			nnz_line[A->colind[i]]++;
		}
		CSRMatrix *AT;
		AT = new CSRMatrix(A->n, A->m, A->nnz);
		AT->rowptr[0] = 0;
		for (int i = 0; i < AT->n; i++)
		{
			AT->rowptr[i + 1] = AT->rowptr[i] + nnz_line[i];
		}
		free(nnz_line);
		int *AT_line_index;
		AT_line_index = (int *)malloc(sizeof(int)*AT->n);
		for (int i = 0; i < AT->n; i++)
		{
			AT_line_index[i] = 0;
		}
		for (int i = 0; i < A->n; i++)
		{
			for (int j = A->rowptr[i]; j <A->rowptr[i + 1]; j++)
			{
				//AT->val[AT->rowptr[A->colind[j]] + AT_line_index[A->colind[j]]] = A->val[j];
				AT->colind[AT->rowptr[A->colind[j]] + AT_line_index[A->colind[j]]] = i;
				AT_line_index[A->colind[j]]++;
			}
		}
		free(AT_line_index);
		return AT;
	}
	CSRMatrix* CSRMul(CSRMatrix *A, CSRMatrix *B)
	{
		if (A->m != B->n)
		{
			cout << "Matrix Mul Error" << endl;
			return 0;
		}
		CSRMatrix *C;
		C = new CSRMatrix();
		C->n = A->n;
		C->m = B->m;
		C->rowptr = (int *)malloc(sizeof(int)*(C->n + 1));
		C->rowptr[0] = 0;
		CSRMatrix *BT = CSRTran(B);
		//BT->show();
		int *C_nnz_line;
		C_nnz_line = (int *)malloc(sizeof(int)*C->n);
		for (int i = 0; i < C->n; i++)
		{
			C_nnz_line[i] = 0;
		}
		for (int i = 0; i < A->n; i++)
		{
			int A_nnz_line = A->rowptr[i + 1] - A->rowptr[i];
			for (int j = 0; j < BT->n; j++)
			{
				int BT_nnz_line = BT->rowptr[j + 1] - BT->rowptr[j];
				//cout << A_nnz_line + BT_nnz_line << endl;
				////////////////////此处可以多线程
				int *AwithB_colind_line;
				AwithB_colind_line = (int *)malloc(sizeof(int)*(A_nnz_line + BT_nnz_line));
				for (int k = A->rowptr[i]; k < A->rowptr[i + 1]; k++)
				{
					AwithB_colind_line[k - A->rowptr[i]] = A->colind[k];
				}
				for (int k = BT->rowptr[j]; k < BT->rowptr[j + 1]; k++)
				{
					AwithB_colind_line[k - BT->rowptr[j] + A_nnz_line] = BT->colind[k];
				}
				//cout << endl;
				//for (int k = 0; k < A_nnz_line + BT_nnz_line; k++)
				//{
				//	cout << AwithB_colind_line[k] << "\t";
				//}

				qsort(AwithB_colind_line, A_nnz_line + BT_nnz_line, sizeof(int), comp);

				//cout << endl;
				//for (int k = 0; k < A_nnz_line + BT_nnz_line; k++)
				//{
				//	cout << AwithB_colind_line[k] << "\t";
				//}
				//free(AwithB_colind_line);
				bool nnz_flag = false;
				for (int k = 0; k < A_nnz_line + BT_nnz_line - 1; k++)
				{
					if (AwithB_colind_line[k] == AwithB_colind_line[k + 1])
					{
						nnz_flag = true;
						break;
					}
				}
				if (nnz_flag)
				{
					C_nnz_line[i]++;
				}
				free(AwithB_colind_line);
			}
		}
		for (int i = 0; i < C->n; i++)
		{
			C->rowptr[i + 1] = C->rowptr[i] + C_nnz_line[i];
		}
		C->nnz = C->rowptr[C->n];
		C->colind = (int *)malloc(sizeof(int)*C->nnz);
		C->val = (float *)malloc(sizeof(float)*C->nnz);
		for (int i = 0; i < C->nnz; i++)
		{
			C->val[i] = 0;
		}

		int nnz_index = 0;
		for (int i = 0; i < A->n; i++)
		{
			int A_nnz_line = A->rowptr[i + 1] - A->rowptr[i];
			for (int j = 0; j < BT->n; j++)
			{
				int BT_nnz_line = BT->rowptr[j + 1] - BT->rowptr[j];
				int *AwithB_colind_line;
				float *AwithB_nnz_line;
				AwithB_colind_line = (int *)malloc(sizeof(int)*(A_nnz_line + BT_nnz_line));
				AwithB_nnz_line = (float *)malloc(sizeof(float)*(A_nnz_line + BT_nnz_line));
				for (int k = A->rowptr[i]; k < A->rowptr[i + 1]; k++)
				{
					AwithB_colind_line[k - A->rowptr[i]] = A->colind[k];
					AwithB_nnz_line[k - A->rowptr[i]] = A->val[k];
				}
				for (int k = BT->rowptr[j]; k < BT->rowptr[j + 1]; k++)
				{
					AwithB_colind_line[k - BT->rowptr[j] + A_nnz_line] = BT->colind[k];
					AwithB_nnz_line[k - BT->rowptr[j] + A_nnz_line] = BT->val[k];
				}
				buckets_sort_v2(AwithB_colind_line, AwithB_nnz_line, A_nnz_line + BT_nnz_line);
				//qsort(AwithB_colind_line, A_nnz_line + BT_nnz_line, sizeof(int), comp);
				bool nnz_flag = false;
				for (int k = 0; k < A_nnz_line + BT_nnz_line - 1; k++)
				{
					bool fisttime = true;
					if (AwithB_colind_line[k] == AwithB_colind_line[k + 1])
					{
						if (fisttime)
						{
							C->colind[nnz_index] = j;
							nnz_flag = true;//改成只执行一次
							fisttime = false;
						}
						C->val[nnz_index] += AwithB_nnz_line[k] * AwithB_nnz_line[k + 1];
					}
				}
				if (nnz_flag)
				{
					nnz_index++;
				}
				free(AwithB_colind_line);
				free(AwithB_nnz_line);
			}
		}

		//for (int i = 0; i < A->n; i++)
		//{
		//	for (int j = A->rowptr[i]; j < A->rowptr[i + 1]; j++)
		//	{
		//		for (int k = B->rowptr[A->colind[j]];k< B->rowptr[A->colind[j]+1]; k++)
		//		{
		//			
		//		}
		//	}
		//}
		return C;

	}
	CSRMatrix* randomCSRMatrix(int m, int n, int nnz_per_line)
	{
		CSRMatrix *csrmatrix = new CSRMatrix();
		csrmatrix->m = m;
		csrmatrix->n = n;
		csrmatrix->rowptr = (int *)malloc(sizeof(int)*(m + 1));
		csrmatrix->rowptr[0] = 0;
		for (int i = 0; i < m; i++)
		{
			csrmatrix->rowptr[i + 1] = csrmatrix->rowptr[i] + nnz_per_line;
		}
		csrmatrix->nnz = csrmatrix->rowptr[m];
		csrmatrix->colind = (int *)malloc(sizeof(int)*csrmatrix->nnz);
		csrmatrix->val = (float *)malloc(sizeof(float)*csrmatrix->nnz);

		for (int i = 0; i < m; i++)
		{

			int *temp = randomN(csrmatrix->rowptr[i + 1] - 1 - csrmatrix->rowptr[i], 0, n - 2);
			for (int j = 0; j < csrmatrix->rowptr[i + 1] - 1 - csrmatrix->rowptr[i]; j++)
			{
				if (temp[j] >= i)
				{
					temp[j]++;
				}
			}
			for (int j = csrmatrix->rowptr[i]; j < csrmatrix->rowptr[i + 1] - 1; j++)
			{
				csrmatrix->colind[j] = temp[j - csrmatrix->rowptr[i]];
			}
			csrmatrix->colind[csrmatrix->rowptr[i + 1] - 1] = i;
			qsort(&(csrmatrix->colind[csrmatrix->rowptr[i]]), csrmatrix->rowptr[i + 1] - csrmatrix->rowptr[i], sizeof(int), comp);
			for (int j = csrmatrix->rowptr[i]; j < csrmatrix->rowptr[i + 1]; j++)
			{
				csrmatrix->val[j] = (float)(rand() % 2000) / 1000;
			}
			free(temp);
		}
		return csrmatrix;
	}
	CSRMatrix* randomSymmetricCSRMatrix(int m, int nnz_per_line)
	{
		CSRMatrix *csrmatrix0 = randomCSRMatrix(m, m, nnz_per_line);
		CSRMatrix *csrmatrix1 = CSRTran(csrmatrix0);
		int *axis0, *axis1;
		axis0 = (int *)malloc(sizeof(int)*m);
		axis1 = (int *)malloc(sizeof(int)*m);
		for (int i = 0; i < m; i++)
		{
			for (int j = csrmatrix0->rowptr[i]; j < csrmatrix0->rowptr[i + 1]; j++)
			{
				if (csrmatrix0->colind[j] == i)
				{
					axis0[i] = j;
					break;
				}
			}
			for (int j = csrmatrix1->rowptr[i]; j < csrmatrix1->rowptr[i + 1]; j++)
			{
				if (csrmatrix1->colind[j] == i)
				{
					axis1[i] = j;
					break;
				}
			}
		}
		CSRMatrix *csrmatrix = new CSRMatrix();
		csrmatrix->m = m;
		csrmatrix->n = m;
		csrmatrix->rowptr = (int *)malloc(sizeof(int)*(csrmatrix->m + 1));
		csrmatrix->rowptr[0] = 0;

		for (int i = 0; i < csrmatrix->m; i++)
		{
			csrmatrix->rowptr[i + 1] = csrmatrix->rowptr[i] + axis0[i] - csrmatrix0->rowptr[i] + csrmatrix1->rowptr[i + 1] - axis1[i];
			//cout << axis0[i] - csrmatrix0->rowptr[i] <<"\t"<< csrmatrix1->rowptr[i + 1] - axis1[i] <<"\t"<<csrmatrix->rowptr[i + 1] << "\n";
		}

		csrmatrix->nnz = csrmatrix->rowptr[csrmatrix->m];
		csrmatrix->colind = (int *)malloc(sizeof(int)*csrmatrix->nnz);
		csrmatrix->val = (float *)malloc(sizeof(float)*csrmatrix->nnz);
		for (int i = 0, k = 0; i < csrmatrix->m; i++)
		{
			for (int j = csrmatrix0->rowptr[i]; j < axis0[i]; j++)
			{
				csrmatrix->colind[k] = csrmatrix0->colind[j];
				csrmatrix->val[k] = csrmatrix0->val[j];
				k++;
			}
			for (int j = axis1[i]; j < csrmatrix1->rowptr[i + 1]; j++)
			{
				csrmatrix->colind[k] = csrmatrix1->colind[j];
				csrmatrix->val[k] = csrmatrix1->val[j];
				k++;
			}
		}

		free(axis0);
		free(axis1);
		return csrmatrix;
	}
	CSRMatrix* randomLapacianMatrix(int grid_size)
	{
		std::vector<float> values;
		std::vector<int> rowInd;
		std::vector<int> colInd;
		int n = build_lapacian_float(grid_size, values, rowInd, colInd);
		int nnz = int(values.size());
		CSRMatrix *A = new CSRMatrix(n, nnz);
		int *nnz_row = (int *)malloc(sizeof(int)*n);
		for (int i = 0; i < n; i++)
		{
			nnz_row[i] = 0;
		}
		for (int i = 0; i < nnz; i++)
		{
			A->val[i] = values[i];
			A->colind[i] = colInd[i];
			nnz_row[rowInd[i]]++;
		}
		A->rowptr[0] = 0;
		for (int i = 0; i < n; i++)
		{
			A->rowptr[i + 1] = A->rowptr[i] + nnz_row[i];
		}
		return A;
	}

}