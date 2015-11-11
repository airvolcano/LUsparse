#include "InjectNum.h"
#include "Bintree.h"
using namespace volcano;
namespace volcano
{

	int *injectNum(CSRMatrix *csrmatrix)
	{
		int *injectnum = (int *)malloc(sizeof(int)*csrmatrix->m);
		int *axis = (int *)malloc(sizeof(int)*csrmatrix->m);
		for (int i = 0; i < csrmatrix->m; i++)
		{
			for (int j = csrmatrix->rowptr[i]; j < csrmatrix->rowptr[i + 1]; j++)
			{
				if (csrmatrix->colind[j] == i)
				{
					axis[i] = j;
				}
			}
		}
		BinTree<int,float> *bintree = new BinTree<int,float>[csrmatrix->m];
		for (int i = 0; i < csrmatrix->m; i++)
		{
			for (int j = axis[i]+1; j < csrmatrix->rowptr[i + 1]; j++)
			{
				bintree[i].insert(csrmatrix->colind[j],0);
			}
		}
		for (int i = 0; i < csrmatrix->m; i++)
		{
			int *s = bintree[i].data0toArray();
			int num = bintree[i].num;
			for (int j = 0; j < num; j++)
			{
				for (int k = j + 1; k < num; k++)
				{
					bintree[s[j]].insert(s[k],0);
				}
			}
			free(s);
			bintree[i].insert(i,0);
		}
		for (int i = 0; i < csrmatrix->m; i++)
		{
			injectnum[i] = bintree[i].num;
		}
		return injectnum;
	}
}