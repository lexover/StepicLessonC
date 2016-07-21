#include <iostream>
#include <set>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <cstddef>
#include <sstream>
#include <fstream>
#include <thread>
#define MAX_EVENTS 1024

int set_nonblock(int fd);
void run_server(char *ip, int port, char* dir);
void process_request(int fd, std::string request);
int parse_request(const std::string &request, std::string *result);
inline std::string getPage(const std::string path);
