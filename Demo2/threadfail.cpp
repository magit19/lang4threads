#include <iostream>
#include <clocale>
#include <windows.h>

using namespace std;
const unsigned N = 10000000;
const unsigned NUM_THREADS = 2;
unsigned long sum = 0;

unsigned long WINAPI worker(void* start)
{
	for (int j = 0; j < N; j++){
		sum++;
	}
	return 0;
}

int main(){
	int i;
	
	setlocale(LC_ALL, "ru");

	HANDLE thread[NUM_THREADS];
	for(i = 0; i < NUM_THREADS; i++){
		thread[i] = CreateThread(NULL, 0, worker, NULL, 0, NULL); 
	}
	WaitForMultipleObjects(NUM_THREADS, thread, true, INFINITE);
	cout << "ÏÎÒÎÊÎÂ: " << NUM_THREADS << ", ñêëàäûâàþò ïî " << N << ", sum = " << sum << "\n";

	return 0;
}

