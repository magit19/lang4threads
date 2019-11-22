#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>

#define N               4      	// число философов
#define M               4       	// число порций, которые надо съесть

#define THINKING 0 /* размышляет */
#define HUNGRY 1 /* пытается получить вилки */
#define EATING 2 /* ест */

#define LEFT (N+i-1) % N
#define RIGHT (i+1) % N
static char indent[N][N*2+1];   	// пробелы для отступов
int state[N];
HANDLE mutex = CreateMutex(0,false,NULL);
HANDLE s[N]; /* Каждому по семафору */ 

void test(int i) {
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
    	state[i] = EATING;
    	ReleaseSemaphore(s[i],1,0);
	}
}
void take_forks(int i) {
	WaitForSingleObject(&mutex,2000);
	state[i] = HUNGRY; 
      test(i);
	ReleaseMutex(mutex);
	WaitForSingleObject(s[i],2000); /* Если вилок не досталось, блокируется*/
}
void put_forks(int i) {
	WaitForSingleObject(&mutex,2000);
	state[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	ReleaseMutex(&mutex);
}
DWORD WINAPI philosopher(void* arg){
	int i = (int) arg;
	int k = 0;
	state[i] = THINKING;
    srand(time(0) + i*10);


	while (k < M) {
    	Sleep(rand()% 2000);	//подумать как следует
 
    	take_forks(i);
		printf("%s Фил%d [ АhААН СА5АЛААТА\n", indent[i], i, k);
    	k++;
    	printf("%s Фил%d ] %d ПОРЦИЯ СИЭТЭ\n", indent[i], i, k);
    	put_forks(i);
	}
	printf("%s Фил%d ТОТТО!\n", indent[i], i);
	return 0;
}

int main(){
	HANDLE phil[N];
 
	int i;
	setlocale(LC_ALL, "Rus");
 
	// создать номера философов, проинициализировать вилки и отступы
	memset(indent, ' ', N*(2*N+1));
	for (i=0; i<N; i++){
    		//forks[i] = 1;
    		indent[i][i*2]='\0';
	}
 
	// запустить потоки
	for (i=0; i < N; i++) {
			s[i] = CreateSemaphore(0,0,1, NULL); 
    		phil[i] = CreateThread(0, 0, philosopher, (void *) i, 0, NULL);
	}
	
	WaitForMultipleObjects(N, phil, TRUE, INFINITE);
 
	printf("БУТТЭ!");
	_getch();
}
