#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
    	int sockfd, newsockfd; //Дескрипторы для присоединенного и слушающего сокетов
    	int clientLen; // Размер адреса клиента
    	int quant; // Количество принятых символов
    	char line[1000]; //Переменная, хранящая принятую строку
    	struct sockaddr_in servaddr, cliaddr; // Структуры для размещения полных адресов сервера и клиента
    	// Создание сокета
    	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        	perror(NULL);                                        //В случае ошибки прекращаем работу с кодом 1
        	exit(1);
    	}

    	//Заполнение структуры-адреса сервера
    	bzero(&servaddr, sizeof(servaddr));
    	servaddr.sin_family= AF_INET;
    	servaddr.sin_port= htons(51001);
    	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	//Настройка адреса сокета
    	if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    	{
        	perror(NULL);                                        //В случае ошибки разрываем соединение и прекращаем работу с кодом 1
        	close(sockfd);
        	exit(1);
    	}

    	// Созданный сокет переводится в пассивное состояние с глубиной установленных соединений = 5
    	if (listen(sockfd, 5) < 0)
    	{
        	perror(NULL);                                        //В случае ошибки разрываем соединение и прекращаем работу с кодом 1
        	close(sockfd);
        	exit(1);
    	}

    	/* Основной цикл сервера */
    	while(1)
    	{

        	записываем максимальную длину адреса клиента
        	clientLen = sizeof(cliaddr);
        	//Установка соединения
        	if ((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clientLen)) < 0)
        	{
            		perror(NULL);                                        //В случае ошибки разрываем соединение и прекращаем работу с кодом 1
            		close(sockfd);
            		exit(1);
        	}

        	// Цикл, в которо осуществляется прием информации от клиента, до тех пор, пока клиент не разорвет соединение или не произойдет ошибки
        	while ((quant = read(newsockfd, line, 999)) > 0)
        	{
            		//Отправка принятых данных обратно
            		if ((n = write(newsockfd, line, strlen(line)+1)) < 0)
            		{
                		perror(NULL);                                        //В случае ошибки разрываем соединение и прекращаем работу с кодом 1
                		close(sockfd);
                		close(newsockfd);
                		exit(1);
            		}
        	}
    		// Если произошла ошибка при чтении
    		if (quant < 0)
    		{
            		perror(NULL);                                        //То разрываем соединение и прекращаем работу с кодом 1
            		close(sockfd);
            		close(newsockfd);
            		exit(1);
        	}
        	//Закрытие дескриптора сокета
        	close(newsockfd);
    	}
}
