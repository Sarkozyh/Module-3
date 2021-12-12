#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
int main()
{
	int *array;
	int shmid, semid;
	int new = 1;
	char pathname[] = "1.c";
	key_t key;
	long i;
	struct sembuf mybuf;
	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Не могу сгенерировать ключ!\n");
		exit(-1);
	}
	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
    	{
        	printf("Не могу получить доступ по ключу\n");
        	exit(-1);
    	}
	if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if (errno != EEXIST)
		{
			printf("Невозможно создать общую память!\n");
			exit(-1);
		}
		else
		{
			if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0)
			{
				printf("Не могу найти общую память!\n");
				exit(-1);
			}
			new = 0;
		}
	}
	if ((array = (int *) shmat(shmid, NULL, 0)) == (int *)(-1))
	{
		printf("Не могу подключить общую память!\n");
		exit(-1);
	}
	if(new)
	{
		array[0] = 1;
		array[1] = 0;
		array[2] = 1;
	}
	else
	{
		if (array[0] + array[1] != array[2])
		{
			mybuf.sem_op = 1;
			mybuf.sem_flg = 0;
			mybuf.sem_num = 0;
			if (semop(semid, &mybuf, 1) < 0)
			{
				printf("Не могу дождаться состояния!\n");
				exit(-1);
			}
		}
		else
		{
			semctl(semid, 0, IPC_RMID, 0);
            		if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
            		{
                		printf("Не может быть полусредним\n");
                		exit(-1);
            		}
            	}
            	array[0] += 1;
		for (i = 0; i < 1000000000L; i++);
            	mybuf.sem_op = 1;
		mybuf.sem_flg = 0;
		mybuf.sem_num = 0;
            	if (semop(semid, &mybuf, 1) < 0)
		{
			printf("Не могу дождаться состояния!\n");
			exit(-1);
		}
            	array[2] += 1;
     	}
     	printf("Программа №1 была создана %d раз(а), программа №2 - %d раз(а), всего - %d раз(а).\n", array[0], array[1], array[2]);
	if (shmdt(array) < 0)
	{
		printf("Не удается отсоединить общую память!\n");
		exit(-1);
	}
	return 0;
}
