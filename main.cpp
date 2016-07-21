/* Веб-сервер который реализует протокл HTTP 1.0 обработку GET запросов, ответы 200, 404
 * а также MIME тип text/html
 * Сборка CMAKE v2.8
 * + Команда запуска /home/box/final/final -h <ip> -p <port> -d <directory>
 * + Парсинг команды запуска - с помощью GetOpt
 * После запуска сервер должен врнуть управление (демонизироваться)
 * Сервер должен быть многопоточным многопроцессным
 *
 * + Используем за основу сервер epoll with set_nonblock()
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "settings.h"
#include "server.h"

int main (int argc, char **argv)
{

	Settings settings;
	settings.readSettingsFromArguments(argc, argv);
	
	printf ("hvalue = %s, pvalue = %d, hvalue = %s\n", settings.addr, settings.port, settings.dir);
	
	pid_t pid, sid;

	pid = fork();
	if(pid < 0) {
		exit(EXIT_FAILURE);
	}

	if(pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir(settings.dir)) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);


	run_server(settings.addr, settings.port, settings.dir);

	exit(EXIT_SUCCESS);
}

