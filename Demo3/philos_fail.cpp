#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
 
#define N               5     	// ����� ���������
#define M               4      	// ����� ������, ������� ���� ������
 
#define THINKING    	0       	// ����������
#define EATING      	2       	// ���
 
#define LEFT        	(i)     	// ����� ����� �����
#define RIGHT       	(i+1) % N   // ����� ������ �����
 
static int forks[N];            	// �����
static int state[N];            	// ���������
static char otstup[N][N*2+1];   	// ������� ��� ��������
 
void take_forks(int i){
	while (forks[LEFT]==0) {}   	// ��������� ����� �����
	forks[LEFT] = 0;            	// �����
	printf("%s[���%d +���� �����%d\n", otstup[i], i, LEFT);
 
    Sleep(rand()%200);	// ��������� � ����� ������ � ����
 
	while (forks[RIGHT]==0) {}  	// ��������� ������ �����
	forks[RIGHT] = 0;           	// �����
	printf("%s ���%d +���� �����%d\n", otstup[i], i, RIGHT);
        	
	state[i] = EATING;
}
 
void put_forks(int i){
	forks[LEFT] = 1;            	// �������� ����� ����� �� �����
	forks[RIGHT] = 1;           	// �������� ������ ����� �� �����
	printf("%s ���%d -������� �����]\n", otstup[i], i);
	state[i] = THINKING;
}
 
DWORD WINAPI philosopher(void* arg){
	int i = (int) arg;
	int k = 0;
	state[i] = THINKING;
	srand (time(NULL)+i);
	while (k < M) {
		int pause = rand()% 1000;
		printf("%s ���%d ������ %d ��\n", otstup[i], i, pause);
    	Sleep(pause);	//�������� ��� �������

    	take_forks(i);
 
    	k++;
    	printf("%s ���%d ���� %d ������\n", otstup[i], i, k);
 
    	put_forks(i);
	}
	printf("%s ���%d ���!\n", otstup[i], i);
	return 0;
}
 
int main(){
	HANDLE phil[N];
 	int i;

	setlocale(LC_ALL, "Rus");
 
	// ������� ������ ���������, ������������������� ����� � �������
	memset(otstup, ' ', N*(2*N+1));
	for (i=0; i<N; i++){
    	forks[i] = 1;
    	otstup[i][i*2]='\0';
	}
 
	// ��������� ������
	for (i=0; i < N; i++) {
    	phil[i] = CreateThread(0, 0, philosopher, (void *) i, 0, NULL);
	}
	
	WaitForMultipleObjects(N, phil, TRUE, INFINITE);
 
	printf("������!");
	_getch();
}
