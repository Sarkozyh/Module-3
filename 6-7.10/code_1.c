#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(){
int *ptr;
int md, fd, flag;
key_t key;
if((key = ftok("./key", 0)) < 0){
printf("%i\n", key);
printf("Error of generate key\n");
exit(-1);
}
if((md = shmget(key, 8192*sizeof(int), 0777|IPC_CREAT|IPC_EXCL)) < 0){
if(errno != EEXIST){
printf("Error of create shared memory\n");
exit(-1);
}
else {
if((md = shmget(key, 8192*sizeof(int), 0)) < 0){
printf("Error of find shared memory\n");
exit(-1);
}
}
}
if((ptr = (int *)shmat(md, NULL, 0)) == (int *)(-1)){
printf("Error attach shared memory\n");
exit(-1);
}
int i;
i = 0;
fd = open ("./code_1.c", O_RDONLY);
while ((flag = read (fd, &ptr[i], 1)) > 0){
i++;
}
ptr[i+1]=EOF;
if (flag < 0){
printf ("Can not read file\n");
exit (1);
}
if(shmdt(ptr) < 0){
printf("Can't detach shared memory\n");
exit(-1);
}
printf("\nWriting of text this programm successfull\n\n");
return 0;
}
