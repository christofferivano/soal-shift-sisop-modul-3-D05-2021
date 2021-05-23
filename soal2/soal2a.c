#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <pthread.h>

int main()
{
	key_t key = 1234;
	//int (*mat)[10];
	int shmid = shmget(key, sizeof(int) * 4 * 6, IPC_CREAT | 0666);
	int *mat = (int *)shmat(shmid, NULL, 0);
	int matrix1[4][3];
	int matrix2[3][6];
	int matrix[4][6];
	printf("Matrix 1 : [4][3]\n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			scanf("%d", &matrix1[i][j]);
		}
	}
	printf("Matrix 2 : [3][6]\n");
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			scanf("%d", &matrix2[i][j]);
		}
	}
	int hasil = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				hasil += matrix1[i][k] * matrix2[k][j];
			}
			matrix[i][j] = hasil;
			hasil = 0;
		}
	}
	printf("Hasil : \n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			printf("[%d]\t", matrix[i][j]);
		}
		printf("\n");
	}
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			//mat[i * 6 + j] = matrix[i][j];
			matrix[i][j] = mat[i * 6 + j];
		}
	}
	printf("Run Program 2b.\n");
	for(int i = 30; i > 0; i--)
	{
		printf("%d...\n", i);
		sleep(1);
	}
	//sleep(10);
	shmdt(mat);
	shmctl(shmid, IPC_RMID, NULL);
}

