#ifndef _MATRIX_H
#define _MATRIX_H
#include <iostream>
using namespace std;
namespace volcano
{
	class DenseMatrix
	{
	public:
		float **val;
		int n;
		int m;
		DenseMatrix()
		{
			val = NULL;
			n = 0;
			m = 0;
		}
		DenseMatrix(int m, int n)
		{
			this->m = m;
			this->n = n;
			val = (float **)malloc(sizeof(int *)*this->n);
			for (int i = 0; i < this->n; i++)
			{
				val[i] = (float *)malloc(sizeof(int)*this->m);
			}
		}
		~DenseMatrix()
		{
			if (val != NULL)
			{
				free(this->val);
			}
		}
		/*CSRMatrix* toCSRMatrix()
		{
		int *nnz_line;
		int nnz = 0;
		int *rowptr;
		nnz_line = (int *)malloc(sizeof(int)*n);
		rowptr = (int *)malloc(sizeof(int)*(n + 1));
		rowptr[0] = 0;
		for (int i = 0; i < n; i++)
		{
		nnz_line[i] = 0;
		}
		for (int i = 0; i < n; i++)
		{
		for (int j = 0; j < m; j++)
		{
		if (val[i][j] != 0)
		{
		nnz_line[i]++;
		}
		}
		}
		for (int i = 0; i < n; i++)
		{
		nnz += nnz_line[i];
		rowptr[i + 1] = rowptr[i] + nnz_line[i];
		}
		float *nnz_val;
		int *colind;
		nnz_val = (float *)malloc(sizeof(float)*nnz);
		colind = (int *)malloc(sizeof(int)*nnz);
		int nnz_index = 0;
		for (int i = 0; i < n; i++)
		{
		for (int j = 0; j < m; j++)
		{
		if (val[i][j] != 0)
		{
		nnz_val[nnz_index] = val[i][j];
		colind[nnz_index] = j;
		}
		}
		}
		free(nnz_line);
		CSRMatrix *csrmatirx;
		csrmatirx = new CSRMatrix();
		csrmatirx->m = m;
		csrmatirx->n = n;
		csrmatirx->nnz = nnz;
		csrmatirx->colind = colind;
		csrmatirx->rowptr = rowptr;
		csrmatirx->val = nnz_val;
		return csrmatirx;
		}*/
		void show()
		{
			for (int i = 0; i < n; i++)
			{
				cout << endl;
				for (int j = 0; j < m; j++)
				{
					cout << val[i][j] << "\t";
				}
			}
		}
	};
	class CSRMatrix
	{
	public:
		float *val;
		int *rowptr;
		int *colind;
		int m;
		int n;
		int nnz;
		CSRMatrix()
		{
			val = NULL;
			rowptr = NULL;
			colind = NULL;
			m = 0;
			n = 0;
			nnz = 0;
		}
		CSRMatrix(int m, int n, int nnz)
		{
			this->m = m;
			this->n = n;
			this->nnz = nnz;
			val = (float*)malloc(sizeof(float)*nnz);
			colind = (int *)malloc(sizeof(int)*nnz);
			rowptr = (int *)malloc(sizeof(int)*(n + 1));
		}
		CSRMatrix(int n, int nnz)
		{
			this->m = n;
			this->n = n;
			this->nnz = nnz;
			val = (float*)malloc(sizeof(float)*nnz);
			colind = (int *)malloc(sizeof(int)*nnz);
			rowptr = (int *)malloc(sizeof(int)*(n + 1));
		}
		~CSRMatrix()
		{
			if (val != NULL)
			{
				free(val);
			}
			if (colind != NULL)
			{
				free(colind);
			}
			if (rowptr != NULL)
			{
				free(rowptr);
			}
		}
		void show()
		{
			cout << endl << "m=" << m << "\t" << "n=" << n << "\t" << "nnz=" << nnz;
			cout << endl << "val= ";
			for (int i = 0; i < n; i++)
			{
				cout << "\n";
				for (int j = rowptr[i]; j < rowptr[i + 1]; j++)
				{
					cout << val[j] << "\t";
				}
			}
			cout << endl << endl << "rowptr= ";
			for (int i = 0; i < n + 1; i++)
			{
				cout << rowptr[i] << "\t";
			}
			cout << endl << endl << "colind= ";
			for (int i = 0; i < nnz; i++)
			{
				cout << colind[i] << "\t";
			}
		}
		DenseMatrix *toDenseMatrix()
		{
			DenseMatrix *densematrix;
			densematrix = new DenseMatrix(m, n);
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < m; j++)
				{
					densematrix->val[i][j] = 0;
				}
			}
			for (int i = 0; i < n; i++)
			{
				for (int j = rowptr[i]; j < rowptr[i + 1]; j++)
				{
					densematrix->val[i][colind[j]] = val[j];
				}
			}
			return densematrix;
		}
		void showDense()
		{
			DenseMatrix *densematrix = this->toDenseMatrix();
			densematrix->show();
			free(densematrix);
		}

	};

	CSRMatrix* CSRTran(CSRMatrix *);
	CSRMatrix* CSRTran_noVal(CSRMatrix *);
	CSRMatrix* CSRMul(CSRMatrix *, CSRMatrix *);
	CSRMatrix* randomCSRMatrix(int m, int n, int nnz_per_line);
	CSRMatrix* randomSymmetricCSRMatrix(int m, int nnz_per_line);
	CSRMatrix* randomLapacianMatrix(int grid_size);
}
#endif
