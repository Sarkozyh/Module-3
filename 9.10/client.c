#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int msqid;
	char pathname[] = "server.c";
	key_t key;
    	int pid = getpid();
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
		printf("Error! Unable to get msqid!\n");
		exit(-1);
	}
    	mybuf.mtype = 1;
	mybuf.msg = pid;
	if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(int), 0) < 0)
    	{
		printf("Error! Unable to send message!\n");
		exit(-1);
	}
	printf("Message send, PID = %d, ", pid);
	if ((msgrcv(msqid, (struct msgbuf *) &mybuf, 0, pid, 0) < 0))
    	{
		printf("Error! unable to get message!\n");
		exit(-1);
	}
	printf("Incoming message\n");
	return 0;
}
