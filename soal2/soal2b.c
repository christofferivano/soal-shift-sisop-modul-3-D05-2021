#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>

struct stemp
{
	int temp1;
	int temp2;
};

int faktorial(int x)
{
	for(int i = x; x > 0; i--)
	{
		printf("%d", x);
	}
}

void print(int x, int y)
{
	int flag = 0;
	int bawah = x - y;
	printf("[");
	if(x == 0 || y == 0)
	{
		printf("0");
	}
	else
	{
		for(int i = x; i > 0; i--, x--)
		{
			if(i == bawah)
			{
				break;
			}
			else
			{
				printf("%d", x);
			}
			if(flag != y - 1)
			{
				printf("*");
				flag++;
			}
		}
	}
}

void *func(void *temp)
{
	struct stemp *stemp = (struct stemp *) temp;
	print(stemp->temp1, stemp->temp2);
}

void main()
{
	key_t key = 1234;
	//int (*mat)[10];
	int shmid = shmget(key, sizeof(int[10][10]), IPC_CREAT | 0666);
	int *mat = (int *)shmat(shmid, NULL, 0);
	int matrix[10][10];
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			matrix[i][j] = mat[i * 6 + j];
		}
	}
	printf("Hasil Matrix : \n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			printf("[%d]\t", matrix[i][j]);
		}
		printf("\n");
	}
	int matBaru[10][10];
	printf("Masukkan nilai matrix[4][6] ; \n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			scanf("%d", &matBaru[i][j]);
		}
	}
	int x, y; 
	int flag = 0;
	pthread_t tid[24];
	struct stemp stemp;
	printf("Output Matrix : \n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			if(matrix[i][j] >= matBaru[i][j])
			{
				x = matrix[i][j];
				y = matBaru[i][j];
				stemp.temp1 = x;
				stemp.temp2 = y;
			}
			else if(matBaru[i][j] > matrix[i][j])
			{
				x = matrix[i][j];
				stemp.temp1 = x;
				stemp.temp2 = y;
			}
			else if(matrix[i][j] == 0)
			{
				stemp.temp1 = 0;
				stemp.temp2 = 0;
			}
			pthread_create(&tid[flag], NULL, &func, (void *)&stemp);
			pthread_join(tid[flag], NULL);
			flag++;
		}
		printf("n");
	}
	shmdt(mat);
	shmctl(shmid, IPC_RMID, NULL);
}
