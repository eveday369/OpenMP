#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <omp.h>

void setup(int* array, int len)
{
	for (int i = 0; i < len; i++)
	{
		array[i] = (i * 7 - 3224) ^ 20435;
	}
}

void quick_sort_range(int* array, int lower, int upper)
{
	int tmp;
	int mid = (upper + lower) / 2;
	int pivot = array[mid];
	int tlower = lower;
	int tupper = upper;

	while (tlower <= tupper)
	{
		while (array[tlower] < pivot)
		{
			tlower++;
		}
		while (array[tupper] > pivot)
		{
			tupper--;
		}
		if (tlower <= tupper)
		{
			tmp = array[tlower];
			array[tlower] = array[tupper];
			array[tupper] = tmp;
			tupper--;
			tlower++;
		}
	}

	#pragma omp task shared(array) firstprivate(lower, tupper)
	if (lower < tupper)
	{
		quick_sort_range(array, lower, tupper);
	}
	#pragma omp task shared(array) firstprivate(tlower, upper)
	if (tlower < upper)
	{
		quick_sort_range(array, tlower, upper);
	}
}

void quick_sort(int* array, int elements)
{
	#pragma omp parallel
	{
		#pragma omp single nowait
		quick_sort_range(array, 0, elements);
	}
}

int _tmain(int argc, TCHAR* argv[])
{
	int size = 10 * 1024 * 1024;
	int* array = (int*)malloc(sizeof(int) * size);
	setup(array, size);
	quick_sort(array, size - 1);
	return 0;
}