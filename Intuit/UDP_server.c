/* ������� ������ UDP-������� ��� ������� echo */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int sockfd; // ���������� ������
    int len, quant; // ����������? �������� ���������� ��������
    char line[1000]; //���������� � ����������� ������
    struct sockaddr_in servaddr, cliaddr; // ��������� ��� ������� ������� � �������

    // ���������� ��������� ������ �������
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // �������� UDP-������
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);                         // � ������ ������ ������� ��������� �� ����� � ���������� ������ � ����� 1
        exit(1);
    }
    /* ����������� ����� ������ */
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);                        // � ������ ������ ������� ��������� �� �����, ��������� ���������� � ���������� ������ � ����� 1
        close(sockfd);
        exit(1);
    }
    while (1)
    {
        // ������������ ����� ������ �������
        len = sizeof(cliaddr);

        // ������� ������ �� ������� � ������ ���
        if ((quant = recvfrom(sockfd, line, 999, 0, (struct sockaddr*)&cliaddr, &len)) < 0)
        {
            perror(NULL);                       // � ������ ������ ������� ��������� �� �����, ��������� ���������� � ���������� ������ � ����� 1
            close(sockfd);
            exit(1);
        }
        // ����� ����������� ��������� �� �����
        printf("%s\quant", line);

        // �������� ��������� ��������� �������
        if (sendto(sockfd, line, strlen(line), 0, (struct sockaddr*)&cliaddr, len) < 0)
        {
            perror(NULL);                       // � ������ ������ ������� ��������� �� �����, ��������� ���������� � ���������� ������ � ����� 1
            close(sockfd);
            exit(1);
        }
    }
    return 0;
}
