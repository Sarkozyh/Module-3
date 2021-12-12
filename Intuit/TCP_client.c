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
    	int sockfd; // Переменная, хранящая дескриптор сокета
    	int quant;// Количество переданных или полученных символов
    	int i; /* Счетчик цикла */
    	char sendline[1000]; // Отсылаемая строка
    	char recvline[1000]; // Получаемая строка
    	struct sockaddr_in servaddr; // Структура-адрес сервера

    	// Если второго аргумента нет
    	if(argc != 2){
        	printf("Использование: a.out <IP-адрес>\n");        // То выводим сообщение об ошибке
        	exit(1);                                            // И прекращаем работу с кодом 1
    	}

    	// Зануляем строки
    	bzero(sendline,1000);
    	bzero(recvline,1000);

    	// Создаем сокет
    	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	perror(NULL); // В случае ошибки выходим с кодом 1
        	exit(1);
    	}


    	// Заполнение структуры-адреса сервера.
    	bzero(&servaddr, sizeof(servaddr));
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_port = htons(51001);
    	// Проверяем IP-адрес
    	if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    	{
        	printf("Неверный IP-адрес\n"); // В случае несовпадения выводим сообщение об ошибке
        	close(sockfd);                 // Прекращаем соединение
        	exit(1);                       // И выходим из программы с кодом 1
    	}
    	/* Устанавливаем логическое соединение через
	созданный сокет с сокетом сервера, адрес которого мы занесли
	в структуру */

        //Установка соединения через сокеты
    	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    	{
        	perror(NULL);
        	close(sockfd);
        	exit(1);
    	}


    	// Пока не отправим строку три раза
    	for (i = 0; i < 3; i++)
    	{
        	printf("Строка: ");                                             // Вводим ее с клавиатуры
        	fflush(stdin);
        	fgets(sendline, 1000, stdin);
        	// Отправляем строку
        	if ((quant = write(sockfd, sendline, strlen(sendline)+1)) < 0)  // В случае ошибки выводим соответствующее сообщение на экран и прекращаем работу с кодом 1
        	{
            		perror("Не могу писать\n");
            		close(sockfd);
            		exit(1);
        	}
        	// Получаем строку
        	if ((quant = read(sockfd,recvline, 999)) < 0)
        	{
            		perror("Не могу читать\n");
            		close(sockfd);
            		exit(1);
        	}
        	printf("%s", recvline);                                         // Выводим полученную строку
    	}
    	// Завершение соединения
    	close(sockfd);
}
