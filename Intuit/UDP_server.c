/* Простой пример UDP-сервера для сервиса echo */
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
    int sockfd; // Дескриптор сокета
    int len, quant; // Переменные? хранящие количество символов
    char line[1000]; //Отсылаемая и принимаемая строка
    struct sockaddr_in servaddr, cliaddr; // Структуры для адресов сервера и клиента

    // Заполнение структуры адреса сервера
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Создание UDP-сокета
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);                         // В случае ошибки выводим сообщение на экран и прекращаем работу с кодом 1
        exit(1);
    }
    /* Настраиваем адрес сокета */
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);                        // В случае ошибки выводим сообщение на экран, прерываем соединение и прекращаем работу с кодом 1
        close(sockfd);
        exit(1);
    }
    while (1)
    {
        // Максимальная длина адреса клиента
        len = sizeof(cliaddr);

        // Ожидаем запрос от клиента и читаем его
        if ((quant = recvfrom(sockfd, line, 999, 0, (struct sockaddr*)&cliaddr, &len)) < 0)
        {
            perror(NULL);                       // В случае ошибки выводим сообщение на экран, прерываем соединение и прекращаем работу с кодом 1
            close(sockfd);
            exit(1);
        }
        // Вывод полученного сообщения на экран
        printf("%s\quant", line);

        // Отправка принятого сообщения обратно
        if (sendto(sockfd, line, strlen(line), 0, (struct sockaddr*)&cliaddr, len) < 0)
        {
            perror(NULL);                       // В случае ошибки выводим сообщение на экран, прерываем соединение и прекращаем работу с кодом 1
            close(sockfd);
            exit(1);
        }
    }
    return 0;
}
