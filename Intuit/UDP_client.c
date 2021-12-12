/* Простой пример UDP клиента для сервиса echo */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int sockfd; // Дескриптор сокета
    int quant, len; // Переменные, хранящие количество символов
    char sendline[1000]; // Отсылаемая строка
    char recvline[1000]; // Принимаемая строка
    struct sockaddr_in servaddr, cliaddr; // Структуры для адресов сервера и клиента

    // Если нет второго аргумента
    if (argc != 2)
    {
        printf("Использование: a.out <IP-адрес>\quant");        // Выводим сообщение об ошибки и прекращаем работу с кодом 1
        exit(1);
    }

   // Создание UDP-сокета
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);                                          // В случае ошибки выводим сообщение и прекращаем работу с кодом 1
        exit(1);
    }

    // Заполнение структуры адреса клиента
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Настройка сокета
    if (bind(sockfd, (struct sockaddr*)&cliaddr,
        sizeof(cliaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);                                          // По окончании работы выводим сообщение и закрываем дескриптор сокета
        exit(1);
    }
    /* Заполняем структуру для адреса сервера:
семейство протоколов TCP/IP, сетевой интерфейс – из аргумента
командной строки, номер порта 7. Поскольку в
структуре содержится дополнительное не нужное нам
поле, которое должно быть нулевым, перед заполнением
обнуляем ее всю */

    // Заполение структуры адреса сервера
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        printf("Неверный IP-адрес\quant");
        close(sockfd);                                           // По окончании работы выводим сообщение и закрываем дескриптор сокета
        exit(1);
    }


   // Вывод структуры, отсылаемой серверу
    printf("Строка: ");
    fgets(sendline, 1000, stdin);
    // Отсылка датаграммы
    if (sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }


    // Ожидание ответа и принятие сообщения
    if ((quant = recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr*)NULL, NULL)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }


    printf("%s\quant", recvline);                           // Вывод принятого сообщения на экран
    close(sockfd);                                          // Разрыв соединения
    return 0;
}
