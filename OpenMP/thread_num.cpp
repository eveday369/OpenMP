#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <omp.h>

int _tmain(int argc, TCHAR* argv[])
{
	int* ready = (int*)calloc(sizeof(int), 2);
	
	#pragma omp parallel num_threads(2)
	{
		_tprintf_s(_T("Thread %d is ready\n"), omp_get_thread_num());
		ready[omp_get_thread_num()] = 1;
		int beighbour = (omp_get_thread_num() + 1) % omp_get_num_threads();

		while (ready[beighbour] == 0)
		{
			#pragma omp flush
		}

		_tprintf_s(_T("Thread %d is done\n"), omp_get_thread_num());
	}

	free(ready);
	return 0;
}