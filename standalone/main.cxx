/**
 * The main function for the standalone program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */
#include <stdio.h>
#include "core/Manager.h"
#include <string>

using namespace openvideo;

int main(int argc, char **argv)
{
   if( argc != 2 )
    {
        printf("Usage : %s  configfile",argv[0]);
        return 1;
    }

    printf("**************************************\n");
    printf("**** OpenVideo -standalone- v1.0 *****\n");
    printf("**************************************\n");
    
	

	Manager* manager=Manager::getInstance();
    std::string ovConfig=argv[1];
    manager->parseConfiguration(ovConfig);
    printf("Parsing complete.\n");
    
    manager->run();

    return 0;
}
