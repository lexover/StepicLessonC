#include "server.h"

int set_nonblock(int fd) {
	int flags;
#if defined(O_NONBLOCK)
	if(-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

void run_server(char* ip, int port, char* dir) {

	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(port);
	SockAddr.sin_addr.s_addr = inet_addr(ip);
	if(bind (MasterSocket, (struct sockaddr*) (&SockAddr), sizeof(SockAddr))==-1){
		perror("bind master socket");
		exit(EXIT_FAILURE);
	}
	
	set_nonblock(MasterSocket);

	listen (MasterSocket, SOMAXCONN);
	
	int EPoll = epoll_create1(0);
	if (EPoll == -1) {
		perror("epoll create");
		exit(EXIT_FAILURE);
	}
	
	struct epoll_event Event;
	Event.data.fd = MasterSocket;
	Event.events = EPOLLIN;
	if (epoll_ctl(EPoll, EPOLL_CTL_ADD, MasterSocket, &Event)==-1){
		perror("epoll_ctl: MasterSocket");
		exit(EXIT_FAILURE);
	}
	
	while(true){
		struct epoll_event Events[MAX_EVENTS];
		int N = epoll_wait(EPoll, Events, MAX_EVENTS, -1);
		if(N == -1){
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for (int i=0; i < N; i++){
			//Master SOCKET
			if(Events[i].data.fd == MasterSocket) {
		//		std::cout<<"Mater socket\n";
				int SlaveSocket = accept(MasterSocket, 0, 0);
				if (SlaveSocket == -1){
					perror("accept");
					exit(EXIT_FAILURE);
				}
				set_nonblock(SlaveSocket);
				struct epoll_event Event;
				Event.data.fd = SlaveSocket;
				Event.events = EPOLLIN;
				epoll_ctl(EPoll, EPOLL_CTL_ADD, SlaveSocket, &Event);
			} else {
			//SLAVE SOCKET
		//		std::cout<<"Slave socket\n";
				int fd= Events[i].data.fd;
				static char Buffer[1024];
				int RecvResult = recv(fd, Buffer, 1024, MSG_NOSIGNAL);
				if ((RecvResult == 0) && (errno != EAGAIN)){
					std::cout<<"Try shutdown fd = "<<fd<<std::endl;
					shutdown(fd, SHUT_RDWR);
					close(fd);
			} else if (RecvResult > 0){ 
				std::thread resp(process_request, fd, Buffer);
				resp.detach();
		//		std::cout<<"Thread created"<<std::endl;
		//		if (resp.joinable()) resp.join();
//				process_request(fd);	
		
			}
		}
	}
	}
}

void process_request(int fd, std::string request){
	{
		//		std::cout<<"Recived request "<<request<<std::endl;
		//		std::string request (Buffer);
		//		char request[RecvResult+1];
		//		strcpy(request, Buffer);
				std::string path;
				std::string result;
				if (parse_request(request, &path) != 0){
					result = "Error";
				}else{
					result = getPage(path);
				}
			//		std::cout<<result<<std::endl;
				send(fd, result.c_str(), result.length(), MSG_NOSIGNAL);
			}
}	

inline std::string getPage(const std::string path){
	std::stringstream ss;
	std::ifstream file_in;
	std::string data;
	file_in.open(path.c_str());
	getline (file_in, data, '\0');

	     if (file_in) {
		         // Create a result with "HTTP/1.0 200 OK"
				ss << "HTTP/1.0 200 OK";
				ss << "\r\n";
				ss << "Content-length: ";
				ss << data.length();
				ss << "\r\n";
				ss << "Content-Type: text/html";
				ss << "\r\n\r\n";
				ss << data;
	     } else {
		         // Create a result with "HTTP/1.0 404 NOT FOUND"
				 ss << "HTTP/1.0 404 NOT FOUND";
				 ss << "\r\n";
				 ss << "Content-length: ";
				 ss << 0;
				 ss << "\r\n";
				 ss << "Content-Type: text/html";
				 ss << "\r\n\r\n";
		 }
	file_in.close();
	return ss.str();
}

int parse_request(const std::string &request, std::string *result){
		size_t getPos = request.find("GET");
		//std::cout<<"Request=\n"<<request<<std::endl;
		if (getPos != std::string::npos){
			size_t endPos = request.find(" HTTP/1.0");
			if (endPos != std::string::npos){
				size_t paramPos = request.find("?");
				if (paramPos != std::string::npos){
				   	endPos = paramPos;
				}
				*result = request.substr(getPos+5, endPos-(getPos+5)); 
			//	std::cout<<"From "<<getPos+5<<" to "<<endPos-(getPos+5)<<" Path = "<<*result<<std::endl;
				return 0;
			}
		}
		return -1;
}
