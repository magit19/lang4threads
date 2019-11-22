#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
 
#define N               5     	// число философов
#define M               4      	// число порций, которые надо съесть
 
#define THINKING    	0       	// размышляет
#define EATING      	2       	// ест
 
#define LEFT        	(i)     	// номер левой вилки
#define RIGHT       	(i+1) % N   // номер правой вилки
 
static int forks[N];            	// вилки
static int state[N];            	// состояния
static char otstup[N][N*2+1];   	// пробелы для отступов
 
void take_forks(int i){
	while (forks[LEFT]==0) {}   	// подождать левую вилку
	forks[LEFT] = 0;            	// взять
	printf("%s[Фил%d +взял вилку№%d\n", otstup[i], i, LEFT);
 
    Sleep(rand()%200);	// подремать с левой вилкой в руке
 
	while (forks[RIGHT]==0) {}  	// подождать правую вилку
	forks[RIGHT] = 0;           	// взять
	printf("%s Фил%d +взял вилку№%d\n", otstup[i], i, RIGHT);
        	
	state[i] = EATING;
}
 
void put_forks(int i){
	forks[LEFT] = 1;            	// положить левую вилку на место
	forks[RIGHT] = 1;           	// положить правую вилку на место
	printf("%s Фил%d -положил вилки]\n", otstup[i], i);
	state[i] = THINKING;
}
 
DWORD WINAPI philosopher(void* arg){
	int i = (int) arg;
	int k = 0;
	state[i] = THINKING;
	srand (time(NULL)+i);
	while (k < M) {
		int pause = rand()% 1000;
		printf("%s Фил%d думает %d мс\n", otstup[i], i, pause);
    	Sleep(pause);	//подумать как следует

    	take_forks(i);
 
    	k++;
    	printf("%s Фил%d съел %d порций\n", otstup[i], i, k);
 
    	put_forks(i);
	}
	printf("%s Фил%d СЫТ!\n", otstup[i], i);
	return 0;
}
 
int main(){
	HANDLE phil[N];
 	int i;

	setlocale(LC_ALL, "Rus");
 
	// создать номера философов, проинициализировать вилки и отступы
	memset(otstup, ' ', N*(2*N+1));
	for (i=0; i<N; i++){
    	forks[i] = 1;
    	otstup[i][i*2]='\0';
	}
 
	// запустить потоки
	for (i=0; i < N; i++) {
    	phil[i] = CreateThread(0, 0, philosopher, (void *) i, 0, NULL);
	}
	
	WaitForMultipleObjects(N, phil, TRUE, INFINITE);
 
	printf("ГОТОВО!");
	_getch();
}
