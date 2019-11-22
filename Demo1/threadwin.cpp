#include <iostream>
#include <ctime>
#include <clocale>
#include <windows.h>

using namespace std;
const unsigned ARRAY_SIZE = 100*1000*1000;
const unsigned NUM_THREADS = 2;

unsigned long WINAPI worker(void*);

int main(){
	int i, *A = new int[ARRAY_SIZE];
	unsigned long sum, thread_result;
	clock_t t0, t1;

	setlocale(LC_ALL, "ru");

	t0 = clock();
	sum = 0;
	for (i=0; i<ARRAY_SIZE; i++){
		sum += A[i]*A[i];
	}
	t1 = clock();
	cout << "ÏÎÒÎÊÎÂ: 1. sum = " << sum << ", time = " << clock()-t0 << "ms\n";

	
	t0 = clock();
	sum = 0;
	HANDLE thread[NUM_THREADS];
	for(i = 0; i < NUM_THREADS; i++){
		thread[i] = CreateThread(NULL, 0, worker, &A[i*(ARRAY_SIZE / NUM_THREADS)], 0, NULL); 
	}
	WaitForMultipleObjects(NUM_THREADS, thread, true, INFINITE);
	for(i = 0; i < NUM_THREADS; i++){
		GetExitCodeThread(thread[i], &thread_result); 
		sum += thread_result;
	}
	t1 = clock();
	cout << "ÏÎÒÎÊÎÂ: " << NUM_THREADS << ", sum = " << sum << ", time = " << clock()-t0 << "ms\n";

	return 0;
}

unsigned long WINAPI worker(void* start)
{
	unsigned s = 0;
	int* B = (int*) start;
	for (int j = 0; j<ARRAY_SIZE / NUM_THREADS; j++){
		s += B[j]*B[j];
	}
	return s;
}
