#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 7
int A[N][N];
int B[N][N];
int C[N][N];
unsigned int seeds[N][N];
int MAX_ROW_SUM = 0;
pthread_mutex_t lock;

struct timeSeed{
	unsigned int seed1;
	unsigned int seed2;
	int xLoc;
	int yLoc;
};

void *assignNumber(void *args);
void *multiply(void *args);
void *maxRowSum(void *args);
void showMatrix(int[][] Matrix);

int main(int argc, char **argv) {
	pthread_t threads[N][N];
	struct timeSeed seeds[N][N];
	int aThread;
	pthread_mutex_init(&lock, NULL);
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			for(int k = 0; k < 1; k++) {
				if(k) {
					seeds[i][j].seed1 = time(NULL);
				}
				else {
					seeds[i][j].seed2 = time(NULL);
					seeds[i][j].xLoc = i;
					seeds[i][j].yLoc = j;
				}
			}
	
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++) {		
			aThread = pthread_create(&threads[i][j], NULL, &assignNumber, &seeds[i][j]);
			if(aThread != 0) {
				printf("Fail to create thread! Program exit in one second!\n");
				sleep(1);
				exit(1);
			}
		}

	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++) {	
			aThread = pthread_join(threads[i][j], NULL);
			if(aThread != 0) {
				printf("Fail to join thread! Program exit in one second!\n");
				sleep(1);
				exit(1);
			}
		}

	printf("Matrix A:\n");
	showMatrix(A);
	printf("Matrix B:\n");
	showMatrix(B);

	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++) {		
			aThread = pthread_create(&threads[i][j], NULL, &multiply, &seeds[i][j]);
			if(aThread != 0) {
				printf("Fail to create thread! Program exit in one second!\n");
				sleep(1);
				exit(1);
			}
		}

	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++) {	
			aThread = pthread_join(threads[i][j], NULL);
			if(aThread != 0) {
				printf("Fail to join thread! Program exit in one second!\n");
				sleep(1);
				exit(1);
			}
		}

	printf("Matrix C:\n");
	showMatrix(C);

	for(int j = 0; j < N; j++) {		
			aThread = pthread_create(&threads[j][0], NULL, &multiply, &seeds[j][0]);
			if(aThread != 0) {
				printf("Fail to create thread! Program exit in one second!\n");
				sleep(1);
				exit(1);
	}

	for(int j = 0; j < N; j++) {	
			aThread = pthread_join(threads[j][0], NULL);
			if(aThread != 0) {
				printf("Fail to join thread! Program exit in one second!\n");
				sleep(1);
				exit(1);
	}

	printf("MAX_ROW_SUM is: %d\n", MAX_ROW_SUM);
	pthread_mutex_destroy(&lock);
	return 0;	
}

void *assignNumber(void *args) {
	struct timeSeed *seeds = args;
	int i = args.xLoc;
	int j = args.yLoc;
	unsigned int seed1 = args.seed1;
	unsigned int seed2 = args.seed2;
	A[i][j] = rand_r(&seed1) % 10;
	B[i][j] = rand_r(&seed2) % 10;
}

void *multiply(void *args) {
	struct timeSeed *seeds = args;
	int i = args.xLoc;
	int j = args.yLoc;
	C[i][j] = 0;
	for(int x = 0; x < N; x++) {
		C[i][j] += A[i][x] * B[x][j];
	}
	return;
}

void *maxRowSum(void *args) {
	struct timeSeed *seeds = args;
	int i = args.xLoc;
	int j = args.yLoc;
	int sum = 0
	for(int x = 0; x < N; x++) {
		sum += C[i][x];
	}
	pthread_mutex_lock(&lock);
	if(sum >= MAX_ROW_SUM) {
		MAX_ROW_SUM = sum;
	}
	pthread_mutex_unlock(&lock);
	return;
}

void showMatrix(int[][] Matrix){
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			printf("%d ", Matrix[i][j]);
		}
		printf("\n");
	}
}

