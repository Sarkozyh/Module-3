#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int msqid;
	char pathname[] = "server.c";
	key_t key;
    	int len;
	long i;
	struct mymsgbuf
	{
		long mtype;
		int msg;
	} mybuf;
	if ((key = ftok(pathname, 0)) < 0)
    	{
		printf("Error! Unable to generate the key!\n");
		exit(-1);
	}
	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    	{
		printf("Error! Cannot get msqid!\n");
		exit(-1);
	}
	while (1)
    	{
		if (len = (msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(int), 1, 0) < 0))
        	{
			printf("Error! unable to get message!\n");
			exit(-1);
		}
        	printf("Incoming message from PID = %d, ", mybuf.msg);
		for (i = 0; i < 500000000L; i++);
		printf("Message send\n");
        	mybuf.mtype = mybuf.msg;
	    	if (msgsnd(msqid, (struct msgbuf *) &mybuf, 0, 0) < 0)
        	{
	        	printf("Error! Unable to send message!\n");
	        	exit(-1);
	    	}
	}
	return 0;
}
