#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <pthread.h>

int main()
{
	key_t key = 1234;
	int (*mat)[10];
	int shmid = shmget(key, sizeof(int[10][10]), IPC_CREAT | 0666);
	mat = shmat(shmid, NULL, 0);
	int matrix1[10][10];
	int matrix2[10][10];
	int matrix[10][10];
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
	//for(int i = 0; i < 4; i++)
	//{
	//	for(int j = 0; j < 6; j++)
	//	{
	//		mat[i * 6 + j] = matrix[i][j];
	//	}
	//}
	//shmdt(mat);
	//shmctl(shmid, IPC_RMID, NULL);
}

