#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>

#define N               4      	// ����� ���������
#define M               4       	// ����� ������, ������� ���� ������

#define THINKING 0 /* ���������� */
#define HUNGRY 1 /* �������� �������� ����� */
#define EATING 2 /* ��� */

#define LEFT (N+i-1) % N
#define RIGHT (i+1) % N
static char indent[N][N*2+1];   	// ������� ��� ��������
int state[N];
HANDLE mutex = CreateMutex(0,false,NULL);
HANDLE s[N]; /* ������� �� �������� */ 

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
	WaitForSingleObject(s[i],2000); /* ���� ����� �� ���������, �����������*/
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
    	Sleep(rand()% 2000);	//�������� ��� �������
 
    	take_forks(i);
		printf("%s ���%d [ �h��� ��5������\n", indent[i], i, k);
    	k++;
    	printf("%s ���%d ] %d ������ �����\n", indent[i], i, k);
    	put_forks(i);
	}
	printf("%s ���%d �����!\n", indent[i], i);
	return 0;
}

int main(){
	HANDLE phil[N];
 
	int i;
	setlocale(LC_ALL, "Rus");
 
	// ������� ������ ���������, ������������������� ����� � �������
	memset(indent, ' ', N*(2*N+1));
	for (i=0; i<N; i++){
    		//forks[i] = 1;
    		indent[i][i*2]='\0';
	}
 
	// ��������� ������
	for (i=0; i < N; i++) {
			s[i] = CreateSemaphore(0,0,1, NULL); 
    		phil[i] = CreateThread(0, 0, philosopher, (void *) i, 0, NULL);
	}
	
	WaitForMultipleObjects(N, phil, TRUE, INFINITE);
 
	printf("�����!");
	_getch();
}
