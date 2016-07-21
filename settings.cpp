#include "settings.h"

Settings::Settings(){
	addr = NULL;
	port = -1;
	dir = NULL;
}

int Settings::readSettingsFromArguments(int argc, char ** argv){
  
  int index;
  int c;

  opterr = 0;
  while ((c = getopt (argc, argv, "h:p:d:")) != -1)
      switch (c)
        {
	  case 'h':
			          addr = optarg;
			          break;
          case 'p':
			          port = atoi(optarg);
			          break;
          case 'd':
			          dir = optarg;
			          break;
         case '?':
			          if (optopt == 'h' || optopt == 'p' || optopt == 'd')
				          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			          else if (isprint (optopt))
				          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				      else
				          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
					  return -1;
		 default:	  
					  abort ();
		}
	return 0;
}

