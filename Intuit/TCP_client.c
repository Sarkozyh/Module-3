#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
void main(int argc, char **argv)
{
    	int sockfd; // ����������, �������� ���������� ������
    	int quant;// ���������� ���������� ��� ���������� ��������
    	int i; /* ������� ����� */
    	char sendline[1000]; // ���������� ������
    	char recvline[1000]; // ���������� ������
    	struct sockaddr_in servaddr; // ���������-����� �������

    	// ���� ������� ��������� ���
    	if(argc != 2){
        	printf("�������������: a.out <IP-�����>\n");        // �� ������� ��������� �� ������
        	exit(1);                                            // � ���������� ������ � ����� 1
    	}

    	// �������� ������
    	bzero(sendline,1000);
    	bzero(recvline,1000);

    	// ������� �����
    	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	perror(NULL); // � ������ ������ ������� � ����� 1
        	exit(1);
    	}


    	// ���������� ���������-������ �������.
    	bzero(&servaddr, sizeof(servaddr));
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_port = htons(51001);
    	// ��������� IP-�����
    	if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    	{
        	printf("�������� IP-�����\n"); // � ������ ������������ ������� ��������� �� ������
        	close(sockfd);                 // ���������� ����������
        	exit(1);                       // � ������� �� ��������� � ����� 1
    	}
    	/* ������������� ���������� ���������� �����
	��������� ����� � ������� �������, ����� �������� �� �������
	� ��������� */

        //��������� ���������� ����� ������
    	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    	{
        	perror(NULL);
        	close(sockfd);
        	exit(1);
    	}


    	// ���� �� �������� ������ ��� ����
    	for (i = 0; i < 3; i++)
    	{
        	printf("������: ");                                             // ������ �� � ����������
        	fflush(stdin);
        	fgets(sendline, 1000, stdin);
        	// ���������� ������
        	if ((quant = write(sockfd, sendline, strlen(sendline)+1)) < 0)  // � ������ ������ ������� ��������������� ��������� �� ����� � ���������� ������ � ����� 1
        	{
            		perror("�� ���� ������\n");
            		close(sockfd);
            		exit(1);
        	}
        	// �������� ������
        	if ((quant = read(sockfd,recvline, 999)) < 0)
        	{
            		perror("�� ���� ������\n");
            		close(sockfd);
            		exit(1);
        	}
        	printf("%s", recvline);                                         // ������� ���������� ������
    	}
    	// ���������� ����������
    	close(sockfd);
}
