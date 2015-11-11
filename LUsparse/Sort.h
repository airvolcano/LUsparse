#ifndef __volcano_Sort_H__
#define	__volcano_Sort_H__
#include<stdlib.h>

namespace volcano
{
	void buckets_sort_v2(int *data, int n);
	void buckets_sort_v2(int *data, float *data1, int n);
	int comp(const void*a, const void*b);
	void Qsort(int *a, int low, int high);
	void Qsort(int *data1, float *data2, int low, int high);
	int* randomN(int n, int min, int max);
}

#endif