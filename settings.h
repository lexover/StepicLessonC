#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
class Settings{                   
public:                           
     char *addr;            
     int port;              
     char *dir;             
	 
	 Settings();	 
     int readSettingsFromArguments(int argc, char **argv);
};    

